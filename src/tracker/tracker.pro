TEMPLATE = app
TARGET = tracker

include(../../config.pri)

target.path = /usr/bin
INSTALLS += target

QT += network

CONFIG += mobility

MOBILITY = location \
           bearer

DEPENDPATH += . \
              ./requests \
              ../gui/inc \
              ../gui/src
	      
INCLUDEPATH += . \
               ./requests \
               ../gui/inc \
               ../common/inc

# Input
SOURCES += main.cpp \
            trackerdaemon.cpp \
           ApplyChannelQuery.cpp \
           RequestReceiver.cpp \
           AddUserQuery.cpp \
           ../gui/src/SubscribeChannelQuery.cpp \
           ../gui/src/SubscribeChannelJSON.cpp \
           ../gui/src/ApplyMarkQuery.cpp \
           ../gui/src/LoginQuery.cpp \
RequestSender.cpp \
    TrackerGUI.cpp \
    TitleWidget.cpp \
    OptionsWidget.cpp \
    LogWidget.cpp

HEADERS += trackerdaemon.h \
           tracker.h \
           ApplyChannelQuery.h \
           RequestReceiver.h \
           AddUserQuery.h \
           ../common/inc/defines.h \
           ../gui/inc/SubscribeChannelQuery.h \
           ../gui/inc/ApplyMarkQuery.h \
           ../gui/inc/LoginQuery.h \
           RequestSender.h \
           Status.h \
           ../gui/inc/LoginQuery.h \
    TrackerGUI.h \
    TitleWidget.h \
    OptionsWidget.h \
    LogWidget.h

#FORMS += formCreateChannel.ui

LIBS += -lqjson

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
