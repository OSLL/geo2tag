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
 * \file libgeo2tag_Session.cpp
 * \brief Unit test suite for libgeo2tag / Session
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <QtTest/QTest>
#include "../../inc/session.h"

// All unit test should be in UnitTest namespase
namespace UnitTest
{

// Unit test is just a Qt Class
class Session : public QObject
{
    Q_OBJECT

    //Geo::Session m_object;

private slots:


    void instance()
    {
       // QVERIFY(Geo::Session::instance() == NULL);
    }

    void initTestCase()
    {
        Geo::Session::init("url=http://demo.geo2tag.org:80/;user=test;password=test");
    }

    void isValid()
    {
        QVERIFY(Geo::Session::instance().isValid());
    }


    void saveState()
    {
        //QVERIFY(FALSE);
    }

    void restoreState()
    {
        //QVERIFY(FALSE);
    }

    void login()
    {
        QVERIFY(Geo::Session::instance().login());


    }
};

}


QTEST_MAIN(UnitTest::Session)
#include "libgeo2tag_Session.moc"
