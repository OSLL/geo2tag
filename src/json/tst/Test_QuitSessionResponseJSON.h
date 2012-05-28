/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com>
 */

/*!
* \file Test_QuitSessionResponseJSON.h
* \brief Test suite for QuitSessionResponseJSON class
*
* PROJ: OSLL/geo2tag
* ----------------------------------------------------------- */

#ifndef TEST_QUITSESSIONRESPONSEJSON_H
#define TEST_QUITSESSIONRESPONSEJSON_H

#include <QObject>
#include <QtTest>
#include "../inc/QuitSessionResponseJSON.h"

namespace Test
{
    class Test_QuitSessionResponseJSON : public QObject
    {
        Q_OBJECT

    private slots:
          void getJson();
          void parseJson();
    };
}

#endif // TEST_QUITSESSIONRESPONSEJSON_H
