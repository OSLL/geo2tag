/*
 * Copyright 2010  Open Source & Linux Lab (OSLL)  osll@osll.spb.ru
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

/*! ---------------------------------------------------------------
 * $Id$
 *
 * \file SubscribedChannelsQuery.cpp
 * \brief SubscribedChannelsQuery implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */
#include "SubscribedChannelsQuery.h"
#include "common/inc/defines.h"
#include <QDebug>
#include "json/inc/SubscribedChannelsResponseJSON.h"
#include "json/inc/SubscribedChannelsRequestJSON.h"
#include "json/inc/JsonDataMark.h"
#include "json/inc/JsonUser.h"

SubscribedChannelsQuery::SubscribedChannelsQuery(const QSharedPointer<Session>& session, QObject *parent)
: DefaultQuery(parent),
m_session(session)
{
}


SubscribedChannelsQuery::SubscribedChannelsQuery(QObject *parent): DefaultQuery(parent)
{
}


void SubscribedChannelsQuery::setQuery(const QSharedPointer<Session>& session)
{
  m_session = session;
}


QString SubscribedChannelsQuery::getUrl() const
{
  return SUBSCRIBED_LIST_HTTP_URL;
}


QByteArray SubscribedChannelsQuery::getRequestBody() const
{
  SubscribedChannelsRequestJSON request;
  request.addSession(m_session);
  return request.getJson();
}


void SubscribedChannelsQuery::processReply(QNetworkReply *reply)
{
  SubscribedChannelsResponseJSON response;
  response.parseJson(reply->readAll());
  if(response.getErrno() == SUCCESS)
  {
    m_channels = response.getChannels();

    Q_EMIT responseReceived();
  }
  else
  {
    Q_EMIT errorOccured(response.getErrno());
  }
}


const QSharedPointer<Channels>& SubscribedChannelsQuery::getChannels() const
{
  return m_channels;
}


SubscribedChannelsQuery::~SubscribedChannelsQuery()
{

}
