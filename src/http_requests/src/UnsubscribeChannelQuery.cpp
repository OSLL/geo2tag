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
 * \file UnsubscribeChannelQuery.cpp
 * \brief UnsubscribeChannelQuery implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#include "defines.h"
#include "UnsubscribeChannelQuery.h"
#include "SubscribeChannelJSON.h"
#include <QDebug>

namespace GUI
{

  UnsubscribeChannelQuery::UnsubscribeChannelQuery(QObject *parent)
    : DefaultQuery(parent)
  {
  }

  UnsubscribeChannelQuery::UnsubscribeChannelQuery(QSharedPointer<User> user, QSharedPointer<Channel> channel, QObject *parent)
    : DefaultQuery(parent),m_user(user),m_channel(channel)
  {
  }

  void UnsubscribeChannelQuery::setQuery(QSharedPointer<User> user, QSharedPointer<Channel>  channel)
  {
    m_user=user;
    m_channel=channel;
  }

  UnsubscribeChannelQuery::~UnsubscribeChannelQuery()
  {
  }

  QString UnsubscribeChannelQuery::getUrl() const
  {
    return UNSUBSCRIBE_HTTP_URL;
  }

  QByteArray UnsubscribeChannelQuery::getRequestBody() const
  {
    SubscribeChannelRequestJSON request(m_channel,m_user);
    return request.getJson();
  }

  const QString& UnsubscribeChannelQuery::getStatus() const
  {
    return m_status;
  }

  void UnsubscribeChannelQuery::processReply(QNetworkReply *reply)
  {
    DefaultResponseJSON response;
    response.parseJson(reply->readAll());
    if(response.getStatus() == "Ok")
    {
      m_status="Ok";
      Q_EMIT responseReceived();
    }
    else
    {
      Q_EMIT errorOccured(response.getStatusMessage());
    }
  }

  UnsubscribeChannelQuery::~UnsubscribeChannelQuery()
  {
  }

}                                       // namespace GUI


/* ===[ End of file $HeadURL$ ]=== */
