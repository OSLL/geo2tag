#include(../../../config.pri)

INCLUDEPATH +=  ../../../common/inc

#DEFINES +=
#LIBS += 

SOURCES += libgeo2tag_Session.cpp \
    ../../src/session.cpp \
    ../../src/queryobject.cpp

CONFIG +=qtestlib console
CONFIG -=app_bundle
QT += network

TARGET = utest.libgeo2tag_Session

#HEADERS += 

HEADERS += \
    ../../inc/session.h \
    ../../src/queryobject.h
