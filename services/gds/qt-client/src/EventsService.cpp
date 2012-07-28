/*
 * Copyright 2012  Ivan Bezyazychnyy  ivan.bezyazychnyy@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

#include "inc/EventsService.h"
#include <QDebug>
#include <QTimer>
#include "defines.h"
#include "Channel.h"
#include "DataChannel.h"
#include "JsonUser.h"
#include <QThread>

EventsService::EventsService(LocationManager *locationManager, QObject *parent) :
QObject(parent),
m_locationManager(locationManager),
m_filterCircleQuery(0),
m_mediaPlayer(0)
{
}


EventsService::~EventsService()
{
  if (m_mediaPlayer)
  {
    m_mediaPlayer->stop();
    m_mediaPlayer->deleteLater();
  }
}


void EventsService::startService(QString name, QString password, QString authToken, QString serverUrl)
{
  qDebug() << "EventsService::startService " << this->thread()->currentThreadId();

  m_eventsRadius = m_settings.getRadius();
  m_eventsChannel = QSharedPointer<Channel>(new Channel(EVENTS_CHANNEL, "", ""));

  if (m_filterCircleQuery != 0)
    m_filterCircleQuery->deleteLater();

  m_filterCircleQuery = new FilterCircleQuery(this);
  connect(m_filterCircleQuery, SIGNAL(tagsReceived()), this, SLOT(onTagsReceived()));
  connect(m_filterCircleQuery, SIGNAL(errorOccured(QString)), this, SLOT(onError(QString)));

  m_user = QSharedPointer<JsonUser>(new JsonUser(name, password, authToken));
  m_filterCircleQuery->setUrl(serverUrl + FILTER_CIRCLE_HTTP_URL);

  requestEvents();
}


void EventsService::stopService()
{
}


void EventsService::requestEvents()
{
  qDebug() << "EventsService::requestEvents";
  QGeoPositionInfo info = m_locationManager->getInfo();
  if (info.isValid())
  {
    QDateTime currentTime = QDateTime::currentDateTimeUtc();

    // just for sure that all newest events will be received
    QDateTime timeTo = currentTime.addYears(1);

    QDateTime timeFrom = currentTime.addSecs( - RELEVANT_PERIOD_IN_HOURS * 60 * 60);

    m_filterCircleQuery->setQuery(m_user, info.coordinate().latitude(),
      info.coordinate().longitude(), m_eventsRadius,
      timeFrom, timeTo, m_eventsChannel);

    qDebug() << "do events service request";
    m_filterCircleQuery->doRequest();
  }
  else
  {
    qDebug() << "invalid geo info, waitin and trying again";
    QTimer::singleShot(DEFAULT_EVENTS_PERIOD * 1000, this, SLOT(requestEvents()));
  }
}


void EventsService::onTagsReceived()
{
  qDebug() << "EventsService::onTagsReceived";
  DataChannels data = m_filterCircleQuery->getData();
  QList<QSharedPointer<Channel> > channels = data.keys();
  Events events;
  bool found = false;
  for (int i = 0; i < channels.size(); ++i)
  {
    qDebug() << "channel: " << channels[i]->getName();
    if (channels[i]->getName() == "Events")
    {
      found = true;
      events = data.values(channels[i]);
      break;
    }
  }

  if (!NOT_RECEIVE_OWN_EVENTS)
  {
    QMutableListIterator<QSharedPointer<DataMark> > iter(events);
    while (iter.hasNext())
    {
      QSharedPointer<DataMark> event = iter.next();
      qDebug() << event->getUser()->getLogin();
      if (event->getUser()->getLogin() == m_user->getLogin())
      {
        iter.remove();
      }
    }
  }

  if (found)
  {
    bool newEvents = false;
    bool expiredEvents = false;
    QSet<int> newEventsIds;
    foreach (QSharedPointer<DataMark> event, events)
    {
      int id = event->getId();
      qDebug() << "id: " << id;
      newEventsIds.insert(id);
      if (!m_eventsIds.contains(id))
        newEvents = true;
    }
    if (!newEventsIds.contains(m_eventsIds))
    {
      expiredEvents = true;
    }
    if (newEvents)
    {
      // play alert
      if (m_mediaPlayer)
      {
        m_mediaPlayer->stop();
        m_mediaPlayer->deleteLater();
      }
      qDebug() << "start player";
      using namespace Phonon;
      #ifdef Q_OS_SYMBIAN
      m_mediaPlayer = createPlayer(MusicCategory, MediaSource(":/data/siren.wav"));
      m_mediaPlayer->play();
      #endif
      qDebug() << "player finished";
    }
    if (newEvents || expiredEvents)
    {
      m_eventsIds = newEventsIds;
      emit eventsReceived(events);
    }
  }

  QTimer::singleShot(DEFAULT_EVENTS_PERIOD * 1000, this, SLOT(requestEvents()));
}


void EventsService::onError(QString error)
{
  qDebug() << "EventsService::onErrorOccured error: " << error;
  emit errorOccured(error);
  QTimer::singleShot(DEFAULT_EVENTS_PERIOD * 1000, this, SLOT(requestEvents()));
}


void EventsService::updateSettings()
{
  qDebug() << "Updating EventsService settings";
  m_eventsRadius = m_settings.getRadius();
}
