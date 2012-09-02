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

#ifndef QUERYEXECUTOR_H
#define QUERYEXECUTOR_H

#include <QObject>

#include "User.h"
#include "Channel.h"
#include "DataMarks.h"
#include "Session.h"
#include "Geo2tagDatabase.h"

class QueryExecutor : public QObject
{
  Q_OBJECT

    Geo2tagDatabase m_database;

  qlonglong nextKey(const QString& sequence) const;

  public:

    QueryExecutor(const Geo2tagDatabase& db, QObject* parent = 0);

    bool isConnected();
    bool connect();
    void disconnect();
    QSqlError lastError();

    qlonglong nextUserKey() const;
    qlonglong nextChannelKey() const;
    qlonglong nextTagKey() const;
    qlonglong nextSessionKey() const;

    const QString generateNewToken(const QString& email, const QString& login,const QString& password) const;
    const QString generateNewToken(const QString& accessTime, const QString& email, const QString& login,const QString& password) const;

    bool                     subscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel);
    bool                     unsubscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel);
    bool                 doesTmpUserExist(const QSharedPointer<common::User> &user);
    bool                     doesUserWithGivenEmailExist(const QSharedPointer<common::User> &user);
    bool                     deleteTmpUser(const QSharedPointer<common::User> &user);
    const QString  insertNewTmpUser(const QSharedPointer<common::User> &user);
    bool                     doesRegistrationTokenExist(const QString &token);
    QSharedPointer<common::User> insertTmpUserIntoUsers(const QString &token);
    bool                     deleteTmpUser(const QString &token);
    QSharedPointer<DataMark> insertNewTag(const QSharedPointer<DataMark>&);
    QSharedPointer<common::User>    insertNewUser(const QSharedPointer<common::User>&);
    QSharedPointer<Channel>  insertNewChannel(const QSharedPointer<Channel>&);
    bool                     deleteUser(const QSharedPointer<common::User> &user);
    QSharedPointer<common::User>  updateUserPassword(const QSharedPointer<common::User>& user, const QString& password);
    // Sessions
    QSharedPointer<Session>  insertNewSession(const QSharedPointer<Session>& session);
    bool                     updateSession(const QSharedPointer<Session>& session);
    bool                     deleteSession(const QSharedPointer<Session>& session);

    void checkTmpUsers();
    void checkSessions(UpdateThread* thread);

    void loadUsers(common::Users &);
    void loadTags(DataMarks &);
    void loadChannels(Channels &);
    void loadSessions(Sessions &);
    void updateReflections(DataMarks&, common::Users&, Channels&, Sessions&);

    qlonglong getFactTransactionNumber();

    signals:

  public slots:

};
#endif                                  // QUERYEXECUTOR_H
