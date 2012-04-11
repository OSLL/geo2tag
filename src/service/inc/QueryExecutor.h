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
#include <QSqlDatabase>

#include "User.h"
#include "Channel.h"
#include "DataMarks.h"

class QueryExecutor : public QObject
{
  Q_OBJECT
    QSqlDatabase m_database;

  qlonglong nextKey(const QString& sequence) const;

  public:

    QueryExecutor(const QSqlDatabase &db, QObject *parent = 0);

    bool isConnected();
    bool connect();

    qlonglong nextUserKey() const;
    qlonglong nextSessionKey() const;
    qlonglong nextChannelKey() const;
    qlonglong nextTagKey() const;
    qlonglong nextTimeSlotKey() const;

    const QString generateNewToken(const QString& login,const QString& password) const;
    const QString generateNewToken(const QString& email, const QString& login,const QString& password) const;
    const QString generateNewToken(const QString& email, const QString& login,const QString& password,const QString& token) const;

    bool                     subscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel);
    bool                     unsubscribeChannel(const QSharedPointer<common::User>& user,const QSharedPointer<Channel>& channel);
    QSharedPointer<common::User> doesTmpUserExist(const QSharedPointer<common::User> &user);
    bool                     doesUserWithGivenEmailExist(const QSharedPointer<common::User> &user);
    bool                     deleteTmpUser(const QSharedPointer<common::User> &user);
    QSharedPointer<common::User>  insertNewTmpUser(const QSharedPointer<common::User> &user);
    bool                     doesRegistrationTokenExist(const QString &token);
    bool                     insertTmpUserIntoUsers(const QString &token);
    bool                     deleteTmpUser(const QString &token);
    bool                     insertNewSession(const QSharedPointer<common::User>& user);
    QSharedPointer<common::User>  doesSessionExist(const QSharedPointer<common::User>& user);
    bool                     updateSessionForUser(const QSharedPointer<common::User>& user);
    QSharedPointer<DataMark> insertNewTag(const QSharedPointer<DataMark>&);
    QSharedPointer<common::User>    insertNewUser(const QSharedPointer<common::User>&);
    QSharedPointer<Channel>  insertNewChannel(const QSharedPointer<Channel>&);
    QSharedPointer<TimeSlot> insertNewTimeSlot(const QSharedPointer<TimeSlot>&);
    bool                     insertNewChannelTimeSlot(const QSharedPointer<Channel>&, const QSharedPointer<TimeSlot>&);
    bool                     changeChannelTimeSlot(const QSharedPointer<Channel>&, const QSharedPointer<TimeSlot>&);
    bool                     insertNewMarkTimeSlot(const QSharedPointer<DataMark>&, const QSharedPointer<TimeSlot>&);
    bool                     changeMarkTimeSlot(const QSharedPointer<DataMark>&, const QSharedPointer<TimeSlot>&);
    bool                     deleteChannelTimeSlot(const QSharedPointer<Channel>&);
    bool                     deleteMarkTimeSlot(const QSharedPointer<DataMark>&);
    bool                     isChannelSubscribed(QSharedPointer<Channel>&, QSharedPointer<common::User>&);


    signals:

  public slots:

};
// QUERYEXECUTOR_H
#endif
