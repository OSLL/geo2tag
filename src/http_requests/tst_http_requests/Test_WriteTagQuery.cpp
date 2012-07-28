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
 * \file Test_WriteTagQuery.cpp
 * \brief Test suite for WriteTagQuery class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include "Test_WriteTagQuery.h"
#include "signals.h"
#include "Session.h"
#include "User.h"
#include "JsonDataMark.h"

#include <QString>
#include <QTimer>
#include <QDateTime>

namespace Test
{
  void Test_WriteTagQuery::response()
  {
    WriteTagQuery query(this);
    QSharedPointer<Channel> channel(new Channel("Test channel", "description"));
    QSharedPointer<common::User> user(new common::User("Paul", "test"));
    QSharedPointer<Session> session(new Session("ppppppppp", QDateTime::currentDateTime().toUTC(), user));
    JsonDataMark tag(30.0, 60.0, 30.0, "Test mark", "Test description", "", QDateTime::currentDateTime());
    tag.setChannel(channel);
    tag.setSession(session);
    query.setSession(session);
    query.setChannel(channel);
    query.setTag(QSharedPointer<DataMark>(&tag));
    query.doRequest();
    //connect(&query, SIGNAL(errorOccured(QString)), this, SLOT(ok()));
    QVERIFY(waitForSignal(&query, SIGNAL(errorOccured(int)), 5000));
  }

}                                       // end of namespace Test
