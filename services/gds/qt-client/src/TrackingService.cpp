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

#include "inc/TrackingService.h"
#include "JsonUser.h"

#include <QTimer>
#include <QDebug>

TrackingService::TrackingService(LocationManager *locationManager, QObject *parent) :
QObject(parent),
m_writeTagQuery(0),
m_locationManager(locationManager)
{
  m_dataMark = QSharedPointer<DataMark>(
    new DataMark(0, 0, 0, "Tracking", "Tracking", "", QDateTime::currentDateTime()));
}


void TrackingService::startTracking(QString name, QString password, QString authToken, QString serverUrl)
{
  qDebug() << "startTracking url: " << serverUrl;

  m_period = m_settings.getTrackingPeriod() * 60;

  if (m_writeTagQuery != 0)
    m_writeTagQuery->deleteLater();

  m_writeTagQuery = new WriteTagQuery(this);
  m_writeTagQuery->setTag(m_dataMark);
  connect(m_writeTagQuery, SIGNAL(tagAdded()), this, SLOT(onMarkSent()));
  connect(m_writeTagQuery, SIGNAL(errorOccured(QString)), this, SLOT(onError(QString)));

  m_user = QSharedPointer<JsonUser>(new JsonUser(name, password, authToken));
  m_channel = QSharedPointer<Channel>(new Channel(name, name + "'s channel", ""));
  m_dataMark->setUser(m_user);
  m_dataMark->setChannel(m_channel);
  m_writeTagQuery->setUrl(serverUrl);

  sendMark();
}


void TrackingService::stopTracking()
{
}


void TrackingService::sendMark()
{
  qDebug() << "sendMark";
  QGeoPositionInfo info = m_locationManager->getInfo();
  if (info.isValid())
  {
    m_dataMark->setLatitude(info.coordinate().latitude());
    m_dataMark->setLongitude(info.coordinate().longitude());
    m_dataMark->setTime();
    m_writeTagQuery->doRequest();
  }
  else
  {
    qDebug() << "invalid geo info, waitin and trying again";
    QTimer::singleShot(m_period * 1000, this, SLOT(sendMark()));
  }
}


void TrackingService::onMarkSent()
{
  qDebug() << "TrackingService::onMarkSent";
  emit markSent(QPointF(m_dataMark->getLatitude(), m_dataMark->getLongitude()));
  QTimer::singleShot(m_period * 1000, this, SLOT(sendMark()));
}


void TrackingService::onError(QString error)
{
  qDebug() << "TrackingService::onErrorOccured error: " << error;
  emit errorOccured(error);
  QTimer::singleShot(m_period * 1000, this, SLOT(sendMark()));
}


void TrackingService::updateSettings()
{
  qDebug() << "Updating TrackingService settings";
  m_period = m_settings.getTrackingPeriod();
}
