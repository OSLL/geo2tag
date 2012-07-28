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

#include "AddUserQuery.h"
#include "common/inc/defines.h"

#include "json/inc/JsonUser.h"
#include "json/inc/AddUserRequestJSON.h"
#include "json/inc/AddUserResponseJSON.h"

#ifndef Q_OS_SYMBIAN
#include <syslog.h>
#else
#include "symbian.h"
#endif

AddUserQuery::AddUserQuery(const QString &login, const QString &password, QObject *parent, const QString& email)
: DefaultQuery(parent),
m_login(login),
m_password(password),
m_email(email)
{
}


AddUserQuery::AddUserQuery(QObject *parent):
DefaultQuery(parent)
{
}


QString AddUserQuery::getUrl() const
{
  return ADD_USER_HTTP_URL;
}


QByteArray AddUserQuery::getRequestBody() const
{
  QSharedPointer<common::User> dummyUser(new JsonUser(m_login, m_password, m_email));
  AddUserRequestJSON request;
  request.addUser(dummyUser);
  return request.getJson();
}


void AddUserQuery::processReply(QNetworkReply *reply)
{
  #ifndef Q_OS_SYMBIAN
  AddUserResponseJSON response;
  response.parseJson(reply->readAll());
  m_errno = response.getErrno();
  if(response.getErrno() == SUCCESS)
  {
    m_session = response.getSessions()->at(0);
    m_user = QSharedPointer<common::User>(new JsonUser(m_login, m_password, m_email));

    syslog(LOG_INFO,"!!connected!");
    Q_EMIT connected();
  }
  else
  {
    Q_EMIT errorOccured(response.getErrno());
  }
  #endif
}


void AddUserQuery::setQuery(const QString& login, const QString& password)
{
  m_login=login;
  m_password=password;
}


void AddUserQuery::setQuery(const QString& login, const QString& password, const QString& email)
{
  m_login = login;
  m_password = password;
  m_email = email;
}


QSharedPointer<common::User> AddUserQuery::getUser() const
{
  return m_user;
}


QSharedPointer<Session> AddUserQuery::getSession() const
{
  return m_session;
}


AddUserQuery::~AddUserQuery()
{

}
