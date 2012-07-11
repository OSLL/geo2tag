include(../../../config.pri)

INCLUDEPATH += ../inc/ ../../json/inc/

SOURCES += main.cpp \ 
    ../src/BasicUser.cpp \
    ../src/User.cpp \ 
    ../src/DataMarks.cpp \
    ../src/GpsInfo.cpp \
    ../src/GpsModeller.cpp \ 
    Channel_TestObj.cpp \
    ../src/Channel.cpp \
    ../../json/src/JsonDataMark.cpp \
    ../src/Region.cpp
HEADERS += User_Test.h \
    ../inc/BasicUser.h \
    ../inc/User.h \
    ../inc/DataMarks.h \
    ../inc/GpsInfo.h \
    ../inc/GpsModeller.h \
    GpsInfo_Test.h \
    User_TestObj.h \
    Channel_Test.h \
    Channel_TestObj.h \
    Region_Test.h \
    ../inc/Channel.h \
    ../../json/inc/JsonDataMark.h \
    ../inc/Region.h 

DEFINES += SIMULATE_GPS

CONFIG   += qtestlib

TEMPLATE = app

LIBS += -lxml2

QT += testlib sql

INCLUDEPATH += /usr/include/libxml2

TARGET = test.suite
