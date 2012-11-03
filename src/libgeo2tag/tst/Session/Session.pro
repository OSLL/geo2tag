#include(../../../config.pri)

#INCLUDEPATH +=

#DEFINES +=
#LIBS += 

SOURCES += libgeo2tag_Session.cpp \
    ../../src/session.cpp

CONFIG +=qtestlib console
CONFIG -=app_bundle

TARGET = utest.libgeo2tag_Session

#HEADERS += 

HEADERS += \
    ../../inc/session.h
