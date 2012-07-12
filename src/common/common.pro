TEMPLATE = lib

include(../../config.pri)

TARGET = ../lib/common
target.path = /usr/lib

INSTALLS += target geo2tag_conf

geo2tag_conf.files=geo2tag.conf
geo2tag_conf.path=/opt/geo2tag

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
    inc/Channel.h \
    inc/User.h \
    inc/DataMarks.h \
    inc/GpsInfo.h \
    inc/defines.h \
    inc/DataChannel.h\
    inc/ChannelAction.h\
    inc/ErrnoTypes.h \
    inc/Filtration.h \
    inc/Filter.h \
    inc/ShapeFilter.h \
    inc/TimeFilter.h \
    inc/AltitudeFilter.h \    
    inc/FShape.h \
    inc/FShapeCircle.h \
    inc/FShapePolygon.h \
    inc/FShapeRectangle.h \
    inc/Region.h \
    inc/signals.h \
    inc/PerformanceCounter.h \
    inc/SettingsStorage.h \
    inc/Session.h \
    inc/EmailMessage.h \
    inc/BasicUser.h

SOURCES += \
    src/Channel.cpp \
    src/User.cpp \
    src/ChannelAction.cpp \
    src/GpsInfo.cpp \ 
    src/DataMarks.cpp \
    src/defines.cpp \
    src/Filtration.cpp \
    src/ShapeFilter.cpp \
    src/TimeFilter.cpp \
    src/AltitudeFilter.cpp \    
    src/FShapeCircle.cpp \
    src/FShapePolygon.cpp \
    src/FShapeRectangle.cpp \
    src/Region.cpp \ 
    src/signals.cpp \
    src/PerformanceCounter.cpp \
    src/SettingsStorage.cpp \
    src/Session.cpp \
    src/EmailMessage.cpp \
    src/BasicUser.cpp

linux: {
    HEADERS += inc/GpsModeller.h

    SOURCES += src/GpsModeller.cpp

    LIBS += -lxml2

    DEFINES += SIMULATE_GPS


    INCLUDEPATH += /usr/include/libxml2
}

maemo5: {

    HEADERS += inc/MobilityGps.h

    SOURCES += src/MobilityGps.cpp
    LIBS += -lQtLocation
}


webside: {
    HEADERS += inc/GpsModeller.h

    SOURCES += src/GpsModeller.cpp

    LIBS += -lxml2 -lodbc

    DEFINES += NO_GPS SIMULATE_GPS
}
