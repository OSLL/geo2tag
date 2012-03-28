/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
 * \file Test_AvailableChannelsResponseJSON.h
 * \brief Test suite for AvailableChannelsResponseJSON class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#ifndef TEST_AVAILABLECHANNELSRESPONSEJSON_H
#define TEST_AVAILABLECHANNELSRESPONSEJSON_H

#include <QObject>
#include <QtTest>
#include "../inc/AvailableChannelsResponseJSON.h"

namespace Test
{
    class Test_AvailableChannelsResponseJSON : public QObject
    {
        Q_OBJECT

    private slots:
          void getJson();
          void parseJson();
    };               // class Test_AvailableChannelsResponseJSON
}                // end of namespace Test

#endif // TEST_AVAILABLECHANNELSRESPONSEJSON_H
