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
/*!
 * \file Test_QueryExecutor.h
 * \brief Test suite for QueryExecutor class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#ifndef TEST_QUERYEXECUTOR_H
#define TEST_QUERYEXECUTOR_H

#include <QObject>
#include <QtTest>
#include <QtSql>
#include "../inc/QueryExecutor.h"
#include "../inc/Geo2tagDatabase.h"

namespace Test
{
  class Test_QueryExecutor : public QObject
  {
    Q_OBJECT

      QSharedPointer<QueryExecutor> m_queryExecutor;
    Geo2tagDatabase m_database;

    private:
      QSharedPointer<common::User> createTestUser(const QString& login,
        const QString& password,
        const QString& email);
      QSharedPointer<common::User> createTestTmpUser(const QString& login,
        const QString& password,
        const QString& email);
      QSharedPointer<Session> createTestSession(const QString& token,
        const QDateTime& time,
        const QSharedPointer<common::User>& user);
      QSharedPointer<Channel> createTestChannel(const QString& name, const QString& description, const QString& url,
        const QSharedPointer<common::User>& owner);
      QSharedPointer<DataMark> createTestTag(double altitude, double latitude, double longitude,
        const QString& label, const QString& description, const QString& url, const QDateTime& time,
        const QSharedPointer<common::User>& user,
        const QSharedPointer<Channel>& channel);
      bool deleteTestUser(const QSharedPointer<common::User>& user);
      bool deleteTestTmpUser(const QSharedPointer<common::User>& user);
      bool deleteTestSession(const QSharedPointer<Session>& session);
      bool deleteTestChannel(const QSharedPointer<Channel>& channel);
      bool deleteTestTag(const QSharedPointer<DataMark>& tag);

    private slots:
      void subscribeChannel();
      void unsubscribeChannel();
      void doesTmpUserExist();
      void doesUserWithGivenEmailExist();
      void deleteTmpUser();
      void insertNewTmpUser();
      void doesRegistrationTokenExist();
      void insertTmpUserIntoUsers();
      void deleteTmpUserWithToken();
      void insertNewTag();
      void insertNewUser();
      void insertNewChannel();
      void deleteUser();
      void insertNewSession();
      void updateSession();
      void deleteSession();

      void checkTmpUsers();

      void loadUsers();
      void loadTags();
      void loadChannels();
      void loadSessions();

    public:
      Test_QueryExecutor();

  };                                    // class Test_QueryExecutor
}                                       // end of namespace Test
#endif                                  // TEST_QUERYEXECUTOR_H
