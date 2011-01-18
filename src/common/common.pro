TEMPLATE = lib

include(../../config.pri)

TARGET = ../lib/common
target.path = /usr/lib

INSTALLS += target

DEPENDPATH += . \
    inc \
    src

INCLUDEPATH += . \
    inc \
    /usr/include/libxml2

# Input
HEADERS += \
    inc/DataMarks.h \
    inc/DefaultException.h \
    inc/GpsInfo.h \
    inc/User.h \
    inc/Picture.h \
    inc/PngPicture.h \
    inc/Channel.h \
    inc/defines.h \
    inc/ConcurrentVector.h \
    inc/DataChannel.h

SOURCES += \
    src/DataMarks.cpp \
    src/DefaultException.cpp \
    src/GpsInfo.cpp \
    src/Picture.cpp \
    src/PngPicture.cpp \
    src/Channel.cpp \
    src/User.cpp

linux: {
    HEADERS +=  \
                inc/GpsModeller.h

    SOURCES +=  \
                src/GpsModeller.cpp

    LIBS += -lxml2 -lodbc

    DEFINES += SIMULATE_GPS
}

maemo5: {

    CONFIG += mobility
    MOBILITY = location

    HEADERS +=  \
               inc/MobilityGps.h

    SOURCES +=  \
               src/MobilityGps.cpp

    LIBS += -lQtLocation
}

symbian: {
}

webside: {
    HEADERS +=  \
                inc/GpsModeller.h

    SOURCES +=  \
                src/GpsModeller.cpp

    LIBS += -lxml2 -lodbc

    DEFINES += NO_GPS SIMULATE_GPS
}
