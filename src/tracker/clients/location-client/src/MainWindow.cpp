#include "MainWindow.h"
#include <QMenuBar>
#include <QVBoxLayout>
#include <QGraphicsObject>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDebug>
#include <QtDeclarative>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),contactModel(parent)
{

  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  createActions();

  createMenus();
  const QString mainQmlApp = QLatin1String("qrc:///qml/main.qml");
  view =new QDeclarativeView(this);
  view->rootContext()->setContextProperty("Main", this);
  view->rootContext()->setContextProperty("contactModel", &contactModel);

  view->setSource(QUrl(mainQmlApp));
  // view->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
  view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
  view->engine()->addImportPath(QString("/opt/qtm12/imports"));
  view->engine()->addPluginPath(QString("/opt/qtm12/plugins"));

  /*test*/

  //  QSharedPointer<Contact> co = QSharedPointer<Contact>(new Contact("piter88", "Petja"));
  //  co->setLastDataMark(QSharedPointer<DataMark>(new DataMark(0, 59.938994,30.315857, "QString label",
  //                                                                 "QString description", "QString url", QDateTime::currentDateTime())));
  //  contactModel.addContact(co);

  //  QSharedPointer<Contact> con = QSharedPointer<Contact>(new Contact("marja", "Mother"));
  //  con->setLastDataMark(QSharedPointer<DataMark>(new DataMark(0,59.925657,30.296165, "QString label",
  //                                                                 "QString description", "QString url", QDateTime::currentDateTime().addSecs(-3800))));
  //  contactModel.addContact(con);

  //  QSharedPointer<Contact> contact = QSharedPointer<Contact>(new Contact("vred", "Vasja"));
  //  contact->setLastDataMark(QSharedPointer<DataMark>(new DataMark(0,59.945152,31.371842, "QString label",
  //                                                                 "QString description", "QString url", QDateTime::currentDateTime().addSecs(-9600))));
  //  contactModel.addContact(contact);
  //  QSharedPointer<Contact> cont= QSharedPointer<Contact>(new Contact("quen", "daughter"));
  //  cont->setLastDataMark(QSharedPointer<DataMark>(new DataMark(0,59.94376,30.368825, "QString label",
  //                                                                 "QString description", "QString url", QDateTime::currentDateTime())));
  //  contactModel.addContact(cont);

  /*test*/

  client =new Client(&contactModel, this);
  //   view->rootContext()->setContextProperty("Client", client);

  //qmlRegisterType<ContactModel>();

  QObject* rootObject = dynamic_cast<QObject*>(view->rootObject());
  QObject::connect(authAction, SIGNAL(triggered()), rootObject, SLOT(showLoginView()));
  QObject::connect(client, SIGNAL(error(QVariant)), rootObject, SLOT(incorrect(QVariant)));
  QObject::connect(client, SIGNAL(authentificated(QVariant)), rootObject, SLOT(entered(QVariant)));

  QObject::connect(trackingAction, SIGNAL(triggered()), rootObject, SLOT(showTrackSettings()));
  connect(client, SIGNAL(registrationRequestConfirm()),rootObject,SLOT(showNotify()));

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
  // menu->addAction(shedulerAction);
  menu->addAction(trackingAction);
  menu->addAction(authAction);
  // menu->addAction(pathAction);

}


void MainWindow::createActions()
{
  // shedulerAction = new QAction(tr("Sheduler"), this);
  trackingAction = new QAction(tr("Tracking"), this);
  authAction = new QAction(tr("Authorization data"),this);
  //pathAction = new QAction(tr("Path"), this);

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


void MainWindow::onAuth(QString login, QString pass)
{
  client->auth(login, pass);
}


void MainWindow::onReg(const QString &email, const QString &login, const QString &pass)
{
  client->registration(email, login, pass);
}


void MainWindow::trackingOnOff()
{
  if (!client->isTracking())
    if (client->isAuthentificated())
  {
    client->startTrack();
    emit trackingStarted();
  }
  else
    authAction->trigger();
  else
    client->stopTrack();

}


void MainWindow::changeSettings(int track_interval, bool permission)
{

  client->setHistoryLimit(track_interval);
  client->setPermission(permission);

}


void MainWindow::onSubscribe(const QString &channelName)
{
  if (client->isAuthentificated())
    client->subscribeChannel(channelName);
}


void MainWindow::unsubscribeChannel(const QString &channelName)
{
  client->unSubscribeChannelRequest(channelName);
}


void MainWindow::logout()
{
  client->logout();

}
