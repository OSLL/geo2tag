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

#define DEFAULT_LATITUDE 60.17
#define DEFAULT_LONGITUDE 24.95
#define DAEMON_PORT 34243

TrackerDaemon::TrackerDaemon(): m_settings("osll","tracker"),
            m_loginQuery(NULL),
            m_tagQuery(NULL),
            m_pauseFlag(true),
            m_isConnected(false)
{
//    moveToThread(this);
    m_controlServer = new QTcpServer(NULL);
    connect(m_controlServer, SIGNAL(newConnection()),SLOT(newControlConnection()));
    m_controlServer->listen(QHostAddress::LocalHost, 31234);
}

void TrackerDaemon::newControlConnection()
{
    qDebug() << "new connection to control socket";
    while(m_controlServer->hasPendingConnections())
    {
        new ControlThread(m_controlServer->nextPendingConnection(),this,this);
    }
}

void TrackerDaemon::run()
{
    qDebug() << "thread started";
    QEventLoop eventLoop;
    QString login = m_settings.value("user").toString();
    QString password = m_settings.value("password").toString();
    m_channelName = m_settings.value("channel").toString();
    if(login.isEmpty())
        login = "Mark";
    if(password.isEmpty())
        password = "test";
    m_loginQuery = new LoginQuery(login, password, this);
    connect(m_loginQuery, SIGNAL(connected()), SLOT(onConnected()));
    connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
    m_loginQuery->doRequest();
    qDebug() << "sended LoginRequest";
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


void TrackerDaemon::startTracking()
{
    m_pauseFlag = false;
    onTagAdded();
//    start();
    run();
}

void TrackerDaemon::stopTracking()
{
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
    qDebug() << "Error occured: " << message;
}

void TrackerDaemon::onConnected()
{
    m_isConnected = true;
    qDebug() << "Connected";
    if(m_tagQuery == NULL)
    {
        QSharedPointer<DataMark> mark(
                new JsonDataMark(common::GpsInfo::getInstance().getLatitude(),
                                 common::GpsInfo::getInstance().getLongitude(),
                        //DEFAULT_LATITUDE,DEFAULT_LONGITUDE,
                                 "tracker's tag",
                                 "this tag was generated automaticaly by tracker application",
                                 "unknown",
                                 QDateTime::currentDateTime()));
		m_lastCoords.setX(common::GpsInfo::getInstance().getLatitude());
		m_lastCoords.setY(common::GpsInfo::getInstance().getLongitude());
        QSharedPointer<Channel> channel(new JsonChannel(m_channelName,"dummy channel"));
        mark->setChannel(channel);
        mark->setUser(m_loginQuery->getUser());
        m_tagQuery = new AddNewMarkQuery(mark,this);
        connect(m_tagQuery, SIGNAL(tagAdded()), SLOT(onTagAdded()));
        connect(m_tagQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
	onTagAdded();
    }

}

void TrackerDaemon::onTagAdded()
{
    qDebug() << "Eeeh!! We did it";
     QEventLoop eventLoop;
     QTimer::singleShot(5000, &eventLoop, SLOT(quit())); eventLoop.exec();
        if(!m_pauseFlag && m_isConnected)
        {
            qDebug() << "connected: auth_token=" << m_loginQuery->getUser()->getToken();
            qDebug() << "trying push new tag";
           if(m_tagQuery)
           {
                m_tagQuery->getTag()->setTime(QDateTime::currentDateTime());
                m_tagQuery->getTag()->setLatitude(common::GpsInfo::getInstance().getLatitude());
                m_tagQuery->getTag()->setLongitude(common::GpsInfo::getInstance().getLongitude());
		m_lastCoords.setX(common::GpsInfo::getInstance().getLatitude());
		m_lastCoords.setY(common::GpsInfo::getInstance().getLongitude());
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
