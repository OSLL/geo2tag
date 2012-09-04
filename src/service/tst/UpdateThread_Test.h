/*
 * Copyright 2012  Mark Zaslavskiy  mark.zaslavskiy@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
 * \file UpdateThread_Test.h
 * \brief Test suite for UpdateThread class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include <QObject>
#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QtSql>
//include Application class
#include "UpdateThread.h"
#include "QueryExecutor.h"
#include "signals.h"
#include "defines.h"
#include "JsonDataMark.h"
#include "JsonUser.h"
#include "JsonChannel.h"
#include "JsonSession.h"
#include "SettingsStorage.h"
#include <QSharedPointer>
#include <QSignalSpy>

namespace Test
{

  #define DATAMARKS_TO_ADD 10
  #define USERS_TO_ADD 10
  #define CHANNELS_TO_ADD 10
  #define SESSIONS_TO_ADD 9

  class UpdateThread_Test : public QObject
  {
    Q_OBJECT;

    UpdateThread*  m_tstObject;         // Object for testing
    QueryExecutor* m_queryExecutor;
    QSqlDatabase m_database;

    qlonglong m_updateInterval;

    QSharedPointer<DataMarks> m_tags;
    QSharedPointer<common::Users> m_users;
    QSharedPointer<Channels> m_channels;
    QSharedPointer<DataChannels> m_dataChannelsMap;
    QSharedPointer<Sessions> m_sessions;

    public:

      UpdateThread_Test(QObject *parent =NULL) : QObject(parent), m_tags(new DataMarks()),
        m_users(new common::Users()), m_channels(new Channels()),
        m_dataChannelsMap(new DataChannels), m_sessions(new Sessions())
      {
        // initialization here
        // m_tstObject = new UpdateThread;

        m_database = QSqlDatabase::addDatabase("QPSQL");
        m_database.setHostName("localhost");
        m_database.setDatabaseName("test_db");
        m_database.setUserName("test_user");
        m_queryExecutor = new QueryExecutor(Geo2tagDatabase(QSqlDatabase::cloneDatabase(m_database, "QueryExecutor")));
        m_database.open();

        SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
        m_updateInterval = storage.getValue("General_Settings/db_update_interval", QVariant(DEFAULT_DB_UPDATE_INTERVAL)).toLongLong();

      }

      ~UpdateThread_Test()
      {
        // destroying  here
        // m_tstObject = NULL;
        m_database.close();
      }

    private slots:

      void testAfterInitSyncronization()
      {

        // #1 Create updateThread and check that it syncronize with DB
        m_tstObject = new UpdateThread(m_tags, m_users, m_channels, m_dataChannelsMap, m_sessions, m_queryExecutor);
        m_tstObject->start();
        QVERIFY(waitForSignal(m_tstObject, SIGNAL(syncronizationComplete()), 2 * m_updateInterval));
        QVERIFY(m_tags->size() > 0);
        QVERIFY(m_channels->size() > 0);
        QVERIFY(m_users->size() > 0);
        QVERIFY(m_dataChannelsMap->size() > 0);
        QVERIFY(m_sessions->size() > 0);

        // see docs: http://doc.qt.nokia.com/4.7/qtest.html

        //QCOMPARE();
        //QWARN();
        //QVERIFY();
        //QTEST();
      }

      void testDataMarksSyncronization()
      {

        // #2 Add manualy dataMarks, check that they appear at cache
        QSharedPointer<DataMark> testMark (new JsonDataMark(0.,0.,0.,"","","",QDateTime::currentDateTime()));
        testMark->setUser(m_users->at(0));
        testMark->setChannel(m_users->at(0)->getSubscribedChannels()->at(0));

        qlonglong tagsOldSize = m_tags->size();
        qlonglong dataChannelsOldSize = m_dataChannelsMap->size();
        qDebug() << "tagsOldSize = " << tagsOldSize;
        m_queryExecutor->connect();
        for (int i = 0; i < DATAMARKS_TO_ADD; i++ )
          QVERIFY(m_queryExecutor->insertNewTag(testMark));

        QVERIFY(waitForSignal(m_tstObject, SIGNAL(syncronizationComplete()), 2 * m_updateInterval));
        qDebug() << "tagsNewSize = " << m_tags->size();
        QVERIFY(m_tags->size() == tagsOldSize + DATAMARKS_TO_ADD);
        QVERIFY(m_dataChannelsMap->size() == dataChannelsOldSize + DATAMARKS_TO_ADD);

      }

      void testUsersSyncronization()
      {

        // #3 add USERS_TO_ADD users to db manualy, wait for syncronizationComplete signal, check how many users where added
        qlonglong usersOldSize = m_users->size();
        qDebug() << "usersOldSize = " << usersOldSize;
        // Add users manualy
        m_queryExecutor->connect();
        for (int i = 0; i < USERS_TO_ADD; i++ )
        {
          // Random users credentials
          QString testUserName = QString::number(rand()%10000),
            testUserPassword = QString::number(rand()%10000),
            testUserEmail = QString("%1@test.com").arg(QString::number(rand()%10000));
          QSharedPointer<common::User> testUser(new JsonUser(testUserName, testUserPassword, testUserEmail));
          // Adding random user into db
          QVERIFY(m_queryExecutor->insertNewUser(testUser));
        }

        QVERIFY(waitForSignal(m_tstObject, SIGNAL(syncronizationComplete()), 2 * m_updateInterval));
        qDebug() << "usersNewSize = " << m_users->size();
        QVERIFY(m_users->size() == usersOldSize + USERS_TO_ADD);
      }

      void testChannelsSyncronization()
      {

        // #3 add CHANNELS_TO_ADD channels to db manualy, wait for syncronizationComplete signal, check how many channels where added
        qlonglong channelsOldSize = m_channels->size();
        qDebug() << "channelsOldSize = " << channelsOldSize;
        // Add channels manualy
        m_queryExecutor->connect();
        for (int i = 0; i < CHANNELS_TO_ADD; i++ )
        {
          // Random channels credentials
          QString testChannelName = QString::number(rand()%10000),
            testChannelDescription = QString::number(rand()%10000),
            testChannelUrl = QString("%1@test.com").arg(QString::number(rand()%10000));
          QSharedPointer<Channel> testChannel(new JsonChannel(testChannelName, testChannelDescription, testChannelUrl, m_users->at(0)));
          // Adding random channel into db
          QVERIFY(m_queryExecutor->insertNewChannel(testChannel));
        }

        QVERIFY(waitForSignal(m_tstObject, SIGNAL(syncronizationComplete()), 2 * m_updateInterval));
        qDebug() << "channelsNewSize = " << m_channels->size();
        QVERIFY(m_channels->size() == channelsOldSize + CHANNELS_TO_ADD);
      }

      void testSessionsSyncronization()
      {

        // #3 add SESSIONS_TO_ADD channels to db manualy, wait for syncronizationComplete signal, check how many channels where added
        qlonglong sessionsOldSize = m_sessions->size();
        qDebug() << "sessionsOldSize = " << sessionsOldSize;
        // Add sessions manualy
        m_queryExecutor->connect();
        for (int i = 0; i < SESSIONS_TO_ADD; i++ )
        {
          // Random sessions credentials
          QString testSessionToken = QString::number(rand()%10000);
          // m_users->at(m_users->size() - 1 - i) means that we use prevously added random users
          QSharedPointer<Session> testSession(new JsonSession(testSessionToken, QDateTime::currentDateTime(), m_users->at(m_users->size() - 1 - i)));
          // Adding random session into db
          QVERIFY(m_queryExecutor->insertNewSession(testSession));
        }

        QVERIFY(waitForSignal(m_tstObject, SIGNAL(syncronizationComplete()), 2 * m_updateInterval));
        qDebug() << "sessionsNewSize = " << m_sessions->size();
        QVERIFY(m_sessions->size() == sessionsOldSize + SESSIONS_TO_ADD);
      }

      void testDataChannelSync()
      {
        // Add marks and check newTagInsertionComplete(int)
        QSharedPointer<DataMark> testMark (new JsonDataMark(0.,0.,0.,"","","",QDateTime::currentDateTime()));
        testMark->setUser(m_users->at(0));
        testMark->setChannel(m_users->at(0)->getSubscribedChannels()->at(0));

        m_queryExecutor->connect();

        QSignalSpy spy_tagsSync_1(m_tstObject, SIGNAL(newTagInsertionComplete(int)));

        for (int i = 0; i < DATAMARKS_TO_ADD; i++ )
          QVERIFY(m_queryExecutor->insertNewTag(testMark));

        waitForSignal(m_tstObject, SIGNAL(syncronizationComplete()), 2 * m_updateInterval);
        QCOMPARE(spy_tagsSync_1.count(), 1);
        // new  QSignalSpy
        QSignalSpy spy_tagsSync_2(m_tstObject, SIGNAL(newTagInsertionComplete(int)));
        // Add users and check that newTagInsertionComplete(int) is not occured
        m_queryExecutor->connect();
        for (int i = 0; i < USERS_TO_ADD; i++ )
        {
          // Random users credentials
          QString testUserName = QString::number(rand()%10000),
            testUserPassword = QString::number(rand()%10000),
            testUserEmail = QString("%1@test.com").arg(QString::number(rand()%10000));
          QSharedPointer<common::User> testUser(new JsonUser(testUserName, testUserPassword, testUserEmail));
          // Adding random user into db
          QVERIFY(m_queryExecutor->insertNewUser(testUser));
        }

        waitForSignal(m_tstObject, SIGNAL(syncronizationComplete()), 2 * m_updateInterval);
        QCOMPARE(spy_tagsSync_2.count(), 0);
      }

  };                                    // class UpdateThread_Test

}                                       // end of namespace Test
