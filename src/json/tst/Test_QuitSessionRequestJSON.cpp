/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
 * \file Test_QuitSessionRequestJSON_Test.cpp
 * \brief Test suite for QuitSessionRequestJSON class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include "Test_QuitSessionRequestJSON.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>


namespace Test
{
    void Test_QuitSessionRequestJSON::getJson()
    {
        QuitSessionRequestJSON request;
        QByteArray data;
        QJson::Serializer serializer;
        QVariantMap obj;

        data = QString("{\"session_token\":\"MMMMMMMMMM\"}").toAscii();
        obj.insert("session_token", QString("MMMMMMMMMM"));

        QByteArray true_json = serializer.serialize(obj);

        request.parseJson(data);
        QCOMPARE(request.getJson(), true_json);
    }

    void Test_QuitSessionRequestJSON::parseJson()
    {
        QuitSessionRequestJSON request;
        QByteArray data;

        data = QString("{\"session_token\":\"MMMMMMMMMM\"}").toAscii();
        QCOMPARE(request.parseJson(data), true);
        QCOMPARE(request.getSessionToken(), QString("MMMMMMMMMM"));
    }

} // end of namespace Test
