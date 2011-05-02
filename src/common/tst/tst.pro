

INCLUDEPATH = ../inc/

SOURCES += main.cpp \ 
    ../src/User.cpp
HEADERS += User_Test.h \
    ../inc/User.h \
    User_TestObj.h

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib

TARGET = test.suite
