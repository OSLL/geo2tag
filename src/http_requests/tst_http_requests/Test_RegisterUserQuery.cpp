/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
 * \file Test_RegisterUserQuery.cpp
 * \brief Test suite for RegisterUserQuery class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include "Test_RegisterUserQuery.h"
#include "signals.h"

#include <QString>
#include <QTimer>

namespace Test
{
    void Test_RegisterUserQuery::response()
    {
        RegisterUserQuery query(this);
        QString email = "email5@test1.org";
        QString login = "Mark";
        QString password = "test";
        query.setQuery(email, login, password);
        query.doRequest();
        //connect(&query, SIGNAL(errorOccured(QString)), this, SLOT(ok()));
        QVERIFY(waitForSignal(&query, SIGNAL(errorOccured(int)), 5000));

    }

} // end of namespace Test
