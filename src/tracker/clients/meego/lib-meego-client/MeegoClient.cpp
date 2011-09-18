#include "MeegoClient.h"
#include <QDebug>
#include <QEventLoop>
#include "JsonDataMark.h"
#include "JsonChannel.h"
#include "JsonUser.h"
#include "GpsInfo.h"

// Interval for different requests
#define PAUSE_INTERVAL 250
#define DEFAULT_CHANNEL "default"

MeegoClient::MeegoClient(QObject * parent):QObject(parent),m_trackInterval(5)
{

        m_loginQuery = new LoginQuery(this);
        connect(m_loginQuery, SIGNAL(connected()), SLOT(onAuthentificated()));
        connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
        connect(m_loginQuery, SIGNAL(errorOccured(QString)), SIGNAL(errorOccured(QString)));

        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), SLOT(track()));

        m_netManager = new QNetworkConfigurationManager(this);
        connect(m_netManager, SIGNAL(onlineStateChanged(bool)), SLOT(onNetworkEvent(bool)));

        m_history = new MarksHistory(this);
        connect(m_history,SIGNAL(isFull()),SLOT(onHistoryFull()));

        m_addNewMarkQuery = new AddNewMarkQuery(this);
        connect(m_addNewMarkQuery,SIGNAL(tagAdded()),SLOT(onMarkAdded()));
        connect(m_addNewMarkQuery, SIGNAL(errorOccured(QString)), SIGNAL(errorOccured(QString)));
}

void MeegoClient::auth(QString user, QString pass)
{
	//STUB use our auth
        m_loginQuery->setQuery(user,pass);
        m_loginQuery->doRequest();
}
// Starting write marks into history
void MeegoClient::startTrack()
{
    if (!m_timer->isActive()) m_timer->start(m_trackInterval*1000);
}
// Stop write marks
void MeegoClient::stopTrack()
{
    if (m_timer->isActive()) m_timer->stop();
}

bool MeegoClient::isTracking()
{
    return m_timer->isActive();
}

void MeegoClient::setTrackInterval(int sec)
{
    m_trackInterval = sec;
    if (m_timer->isActive()){
        m_timer->stop();
        m_timer->start(m_trackInterval*1000);
    }
}

int MeegoClient::getTrackInterval()
{
    return m_trackInterval;
}

void MeegoClient::setHistoryLimit(int maxMarks)
{
    m_history->setHistoryLimit(maxMarks);
}

int MeegoClient::getHistoryLimit()
{
    return m_history->getHistoryLimit();
}

// Send all stored in history coordinates to the server
void MeegoClient::sendHistory()
{
    while(!m_history->isEmpty() && isOnline())
    {
        qDebug() << "sending coordinates";
        sendLastCoordinate();
        pause(250);
    }
}
// Send last coordinate to the server and remove it from history
void MeegoClient::sendLastCoordinate()
{
    m_addNewMarkQuery->setTag(m_history->popMark());
    m_addNewMarkQuery->getTag()->setUser(m_user);
    m_addNewMarkQuery->doRequest();
}

MarksHistory * MeegoClient::getAllMarks()
{
    return m_history;
}

void MeegoClient::track()
{
    QSharedPointer<DataMark> mark(new JsonDataMark(common::GpsInfo::getInstance().getLatitude(),
                                                           common::GpsInfo::getInstance().getLongitude(),"t",
                            "this tag was generated","unknown",     QDateTime::currentDateTime()));
    QSharedPointer<Channel> channel(new JsonChannel(DEFAULT_CHANNEL,"dummy channel"));
    mark->setChannel(channel);
    m_history->pushMark(mark);
    qDebug() << "Tracked mark" << mark->getLatitude() << " " <<mark->getLongitude();
}

void MeegoClient::onError(QString error)
{
    m_lastError = error;
    if(!m_authentificated)
    {
        // Retry authentification
        qDebug() << "Error during authentification";
        pause(1000);
        m_loginQuery->doRequest();

    }else{
        qDebug() << "Error during mark sending";
    }
}

bool MeegoClient::isAuthentificated()
{
    return m_authentificated;
}


QString MeegoClient::getLastError()
{
    return m_lastError;
}
// Return internete state
bool MeegoClient::isOnline()
{
    return m_netManager->isOnline();
}

void MeegoClient::onAuthentificated()
{
    m_user =  m_loginQuery->getUser();
    emit authentificated();
}

void MeegoClient::onMarkAdded()
{
   qDebug() << "markAdded succesfuly";
    //If MarkHistory not empty than repeate mark sending to the server
}
void MeegoClient::onNetworkEvent(bool state)
{
    if (state) sendHistory();
    //Slot for online|offline
}

void MeegoClient::onGoOffEvent()
{
    sendHistory();

    //Slot for on closed || on low battery
}

void MeegoClient::onHistoryFull()
{
    if (isOnline()) sendHistory();
}

void MeegoClient::pause(int msecs)
{
    QEventLoop eventLoop;
    QTimer::singleShot(msecs, &eventLoop, SLOT(quit()));
    eventLoop.exec();
}
