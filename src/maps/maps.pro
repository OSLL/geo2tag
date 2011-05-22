TEMPLATE = lib

TARGET = ../lib/maps

target.path = /usr/lib
INSTALLS += target
include(../../config.pri)

QT += network

DEPENDPATH += . inc src

INCLUDEPATH += . inc ../common/inc ../common/common/inc ../common/thread/inc \
               ../common/exception/inc

# Input
HEADERS += inc/OSMCoordinatesConverter.h \
        inc/MapsUploader.h \
        inc/MapScene.h \
        inc/MapsUploadThread.h \
        inc/Preloading.h
SOURCES += src/OSMCoordinatesConverter.cpp \
        src/MapsUploader.cpp \
        src/MapScene.cpp \
        src/MapsUploadThread.cpp \
        src/Preloading.cpp

!symbian{
OBJECTS_DIR = .obj

MOC_DIR = .moc
}


RESOURCES += \
    images.qrc
