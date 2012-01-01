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

#include "AddNewMarkQuery.h"
#include "defines.h"
#include "AddNewMarkRequestJSON.h"
#include "AddNewMarkResponseJSON.h"

AddNewMarkQuery::AddNewMarkQuery(const QSharedPointer<DataMark> &tag, QObject *parent): DefaultQuery(parent), m_tag(tag)
{

}


AddNewMarkQuery::AddNewMarkQuery(QObject *parent): DefaultQuery(parent)
{
}


void AddNewMarkQuery::setTag(const QSharedPointer<DataMark> &tag)
{
  m_tag = tag;
}


QString AddNewMarkQuery::getUrl() const
{
  return APPLY_HTTP_URL;
}


QByteArray AddNewMarkQuery::getRequestBody() const
{
  AddNewMarkRequestJSON request;
  request.addTag(m_tag);
  return request.getJson();
}


void AddNewMarkQuery::processReply(QNetworkReply *reply)
{
  AddNewMarkResponseJSON response;
  response.parseJson(reply->readAll());
  if(response.getStatus() == "Ok")
  {
    Q_EMIT tagAdded();
  }
  else
  {
    Q_EMIT errorOccured(response.getStatusMessage());
  }
}


AddNewMarkQuery::~AddNewMarkQuery()
{

}


QSharedPointer<DataMark> AddNewMarkQuery::getTag()
{
  return m_tag;
}


const QSharedPointer<DataMark>& AddNewMarkQuery::getTag() const
{
  return m_tag;
}
