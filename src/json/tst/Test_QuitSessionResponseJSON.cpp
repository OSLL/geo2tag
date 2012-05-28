/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
* \file Test_QuitSessionResponseJSON.cpp
* \brief Test suite for QuitSessionResponseJSON class
*
* PROJ: OSLL/geo2tag
* ----------------------------------------------------------- */

#include "Test_QuitSessionResponseJSON.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace Test
{
    void Test_QuitSessionResponseJSON::getJson()
    {
        QuitSessionResponseJSON response;
        QByteArray data;
        QJson::Serializer serializer;
        QVariantMap obj;

        data = QString("{\"errno\":\"0\"}").toAscii();
        obj.insert("errno", 0);

        QByteArray true_json = serializer.serialize(obj);

        response.parseJson(data);
        QCOMPARE(response.getJson(), true_json);
    }

    void Test_QuitSessionResponseJSON::parseJson()
    {
        QuitSessionResponseJSON response;
        QByteArray data;

        data = QString("{\"errno\":\"0\"}").toAscii();
        QCOMPARE(response.parseJson(data), true);
        QCOMPARE(response.getErrno(), 0);
    }

} // end of namespace Test
