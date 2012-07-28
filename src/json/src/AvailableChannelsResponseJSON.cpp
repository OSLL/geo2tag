/*
 * Copyright 2010  Open Source & Linux Lab (OSLL) osll@osll.spb.ru
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
/*  */
/*!
 * \file ChannelListJSON.cpp
 * \brief implementation of Channels2JSON
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 *
 * ------------------------------------------------------------------------ */

#include <qjson/parser.h>
#include <qjson/serializer.h>
//#include "parser.h"
//#include "serializer.h"

#include "AvailableChannelsResponseJSON.h"
#include "JsonChannel.h"

AvailableChannelsResponseJSON::AvailableChannelsResponseJSON(QObject *parent) : JsonSerializer(parent)
{

}


AvailableChannelsResponseJSON::~AvailableChannelsResponseJSON()
{

}


void AvailableChannelsResponseJSON::setChannels(QSharedPointer<Channels> channels)
{
  if (!channels.isNull()) m_channelsContainer=channels;
}


bool AvailableChannelsResponseJSON::parseJson(const QByteArray &data)
{
  clearContainers();
  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(data, &ok).toMap();
  if (!ok)
  {
    return false ;
  }
  m_errno = result["errno"].toInt();
  QVariantList channels = result["channels"].toList();
  int size = channels.size();
  for (int i = 0; i < size; i++)
  {
    QVariantMap channelVariant = channels.at(i).toMap();
    QString name = channelVariant["name"].toString();
    QString description = channelVariant["description"].toString();
    QString url = channelVariant["url"].toString();
    QSharedPointer<Channel> channel(new JsonChannel(name, description, url));
    m_channelsContainer->push_back(channel);
  }
  return true;
}


QByteArray AvailableChannelsResponseJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantList channelsList;
  for (int i=0; i<m_channelsContainer->size(); i++)
  {
    QSharedPointer<Channel> ch=m_channelsContainer->at(i);
    QVariantMap channelMap;
    channelMap.insert("name", ch->getName());
    channelMap.insert("description", ch->getDescription());
    channelMap.insert("url", ch->getUrl());
    QVariantList tagsList;
    channelMap.insert("tags", tagsList);
    channelsList.append(channelMap);
  }

  QVariantMap channelsMap;
  if (!m_channelsContainer->size() == 0)
  {
    channelsMap.insert("channels", channelsList);
  }
  channelsMap.insert("errno", m_errno);
  return serializer.serialize(channelsMap);
}


/* ===[ End of file $HeadURL$ ]=== */
