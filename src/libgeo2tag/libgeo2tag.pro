TEMPLATE = lib
CONFIG += console
CONFIG -= qt

TARGET = geo2tag

HEADERS += \
    inc/session.h \
    inc/channel.h \
    inc/geotag.h

SOURCES += \
    src/session.cpp \
    src/channel.cpp \
    src/geotag.cpp


