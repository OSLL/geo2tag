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
#include "ErrnoTypes.h"
#include <QDebug>

namespace Test
{
    void Test_RegisterUserResponseJSON::getJson()
    {
        RegisterUserResponseJSON response;
        QString data = "{ \"confirmUrl\" : \"\", \"errno\" : 0 }";
	response.setErrno(SUCCESS);
	qDebug() << data;
	qDebug() << response.getJson();
        QCOMPARE(response.getJson(), data.toAscii());
    }

    void Test_RegisterUserResponseJSON::parseJson()
    {
        RegisterUserResponseJSON response;
        QByteArray data;

        data = QString("{ \"errno\" : 0 }").toAscii();
        QCOMPARE(response.parseJson(data), true);
        QCOMPARE(response.getErrno(), 0);
    }

} // end of namespace Test
