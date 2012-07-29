include(../../../config.pri)

INCLUDEPATH +=../inc/ ../../json/inc/

DEFINES += SIMULATE_GPS
LIBS += -lxml2

SOURCES += \
    test_CommonTypes.cpp \
    ../src/Region.cpp \
    ../src/GpsInfo.cpp \
    ../src/GpsModeller.cpp \
    ../../json/src/JsonDataMark.cpp \
    ../src/DataMarks.cpp

CONFIG +=qtestlib console
CONFIG -=app_bundle

TARGET = utest.CommonTypes

HEADERS += \
    ../inc/Region.h \
    ../inc/GpsInfo.h \
    ../../json/inc/JsonDataMark.h \
    ../inc/DataMarks.h

