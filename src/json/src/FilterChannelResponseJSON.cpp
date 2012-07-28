/*
 * Copyright 2012  bac1ca  bac1ca89@gmail.com
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
 * \file FilterChannelResponseJSON.cpp
 * \brief FilterChannelResponseJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "FilterChannelResponseJSON.h"

#include "JsonUser.h"

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include <qjson/parser.h>
#include <qjson/serializer.h>
#else
#include "parser.h"
#include "serializer.h"
#endif

FilterChannelResponseJSON::FilterChannelResponseJSON(QObject *parent) : JsonSerializer(parent)
{
}


QByteArray FilterChannelResponseJSON::getJson() const
{
  QJson::Serializer serializer;
  QVariantMap obj;

  QVariantList jtags;
  QVariantMap jchannel;
  QVariantMap channel;

  for(int i = 0; i < m_tags.size(); i++)
  {
    QSharedPointer<DataMark> tag = m_tags.at(i);
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
  channel["name"] = m_channel.isNull() ? "" : m_channel->getName();

  obj["channel"] = channel;
  obj["errno"] = getErrno();
  return serializer.serialize(obj);
}


bool FilterChannelResponseJSON::parseJson(const QByteArray&)
{
  // TODO TBD
  return false;
}


void FilterChannelResponseJSON::setData(QSharedPointer<Channel> channel,
QList<QSharedPointer<DataMark> > tags)
{
  m_channel = channel;
  m_tags = tags;
}
