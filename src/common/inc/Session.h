/*
 * Copyright 2012  OSLL osll@osll.spb.ru
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
/*!
 * \file  Session.h
 * \brief Declaration of the class Session
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef SESSION_H
#define SESSION_H

#include <QSharedPointer>
#include <QString>
#include <QDateTime>

#include "User.h"

class Session : public QObject
{
  private:
    QString      m_sessionToken;
    QDateTime    m_lastAccessTime;
    QSharedPointer<common::User> m_user;

  public:
    Session(const QString& sessionToken, const QDateTime& lastAccessTime, const QSharedPointer<common::User>& user);

    void setSessionToken(const QString& sessionToken);
    void setLastAccessTime(const QDateTime lastAccessTime);
    void setUser(const QSharedPointer<common::User>& user);

    const QString& getSessionToken() const;
    const QDateTime& getLastAccessTime() const;
    const QSharedPointer<common::User>& getUser() const;

    virtual qlonglong getId() const;

    ~Session();
};

typedef ConcurrentVector<Session> Sessions;
#endif                                  // SESSION_H
