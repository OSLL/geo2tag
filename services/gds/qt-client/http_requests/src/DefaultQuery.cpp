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

#include <QDebug>
#include <QNetworkConfiguration>
#include "DefaultQuery.h"
#include "defines.h"

DefaultQuery::DefaultQuery(QObject *parent): QObject(parent),
m_manager(new QNetworkAccessManager(parent)),
m_url(DEFAULT_URL)
{
  connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(process(QNetworkReply*)));
  connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
    this, SLOT(handleError()));
}


void DefaultQuery::setUrl(const QString &url)
{
  m_url = url;
}


void DefaultQuery::doRequest()
{
  QNetworkRequest request;

  QUrl url(m_url);
  request.setUrl(url);
  qDebug() << "doing post to" << url << " with body: " << getRequestBody();
  QNetworkReply *reply = m_manager->post(request, getRequestBody());
  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError()));
}


void DefaultQuery::process(QNetworkReply *reply)
{
  processReply(reply);
}


void DefaultQuery::handleError()
{
  qDebug() << "Network error occured while sending request";
  Q_EMIT networkErrorOccured("network error");
}
