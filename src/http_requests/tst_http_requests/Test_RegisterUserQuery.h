/*
 * Copyright <C> 2012 Edward Ryabikov <edward.ryabikov@gmail.com
 */

/*!
 * \file Test_RegisterUserQueryJSON.h
 * \brief Test suite for Test_RegisterUserQuery class
 *
 * PROJ: OSLL/geo2tag
 * ----------------------------------------------------------- */

#ifndef TEST_REGISTERUSERQUERY_H
#define TEST_REGISTERUSERQUERY_H

#include <QObject>
#include <QTest>
#include "../inc/RegisterUserQuery.h"

namespace Test
{

    class Test_RegisterUserQuery : public QObject
    {
        Q_OBJECT

        bool m_ok;

    private slots:
        void response();

    public slots:
        void ok();

    };  // class Test_RegisterUserQuery
} // namespace Test

#endif // TEST_REGISTERUSERQUERY_H
