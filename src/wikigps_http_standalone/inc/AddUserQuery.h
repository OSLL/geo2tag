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

#ifndef ADDUSERQUERY_H
#define ADDUSERQUERY_H
#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "common/inc/Session.h"

class AddUserQuery: public DefaultQuery
{
  Q_OBJECT

    QString m_login;
  QString m_password;
  QString m_email;

  //!< full information about user
  QSharedPointer<common::User> m_user;
  QSharedPointer<Session> m_session;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;

  private Q_SLOTS:

    virtual void processReply(QNetworkReply *reply);

  public:

    AddUserQuery(const QString& login, const QString& password, QObject *parent = 0, const QString& email = "unknown");

    AddUserQuery(QObject *parent = 0);

    void setQuery(const QString& login, const QString& password);
    void setQuery(const QString& login, const QString& password, const QString& email);

    QSharedPointer<common::User> getUser() const;
    QSharedPointer<Session> getSession() const;

    ~AddUserQuery();

    Q_SIGNALS:

    void connected();

};
#endif                                  // ADDUSERQUERY_H
