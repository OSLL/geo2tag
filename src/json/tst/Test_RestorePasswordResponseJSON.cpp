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
 * \file Test_RestorePasswordRequestJSON.cpp
 * \brief Test suite for RestorePasswordRequestJSON class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include "Test_RestorePasswordResponseJSON.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include "ErrnoTypes.h"
#include <QDebug>

namespace Test
{
  void Test_RestorePasswordResponseJSON::getJson()
  {
    RestorePasswordResponseJSON response;
    QByteArray data;
    QJson::Serializer serializer;
    QVariantMap obj;

    data = QString("{\"errno\":\"0\"}").toAscii();
    obj.insert("errno", 0);

    QByteArray true_json = serializer.serialize(obj);

    response.parseJson(data);
    QCOMPARE(response.getJson(), true_json);
  }

  void Test_RestorePasswordResponseJSON::parseJson()
  {
    RestorePasswordResponseJSON response;
    QByteArray data;

    data = QString("{\"errno\":\"0\"}").toAscii();
    QCOMPARE(response.parseJson(data), true);
    QCOMPARE(response.getErrno(), 0);
  }

}                                       // end of namespace Test
