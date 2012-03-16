/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com>
 */

/*!
 * \file main.cpp
 * \brief Test suite for http_requests
 *
 * PROJ: OSLL/geo2tag
 * ------------------------------------------------------------------------ */

#include "Test_RegisterUserQuery.h"

int main()
{
    QObject *tests[] =
    {
        new Test::Test_RegisterUserQuery
    };

    for (int i = 0; i < sizeof(tests)/sizeof(QObject*); i++) {
        QTest::qExec(tests[i]);
    }

    return 0;
}
