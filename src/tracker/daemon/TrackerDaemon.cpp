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

#define DEFAULT_LATITUDE 60.17
#define DEFAULT_LONGITUDE 24.95
#define DAEMON_PORT 34243

#define LOG QString("/var/wikigps-tracker")

TrackerDaemon::TrackerDaemon(): m_settings("osll","tracker"),
            m_exitFlag(false),
            m_isConnected(false),
            m_loginQuery(NULL),
            m_tagQuery(NULL)
{
    moveToThread(this);
}

void TrackerDaemon::run()
{
    QEventLoop eventLoop;
    QString login = m_settings.value("user").toString();
    QString password = m_settings.value("password").toString();
    if(login.isEmpty())
        login = "Mark";
    if(password.isEmpty())
        password = "test";
    m_loginQuery = new LoginQuery(login, password, this);
    connect(m_loginQuery, SIGNAL(connected()), SLOT(onConnected()));
    connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
    m_loginQuery->doRequest();
    for(;;)
    {
        if(m_exitFlag) break;
        if(m_isConnected)
        {
            qDebug() << "connected: auth_token=" << m_loginQuery->getUser()->getToken();
            qDebug() << "trying push new tag";

            if(m_tagQuery)
            {
                m_tagQuery->getTag()->setTime(QDateTime::currentDateTime());
                m_tagQuery->doRequest();
            }
        }
        eventLoop.processEvents(QEventLoop::ExcludeUserInputEvents, 1000);
        QThread::msleep(5000);
    }
}


void TrackerDaemon::startTracking()
{
    m_exitFlag = false;
    start();
}

void TrackerDaemon::stopTracking()
{
    m_exitFlag = true;
}

void TrackerDaemon::onError(QString message)
{
    qDebug() << "Error occured: " << message;
}

void TrackerDaemon::onConnected()
{
    m_isConnected = true;
    if(m_tagQuery == NULL)
    {
        QSharedPointer<DataMark> mark(new JsonDataMark(DEFAULT_LATITUDE,DEFAULT_LONGITUDE,"tracker's tag",
                                                       "this tag was generated automaticaly by tracker application",
                                                       "unknown", QDateTime::currentDateTime()));
        QSharedPointer<Channel> channel(new JsonChannel("My channel","dummy channel"));
        mark->setChannel(channel);
        mark->setUser(m_loginQuery->getUser());
        m_tagQuery = new AddNewMarkQuery(mark,this);
        connect(m_tagQuery, SIGNAL(tagAdded()), SLOT(onTagAdded()));
        connect(m_tagQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
    }
}

void TrackerDaemon::onTagAdded()
{
    qDebug() << "Eeeh!! We did it";
}

TrackerDaemon::~TrackerDaemon()
{
}
