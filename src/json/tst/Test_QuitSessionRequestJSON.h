/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
 * \file QuitSessionRequestJSON_Test.h
 * \brief Test suite for QuitSessionRequestJSON class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#ifndef TEST_QUITSESSIONREQUESTJSON_H
#define TEST_QUITSESSIONREQUESTJSON_H

#include <QObject>
#include <QtTest>
#include "../inc/QuitSessionRequestJSON.h"

namespace Test
{
    class Test_QuitSessionRequestJSON : public QObject
    {
        Q_OBJECT

    private slots:
          void getJson();
          void parseJson();
    };
}

#endif // TEST_QUITSESSIONREQUESTJSON_H
