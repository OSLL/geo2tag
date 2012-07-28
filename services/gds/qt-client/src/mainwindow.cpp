#include "mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QVBoxLayout>
#include <QDebug>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
  if (m_settings.isSettingsEmpty())
  {
    m_settings.setDefaultSettings();
  }

  m_stackedWidget = new QStackedWidget(this);
  m_loginWidget = new LoginWidget(this);
  m_loginWidget->fill();
  m_createAccountWidget = new CreateAccountWidget(this);
  m_mainWidget = new MainWidget(this);
  m_settingsWidget = new SettingsWidget(this);

  createActions();
  createMenus();
  initGUI();

  m_GDSService = QSharedPointer<GDSService>(new GDSService());

  connect(m_loginWidget, SIGNAL(signedIn(QString)),
    this, SLOT(onLoginSignedIn(QString)));
  connect(m_loginWidget, SIGNAL(createAccountRequested()),
    this, SLOT(onCreateAccountRequested()));
  connect(m_createAccountWidget, SIGNAL(finished()),
    this, SLOT(onCreateAccountFinished()));
  connect(m_settingsWidget, SIGNAL(saved()),
    this, SLOT(onSettingsSaved()));
  connect(m_settingsWidget, SIGNAL(cancelled()),
    this, SLOT(onSettingsCancelled()));
  connect(m_settingsWidget, SIGNAL(saved()),
    m_mainWidget, SLOT(onSettingsUpdated()));

  qDebug() << "MainWindow created";
}


MainWindow::~MainWindow()
{
  qDebug() << "MainWindow destructor";
  #if defined(Q_OS_SYMBIAN) ||  defined(Q_WS_SIMULATOR)
  menuBar()->removeAction(m_settingsAction);
  menuBar()->removeAction(m_signOutAction);
  menuBar()->removeAction(m_exitAction);
  #endif
}


void MainWindow::createActions()
{
  m_settingsAction = new QAction("Settings", this);
  m_signOutAction = new QAction("Sign out", this);
  m_exitAction = new QAction("Exit", this);
  m_signOutAction->setVisible(false);
  m_isSignedIn = false;
  connect(m_settingsAction, SIGNAL(triggered()),
    this, SLOT(onSettingsAction()));
  connect(m_signOutAction, SIGNAL(triggered()),
    this, SLOT(onSignOutAction()));
  connect(m_exitAction, SIGNAL(triggered()),
    this, SLOT(onExitAction()));
}


void MainWindow::createMenus()
{
  #if defined(Q_OS_SYMBIAN) ||  defined(Q_WS_SIMULATOR)
  menuBar()->addAction(m_settingsAction);
  menuBar()->addAction(m_signOutAction);
  menuBar()->addAction(m_exitAction);
  #else
  m_menu = menuBar()->addMenu("Menu");
  m_menu->addAction(m_settingsAction);
  m_menu->addAction(m_signOutAction);
  m_menu->addAction(m_exitAction);
  #endif
}


void MainWindow::initGUI()
{
  this->setCentralWidget(m_stackedWidget);
  m_stackedWidget->addWidget(m_loginWidget);
  m_stackedWidget->addWidget(m_createAccountWidget);
  m_stackedWidget->addWidget(m_mainWidget);
  m_stackedWidget->addWidget(m_settingsWidget);

}


void MainWindow::onLoginSignedIn(const QString &authToken)
{
  m_stackedWidget->setCurrentWidget(m_mainWidget);
  m_signOutAction->setVisible(true);
  m_mainWidget->signIn(authToken);
  m_isSignedIn = true;
}


void MainWindow::onCreateAccountRequested()
{
  m_stackedWidget->setCurrentWidget(m_createAccountWidget);
}


void MainWindow::onCreateAccountFinished()
{
  m_stackedWidget->setCurrentWidget(m_loginWidget);
}


void MainWindow::onSettingsSaved()
{
  if (m_isSignedIn)
  {
    m_stackedWidget->setCurrentWidget(m_mainWidget);
  }
  else
  {
    m_stackedWidget->setCurrentWidget(m_loginWidget);
  }
}


void MainWindow::onSettingsCancelled()
{
  if (m_isSignedIn)
  {
    m_stackedWidget->setCurrentWidget(m_mainWidget);
  }
  else
  {
    m_stackedWidget->setCurrentWidget(m_loginWidget);
  }
}


void MainWindow::onSettingsAction()
{
  m_settingsWidget->fill();
  m_stackedWidget->setCurrentWidget(m_settingsWidget);
}


void MainWindow::onSignOutAction()
{
  m_mainWidget->signOut();
  m_loginWidget->fill();
  m_stackedWidget->setCurrentWidget(m_loginWidget);
  m_isSignedIn = false;
  m_signOutAction->setVisible(false);
}


void MainWindow::onExitAction()
{
  qApp->exit();
}


void MainWindow::setOrientation(ScreenOrientation orientation)
{
  #if defined(Q_OS_SYMBIAN)
  // If the version of Qt on the device is < 4.7.2, that attribute won't work
  if (orientation != ScreenOrientationAuto)
  {
    const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
    if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702)
    {
      qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
      return;
    }
  }
  #endif                                // Q_OS_SYMBIAN

  Qt::WidgetAttribute attribute;
  switch (orientation)
  {
    #if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
      attribute = static_cast<Qt::WidgetAttribute>(128);
      break;
    case ScreenOrientationLockLandscape:
      attribute = static_cast<Qt::WidgetAttribute>(129);
      break;
    default:
    case ScreenOrientationAuto:
      attribute = static_cast<Qt::WidgetAttribute>(130);
      break;
    #else                               // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
      attribute = Qt::WA_LockPortraitOrientation;
      break;
    case ScreenOrientationLockLandscape:
      attribute = Qt::WA_LockLandscapeOrientation;
      break;
    default:
    case ScreenOrientationAuto:
      attribute = Qt::WA_AutoOrientation;
      break;
    #endif                              // QT_VERSION < 0x040702
  };
  setAttribute(attribute, true);
}


void MainWindow::showExpanded()
{
  #if defined(Q_OS_SYMBIAN)
  Qt::WindowFlags flags = windowFlags();
  flags |= Qt::WindowSoftkeysVisibleHint;
  flags &= ~Qt::WindowSoftkeysRespondHint;
  //flags |= Qt::WindowSoftkeysRespondHint;
  setWindowFlags(flags);                // Hides visible window
  showFullScreen();
  #elif defined(Q_WS_SIMULATOR)
  showFullScreen();
  #elif defined(Q_WS_MAEMO_5)
  showMaximized();
  #else
  show();
  #endif
}
