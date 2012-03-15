/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
 * \file RegisterUserRequestJSON_Test.cpp
 * \brief Test suite for RegisterUserRequestJSON class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#include "Test_RegisterUserRequestJSON.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>


namespace Test
{
    void Test_RegisterUserRequestJSON::getJson()
    {
        RegisterUserRequestJSON request;
        QByteArray data;
        QJson::Serializer serializer;
        QVariantMap obj;

        data = QString("{\"email\":\"dummy@yahoo.com\",\"login\":\"Dummy\",\"password\":\"test\"}").toAscii();
        obj.insert("email", QString("dummy@yahoo.com"));
        obj.insert("login", QString("Dummy"));
        obj.insert("password", QString("test"));

        QByteArray true_json = serializer.serialize(obj);

        request.parseJson(data);
        QCOMPARE(request.getJson(), true_json);
    }

    void Test_RegisterUserRequestJSON::parseJson()
    {
        RegisterUserRequestJSON request;
        QByteArray data;

        data = QString("{\"email\":\"dummy@yahoo.com\",\"login\":\"Dummy\",\"password\":\"test\"}").toAscii();
        QCOMPARE(request.parseJson(data), true);
        QCOMPARE(request.getUsers()->at(0)->getEmail(), QString("dummy@yahoo.com"));
        QCOMPARE(request.getUsers()->at(0)->getLogin(), QString("Dummy"));
        QCOMPARE(request.getUsers()->at(0)->getPassword(), QString("test"));
    }

} // end of namespace Test



