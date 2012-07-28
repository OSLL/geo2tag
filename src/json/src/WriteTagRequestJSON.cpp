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
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "WriteTagRequestJSON.h"
#include "DataMarks.h"
#include "JsonChannel.h"
#include "JsonDataMark.h"
#include "JsonSession.h"

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif

WriteTagRequestJSON::WriteTagRequestJSON(QObject *parent) : JsonSerializer(parent)
{
}


WriteTagRequestJSON::WriteTagRequestJSON(const QSharedPointer<Session> &session,
const QSharedPointer<Channel> &channel,
const QSharedPointer<DataMark> &tag,
QObject *parent)
: JsonSerializer(parent)
{
  m_sessionsContainer->push_back(session);
  m_channelsContainer->push_back(channel);
  m_tagsContainer->push_back(tag);
}


bool WriteTagRequestJSON::parseJson(const QByteArray &data)
{
  clearContainers();
  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok) return false;

  QString auth_token = result["auth_token"].toString();
  QString channel_name = result["channel"].toString();
  QString title = result["title"].toString();
  QString link = result["link"].toString();
  QString description = result["description"].toString();
  double altitude = result["altitude"].toDouble(&ok);
  if (!ok) return false;
  double longitude = result["longitude"].toDouble(&ok);
  if (!ok) return false;

  double latitude = result["latitude"].toDouble(&ok);
  if (!ok) return false;

  QDateTime time = QDateTime::fromString(result["time"].toString(), "dd MM yyyy HH:mm:ss.zzz");

  QSharedPointer<Session> session(new JsonSession(auth_token, QDateTime::currentDateTime(), QSharedPointer<common::User>(NULL)));
  QSharedPointer<Channel> channel(new JsonChannel(channel_name, "unknown"));

  QSharedPointer<DataMark> tag(new JsonDataMark(altitude, latitude, longitude, title, description, link, time));
  //tag->setChannel(channel);
  //tag->setSession(session);
  m_channelsContainer->push_back(channel);
  m_sessionsContainer->push_back(session);
  m_tagsContainer->push_back(tag);

  return true;
}


QByteArray WriteTagRequestJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap request;
  QSharedPointer<DataMark> mark = m_tagsContainer->at(0);
  QSharedPointer<Session> session = m_sessionsContainer->at(0);
  QSharedPointer<Channel> channel = m_channelsContainer->at(0);
  request.insert("auth_token", session->getSessionToken());
  request.insert("channel", channel->getName());
  request.insert("title", mark->getLabel().isEmpty()? "New mark":mark->getLabel());
  request.insert("link", mark->getUrl());
  request.insert("description", mark->getDescription());
  request.insert("latitude", mark->getLatitude());
  request.insert("altitude", mark->getAltitude());
  request.insert("longitude", mark->getLongitude());
  request.insert("time", mark->getTime().toString("dd MM yyyy HH:mm:ss.zzz"));
  return serializer.serialize(request);
}
