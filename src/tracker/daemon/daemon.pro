TEMPLATE = app
TARGET = tracker

include(../../../config.pri)

target.path = /usr/bin
INSTALLS += target

QT += network


DEPENDPATH += . \
              ./requests \ 
              ../../gui/inc \
              ../../common/inc \
              ../../gui/src
	      
INCLUDEPATH += . \
               ./requests \
               ../../gui/inc \
               ../../common/inc \
               ../common/common/inc \
               ../common/thread/inc \
               ../common/exception/inc \
               ../internal/include


# Input
SOURCES += main.cpp \
           TrackerDaemon.cpp \
           ApplyChannelQuery.cpp \
           RequestReceiver.cpp \
           AddUserQuery.cpp \
           ../../gui/src/SubscribeChannelQuery.cpp \
           ../../gui/src/SubscribeChannelJSON.cpp \
           ../../gui/src/ApplyMarkQuery.cpp \
           ../../gui/src/LoginQuery.cpp \

HEADERS += TrackerDaemon.h \
           tracker.h \
           ApplyChannelQuery.h \
           RequestReceiver.h \
           AddUserQuery.h \
           ../../common/inc/defines.h \
           ../../gui/inc/SubscribeChannelQuery.h \
           ../../gui/inc/ApplyMarkQuery.h \
           ../../gui/inc/LoginQuery.h \
           Status.h \
           ../../gui/inc/LoginQuery.h \


LIBS += -lqjson -lcommon

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
