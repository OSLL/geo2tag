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
#include <QSharedPointer>


namespace Test
{
  class UpdateThread_Test : public QObject
  {
    Q_OBJECT;

    UpdateThread*  m_tstObject; // Object for testing
    QueryExecutor* m_queryExecutor;   
    QSqlDatabase m_database;

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
	    //database.setPassword("geo2tag");
	    //m_database = Geo2tagDatabase(database);
	    m_queryExecutor = new QueryExecutor(Geo2tagDatabase(QSqlDatabase::cloneDatabase(m_database, "QueryExecutor")));
	    m_database.open();
	
    }
    
    ~UpdateThread_Test()
    {
  	  // destroying  here
			// m_tstObject = NULL;
	     m_database.close();
    }
  
  private slots:
  
    void testDbSyncronization()
    {
	   // #1 Create updateThread and check that it syncronize with DB
	   m_tstObject = new UpdateThread(m_tags, m_users, m_channels, m_dataChannelsMap, m_sessions, m_queryExecutor);
	   m_tstObject->start();
    	   QVERIFY(waitForSignal(m_tstObject, SIGNAL(syncronizationComplete()), 2 * DEFAULT_DB_UPDATE_INTERVAL));
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
  
  }; // class UpdateThread_Test

} // end of namespace Test
