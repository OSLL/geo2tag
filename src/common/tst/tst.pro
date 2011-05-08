include(../../../config.pri)

INCLUDEPATH += ../inc/

SOURCES += main.cpp \ 
    ../src/User.cpp \ 
    ../src/GpsInfo.cpp \
    ../src/GpsModeller.cpp \ 
    Channel_TestObj.cpp \
    ../src/Channel.cpp \
    ../src/TimeSlot.cpp
HEADERS += User_Test.h \
    ../inc/User.h \
    ../inc/GpsInfo.h \
    ../inc/GpsModeller.h \
    GpsInfo_Test.h \
    User_TestObj.h \
    Channel_Test.h \
    Channel_TestObj.h \
    ../inc/Channel.h \
    ../inc/TimeSlot.h

DEFINES += SIMULATE_GPS

CONFIG   += qtestlib

TEMPLATE = app

LIBS += -lxml2

QT += testlib sql

TARGET = test.suite
