#include "MainWindow.h"
#include <QMenuBar>
#include <QVBoxLayout>
#include <QGraphicsObject>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent)
{
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  createActions();

  createMenus();
  const QString mainQmlApp = QLatin1String("qrc:///main.qml");
  view =new QDeclarativeView(this);

  view->setSource(QUrl(mainQmlApp));
  view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
  view->engine()->addImportPath(QString("/opt/qtm12/imports"));
  view->engine()->addPluginPath(QString("/opt/qtm12/plugins"));

  client =new Client(this);
  view->rootContext()->setContextProperty("Client", client);

  QObject* rootObject = dynamic_cast<QObject*>(view->rootObject());
  QObject::connect(authAction, SIGNAL(triggered()), rootObject, SLOT(showLoginView()));
  QObject::connect(client, SIGNAL(error(QVariant)), rootObject, SLOT(incorrect(QVariant)));
  QObject::connect(client, SIGNAL(authentificated(QVariant)), rootObject, SLOT(entered(QVariant)));

  #if defined(Q_WS_MAEMO_5)
  view->setGeometry(QRect(0,0,800,480));
  view->showFullScreen();
  #elif defined(Q_WS_S60)
  view->setGeometry(QRect(0,0,640,360));
  view->showFullScreen();
  #else
  view->setGeometry(QRect(100,100,800, 480));
  view->show();
  #endif
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(view);
  centralWidget->setLayout(mainLayout);

}


void MainWindow::createMenus()
{
  menu = menuBar()->addMenu(tr("Settings"));
  menu->addAction(shedulerAction);
  menu->addAction(trackingAction);
  menu->addAction(authAction);
  menu->addAction(pathAction);

}


void MainWindow::createActions()
{
  shedulerAction = new QAction(tr("Sheduler"), this);
  trackingAction = new QAction(tr("Tracking"), this);
  authAction = new QAction(tr("Authorization data"),this);
  pathAction = new QAction(tr("Path"), this);

}


void MainWindow::shedulerSettings()
{

}


void MainWindow::trackingSettings()
{

}


void MainWindow::pathSettings()
{

}
