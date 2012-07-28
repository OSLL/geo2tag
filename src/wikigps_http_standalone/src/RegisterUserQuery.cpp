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

#include <QDebug>

#include "RegisterUserQuery.h"
#include "common/inc/defines.h"
#include "ErrnoTypes.h"
#include "json/inc/JsonUser.h"
#include "json/inc/RegisterUserRequestJSON.h"
#include "json/inc/RegisterUserResponseJSON.h"

#ifndef Q_OS_SYMBIAN
#include <syslog.h>
#else
#include "symbian.h"
#endif

RegisterUserQuery::RegisterUserQuery(const QString &email, const QString &login, const QString &password, QObject *parent)
: DefaultQuery(parent),
m_email(email),
m_login(login),
m_password(password)
{
}


RegisterUserQuery::RegisterUserQuery(QObject *parent)
: DefaultQuery(parent)
{
}


QString RegisterUserQuery::getUrl() const
{
  //const QString str = "http://localhost:80/service/registerUser";
  //return str;
  return REGISTER_USER_HTTP_URL;
}


QByteArray RegisterUserQuery::getRequestBody() const
{
  QSharedPointer<common::User> dummyUser(new JsonUser(m_login, m_password, m_email));
  RegisterUserRequestJSON request;
  request.addUser(dummyUser);
  return request.getJson();
}


const QString& RegisterUserQuery::getConfirmUrl() const
{
  return m_confirmUrl;
}


void RegisterUserQuery::processReply(QNetworkReply *reply)
{
  //#ifndef Q_OS_SYMBIAN
  RegisterUserResponseJSON response;
  response.parseJson(reply->readAll());
  if (response.getErrno() == SUCCESS)
  {
    m_confirmUrl = response.getConfirmUrl();
    syslog(LOG_INFO,"!!connected!");
    qDebug("!!connected!");
    Q_EMIT connected();
  }
  else
  {
    qDebug("!!errorOccured!");
    Q_EMIT errorOccured(response.getErrno());
  }
  //#endif
}


void RegisterUserQuery::setQuery(const QString &email, const QString &login, const QString &password)
{
  m_email = email;
  m_login = login;
  m_password = password;
}


RegisterUserQuery::~RegisterUserQuery()
{
}
