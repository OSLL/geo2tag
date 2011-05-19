

INCLUDEPATH = ../inc/ ../../common/inc/

SOURCES += main.cpp \
    ../src/JsonUser.cpp \
    ../../common/src/User.cpp\
    ../../common/src/Session.cpp
HEADERS += JsonUser_Test.h \
    ../inc/JsonUser.h \
    ../../common/inc/User.h \
    ../../common/inc/Session.h

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib

TARGET = test.suite
