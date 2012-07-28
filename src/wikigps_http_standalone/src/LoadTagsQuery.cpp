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
 * \file LoadTagsQuery.cpp
 * \brief LoadTagsQuery implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#include "LoadTagsQuery.h"
#include "common/inc/defines.h"
#include <QDebug>
#include "json/inc/LoadTagsResponseJSON.h"
#include "json/inc/LoadTagsRequestJSON.h"
#include "json/inc/JsonDataMark.h"
#include "json/inc/JsonUser.h"
#include "json/inc/JsonSession.h"

LoadTagsQuery::LoadTagsQuery(QSharedPointer<Session> &session,
double latitude,
double longitude,
double radius,
QObject *parent): DefaultQuery(parent),
m_session(session),
m_latitude(latitude),
m_longitude(longitude),
m_radius(radius)
{
}


LoadTagsQuery::LoadTagsQuery(QObject *parent): DefaultQuery(parent)
{
}


void LoadTagsQuery::setQuery(QSharedPointer<Session> &session,
double latitude,
double longitude,
double radius)
{
  m_session=session;
  m_latitude=latitude;
  m_longitude=longitude;
  m_radius=radius;
}


QString LoadTagsQuery::getUrl() const
{
  return FEED_HTTP_URL;
}


QByteArray LoadTagsQuery::getRequestBody() const
{
  LoadTagsRequestJSON request(m_session, m_latitude, m_longitude, m_radius);
  return request.getJson();
}


void LoadTagsQuery::processReply(QNetworkReply *reply)
{
  LoadTagsResponseJSON response;
  response.parseJson(reply->readAll());
  if(response.getErrno() == SUCCESS)
  {
    m_hashMap = response.getData();
    syslog(LOG_INFO,"!!connected!");
    Q_EMIT tagsReceived();
  }
  else
  {
    qDebug("!!errorOccured!");
    Q_EMIT errorOccured(response.getErrno());
  }
}


const DataChannels& LoadTagsQuery::getData() const
{
  return m_hashMap;
}


LoadTagsQuery::~LoadTagsQuery()
{

}


/* ===[ End of file $HeadURL$ ]=== */
