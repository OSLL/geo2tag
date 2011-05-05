#include "TrackerDaemon.h"
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QEventLoop>

#include "JsonDataMark.h"
#include "JsonChannel.h"
#include "JsonUser.h"
#include "LoginQuery.h"
#include "AddNewMarkQuery.h"
#include "ReportThread.h"

#include "defines.h"

#define DAEMON_PORT 34243

TrackerDaemon::TrackerDaemon():m_settings(QSettings::SystemScope,"osll","tracker"),
            m_loginQuery(NULL),
            m_tagQuery(NULL),
            m_pauseFlag(true),
            m_isConnected(false),
	    m_netManager(this)
{
//    moveToThread(this);
    m_controlServer = new QTcpServer(NULL);
    connect(m_controlServer, SIGNAL(newConnection()),SLOT(newControlConnection()));
    connect(&m_netManager,SIGNAL(onlineStateChanged(bool)),SLOT(onOnlineChanged(bool)));
    m_controlServer->listen(QHostAddress::LocalHost, 31234);
}

void TrackerDaemon::onOnlineChanged(bool state)
{
	if (state ) qDebug() << "Online now";
	else qDebug() << "Offline now";
}

void TrackerDaemon::newControlConnection()
{
    qDebug() << "GUI connected to the control socket";
    while(m_controlServer->hasPendingConnections())
    {
        new ControlThread(m_controlServer->nextPendingConnection(),this,this);
    }
}

void TrackerDaemon::run()
{
    qDebug() << "Thread started";
    QEventLoop eventLoop;
    QString login = m_settings.value("user").toString();
    QString password = m_settings.value("password").toString();
    m_channelName = m_settings.value("channel").toString();
    m_visibleName = m_settings.value("visibleName").toString();
    qDebug() << "Read from QSettings " << login << " ,"<< password << " ," <<m_channelName;
    if(m_visibleName.isEmpty())
        m_visibleName = TRACKER_TAG_LABEL;
    if(login.isEmpty())
        login = DEFAULT_USER_NAME;
    if(password.isEmpty())
        password = DEFAULT_USER_PASSWORD;
    if(m_channelName.isEmpty())
       m_channelName = DEFAULT_CHANNEL;
    m_loginQuery = new LoginQuery(login, password, this);
    connect(m_loginQuery, SIGNAL(connected()), SLOT(onConnected()));
    connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
    while(!m_netManager.isOnline())
    {
	    qDebug() << "Device is offline, waiting for network connection";
	    QTimer::singleShot(5000, &eventLoop, SLOT(quit())); eventLoop.exec();
    }
    m_loginQuery->doRequest();
    qDebug() << "Sended first LoginRequest";
    // NOTE commented due to qt bug linked with threads and network on Maemo
    // NOTE bug - http://bugreports.qt.nokia.com/browse/QTBUG-15004
/*    for(;;)
    {
	    qDebug() << "going in for loop";
        if(!m_pauseFlag && m_isConnected)
        {
            qDebug() << "connected: auth_token=" << m_loginQuery->getUser()->getToken();
            qDebug() << "trying push new tag";

           if(m_tagQuery)
           {
                m_tagQuery->getTag()->setTime(QDateTime::currentDateTime());
                m_tagQuery->getTag()->setLatitude(common::GpsInfo::getInstance().getLatitude());
                m_tagQuery->getTag()->setLongitude(common::GpsInfo::getInstance().getLongitude());
                m_tagQuery->doRequest();
            }
        }
        eventLoop.processEvents(QEventLoop::ExcludeUserInputEvents, 1000);	
	QTimer::singleShot(5000, &eventLoop, SLOT(quit())); eventLoop.exec();


   //     QThread::msleep(5000);
    }*/
}


void TrackerDaemon::reloadSettings(){
	qDebug() << "Doing reload of auth_token";
	m_settings.sync();
	m_isConnected = false;
	m_pauseFlag = true;
	run();
}

void TrackerDaemon::startTracking()
{
	qDebug() << "Starting tracker daemon";
	m_pauseFlag = false;
	onTagAdded();
//    start();
//    run();
}

void TrackerDaemon::stopTracking()
{
	qDebug() << "Stoping tracker daemon";
	m_pauseFlag = true;
}

bool TrackerDaemon::isTracking() const
{
    return !m_pauseFlag;
}

QStringList TrackerDaemon::getLog() const
{
    return QStringList("dummy");
}

void TrackerDaemon::onError(QString message)
{
    qDebug() << "Nework error occured: " << message;
    if (!m_isConnected){
     	    QEventLoop eventLoop;
	    QTimer::singleShot(5000, &eventLoop, SLOT(quit())); eventLoop.exec();
	    qDebug() << "Trying to login one more time";
	    if (m_netManager.isOnline()){
		    qDebug() << "Current network state is Online, runing loginRequest again";
		    run();
	    }
	    else {
		    qDebug() << "Current network state is Offline, wait for connection";
	    }
    }
}

void TrackerDaemon::onConnected()
{
    m_isConnected = true;
    qDebug() << "Auth_token recieved - " << m_loginQuery->getUser()->getToken() ;
    if(m_tagQuery == NULL)
    {
        QSharedPointer<DataMark> mark(new JsonDataMark(common::GpsInfo::getInstance().getLatitude(),
                                 common::GpsInfo::getInstance().getLongitude(),
                        //DEFAULT_LATITUDE,DEFAULT_LONGITUDE,
                                 m_visibleName,
                                 "this tag was generated automaticaly by tracker application",
                                 "unknown",
                                 QDateTime::currentDateTime()));
	m_lastCoords.setX(common::GpsInfo::getInstance().getLatitude());
	m_lastCoords.setY(common::GpsInfo::getInstance().getLongitude());
	QSharedPointer<Channel> channel(new JsonChannel(m_channelName,"dummy channel"));
        mark->setChannel(channel);
        mark->setUser(m_loginQuery->getUser());
        m_tagQuery = new AddNewMarkQuery(mark,this);
	qDebug() << "Sending the first tag";
	qDebug() << "Tag parameters: time " <<  m_tagQuery->getTag()->getTime().toString("dd.MM.yyyy hh:mm:ss.zzz")
		<< ", latitude " << m_tagQuery->getTag()->getLatitude()
		<< ", longitude " << m_tagQuery->getTag()->getLongitude();
        connect(m_tagQuery, SIGNAL(tagAdded()), SLOT(onTagAdded()));
        connect(m_tagQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
	onTagAdded();
    }

}

void TrackerDaemon::onTagAdded()
{
    qDebug() << "Mark added successfuly";
     QEventLoop eventLoop;
     QTimer::singleShot(5000, &eventLoop, SLOT(quit())); eventLoop.exec();
        if(!m_pauseFlag && m_isConnected)
        {
            qDebug() << "Setting and adding new tag";
           if(m_tagQuery)
           {
                m_tagQuery->getTag()->setTime(QDateTime::currentDateTime());
                m_tagQuery->getTag()->setLatitude(common::GpsInfo::getInstance().getLatitude());
                m_tagQuery->getTag()->setLongitude(common::GpsInfo::getInstance().getLongitude());
		m_lastCoords.setX(common::GpsInfo::getInstance().getLatitude());
		m_lastCoords.setY(common::GpsInfo::getInstance().getLongitude());
		qDebug() << "Tag parameters: time " <<  m_tagQuery->getTag()->getTime().toString("dd.MM.yyyy hh:mm:ss.zzz")
			<< ", latitude " << m_tagQuery->getTag()->getLatitude()
			<< ", longitude " << m_tagQuery->getTag()->getLongitude();
                m_tagQuery->doRequest();
            }
	}
}

const QPointF& TrackerDaemon::getLastCoords() const
{
	return m_lastCoords;
}

TrackerDaemon::~TrackerDaemon()
{
    delete m_controlServer;
}
