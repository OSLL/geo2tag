/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
* \file RegisterUserResponseJSON_Test.cpp
* \brief Test suite for RegisterUserResponseJSON class
*
* PROJ: OSLL/geo2tag
* ----------------------------------------------------------- */

#include "Test_RegisterUserResponseJSON.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace Test
{
    void Test_RegisterUserResponseJSON::getJson()
    {
        RegisterUserResponseJSON response;
        QByteArray data;
        QJson::Serializer serializer;
        QVariantMap obj;

        data = QString("{\"errno\":\"0\"}").toAscii();
        obj.insert("errno", 0);

        QByteArray true_json = serializer.serialize(obj);

        response.parseJson(data);
        QCOMPARE(response.getJson(), true_json);
    }

    void Test_RegisterUserResponseJSON::parseJson()
    {
        RegisterUserResponseJSON response;
        QByteArray data;

        data = QString("{\"errno\":\"0\"}").toAscii();
        QCOMPARE(response.parseJson(data), true);
        QCOMPARE(response.getErrno(), 0);
    }

} // end of namespace Test
