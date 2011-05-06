include(../../../config.pri)

INCLUDEPATH += ../inc/

SOURCES += main.cpp \ 
    ../src/User.cpp \ 
    ../src/GpsInfo.cpp \
    ../src/GpsModeller.cpp 
HEADERS += User_Test.h \
    ../inc/User.h \
    ../inc/GpsInfo.h \
    ../inc/GpsModeller.h \
    GpsInfo_Test.h \
    User_TestObj.h

DEFINES += SIMULATE_GPS

CONFIG   += qtestlib

TEMPLATE = app

LIBS += -lxml2

QT += testlib

TARGET = test.suite
