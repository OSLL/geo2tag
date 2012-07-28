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
 * \file LoginQuery.cpp
 * \brief LoginQuery implementation
 *
 * File description
 *
 * PROJ: OSLL/fedexday
 * ---------------------------------------------------------------- */

#include <QDebug>

#include "LoginQuery.h"
#include "common/inc/defines.h"

#include "json/inc/JsonUser.h"
#include "json/inc/LoginRequestJSON.h"
#include "json/inc/LoginResponseJSON.h"

#ifndef Q_OS_SYMBIAN
#include <syslog.h>
#else
#include "symbian.h"
#endif

LoginQuery::LoginQuery(const QString &login, const QString &password, QObject *parent):
DefaultQuery(parent), m_login(login), m_password(password)
{
}


LoginQuery::LoginQuery(QObject *parent):
DefaultQuery(parent)
{
}


QString LoginQuery::getUrl() const
{
  return LOGIN_HTTP_URL;
}


QByteArray LoginQuery::getRequestBody() const
{
  QSharedPointer<common::User> dummyUser(new JsonUser(m_login,m_password));
  LoginRequestJSON request;
  request.addUser(dummyUser);
  return request.getJson();
}


void LoginQuery::processReply(QNetworkReply *reply)
{
  //#ifndef Q_OS_SYMBIAN
  LoginResponseJSON response;
  response.parseJson(reply->readAll());
  m_errno = response.getErrno();
  if(response.getErrno() == SUCCESS)
  {
    m_session = response.getSessions()->at(0);
    syslog(LOG_INFO,"!!connected!");
    Q_EMIT connected();
  }
  else
  {
    qDebug("!!errorOccured!");
    Q_EMIT errorOccured(response.getErrno());
  }
  //  #endif
}


void LoginQuery::setQuery(const QString& login, const QString& password)
{
  m_login=login;
  m_password=password;
}


QSharedPointer<Session> LoginQuery::getSession() const
{
  return m_session;
}


LoginQuery::~LoginQuery()
{

}


/* ===[ End of file $HeadURL$ ]=== */
