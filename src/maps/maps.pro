TEMPLATE = lib

TARGET = ../lib/maps

target.path = /usr/lib
INSTALLS += target
include(../config.pri)

QT += network

DEPENDPATH += . inc src

INCLUDEPATH += . inc ../common/inc ../common/common/inc ../common/thread/inc \
               ../common/exception/inc

# Input
HEADERS += inc/SlippyMap.h \
           inc/LightMap.h
SOURCES += src/SlippyMap.cpp \
           src/LightMap.cpp

OBJECTS_DIR = .obj

MOC_DIR = .moc
