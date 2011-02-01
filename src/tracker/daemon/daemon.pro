TEMPLATE = app

TARGET = wikigpsTracker

target.path = /usr/bin
INSTALLS += target

include(../../../config.pri)


QT += network core
QT -= gui

DEPENDPATH += . \
              ../../common/inc \
               ../../json/inc \
               ../../http_requests/inc

INCLUDEPATH += . \
               ../../common/inc \
               ../../json/inc \
               ../../http_requests/inc

# Input
SOURCES += main.cpp \
           TrackerDaemon.cpp \
           ReportThread.cpp

HEADERS += TrackerDaemon.h \
           tracker.h \
           ReportThread.h \
           Control.h

LIBS += -lqjson -lcommon -lwikigpsHttp -lwikigpsJson

symbian: {
 #   TARGET.UID3 = 0xEEF9CA31

    TARGET.CAPABILITY = NetworkServices \
    Location \
    ReadUserData \
    WriteUserData \
    LocalServices \
    UserEnvironment \
    ReadDeviceData \
    WriteDeviceData
}
