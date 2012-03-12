#include "Client.h"
#include <QDebug>
#include <QEventLoop>
#include "JsonDataMark.h"
#include "JsonChannel.h"
#include "JsonUser.h"
#include "GpsInfo.h"

#include <QTimer>
#include <QVariant>

// Interval for different requests
#define PAUSE_INTERVAL 250
#define DEFAULT_CHANNEL "default"

Client::Client(QObject *parent) :
QObject(parent),m_trackInterval(5),
m_authentificated(0)
{
  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), SLOT(track()));

  m_loginQuery = new LoginQuery(this);
  connect(m_loginQuery, SIGNAL(connected()), SLOT(onAuthentificated()));
  connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
  connect(m_loginQuery, SIGNAL(errorOccured(int)),SLOT(onError(int)));

  m_addUserQuery = new AddUserQuery(this);
  connect(m_addUserQuery, SIGNAL(connected()), SLOT(onRegistered()));
  connect(m_addUserQuery, SIGNAL(errorOccured(int)),SLOT(onError(int)));

  m_netManager = new QNetworkConfigurationManager(this);

  m_history = new MarksHistory(this);
  connect(m_history,SIGNAL(isFull()),SLOT(onHistoryFull()));

  m_addNewMarkQuery = new WriteTagQuery(this);
  connect(m_addNewMarkQuery,SIGNAL(tagAdded()),SLOT(onMarkAdded()));
  //  connect(m_addNewMarkQuery, SIGNAL(errorOccured(QString)), SIGNAL(error(QString)));

}


void Client::auth(QString user, QString pass)
{
  m_loginQuery->setQuery(user,pass);
  m_loginQuery->doRequest();
}

void Client::registration(QString user, QString pass)
{
    m_addUserQuery->setQuery(user,pass);
    m_addUserQuery->doRequest();
}


void Client::onError(QString err)
{

  emit error(QVariant(err));
  m_lastError = err;
  if(!m_authentificated)
  {
    // Retry authentification
    qDebug() << "Error during authentification";
    pause(1000);
    m_loginQuery->doRequest();

  }
  else
  {
    qDebug() << "Error during mark sending";
  }
}


void Client::onError(int err)
{
  if (err==INCORRECT_CREDENTIALS_ERROR)
    emit error(QVariant("Incorrect login or password"));
  else
      if (err==USER_ALREADY_EXIST_ERROR)
          emit error(QVariant("User with that name already exist"));

}


bool Client::isAuthentificated()
{
  return m_authentificated;
}


QString Client::getLastError()
{
  return m_lastError;
}


// Return internete state
bool Client::isOnline()
{
  return m_netManager->isOnline();
}


void Client::sendHistory()
{
  while(!m_history->isEmpty() && isOnline())
  {
    sendLastCoordinate();
    pause(250);
  }
}


void Client::onAuthentificated()
{
  qDebug() << "Authentificated " <<  m_loginQuery->getUser()->getToken();
  m_user =  m_loginQuery->getUser();
  m_authentificated = true;
  emit authentificated(QVariant(m_user->getLogin()));
}

void Client::onRegistered()
{
    qDebug() << "Registered " <<  m_addUserQuery->getUser()->getToken();
    m_user =  m_addUserQuery->getUser();
    m_authentificated = true;
    emit authentificated(QVariant(m_user->getLogin()));
}


void Client::pause(int msecs)
{
  QEventLoop eventLoop;
  QTimer::singleShot(msecs, &eventLoop, SLOT(quit()));
  eventLoop.exec();
}


void Client::startTrack()
{
  if (!m_authentificated) emit authRequest();
  else
  if (!m_timer->isActive())
    m_timer->start(m_trackInterval*1000);

}


void Client::onMarkAdded()
{

}


void Client::track()
{
  // Primitive stub for position source
  double lat=common::GpsInfo::getInstance().getLatitude();
  double lon=common::GpsInfo::getInstance().getLongitude();
  /*  double lat=30+0.01*(qrand()%100);
    double lon=60+0.01*(qrand()%100);*/
  QSharedPointer<DataMark> mark(new JsonDataMark(0,lat,lon,"m_name",
    "this tag was generated","unknown",QDateTime::currentDateTime()));

  QSharedPointer<Channel> channel(new JsonChannel(DEFAULT_CHANNEL,"dummy channel"));
  mark->setChannel(channel);
  m_history->pushMark(mark);
}


void Client::onHistoryFull()
{
  if (isOnline() && isAuthentificated()) sendHistory();
}


void Client::onGoOffEvent()
{
  sendHistory();
}


void Client::onNetworkEvent(bool state)
{
  if (state) sendHistory();
}


void Client::sendLastCoordinate()
{
  m_addNewMarkQuery->setTag(m_history->popMark());
  m_addNewMarkQuery->getTag()->setUser(m_user);
  m_addNewMarkQuery->doRequest();

}


bool Client::isTracking()
{
  return m_timer->isActive();
}


void Client::stopTrack()
{
  if (m_timer->isActive()) m_timer->stop();
  if (isOnline() && isAuthentificated() && !m_history->isEmpty()) sendHistory();
}
