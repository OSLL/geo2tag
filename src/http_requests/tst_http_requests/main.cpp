/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com>
 */

/*!
 * \file main.cpp
 * \brief Test suite for http_requests
 *
 * PROJ: OSLL/geo2tag
 * ------------------------------------------------------------------------ */

#include <QCoreApplication>
#include "Test_RegisterUserQuery.h"
#include "VersionQuery_Test.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QObject *tests[] =
    {
        new Test::Test_RegisterUserQuery,
	new Test::VersionQuery_Test
    };

    for (int i = 0; i < sizeof(tests)/sizeof(QObject*); i++) {
        QTest::qExec(tests[i]);
    }

    return 0;
}
