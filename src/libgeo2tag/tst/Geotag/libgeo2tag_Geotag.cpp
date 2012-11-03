/*
 * Copyright 2012  Kirill Krinkin  kirill.krinkin@gmail.com
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

/*! ---------------------------------------------------------------
 *
 * \file libgeo2tag_Geotag.cpp
 * \brief Unit test suite for libgeo2tag / Geotag
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <QtTest/QTest>
#include "../../inc/geotag.h"

#include <QDebug>
#include <cmath>

// All unit test should be in UnitTest namespase
namespace UnitTest
{

  // Unit test is just a Qt Class
  class Geotag : public QObject
  {
    Geo::Geotag m_obj;

    Q_OBJECT

      private slots:

      void defaultValues()
      {
          time_t t=time(NULL);
          QVERIFY(difftime(t,m_obj.timestamp()) < 1.0 );

          QVERIFY(m_obj.b() == 0.0);
          QVERIFY(m_obj.l() == 0.0);
          QVERIFY(m_obj.h() == 0.0);
          QVERIFY(0==strlen(m_obj.label()));
          QVERIFY(0==strlen(m_obj.data()));
      }


      void equivalence_and_access()
      {
          m_obj.setAltitude(1.0);
          m_obj.setLatitude(2.0);
          m_obj.setLongitude(3.0);

          QVERIFY(fabs((m_obj.altitude() - m_obj.h())<1e-10));
          QVERIFY(fabs((m_obj.latitude() - m_obj.b())<1e-10));
          QVERIFY(fabs((m_obj.longitude() - m_obj.l())<1e-10));

          m_obj.setB(1.0);
          m_obj.setL(2.0);
          m_obj.setH(3.0);

          QVERIFY(fabs((m_obj.altitude() - m_obj.h())<1e-10));
          QVERIFY(fabs((m_obj.latitude() - m_obj.b())<1e-10));
          QVERIFY(fabs((m_obj.longitude() - m_obj.l())<1e-10));
      }

      void label()
      {
          const char l[]="label";
          const char d[]="data";

          m_obj.setData(d);
          m_obj.setLabel(l);

          QVERIFY(!strncmp(d,m_obj.data(),strlen(d)));
          QVERIFY(!strncmp(l,m_obj.label(),strlen(l)));
      }

      void colocation()
      {
          Geo::Geotag a,b,c(1,3,4);
          QVERIFY(a.isColocated(b));
          QVERIFY(b.isColocated(a));
          QVERIFY(!a.isColocated(c));
      }
  };

}


QTEST_APPLESS_MAIN(UnitTest::Geotag)
#include "libgeo2tag_Geotag.moc"
