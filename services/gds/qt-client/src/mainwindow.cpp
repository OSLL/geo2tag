#include "mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QVBoxLayout>
#include <QDebug>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_stackedWidget = new QStackedWidget(this);
    m_loginWidget = new LoginWidget(this);
    m_mainWidget = new MainWidget(this);

    createActions();
    createMenus();
    initGUI();

    m_GDSService = QSharedPointer<GDSService>(new GDSService());

    connect(m_loginWidget, SIGNAL(signedIn(QString)),
            this, SLOT(onLoginSignedIn(QString)));;

    qDebug() << "MainWindow created";
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    m_settingsAction = new QAction("Settings", this);
    m_signOutAction = new QAction("Sign out", this);
    connect(m_settingsAction, SIGNAL(triggered()),
            this, SLOT(testHideLogout()));
}

void MainWindow::createMenus()
{
    m_menu = menuBar()->addMenu("Menu");
    m_menu->addAction(m_settingsAction);
    m_menu->addAction(m_signOutAction);

}

void MainWindow::initGUI()
{
    this->setCentralWidget(m_stackedWidget);
    m_stackedWidget->addWidget(m_loginWidget);
    m_stackedWidget->addWidget(m_mainWidget);

}

void MainWindow::onLoginSignedIn(const QString &authToken)
{
    m_stackedWidget->setCurrentWidget(m_mainWidget);
    m_mainWidget->signIn(authToken);
}

void MainWindow::onMainSignedOut()
{
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
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
#else // QT_VERSION < 0x040702
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
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::testHideLogout()
{
    m_signOutAction->setVisible(false);
}




