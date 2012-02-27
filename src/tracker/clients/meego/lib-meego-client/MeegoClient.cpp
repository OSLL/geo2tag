/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*!
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

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

MeegoClient::MeegoClient(QObject * parent):QObject(parent),m_trackInterval(5),
m_authentificated(0)
{

  m_loginQuery = new LoginQuery(this);
  connect(m_loginQuery, SIGNAL(connected()), SLOT(onAuthentificated()));
  connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
  connect(m_loginQuery, SIGNAL(errorOccured(QString)), SIGNAL(error(QString)));

  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), SLOT(track()));

  m_netManager = new QNetworkConfigurationManager(this);
  connect(m_netManager, SIGNAL(onlineStateChanged(bool)), SLOT(onNetworkEvent(bool)));

  m_history = new MarksHistory(this);
  connect(m_history,SIGNAL(isFull()),SLOT(onHistoryFull()));

  m_addNewMarkQuery = new WriteTagQuery(this);
  connect(m_addNewMarkQuery,SIGNAL(tagAdded()),SLOT(onMarkAdded()));
  connect(m_addNewMarkQuery, SIGNAL(errorOccured(QString)), SIGNAL(error(QString)));
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
  qDebug() << "Start tracking";
  if (!m_timer->isActive()) m_timer->start(m_trackInterval*1000);
}


// Stop write marks
void MeegoClient::stopTrack()
{
  qDebug() << "Stop tracking";
  if (m_timer->isActive()) m_timer->stop();
  //If we stop tracking than check is there any marks in hystory
  if (isOnline() && isAuthentificated() && !m_history->isEmpty()) sendHistory();
}


bool MeegoClient::isTracking()
{
  return m_timer->isActive();
}


void MeegoClient::setTrackInterval(int sec)
{
  m_trackInterval = sec;
  if (m_timer->isActive())
  {
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
    // qDebug() << "sending coordinates";
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


void MeegoClient::setDispName(const QString & name )
{
  m_name = name;
}


void MeegoClient::track()
{
  // Primitive stub for position source
  //  double lat=common::GpsInfo::getInstance().getLatitude();
  //   double lon=common::GpsInfo::getInstance().getLongitude();
  double lat=30+0.01*(qrand()%100);
  double lon=60+0.01*(qrand()%100);
  QSharedPointer<DataMark> mark(new JsonDataMark(0,lat,lon,m_name,
    "this tag was generated","unknown",QDateTime::currentDateTime()));

  QSharedPointer<Channel> channel(new JsonChannel(DEFAULT_CHANNEL,"dummy channel"));
  mark->setChannel(channel);
  m_history->pushMark(mark);
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

  }
  else
  {
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
  qDebug() << "Authentificated " <<  m_loginQuery->getUser()->getToken();
  m_user =  m_loginQuery->getUser();
  m_authentificated = true;
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
  if (isOnline() && isAuthentificated()) sendHistory();
}


void MeegoClient::pause(int msecs)
{
  QEventLoop eventLoop;
  QTimer::singleShot(msecs, &eventLoop, SLOT(quit()));
  eventLoop.exec();
}
