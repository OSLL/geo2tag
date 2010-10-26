#include "TrackerDaemon.h"
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

#include "ApplyMarkQuery.h"
#include "SubscribeChannelQuery.h"
#include "LoginQuery.h"


#define DEFAULT_LATITUDE 60.17
#define DEFAULT_LONGITUDE 24.95
#define DAEMON_PORT 34243
#define ERRORLOG_LENGTH 30

TrackerDaemon::TrackerDaemon():
        m_server(new QTcpServer(this))
{
    qDebug() << "in constructor";
    if(m_server->listen(QHostAddress::LocalHost, DAEMON_PORT))
    {
        qDebug() << "Server is started";
    }
    else
    {
        qDebug() << "Critical error - can not start server!!!!!" ;
    }

    connect(m_server, SIGNAL(newConnection()), this, SLOT(uiConnected()));
    QTimer::singleShot(0, this, SLOT(setupBearer()));
    connect(&m_applyMarkQuery, SIGNAL(responseReceived(QString,QString)), this, SLOT(onApplyMarkResponse(QString,QString)));
    connect(&m_loginQuery, SIGNAL(responseReceived(QString,QString,QString)), this, SLOT(onLoginResponse(QString,QString,QString)));
    connect(&m_subscribeQuery, SIGNAL(responseReceived(QString,QString)), this, SLOT(onSubscribeChannelResponse(QString,QString)));
    connect(&m_applyChannelQuery, SIGNAL(responseReceived(QString,QString)), this, SLOT(onApplyChannelResponse(QString,QString)));
    initSettings();
}

void TrackerDaemon::onApplyChannelResponse(QString status,QString status_description)
{
    setStatus(status,status_description);
}

void TrackerDaemon::cleanLocalSettigns()
{
    QSettings settings("osll","tracker");
    settings.clear();

}

void TrackerDaemon::initSettings()
{
    qDebug() << m_server->errorString();
    QSettings settings("osll","tracker");

    if( settings.value("magic").toString() == APP_MAGIC )
    {
        qDebug() << "magic = " << settings.value("magic").toString();
        emit readSettings();
    }
    else
    {
        emit createSettings();
    }
}

void TrackerDaemon::readSettings()
{
    QSettings settings("osll","tracker");
    m_settings.channel = settings.value("channel").toString();
    m_settings.key = settings.value("key").toString();
    m_settings.user = settings.value("user").toString();
    m_settings.passw = settings.value("passwd").toString();
    m_settings.auth_token = settings.value("auth_token").toString();
    m_settings.initialized = true;
}

void TrackerDaemon::createSettings()
{

    //TODO Add settings initialization

    QSettings settings("osll","tracker");
    settings.setValue("channel",m_settings.channel);
    settings.setValue("key",m_settings.key);
    settings.setValue("user",m_settings.user);
    settings.setValue("passwd",m_settings.passw);
    settings.setValue("auth_token",m_settings.auth_token);
    settings.setValue("magic",APP_MAGIC);
    m_settings.initialized = true;
}

//TODO learn what it is
void TrackerDaemon::setupBearer()
{  /*
    // Set Internet Access Point
    QNetworkConfigurationManager manager;
    const bool canStartIAP = (manager.capabilities()
        & QNetworkConfigurationManager::CanStartAndStopInterfaces);
    // Is there default access point, use it
    QNetworkConfiguration cfg = manager.defaultConfiguration();
    if (!cfg.isValid() || !canStartIAP) {
        return;
    }
    m_session = new QNetworkSession(cfg);
    m_session->open();
    m_session->waitForOpened();
    */
}

void TrackerDaemon::timerEvent(QTimerEvent *te)
{
    killTimer(te->timerId());


    if( m_settings.initialized )
    {
        qDebug() << "   ... dropping mark" << QDateTime().currentDateTime();
        bool result = setMark();
    }

    startTimer(UPDATE_INTERVAL);
}

bool TrackerDaemon::setMark()
{
    qreal latitude = DEFAULT_LATITUDE;
    qreal longitude = DEFAULT_LONGITUDE;

    //    if (m_positionInfo.coordinate().isValid()) {
    latitude = common::GpsInfo::getInstance().getLatitude();
    longitude = common::GpsInfo::getInstance().getLongitude();

    m_applyMarkQuery.setQuery(m_settings.auth_token,
                              m_settings.channel,
                              QString("title"),
                              QString("url"),
                              QString("description"),
                              latitude,
                              longitude,
                              QLocale("english").toString(QDateTime::currentDateTime(),"dd MMM yyyy hh:mm:ss"));
    m_applyMarkQuery.doRequest();
    //}
    // else {
    //   setStatus(QString("Error"),QString("GPS error"));
    //    }



    return true;
}

void TrackerDaemon::onApplyMarkResponse(QString status,QString status_description)
{
    setStatus(status,status_description);
    qDebug() << "added mark, status " << status << " " << status_description;
}

void TrackerDaemon::onLoginResponse(QString status,QString auth_token,QString status_description){
    setStatus(status,status_description);
    if (status == QString("Ok"))
    {
        m_settings.auth_token=auth_token;
    }
}

void TrackerDaemon::onSubscribeChannelResponse(QString status,QString status_description){
    setStatus(status,status_description);
}


void TrackerDaemon::start()
{// start adding marks by timer
    qDebug() << "start() slot executed";
    if(m_settings.auth_token!=QString(""))
    {

        m_timerID=startTimer(100); // first update should be fast*/
    }
    else
    {
        // Write UI that we didnt authentificated
        setStatus(QString("Error"),QString("bad login or password"));
    }
}

void TrackerDaemon::stop(){// stop adding marks by timer;
    qDebug() << "stop() slot executed";
    if (m_timerID){
        killTimer(m_timerID);
        m_timerID=0;
    }
}


void TrackerDaemon::login(QString login,QString password)
{
    qDebug() << "login " << login << " " << password;
    m_settings.user=login;
    m_settings.passw=password;
    m_loginQuery.setQuery(login,password);
    m_loginQuery.doRequest();
}



void TrackerDaemon::uiConnected()
{

   m_uiSocket=m_server->nextPendingConnection();
   qDebug() << "uiConnected, socket " << m_uiSocket;
   m_receiver=new RequestReceiver(m_uiSocket,this);
   connect(m_receiver,SIGNAL(start()),this,SLOT(start()));
   connect(m_receiver,SIGNAL(stop()),this,SLOT(stop()));
   connect(m_receiver,SIGNAL(login(QString,QString)),this,SLOT(login(QString,QString)));
   connect(m_receiver,SIGNAL(setChannel(QString,QString)),this,SLOT(setChannel(QString,QString)));
   connect(m_receiver,SIGNAL(addChannel(QString,QString)),this,SLOT(addChannel(QString,QString)));
   connect(m_receiver,SIGNAL(status()),this,SLOT(status()));
   connect(m_uiSocket, SIGNAL(disconnected()),
                m_uiSocket, SLOT(deleteLater()));

}


void TrackerDaemon::setStatus(QString status,QString status_description)
{
    m_status=status;
    m_statusDescription=status_description;
    m_lastAttempt=QDateTime::currentDateTime();
}

void TrackerDaemon::status()
{
    QDataStream out(m_uiSocket);
    out << m_lastAttempt;
    out << m_status;
    if (m_status!="Ok")
    {
        out << m_statusDescription;
    }
    m_uiSocket->disconnectFromHost();
}

void TrackerDaemon::setChannel(QString channel,QString channelKey)
{
    m_settings.channel=channel;
    m_settings.key=channelKey;
    m_subscribeQuery.setQuery(m_settings.auth_token,m_settings.channel);
    m_subscribeQuery.doRequest();
}

void TrackerDaemon::addChannel(QString channel,QString channelKey)
{
    m_applyChannelQuery.setQuery(m_settings.auth_token,channel,QString(""),QString(""),1000);
}

