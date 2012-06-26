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
 * \file Test_QueryExecutor.cpp
 * \brief Test suite for QueryExecutor class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include "Test_QueryExecutor.h"

namespace Test
{
    Test_QueryExecutor::Test_QueryExecutor()
        : QObject(),
          m_database(QSqlDatabase())
    {
        QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL");
        database.setHostName("localhost");
        database.setDatabaseName("test_db");
        database.setUserName("test_user");
        //database.setPassword("geo2tag");
        m_database = Geo2tagDatabase(database);
        m_queryExecutor = QSharedPointer<QueryExecutor>(
                    new QueryExecutor(Geo2tagDatabase(QSqlDatabase::cloneDatabase(database, "QueryExecutor"))));
        m_database.open();
        m_queryExecutor->connect();
    }

    QSharedPointer<common::User> Test_QueryExecutor::createTestUser(const QString &login,
                                                                    const QString &password,
                                                                    const QString &email)
    {
        DbUser* user = new DbUser(login, password, email, 0);
        qlonglong userId = 0;

        QSqlQuery query(m_database);
        query.prepare("insert into users (email, login, password) values (:email, :login, :password);");
        query.bindValue(":email", user->getEmail());
        query.bindValue(":login", user->getLogin());
        query.bindValue(":password", user->getPassword());
        m_database.transaction();
        bool result = query.exec();
        if (!result) {
            m_database.rollback();
            delete(user);
            return QSharedPointer<common::User>(0);
        } else {
            m_database.commit();
        }

        query.prepare("select id from users where login = :login;");
        query.bindValue(":login", user->getLogin());
        result = query.exec();
        query.next();
        userId = query.value(0).toLongLong();

        user->setId(userId);
        return QSharedPointer<common::User>(user);
    }

    bool Test_QueryExecutor::deleteTestUser(const QSharedPointer<common::User> &user)
    {
        QSqlQuery query(m_database);
        query.prepare("delete from users where id = :id");
        query.bindValue(":id", user->getId());
        m_database.transaction();
        bool result = query.exec();
        if (!result) {
            m_database.rollback();
            return false;
        } else {
            m_database.commit();
            return true;
        }
    }

    QSharedPointer<Session> Test_QueryExecutor::createTestSession(const QString &token,
                                                                  const QDateTime &time,
                                                                  const QSharedPointer<common::User> &user)
    {
        DbSession* session = new DbSession(0, token, time, user);
        qlonglong sessionId = 0;

        QSqlQuery query(m_database);
        query.prepare("insert into sessions (user_id, session_token, last_access_time) values (:id, :token, :time);");
        query.bindValue(":id", session->getUser()->getId());
        query.bindValue(":token", session->getSessionToken());
        query.bindValue(":time", session->getLastAccessTime().toUTC());
        m_database.transaction();
        bool result = query.exec();
        if (!result) {
            m_database.rollback();
            delete(session);
            return QSharedPointer<Session>(0);
        } else {
            m_database.commit();
        }

        query.prepare("select id from sessions where session_token = :token;");
        query.bindValue(":token", session->getSessionToken());
        result = query.exec();
        query.next();
        sessionId = query.value(0).toLongLong();

        session->setId(sessionId);
        return QSharedPointer<Session>(session);
    }

    QSharedPointer<common::User> Test_QueryExecutor::createTestTmpUser(const QString &login,
                                                                       const QString &password,
                                                                       const QString &email)
    {
        DbUser* tmpUser = new DbUser(login, password, email, 0);
        qlonglong userId = 0;

        QSqlQuery query(m_database);
        query.prepare("insert into signups (email,login,password,registration_token,sent) values(:email,:login,:password,:r_token,:sent);");
        query.bindValue(":email", tmpUser->getEmail());
        query.bindValue(":login", tmpUser->getLogin());
        query.bindValue(":password", tmpUser->getPassword());
        query.bindValue(":r_token", "TESTTESTTEST");
        query.bindValue(":sent", FALSE);
        m_database.transaction();
        bool result = query.exec();
        if (!result) {
            m_database.rollback();
            delete(tmpUser);
            return QSharedPointer<common::User>(0);
        } else {
            m_database.commit();
        }

        query.prepare("select id from signups where login = :login;");
        query.bindValue(":login", tmpUser->getLogin());
        result = query.exec();
        query.next();
        userId = query.value(0).toLongLong();

        tmpUser->setId(userId);
        return QSharedPointer<common::User>(tmpUser);
    }

    bool Test_QueryExecutor::deleteTestTmpUser(const QSharedPointer<common::User>& user)
    {
        QSqlQuery query(m_database);
        query.prepare("delete from signups where login = :login and email = :email");
        query.bindValue(":login", user->getLogin());
        query.bindValue(":email", user->getEmail());
        m_database.transaction();
        bool result = query.exec();
        if (!result) {
            m_database.rollback();
            return false;
        } else {
            m_database.commit();
            return true;
        }
    }

    bool Test_QueryExecutor::deleteTestSession(const QSharedPointer<Session> &session)
    {
        QSqlQuery query(m_database);
        query.prepare("delete from sessions where session_token = :token");
        query.bindValue(":token", session->getSessionToken());
        m_database.transaction();
        bool result = query.exec();
        if (!result) {
            m_database.rollback();
            return false;
        } else {
            m_database.commit();
            return true;
        }
    }

    QSharedPointer<Channel> Test_QueryExecutor::createTestChannel(const QString &name, const QString &description, const QString &url)
    {
        DbChannel* channel = new DbChannel(0, name, description, url);
        qlonglong channelId = 0;

        QSqlQuery query(m_database);
        query.prepare("insert into channel (name, description, url) values(:name,:description,:url);");
        query.bindValue(":name", channel->getName());
        query.bindValue(":description", channel->getDescription());
        query.bindValue(":url", channel->getUrl());
        m_database.transaction();
        bool result = query.exec();
        if (!result) {
            m_database.rollback();
            delete(channel);
            return QSharedPointer<Channel>(0);
        } else {
            m_database.commit();
        }

        query.prepare("select id from channel where name = :name;");
        query.bindValue(":name", channel->getName());
        result = query.exec();
        query.next();
        channelId = query.value(0).toLongLong();

        channel->setId(channelId);
        return QSharedPointer<Channel>(channel);

    }

    bool Test_QueryExecutor::deleteTestChannel(const QSharedPointer<Channel> &channel)
    {
        QSqlQuery query(m_database);
        query.prepare("delete from channel where id = :id");
        query.bindValue(":id", channel->getId());
        m_database.transaction();
        bool result = query.exec();
        if (!result) {
            m_database.rollback();
            return false;
        } else {
            m_database.commit();
            return true;
        }
    }

    // SLOTS

    void Test_QueryExecutor::subscribeChannel()
    {
        QSharedPointer<common::User> user = createTestUser("user9", "test", "email9@test.org");
        QVERIFY(user != QSharedPointer<common::User>(0));
        QSharedPointer<Channel> channel = createTestChannel("Test channel 9", "Test description 9", "");
        QVERIFY(channel != QSharedPointer<Channel>(0));

        bool result = m_queryExecutor->subscribeChannel(user, channel);
        QCOMPARE(result, true);

        result = m_queryExecutor->subscribeChannel(user, channel);
        QCOMPARE(result, false);

        QSqlQuery query(m_database);
        query.prepare("select * from subscribe where channel_id = :channel_id and user_id = :user_id;");
        query.bindValue(":channel_id", channel->getId());
        query.bindValue(":user_id", user->getId());
        result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);

        result = deleteTestChannel(channel);
        QCOMPARE(result, true);

        result = deleteTestUser(user);
        QCOMPARE(result, true);
    }

    void Test_QueryExecutor::unsubscribeChannel()
    {
        QSharedPointer<common::User> user = createTestUser("user10", "test", "email10@test.org");
        QVERIFY(user != QSharedPointer<common::User>(0));
        QSharedPointer<Channel> channel = createTestChannel("Test channel 10", "Test description 10", "");
        QVERIFY(channel != QSharedPointer<Channel>(0));

        QSqlQuery query(m_database);
        query.prepare("insert into subscribe (channel_id, user_id) values(:channel_id, :user_id);");
        query.bindValue(":channel_id", channel->getId());
        query.bindValue(":user_id", user->getId());
        m_database.transaction();
        bool result = query.exec();
        if (!result) {
            m_database.rollback();
        } else {
            m_database.commit();
        }
        QCOMPARE(result, true);

        result = m_queryExecutor->unsubscribeChannel(user, channel);
        QCOMPARE(result, true);

        query.prepare("select * from subscribe where channel_id = :channel_id and user_id = :user_id;");
        query.bindValue(":channel_id", channel->getId());
        query.bindValue(":user_id", user->getId());
        result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == false);

        result = deleteTestChannel(channel);
        QCOMPARE(result, true);

        result = deleteTestUser(user);
        QCOMPARE(result, true);
    }

    void Test_QueryExecutor::doesTmpUserExist()
    {
        QString login = "user11";
        QString passw = "test";
        QString email = "emai11@test.org";

        QSharedPointer<common::User> tmpUser = createTestTmpUser(login, passw, email);
        QVERIFY(tmpUser != QSharedPointer<common::User>(0));

        bool result = m_queryExecutor->doesTmpUserExist(tmpUser);
        QCOMPARE(result, true);

        result = deleteTestTmpUser(tmpUser);
        QCOMPARE(result, true);

        result = m_queryExecutor->doesTmpUserExist(tmpUser);
        QCOMPARE(result, false);
    }

    void Test_QueryExecutor::doesUserWithGivenEmailExist()
    {
        QString login = "user12";
        QString passw = "test";
        QString email = "emai12@test.org";

        QSharedPointer<common::User> user = createTestUser(login, passw, email);
        QVERIFY(user != QSharedPointer<common::User>(0));

        bool result = m_queryExecutor->doesUserWithGivenEmailExist(user);
        QCOMPARE(result, true);

        result = deleteTestUser(user);
        QCOMPARE(result, true);

        result = m_queryExecutor->doesUserWithGivenEmailExist(user);
        QCOMPARE(result, false);
    }

    void Test_QueryExecutor::deleteTmpUser()
    {
        QSharedPointer<common::User> tmpUser = createTestTmpUser("user5", "test", "email5@test.org");
        QVERIFY(tmpUser != QSharedPointer<common::User>(0));

        bool result = m_queryExecutor->deleteTmpUser(QSharedPointer<common::User>(tmpUser));
        QCOMPARE(result, true);

        QSqlQuery query(m_database);
        query.prepare("select id from signups where login = :login;");
        query.bindValue(":login", tmpUser->getLogin());
        result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == false);
    }

    void Test_QueryExecutor::insertNewTmpUser()
    {
        QString login = "user4";
        QString passw = "test";
        QString email = "email4@test.org";

        QSharedPointer<common::User> tmpUser(new common::User(login, passw, email));
        QString token = m_queryExecutor->insertNewTmpUser(tmpUser);
        QVERIFY(token != "");

        QString emptyToken = m_queryExecutor->insertNewTmpUser(tmpUser);
        QVERIFY(emptyToken == "");

        QSqlQuery query(m_database);
        query.prepare("select id, email, login, password, registration_token from signups where login = :login;");
        query.bindValue(":login", tmpUser->getLogin());
        bool result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);
        QVERIFY(query.value(1) == email);
        QVERIFY(query.value(2) == login);
        QVERIFY(query.value(3) == passw);
        QVERIFY(query.value(4) == token);

        result = deleteTestTmpUser(tmpUser);
        QCOMPARE(result, true);
    }

    void Test_QueryExecutor::doesRegistrationTokenExist()
    {
        QString login = "user13";
        QString passw = "test";
        QString email = "email13@test.org";

        QSharedPointer<common::User> tmpUser = createTestTmpUser(login, passw, email);
        QVERIFY(tmpUser != QSharedPointer<common::User>(0));

        QSqlQuery query(m_database);
        query.prepare("select registration_token from signups where login = :login;");
        query.bindValue(":login", login);
        bool result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);
        QString token = query.value(0).toString();

        result = m_queryExecutor->doesRegistrationTokenExist(token);
        QCOMPARE(result, true);

        result = deleteTestTmpUser(tmpUser);
        QCOMPARE(result, true);

        result = m_queryExecutor->doesRegistrationTokenExist(token);
        QCOMPARE(result, false);
    }

    void Test_QueryExecutor::insertTmpUserIntoUsers()
    {
        QString login = "user14";
        QString passw = "test";
        QString email = "email14@test.org";

        QSharedPointer<common::User> tmpUser = createTestTmpUser(login, passw, email);
        QVERIFY(tmpUser != QSharedPointer<common::User>(0));

        QSqlQuery query(m_database);
        query.prepare("select registration_token from signups where login = :login;");
        query.bindValue(":login", login);
        bool result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);
        QString token = query.value(0).toString();

        QSharedPointer<common::User> user = m_queryExecutor->insertTmpUserIntoUsers(token);
        QVERIFY(user != QSharedPointer<common::User>(0));
        QVERIFY(user->getLogin() == login);
        QVERIFY(user->getEmail() == email);
        QVERIFY(user->getPassword() == passw);

        result = deleteTestTmpUser(tmpUser);
        QCOMPARE(result, true);

        result = deleteTestUser(user);
        QCOMPARE(result, true);
    }

    void Test_QueryExecutor::deleteTmpUserWithToken()
    {
        QString login = "user15";
        QString passw = "test";
        QString email = "email15@test.org";

        QSharedPointer<common::User> tmpUser = createTestTmpUser(login, passw, email);
        QVERIFY(tmpUser != QSharedPointer<common::User>(0));

        QSqlQuery query(m_database);
        query.prepare("select registration_token from signups where login = :login;");
        query.bindValue(":login", login);
        bool result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);
        QString token = query.value(0).toString();

        result = m_queryExecutor->deleteTmpUser(token);
        QCOMPARE(result, true);

        query.prepare("select id from signups where login = :login;");
        query.bindValue(":login", tmpUser->getLogin());
        result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == false);

    }

    void Test_QueryExecutor::insertNewTag()
    {
        QString login = "user16";
        QString passw = "test";
        QString email = "email16@test.org";
        QString name = "Test channel 16";
        QString descr = "Test description 16";
        QString url = "";
        QString sessionToken = "Token16";
        QDateTime time = QDateTime::currentDateTime();
        double altitude = 30.0;
        double latitude = 60.0;
        double longitude = 90.0;
        QString label = "Test tag 16";
        QString tagDescr = "Tag's description 16";


        QSharedPointer<common::User> user = createTestUser(login, passw, email);
        QVERIFY(user != QSharedPointer<common::User>(0));
        QSharedPointer<Channel> channel = createTestChannel(name, descr, url);
        QVERIFY(channel != QSharedPointer<Channel>(0));
        QSharedPointer<Session> session = createTestSession(sessionToken, time, user);
        QVERIFY(session != QSharedPointer<Session>(0));
        QSharedPointer<DataMark> dummyTag(new DataMark(altitude, latitude, longitude, label, tagDescr, url, time));
        dummyTag->setUser(user);
        dummyTag->setChannel(channel);
        dummyTag->setSession(session);

        QSharedPointer<DataMark> tag = m_queryExecutor->insertNewTag(dummyTag);
        QVERIFY(tag != QSharedPointer<DataMark>(0));

        QSqlQuery query(m_database);
        query.prepare("select time, altitude, latitude, longitude, label, description, url, user_id, channel_id from tag where id = :id;");
        query.bindValue(":id", tag->getId());
        bool result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);
        QVERIFY(query.value(1).toDouble() == altitude);
        QVERIFY(query.value(2).toDouble() == latitude);
        QVERIFY(query.value(3).toDouble() == longitude);
        QVERIFY(query.value(4).toString() == label);
        QVERIFY(query.value(5).toString() == tagDescr);
        QVERIFY(query.value(6).toString() == url);
        QVERIFY(query.value(7).toLongLong() == user->getId());
        QVERIFY(query.value(8).toLongLong() == channel->getId());

        result = deleteTestUser(user);
        QCOMPARE(result, true);

        result = deleteTestChannel(channel);
        QCOMPARE(result, true);

        result = deleteTestSession(session);
        QCOMPARE(result, true);

        query.prepare("delete from tag where id = :id");
        query.bindValue(":id", tag->getId());
        m_database.transaction();
        result = query.exec();
        if (!result) {
            m_database.rollback();
        } else {
            m_database.commit();
        }
        QCOMPARE(result, true);
    }

    void Test_QueryExecutor::insertNewUser()
    {
        QString login = "user1";
        QString passw = "test";
        QString email = "email1@test.org";
        QSharedPointer<common::User> dummyUser(new common::User(login, passw, email));
        QSharedPointer<common::User> user = m_queryExecutor->insertNewUser(dummyUser);
        QCOMPARE(user.isNull(), false);
        QVERIFY(user->getEmail() == email);
        QVERIFY(user->getLogin() == login);
        QVERIFY(user->getPassword() == passw);

        QSharedPointer<common::User> emptyUser = m_queryExecutor->insertNewUser(dummyUser);
        QCOMPARE(emptyUser.isNull(), true);

        QSqlQuery query(m_database);
        query.prepare("select id, email, login, password from users where login = :login;");
        query.bindValue(":login", user->getLogin());
        bool result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);
        QVERIFY(query.value(1) == email);
        QVERIFY(query.value(2) == login);
        QVERIFY(query.value(3) == passw);

        result = deleteTestUser(user);
        QCOMPARE(result, true);
    }

    void Test_QueryExecutor::insertNewChannel()
    {
        QString name = "Test channel 1";
        QString descr = "Test description 1";
        QString url = "";
        QSharedPointer<Channel> dummyChannel(new Channel(name, descr, url));
        QSharedPointer<Channel> channel = m_queryExecutor->insertNewChannel(dummyChannel);
        QCOMPARE(channel.isNull(), false);
        QVERIFY(channel->getName() == name);
        QVERIFY(channel->getDescription() == descr);
        QVERIFY(channel->getUrl() == url);

        QSqlQuery query(m_database);
        query.prepare("select id, name, description, url from channel where name = :name;");
        query.bindValue(":name", channel->getName());
        bool result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);
        QVERIFY(query.value(1) == name);
        QVERIFY(query.value(2) == descr);
        QVERIFY(query.value(3) == url);

        result = deleteTestChannel(channel);
        QCOMPARE(result, true);
    }

    void Test_QueryExecutor::deleteUser()
    {
        QSharedPointer<common::User> user = createTestUser("user2", "test", "email2@test.org");
        QVERIFY(user != QSharedPointer<common::User>(0));
        bool result = m_queryExecutor->deleteUser(user);
        QCOMPARE(result, true);

        QSqlQuery query(m_database);
        query.prepare("select id from users where login = :login;");
        query.bindValue(":login", user->getLogin());
        result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == false);
    }

    void Test_QueryExecutor::insertNewSession()
    {
        QSharedPointer<common::User> user = createTestUser("user6", "test", "email6@test.org");
        QVERIFY(user != QSharedPointer<common::User>(0));
        QSharedPointer<Session> dummySession(new Session("", QDateTime::currentDateTime(), user));
        QSharedPointer<Session> session = m_queryExecutor->insertNewSession(dummySession);
        QCOMPARE(session.isNull(), false);
        QVERIFY(session->getUser()->getEmail() == "email6@test.org");
        QVERIFY(session->getUser()->getLogin() == "user6");
        QVERIFY(session->getUser()->getPassword() == "test");

        QSqlQuery query(m_database);
        query.prepare("select id, user_id from sessions where session_token = :token;");
        query.bindValue(":token", session->getSessionToken());
        bool result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);
        QVERIFY(query.value(1) == user->getId());

        result = deleteTestSession(session);
        QCOMPARE(result, true);

        result = deleteTestUser(user);
        QCOMPARE(result, true);
    }

    void Test_QueryExecutor::updateSession()
    {
        QSharedPointer<common::User> user = createTestUser("user7", "test", "email7@test.org");
        QVERIFY(user != QSharedPointer<common::User>(0));
        QSharedPointer<Session> session = createTestSession("TTTTT22222", QDateTime::currentDateTime(), user);
        QVERIFY(session != QSharedPointer<Session>(0));

        QDateTime timeBeforeUpdate = session->getLastAccessTime();
        m_queryExecutor->updateSession(session);
        QDateTime timeAfterUpdate = session->getLastAccessTime();
        QVERIFY(timeBeforeUpdate != timeAfterUpdate);

        QSqlQuery query(m_database);
        query.prepare("select last_access_time from sessions where session_token = :token;");
        query.bindValue(":token", session->getSessionToken());
        bool result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == true);
        QVERIFY(query.value(0).toDateTime() != timeBeforeUpdate);

        result = deleteTestSession(session);
        QCOMPARE(result, true);

        result = deleteTestUser(user);
        QCOMPARE(result, true);
    }

    void Test_QueryExecutor::deleteSession()
    {
        QSharedPointer<common::User> user = createTestUser("user8", "test", "email8@test.org");
        QVERIFY(user != QSharedPointer<common::User>(0));
        QSharedPointer<Session> session = createTestSession("TTTTT33333", QDateTime::currentDateTime(), user);
        QVERIFY(session != QSharedPointer<Session>(0));
        bool result = m_queryExecutor->deleteSession(session);
        QCOMPARE(result, true);

        QSqlQuery query(m_database);
        query.prepare("select id from sessions where session_token = :token;");
        query.bindValue(":token", session->getSessionToken());
        result = query.exec();
        QCOMPARE(result, true);
        QVERIFY(query.next() == false);

        result = deleteTestUser(user);
        QCOMPARE(result, true);
    }

} // end of namespace Test



