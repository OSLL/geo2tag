/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
 * \file RegisterUserRequestJSON_Test.h
 * \brief Test suite for RegisterUserRequestJSON class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#ifndef TEST_REGISTERUSERREQUESTJSON_H
#define TEST_REGISTERUSERREQUESTJSON_H

#include <QObject>
#include <QtTest>
#include "../inc/RegisterUserRequestJSON.h"

namespace Test
{
    class Test_RegisterUserRequestJSON : public QObject
    {
        Q_OBJECT

    private slots:
          void getJson();
          void parseJson();
    };               // class Test_RegisterUserRequestJSON
}                // end of namespace Test

#endif // TEST_REGISTERUSERREQUESTJSON_H
