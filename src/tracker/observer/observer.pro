TEMPLATE = app

TARGET = wikigps-observer

target.path = /usr/bin
INSTALLS += target

include(../../../config.pri)


QT += network


DEPENDPATH += . \

INCLUDEPATH += . \
               ../../common/inc \
               ../../maps/inc \
               ../../json/inc \
               ../../http_requests/inc

LIBS += -lmaps -lqjson -lcommon -lwikigpsHttp -lwikigpsJson

# Input
SOURCES += main.cpp \
    DataThread.cpp

HEADERS += \
    DataThread.h
