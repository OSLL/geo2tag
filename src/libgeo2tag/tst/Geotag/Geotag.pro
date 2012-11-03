#include(../../../config.pri)

#INCLUDEPATH +=

#DEFINES +=
#LIBS += 

SOURCES += libgeo2tag_Geotag.cpp \
    ../../src/geotag.cpp

CONFIG +=qtestlib console
CONFIG -=app_bundle

TARGET = utest.libgeo2tag_Geotag

#HEADERS += 

HEADERS += \
    ../../inc/geotag.h
