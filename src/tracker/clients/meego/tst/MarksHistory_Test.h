/*
 * Copyright 2011  Mark Zaslavskiy  mark.zaslavskiy@gmail.com
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
 * \file MarksHistory_Test.h
 * \brief Test suite for MarksHistory class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include <QObject>
#include <QtTest/QtTest>
#include <QSignalSpy>

//include Application class
//#include "../inc/MarksHistory.h"
#include "MarksHistory_TestObj.h"
#include "DataMarks.h"
#include "JsonChannel.h"
#include "JsonDataMark.h"

#define TEST_OBJECT_SIZE 10

namespace Test
{
  class MarksHistory_Test : public QObject
  {
    Q_OBJECT;

    // MarksHistory*  m_tstObject; // Object for testing
    MarksHistoryTestImpl * m_history;
    public:

      MarksHistory_Test(QObject *parent =NULL) : QObject(parent)
      {
        // initialization here
        // m_tstObject = new MarksHistory;
        m_history = new MarksHistoryTestImpl;
      }

      ~MarksHistory_Test()
      {
        // destroying  here
        // delete m_tstObject;
        // m_tstObject = NULL;
      }

    private slots:

      void testFilling()
      {
        m_history->setHistoryLimit(TEST_OBJECT_SIZE);
        QSharedPointer<Channel> channel(new JsonChannel("dummy channel","dummy channel"));
        for (int i=0;i<TEST_OBJECT_SIZE;i++)
        {
          QSharedPointer<DataMark> mark(new JsonDataMark(0,i,i,"","","",QDateTime::currentDateTime()));
          mark->setChannel(channel);
          m_history->pushMark(mark);
        }
        // Check that if we decrease size of m_history it removes first elements
        m_history->setHistoryLimit(TEST_OBJECT_SIZE/2);
        for (int i=TEST_OBJECT_SIZE/2;i<TEST_OBJECT_SIZE;i++)
        {
          //		qDebug() << m_history->popMark()->getLatitude();
          QSharedPointer<DataMark> mark(m_history->popMark());
          int lat = mark->getLatitude();
          int lon = mark->getLongitude();
          qDebug() << lat << i;
          QVERIFY(lat==i);
          QVERIFY(lon==i);
        }
        QVERIFY(m_history->isEmpty()==true);
        // see docs: http://doc.qt.nokia.com/4.7/qtest.html

        //QCOMPARE();
        //QWARN();
        //QVERIFY();
        //QTEST();
      }

  };                                    // class MarksHistory_Test

}                                       // end of namespace Test
