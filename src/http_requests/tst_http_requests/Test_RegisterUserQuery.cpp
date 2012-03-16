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

#include <QString>
#include <QTimer>

namespace Test
{
    void Test_RegisterUserQuery::response()
    {
        RegisterUserQuery query;
        m_ok = false;
        QString email = "dummy@test.org";
        QString login = "Dummy";
        QString password = "test";
        query.setQuery(email, login, password);
        query.doRequest();
        connect(&query, SIGNAL(errorOccured(QString)), this, SLOT(ok()));
        QTest::qSleep(1000);

        //QCOMPARE(m_ok , true);

    }

    void Test_RegisterUserQuery::ok()
    {
        m_ok = true;
    }

} // end of namespace Test
