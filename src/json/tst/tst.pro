include(../../../config.pri)

INCLUDEPATH = ../inc/ ../../common/inc/ \
              ../../json/inc/ \

SOURCES += main.cpp \
    ../src/JsonUser.cpp \
    ../../common/src/User.cpp \
    ../../json/src/JsonSerializer.cpp \
    ../../json/src/RegisterUserRequestJSON.cpp \
    ../../json/src/RegisterUserResponseJSON.cpp \
    Test_RegisterUserResponseJSON.cpp \
    Test_RegisterUserRequestJSON.cpp
HEADERS += JsonUser_Test.h \
    ../inc/JsonUser.h \
    ../../common/inc/User.h \
    ../../json/inc/JsonSerializer.h \
    ../../json/inc/RegisterUserRequestJSON.h \
    ../../json/inc/RegisterUserResponseJSON.h \
    Test_RegisterUserResponseJSON.h \
    Test_RegisterUserRequestJSON.h

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib

TARGET = test.suite

LIBS    +=  -lcommon -lqjson
