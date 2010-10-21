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
         #  mainwindow.cpp \
          # settingsdialog.cpp \
         #  LoginDialog.cpp \
            trackerdaemon.cpp \
           ApplyChannelQuery.cpp \
           RequestReceiver.cpp \
           AddUserQuery.cpp \
           ../gui/src/SubscribeChannelQuery.cpp \
           ../gui/src/SubscribeChannelJSON.cpp \
           ../gui/src/ApplyMarkQuery.cpp \
           ../gui/src/LoginQuery.cpp

HEADERS += mainwindow.h \
            trackerdaemon.h \
          # settingsdialog.h \
        #   LoginDialog.h \
           tracker.h \
           ApplyChannelQuery.h \
           RequestReceiver.h \
           AddUserQuery.h \
           ../common/inc/defines.h \
           ../gui/inc/SubscribeChannelQuery.h \
           ../gui/inc/ApplyMarkQuery.h \
           ../gui/inc/LoginQuery.h

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
