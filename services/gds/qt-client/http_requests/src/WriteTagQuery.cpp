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

#include "WriteTagQuery.h"
#include "defines.h"
#include "WriteTagRequestJSON.h"
#include "WriteTagResponseJSON.h"

WriteTagQuery::WriteTagQuery(const QSharedPointer<DataMark> &tag, QObject *parent): DefaultQuery(parent), m_tag(tag)
{

}


WriteTagQuery::WriteTagQuery(QObject *parent): DefaultQuery(parent)
{
}


void WriteTagQuery::setTag(const QSharedPointer<DataMark> &tag)
{
  m_tag = tag;
}


QString WriteTagQuery::getUrl() const
{
  return APPLY_HTTP_URL;
}


void WriteTagQuery::setUrl(const QString &url)
{
  DefaultQuery::setUrl(url + APPLY_HTTP_URL);
}


QByteArray WriteTagQuery::getRequestBody() const
{
  WriteTagRequestJSON request;
  request.addTag(m_tag);
  return request.getJson();
}


void WriteTagQuery::processReply(QNetworkReply *reply)
{
  WriteTagResponseJSON response;
  response.parseJson(reply->readAll());
  if(response.getErrno() == SUCCESS)
  {
    Q_EMIT tagAdded();
  }
  else
  {
    Q_EMIT errorOccured(getErrnoDescription(response.getErrno()));
  }
}


WriteTagQuery::~WriteTagQuery()
{

}


QSharedPointer<DataMark> WriteTagQuery::getTag()
{
  return m_tag;
}


const QSharedPointer<DataMark>& WriteTagQuery::getTag() const
{
  return m_tag;
}
