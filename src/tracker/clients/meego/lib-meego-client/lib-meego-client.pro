TEMPLATE = lib

TARGET = meego-client

target.path = /usr/lib
INSTALLS += target

CONFIG += mobility debug
MOBILITY += location

QT += network
CONFIG += network
#QT -= gui

VERSION = 0.0.1
LIBS+= -lqjson -lQtLocation
DEPENDPATH += .
INCLUDEPATH += . \
	       ../../../../http_requests/inc \
               ../../../../json/inc \
               ../../../../common/inc

HEADERS += MeegoClient.h \
../../../../http_requests/inc/LoginQuery.h \
../../../../http_requests/inc/DefaultQuery.h \
../../../../http_requests/inc/AddNewMarkQuery.h \
../../../../json/inc/JsonSerializer.h \
../../../../json/inc/LoginResponseJSON.h \
../../../../json/inc/LoginRequestJSON.h \
../../../../json/inc/JsonUser.h \
../../../../json/inc/JsonTimeSlot.h \
../../../../json/inc/JsonDataMark.h \
../../../../json/inc/JsonChannel.h \
../../../../json/inc/AddNewMarkResponseJSON.h \
../../../../json/inc/AddNewMarkRequestJSON.h \
../../../../json/inc/DefaultResponseJSON.h \
../../../../common/inc/User.h \
../../../../common/inc/TimeSlot.h \
../../../../common/inc/DataMarks.h \
../../../../common/inc/Channel.h \
../../../../common/inc/ConcurrentVector.h \
../../../../common/inc/defines.h \
../../../../common/inc/DataChannel.h \
../../../../common/inc/GpsInfo.h \
../../../../common/inc/MobilityGps.h \ 
    markshistory.h

SOURCES += MeegoClient.cpp \
../../../../http_requests/src/LoginQuery.cpp \
../../../../http_requests/src/DefaultQuery.cpp \
../../../../http_requests/src/AddNewMarkQuery.cpp \
../../../../common/src/User.cpp \
../../../../common/src/TimeSlot.cpp \
../../../../common/src/DataMarks.cpp \
../../../../common/src/Channel.cpp \
../../../../common/src/GpsInfo.cpp \
../../../../common/src/MobilityGps.cpp \
../../../../common/src/defines.cpp \
../../../../json/src/JsonSerializer.cpp \
../../../../json/src/LoginResponseJSON.cpp \
../../../../json/src/LoginRequestJSON.cpp \
../../../../json/src/JsonUser.cpp \
../../../../json/src/AddNewMarkResponseJSON.cpp \
../../../../json/src/AddNewMarkRequestJSON.cpp \
../../../../json/src/JsonDataMark.cpp \
../../../../json/src/JsonChannel.cpp \
    markshistory.cpp





