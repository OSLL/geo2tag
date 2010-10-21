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
           mainwindow.cpp \
           settingsdialog.cpp \
           LoginDialog.cpp \
           ApplyChannelQuery.cpp \
           AddUserQuery.cpp \
           RequestReceiver.cpp \
           ../gui/src/SubscribeChannelQuery.cpp \
           ../gui/src/SubscribeChannelJSON.cpp \
           ../gui/src/ApplyMarkQuery.cpp \
           ../gui/src/LoginQuery.cpp \
    TrackerRequests/BaseRequest.cpp \
    TrackerRequests/GetStatus.cpp \
    TrackerRequests/Login.cpp \
    TrackerRequests/Receiver.cpp \
    RequestSender.cpp

HEADERS += mainwindow.h \
           settingsdialog.h \
           LoginDialog.h \
           tracker.h \
           ApplyChannelQuery.h \
           AddUserQuery.h \
           RequestReceiver.h \
           ../common/inc/defines.h \
           ../gui/inc/SubscribeChannelQuery.h \
           ../gui/inc/ApplyMarkQuery.h \
           ../gui/inc/LoginQuery.h \
    TrackerRequests/BaseRequest.h \
    TrackerRequests/GetStatus.h \
    TrackerRequests/Login.h \
    TrackerRequests/Receiver.h \
    RequestSender.h

FORMS += formCreateChannel.ui

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
