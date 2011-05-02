

INCLUDEPATH = ../inc/ ../../common/inc/

SOURCES += main.cpp \
    ../src/JsonUser.cpp \
    ../../common/src/User.cpp
HEADERS += JsonUser_Test.h \
    ../inc/JsonUser.h \
    ../../common/inc/User.h

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib

TARGET = test.suite
