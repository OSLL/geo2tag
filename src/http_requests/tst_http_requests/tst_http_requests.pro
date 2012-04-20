
include(../../../config.pri)
INCLUDEPATH = ../inc/ ../../common/inc/ \
              ../../json/inc/ \

SOURCES += \
    main.cpp \
    ../../http_requests/src/RegisterUserQuery.cpp \
    ../../http_requests/src/LoginQuery.cpp \
    ../../http_requests/src/DefaultQuery.cpp \
    ../../http_requests/src/VersionQuery.cpp \
    ../../http_requests/src/DeleteUserQuery.cpp \
    ../../json/src/JsonUser.cpp \
    ../../json/src/JsonSerializer.cpp \
    ../../json/src/LoginRequestJSON.cpp \
    ../../json/src/LoginResponseJSON.cpp \
    ../../json/src/RegisterUserRequestJSON.cpp \
    ../../json/src/RegisterUserResponseJSON.cpp \
    ../../json/src/DefaultResponseJSON.cpp \
    ../../json/src/VersionResponseJSON.cpp \
    ../../common/src/signals.cpp \
    Test_RegisterUserQuery.cpp

HEADERS += \
    ../../http_requests/inc/RegisterUserQuery.h \
    ../../http_requests/inc/LoginQuery.h \
    ../../http_requests/inc/DefaultQuery.h \
    ../../http_requests/inc/VersionQuery.h \
    ../../http_requests/inc/DeleteUserQuery.h \
    ../../json/inc/JsonUser.h \
    ../../json/inc/JsonSerializer.h \
    ../../json/inc/LoginRequestJSON.h \
    ../../json/inc/LoginResponseJSON.h \
    ../../json/inc/RegisterUserRequestJSON.h \
    ../../json/inc/RegisterUserResponseJSON.h \
    ../../json/inc/VersionResponseJSON.h \
    ../../json/inc/DefaultResponseJSON.h \
    ../../common/inc/signals.h \
    VersionQuery_Test.h \
    DeleteUserQuery_Test.h \
    Test_RegisterUserQuery.h

CONFIG += qtestlib debug


TEMPLATE = app

QT += testlib network
Qt -= gui

TARGET = test.suite

LIBS += -lcommon -lqjson
