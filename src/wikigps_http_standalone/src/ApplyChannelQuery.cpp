/*
 * Copyright 2010-2012  OSLL osll@osll.spb.ru
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

#include "ApplyChannelQuery.h"
#include "common/inc/defines.h"
#include "json/inc/AddChannelRequestJSON.h"
#include "json/inc/AddChannelResponseJSON.h"

ApplyChannelQuery::ApplyChannelQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<Session> &session, QObject *parent)
: DefaultQuery(parent),
m_channel(channel),
m_session(session)
{
}


ApplyChannelQuery::ApplyChannelQuery(QObject *parent): DefaultQuery(parent)
{
}


void ApplyChannelQuery::setQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<Session> &session)
{
  m_channel = channel;
  m_session = session;
}


QString ApplyChannelQuery::getUrl() const
{
  return APPLY_CHANNEL_HTTP_URL;
}


QByteArray ApplyChannelQuery::getRequestBody() const
{
  AddChannelRequestJSON request;
  request.addChannel(m_channel);
  request.addSession(m_session);
  return request.getJson();
}


void ApplyChannelQuery::processReply(QNetworkReply *reply)
{
  AddChannelResponseJSON response;
  response.parseJson(reply->readAll());
  m_errno = response.getErrno();
  if(response.getErrno() == SUCCESS)
  {
    Q_EMIT channelAdded(m_channel);
  }
  else
  {
    Q_EMIT errorOccured(response.getErrno());
  }
}


ApplyChannelQuery::~ApplyChannelQuery()
{

}


const QSharedPointer<Channel>& ApplyChannelQuery::getChannel() const
{
  return m_channel;
}
