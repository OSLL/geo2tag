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


# Input
HEADERS += \
    inc/Channel.h \
    inc/User.h \
    inc/DataMarks.h \
    inc/GpsInfo.h \
    inc/defines.h \
    inc/ConcurrentVector.h \
    inc/DataChannel.h \
    inc/TimeSlot.h

SOURCES += \
    src/User.cpp \
    src/Channel.cpp \
    src/DataMarks.cpp \
    src/GpsInfo.cpp \ 
    src/defines.cpp \
    src/TimeSlot.cpp

linux: {
    HEADERS +=  \
                inc/GpsModeller.h

    SOURCES +=  \
                src/GpsModeller.cpp

    QT -= gui
    LIBS += -lxml2

    DEFINES += SIMULATE_GPS

    INCLUDEPATH += /usr/include/libxml2
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
    CONFIG += mobility
    MOBILITY = location
    LIBS += -lQtLocation
    HEADERS +=  \
               inc/MobilityGps.h

    SOURCES +=  \
               src/MobilityGps.cpp
    DEFINES += NO_GPS SIMULATE_GPS
}

webside: {
    HEADERS +=  \
                inc/GpsModeller.h

    SOURCES +=  \
                src/GpsModeller.cpp

    LIBS += -lxml2 -lodbc

    DEFINES += NO_GPS SIMULATE_GPS
}
