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
 * \file VersionQuery_Test.h
 * \brief Test suite for VersionQuery class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include <QObject>
#include <QtTest/QtTest>
#include <QSignalSpy>

//include Application class
#include "VersionQuery.h"
#include "signals.h"

namespace Test
{
  class VersionQuery_Test : public QObject
  {
    Q_OBJECT;

    // VersionQuery*  m_tstObject; // Object for testing

    public:

      VersionQuery_Test(QObject *parent =0) : QObject(parent)
      {
        // initialization here
        // m_tstObject = new VersionQuery;
      }

      ~VersionQuery_Test()
      {
        // destroying  here
        // delete m_tstObject;
        // m_tstObject = NULL;
      }

    private slots:

      void test1()
      {

        VersionQuery * query = new VersionQuery(this);
        QSignalSpy spy_version(query,SIGNAL(success()));
        query->doRequest();
        QVERIFY(waitForSignal(query,SIGNAL(success()),5000));
        QCOMPARE(spy_version.count(), 1);

        // see docs: http://doc.qt.nokia.com/4.7/qtest.html

        //QCOMPARE();
        //QWARN();
        //QVERIFY();
        //QTEST();
      }

  };                                    // class VersionQuery_Test

}                                       // end of namespace Test
