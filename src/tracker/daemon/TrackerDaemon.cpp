#include "TrackerDaemon.h"
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QEventLoop>
#include "LoginQuery.h"


#define DEFAULT_LATITUDE 60.17
#define DEFAULT_LONGITUDE 24.95
#define DAEMON_PORT 34243

#define LOG QString("/var/wikigps-tracker")

TrackerDaemon::TrackerDaemon(): m_settings("osll","tracker"),
            m_exitFlag(false),
            m_isConnected(false)
{
    moveToThread(this);
//    m_settings.channel = settings.value("channel").toString();
//    m_settings.key = settings.value("key").toString();
//    m_settings.user = settings.value("user").toString();
//    m_settings.passw = settings.value("passwd").toString();

}

void TrackerDaemon::run()
{
    qDebug() << "Tracker daemon is started";
    QEventLoop eventLoop;
    QString login = m_settings.value("user").toString();
    QString password = m_settings.value("password").toString();
    if(login.isEmpty())
        login = "Mark";
    if(password.isEmpty())
        password = "test";
    m_loginQuery = new LoginQuery(login, password, this);
    m_loginQuery->doRequest();
    connect(m_loginQuery, SIGNAL(connected()), SLOT(onConnected()));
    connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
    qDebug() << "login query was sent";
    for(;;)
    {
        if(m_exitFlag) break;
        if(m_isConnected)
        {
            qDebug() << "connected";
        }
        qDebug() << ".";
        eventLoop.processEvents(QEventLoop::ExcludeUserInputEvents, 1000);
        QThread::msleep(1500);
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
}

TrackerDaemon::~TrackerDaemon()
{
    delete m_loginQuery;
}
