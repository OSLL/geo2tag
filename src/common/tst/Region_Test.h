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
 * \file Region_Test.h
 * \brief Test suite for Region class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */


#include <QObject>
#include <QtTest/QtTest>
#include <QSignalSpy>

//include Application class
//#include "../inc/Region.h"

#define POINT1 QSharedPointer<DataMark>(new JsonDataMark(0.,0.,0.,"","","",QDateTime::currentDateTime()))
#define POINT2 QSharedPointer<DataMark>(new JsonDataMark(1000.,0.,0.,"","","",QDateTime::currentDateTime()))
#define POINT3 QSharedPointer<DataMark>(new JsonDataMark(1000.,1000.,0.,"","","",QDateTime::currentDateTime()))
#define POINT4 QSharedPointer<DataMark>(new JsonDataMark(0.,1000.,0.,"","","",QDateTime::currentDateTime()))

#define POINT_IN_REGION QSharedPointer<DataMark>(new JsonDataMark(500.,500.,0.,"","","",QDateTime::currentDateTime()))
#define POINT_OUT_REGION QSharedPointer<DataMark>(new JsonDataMark(1500.,500.,0.,"","","",QDateTime::currentDateTime()))


namespace Test
{
  class Region_Test : public QObject
  {
    Q_OBJECT;

		Region*  m_tstObject; // Object for testing
  
  public:
    
    Region_Test(QObject *parent =NULL) : QObject(parent)
    {
  	  // initialization here
		m_tstObject = new Region;
    }
    
		~Region_Test(QObject *parent =NULL)
    {
  	  // destroying  here
		 delete m_tstObject;
			// m_tstObject = NULL;
    }
  
  private slots:
  
    void test1()
    {
     // see docs: http://doc.qt.nokia.com/4.7/qtest.html
  
     //QCOMPARE();
     //QWARN();
     //QVERIFY();
     //QTEST();
		m_tstObject->addPoint(POINT1);
		m_tstObject->addPoint(POINT2);
		m_tstObject->addPoint(POINT3);
		m_tstObject->addPoint(POINT4);

		QVERIFY(m_tstObject->atRegion(POINT_IN_REGION);
		QVERIFY(!m_tstObject->atRegion(POINT_OUT_REGION);
     
    }
  
  }; // class Region_Test

} // end of namespace Test
