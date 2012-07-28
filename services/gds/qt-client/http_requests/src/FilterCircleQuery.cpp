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

#include "FilterCircleQuery.h"
#include "defines.h"
#include <QDebug>
#include "FilterCircleRequestJSON.h"
#include "FilterDefaultResponseJSON.h"
#include "JsonDataMark.h"
#include "JsonUser.h"
#include "FShapeCircle.h"

FilterCircleQuery::FilterCircleQuery(
QSharedPointer<common::User> &user,
double latitude,
double longitude,
double radius,
QDateTime timeFrom,
QDateTime timeTo,
const QSharedPointer<Channel> &channel,
QObject *parent)
: DefaultQuery(parent),
m_user(user),
m_latitude(latitude),
m_longitude(longitude),
m_radius(radius),
m_timeFrom(timeFrom),
m_timeTo(timeTo),
m_channel(channel)
{
}


FilterCircleQuery::FilterCircleQuery(QObject *parent): DefaultQuery(parent)
{
}


void FilterCircleQuery::setQuery(QSharedPointer<common::User> &user,
double latitude,
double longitude,
double radius,
QDateTime timeFrom,
QDateTime timeTo,
const QSharedPointer<Channel> &channel)
{
  m_user = user;
  m_latitude = latitude;
  m_longitude = longitude;
  m_radius = radius;
  m_timeFrom = timeFrom;
  m_timeTo = timeTo;
  m_channel = channel;
}


QString FilterCircleQuery::getUrl() const
{
  return FILTER_CIRCLE_HTTP_URL;
}


QByteArray FilterCircleQuery::getRequestBody() const
{
  FilterCircleRequestJSON request;
  QSharedPointer<FShapeCircle> shape(new FShapeCircle(m_latitude, m_longitude, m_radius));
  request.setShape(shape);
  request.setTimeFrom(m_timeFrom);
  request.setTimeTo(m_timeTo);
  if (m_channel != 0)
    request.setChannel(m_channel);
  request.addUser(m_user);
  return request.getJson();
}


void FilterCircleQuery::processReply(QNetworkReply *reply)
{
  FilterDefaultResponseJSON response;
  response.parseJson(reply->readAll());
  if (response.getErrno() == SUCCESS)
  {
    m_hashMap = response.getDataChannels();
    emit tagsReceived();
  }
  else
  {
    emit errorOccured(getErrnoDescription(response.getErrno()));
  }
}


const DataChannels& FilterCircleQuery::getData() const
{
  return m_hashMap;
}


FilterCircleQuery::~FilterCircleQuery()
{
}
