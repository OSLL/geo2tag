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
    inc/Channel.h \
    inc/defines.h \
    inc/ConcurrentVector.h \
    inc/DataChannel.h \
    inc/TimeSlot.h

SOURCES += \
    src/DataMarks.cpp \
    src/DefaultException.cpp \
    src/GpsInfo.cpp \
    src/Channel.cpp \
    src/defines.cpp \
    src/User.cpp \
    src/TimeSlot.cpp

linux: {
    HEADERS +=  \
                inc/GpsModeller.h

    SOURCES +=  \
                src/GpsModeller.cpp

    QT -= gui
    LIBS += -lxml2

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
