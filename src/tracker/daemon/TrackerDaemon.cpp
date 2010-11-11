#include "TrackerDaemon.h"
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QFile>
//#include "ApplyMarkQuery.h"
//#include "SubscribeChannelQuery.h"
#include "LoginQuery.h"


#define DEFAULT_LATITUDE 60.17
#define DEFAULT_LONGITUDE 24.95
#define DAEMON_PORT 34243
#define LOG QString("/var/wikigps-tracker")

TrackerDaemon::TrackerDaemon()
{
    qDebug() << "Starting server";
    // Create log file and textStream for it
    m_log = new QFile(LOG,this);
    if (!m_log->open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "cant open file!!!!";
    }
//    m_logOut = new QTextStream(m_log);
    QTimer::singleShot(0, this, SLOT(setupBearer()));
    connect(&m_applyMarkQuery,SIGNAL(replyError(QNetworkReply::NetworkError)),this,SLOT(onNetworkError(QNetworkReply::NetworkError)));
    connect(&m_applyMarkQuery,SIGNAL(manageSslErrors()),this,SLOT(onSslError()));
    connect(&m_applyMarkQuery, SIGNAL(responseReceived(QString,QString)), this, SLOT(onApplyMarkResponse(QString,QString)));
    connect(&m_loginQuery, SIGNAL(responseReceived(QString,QString,QString)), this, SLOT(onLoginResponse(QString,QString,QString)));
    initSettings();
    login(m_settings.user,m_settings.passw);
}
void TrackerDaemon::initSettings()
{
    QSettings settings("osll","tracker");

    if( settings.value("magic").toString() == APP_MAGIC )
    {
        qDebug() << "magic = " << settings.value("magic").toString();
        emit readSettings();
    }
    else
    {
        setStatus(QString("Error"),QString("No settings!"));
    }
}

void TrackerDaemon::readSettings()
{
    QSettings settings("osll","tracker");
    m_settings.channel = settings.value("channel").toString();
    m_settings.key = settings.value("key").toString();
    m_settings.user = settings.value("user").toString();
    m_settings.passw = settings.value("passwd").toString();
//    m_settings.auth_token = settings.value("auth_token").toString();
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
        start();
    }
}

void TrackerDaemon::onNetworkError(QNetworkReply::NetworkError){
        //Add here message boxes for error visualisation
        // Add reconnecting when shirt happens
        setStatus("Error","Network error occure. Connection will be reopened");
        stop();
        start();

}

void TrackerDaemon::onSslError(){
        //Add visualisation and reconnection for errors.
        setStatus("Error","Ssl Error occuried");
        stop();
        start();
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
    m_log->close();
}


void TrackerDaemon::login(QString login,QString password)
{
    qDebug() << "login " << login << " " << password;
    m_settings.user=login;
    m_settings.passw=password;
    m_loginQuery.setQuery(login,password);
    m_loginQuery.doRequest();
}





void TrackerDaemon::setStatus(QString status,QString status_description)
{
    m_status=status;
    m_statusDescription=status_description;
    m_lastAttempt=QDateTime::currentDateTime();
    QTextStream out(m_log);
    out << m_lastAttempt.toString(QString("hh:mm:ss dd.MM.yyyy")) << " " << m_status << " " 
        << m_statusDescription << "\n";


}

