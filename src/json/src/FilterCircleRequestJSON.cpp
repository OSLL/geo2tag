/*
 * Copyright 2011  bac1ca  bac1ca89@gmail.com
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

/*! ---------------------------------------------------------------
 *
 * \file FilterCircleRequestJSON.cpp
 * \brief FilterCircleRequestJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "FilterCircleRequestJSON.h"
#include "JsonSession.h"

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif

FilterCircleRequestJSON::FilterCircleRequestJSON(QObject *parent) : FilterRequestJSON(parent)
{
}


QByteArray FilterCircleRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;
  obj.insert("auth_token",m_sessionsContainer->at(0)->getSessionToken());
  obj.insert("latitude", m_latitude);
  obj.insert("longitude", m_longitude);
  obj.insert("radius", m_radius);
  obj.insert("time_from", getTimeFrom().toString("dd MM yyyy HH:mm:ss.zzz"));
  obj.insert("time_to", getTimeTo().toString("dd MM yyyy HH:mm:ss.zzz"));
  if (m_channelsContainer->size() > 0)
    obj.insert("channel", m_channelsContainer->at(0)->getName());
  return serializer.serialize(obj);
}


bool FilterCircleRequestJSON::parseJson(const QByteArray&data)
{
  clearContainers();
  FilterRequestJSON::parseJson(data);

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok)
  {
    return false;
  }
  QString auth_token = result["auth_token"].toString();
  setTimeFrom(QDateTime::fromString(result["time_from"].toString(), "dd MM yyyy HH:mm:ss.zzz"));
  setTimeTo(QDateTime::fromString(result["time_to"].toString(), "dd MM yyyy HH:mm:ss.zzz"));
  double latitude = result["latitude"].toDouble(&ok);
  if (!ok) return false;

  double longitude = result["longitude"].toDouble(&ok);
  if (!ok) return false;

  double radius = result["radius"].toDouble(&ok);
  if (!ok) return false;

  FilterRequestJSON::setShape(QSharedPointer<FShape>(new FShapeCircle(latitude, longitude, radius)));
  m_sessionsContainer->push_back(QSharedPointer<Session>(new JsonSession(auth_token, QDateTime::currentDateTime(), QSharedPointer<common::User>(NULL))));
  return true;
}


void FilterCircleRequestJSON::setShape(const QSharedPointer<FShapeCircle> &shape)
{
  FilterRequestJSON::setShape(shape);
  m_latitude = shape->getLatitude();
  m_longitude = shape->getLongitude();
  m_radius = shape->getRadius();
}


double FilterCircleRequestJSON::getLatitude() const
{
  return m_latitude;
}


double FilterCircleRequestJSON::getLongitude() const
{
  return m_longitude;
}


double FilterCircleRequestJSON::getRadius() const
{
  return m_radius;
}
