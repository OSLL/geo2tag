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
 * \file main.cpp
 * \brief Test suite for http_requests
 *
 * PROJ: OSLL/geo2tag
 * ------------------------------------------------------------------------ */

#include <QCoreApplication>

#include "DeleteUserQuery_Test.h"
#include "Test_AddUserQuery.h"
#include "Test_ApplyChannelQuery.h"
#include "Test_AvailableChannelsQuery.h"
#include "Test_LoadTagsQuery.h"
#include "Test_LoginQuery.h"
#include "Test_RegisterUserQuery.h"
#include "Test_SubscribeChannelQuery.h"
#include "Test_SubscribedChannelsQuery.h"
#include "Test_WriteTagQuery.h"
#include "VersionQuery_Test.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  QObject *tests[] =
  {
    new Test::DeleteUserQuery_Test,
    new Test::Test_AddUserQuery,
    new Test::Test_ApplyChannelQuery,
    new Test::Test_AvailableChannelsQuery,
    new Test::Test_LoadTagsQuery,
    new Test::Test_LoginQuery,
    new Test::Test_RegisterUserQuery,
    new Test::Test_SubscribeChannelQuery,
    new Test::Test_SubscribedChannelsQuery,
    new Test::Test_WriteTagQuery,
    new Test::VersionQuery_Test
  };

  for (unsigned int i = 0; i < sizeof(tests)/sizeof(QObject*); i++)
  {
    QTest::qExec(tests[i]);
  }

  return 0;
}
