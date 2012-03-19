/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
* \file RegisterUserResponseJSON_Test.h
* \brief Test suite for RegisterUserResponseJSON class
*
* PROJ: OSLL/geo2tag
* ----------------------------------------------------------- */

#ifndef TEST_REGISTERUSERRESPONSEJSON_H
#define TEST_REGISTERUSERRESPONSEJSON_H

#include <QObject>
#include <QtTest>
#include "../inc/RegisterUserResponseJSON.h"

namespace Test
{
    class Test_RegisterUserResponseJSON : public QObject
    {
        Q_OBJECT

    private slots:
          void getJson();
          void parseJson();
    };               // class Test_RegisterUserResponseJSON
}                // end of namespace Test

#endif // TEST_REGISTERUSERRESPONSEJSON_H
