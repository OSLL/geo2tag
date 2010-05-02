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
HEADERS += inc/MapLoader.h \
           inc/GoogleMapLoader.h \
           inc/GoogleClientLogin.h \
           inc/MapLoaderFactory.h \
           inc/GoogleMapsApiKey.h \
           inc/OpenStreetMapLoader.h
SOURCES += src/GoogleClientLogin.cpp \
           src/GoogleMapLoader.cpp \
           src/MapLoader.cpp \
           src/MapLoaderFactory.cpp \
           src/OpenStreetMapLoader.cpp

OBJECTS_DIR = .obj

MOC_DIR = .moc
