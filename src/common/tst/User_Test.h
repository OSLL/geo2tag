/*
 * Copyright 2011  Kirill Krinkin  kirill.krinkin@gmail.com
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
 * \file Test_User.h
 * \brief Test suite for User class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include <QObject>
#include <QtTest/QtTest>
#include <QSignalSpy>

#include "User_TestObj.h"

namespace
{
  const QString LOGIN("name of user");
  const QString PASSWD("parol");
}


namespace Test
{
  class User_Test : public QObject
  {
    Q_OBJECT;

    Test::UserTestImpl *m_object;

    public:

      User_Test(QObject *parent = NULL) : QObject(parent)
      {
        m_object = new Test::UserTestImpl(LOGIN,PASSWD);
      }

      ~User_Test()
      {
        delete m_object;
        m_object = NULL;
      }

    private slots:

      void testInit() const
      {
        QCOMPARE(m_object->getLogin(), LOGIN);
        QCOMPARE(m_object->getPassword(), PASSWD);
      }

      // class User_Test
  };
}
