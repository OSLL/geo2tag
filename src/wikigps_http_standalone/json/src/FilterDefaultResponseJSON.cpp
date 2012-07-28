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
 * \file FilterDefaultResponseJSON.cpp
 * \brief FilterDefaultResponseJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "json/inc/FilterDefaultResponseJSON.h"

#include <QVariant>
#include <QDebug>

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif

#include "common/inc/User.h"
#include "common/inc/Channel.h"

#include "json/inc/JsonUser.h"
#include "json/inc/JsonChannel.h"
#include "json/inc/JsonDataMark.h"

FilterDefaultResponseJSON::FilterDefaultResponseJSON(QObject *parent) : JsonSerializer(parent)
{
}


FilterDefaultResponseJSON::~FilterDefaultResponseJSON()
{
}


const DataChannels& FilterDefaultResponseJSON::getDataChannels()
{
  return m_hashMap;
}


void FilterDefaultResponseJSON::setDataChannels(const DataChannels& dataChannels)
{
  m_hashMap = dataChannels;
}


bool FilterDefaultResponseJSON::parseJson(const QByteArray& data)
{
  clearContainers();

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();

  if (!ok) return false;

  result["errno"].toInt(&ok);
  if (!ok) return false;
  m_errno = result["errno"].toInt(&ok);

  //QVariantMap channelVariant = result["channels"].toMap();
  QVariantList channelsList = result["channels"].toList();
  int size = channelsList.size();

  for (int i = 0; i < size; i++)
  {
    QVariantMap channelDesc = channelsList.at(i).toMap();
    QVariantMap channelInternal = channelDesc["channel"].toMap();
    QVariantList markList = channelInternal["items"].toList();
    QString channelName = channelInternal["name"].toString();

    QSharedPointer<Channel> channel(new JsonChannel(channelName,"dummy channel[LoadTagsResponse]"));

    for(int j=0; j<markList.size(); j++)
    {
      QVariantMap markMap = markList.at(j).toMap();

      int id = markMap["id"].toInt();
      QString title = markMap["title"].toString();
      QString link = markMap["link"].toString();
      QString description = markMap["description"].toString();
      double altitude = markMap["altitude"].toString().toDouble(&ok);
      if (!ok) return false;
      double latitude = markMap["latitude"].toString().toDouble(&ok);
      if (!ok) return false;
      double longitude = markMap["longitude"].toString().toDouble(&ok);
      if (!ok) return false;

      QString userName = markMap["user"].toString();
      QString timeStr =  markMap["pubDate"].toString();
      QDateTime time = QDateTime::fromString(timeStr, "dd MM yyyy HH:mm:ss.zzz");
      time.setTimeSpec(Qt::UTC);

      QVector<QSharedPointer<common::User> > v = m_usersContainer->vector();
      QSharedPointer<common::User> user(new JsonUser(userName));
      m_usersContainer->push_back(user);

      QSharedPointer<JsonDataMark> newMark(new JsonDataMark(altitude,
        latitude,
        longitude,
        title,
        description,
        link,
        time));
      newMark->setId(id);
      newMark->setUser(user);
      m_hashMap.insert(channel, newMark);
    }
  }
  return true;
}


QByteArray FilterDefaultResponseJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;

  QList<QSharedPointer<Channel> > hashKeys = m_hashMap.uniqueKeys();
  QVariantList jchannels;

  for(int i=0; i<hashKeys.size(); i++)
  {
    QList<QSharedPointer<DataMark> > tags = m_hashMap.values(hashKeys.at(i));
    QVariantList jtags;
    QVariantMap jchannel;
    QVariantMap channel;

    for(int j=0; j<tags.size(); j++)
    {
      QSharedPointer<DataMark> tag = tags.at(j);
      QVariantMap jtag;
      jtag["id"] = tag->getId();
      jtag["title"] = tag->getLabel();
      jtag["link"] = tag->getUrl();
      jtag["description"] = tag->getDescription();
      jtag["latitude"] = tag->getLatitude();
      jtag["longitude"] = tag->getLongitude();
      jtag["altitude"] = tag->getAltitude();
      jtag["user"] = tag->getUser()->getLogin();
      jtag["pubDate"] = tag->getTime().toString("dd MM yyyy HH:mm:ss.zzz");
      jtags.append(jtag);
    }
    channel["items"] = jtags;
    channel["name"] = hashKeys.at(i)->getName();
    jchannel["channel"] = channel;
    jchannels.append(jchannel);
  }
  obj["channels"] = jchannels;
  obj.insert("errno", getErrno());
  return serializer.serialize(obj);
}
