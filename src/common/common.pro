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


CONFIG += mobility
MOBILITY = location


# Input
HEADERS += \
    inc/ConcurrentVector.h \
    inc/TimeSlot.h \
    inc/Channel.h \
    inc/User.h \
    inc/DataMarks.h \
    inc/GpsInfo.h \
    inc/defines.h \
    inc/DataChannel.h \
    inc/TimeSlot.h \
    inc/ChannelAction.h\
		inc/DataMarks.h

SOURCES += \
    src/TimeSlot.cpp \
    src/Channel.cpp \
    src/User.cpp \
    src/ChannelAction.cpp \
    src/DataMarks.cpp \
    src/GpsInfo.cpp \ 
    src/defines.cpp

linux: {
    HEADERS += inc/GpsModeller.h

    SOURCES += src/GpsModeller.cpp

    QT -= gui
    LIBS += -lxml2

    DEFINES += SIMULATE_GPS

    INCLUDEPATH += /usr/include/libxml2
}

maemo5: {

    HEADERS +=  \
               inc/MobilityGps.h

    SOURCES +=  \
               src/MobilityGps.cpp
    SOURCES += src/MobilityGps.cpp
    LIBS += -lQtLocation
}

symbian: {
    CONFIG += mobility

    MOBILITY = location

    LIBS += -lQtLocation

    HEADERS += inc/MobilityGps.h

    SOURCES += src/MobilityGps.cpp

    DEFINES += NO_GPS SIMULATE_GPS
}

webside: {
    HEADERS += inc/GpsModeller.h

    SOURCES += src/GpsModeller.cpp

    LIBS += -lxml2 -lodbc

    DEFINES += NO_GPS SIMULATE_GPS
}
