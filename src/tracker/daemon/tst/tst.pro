
INCLUDEPATH += ../inc/ \
  ../../../common/inc/ \
  ../../../http_requests/inc/ \
  ../../../json/inc/ \
  /usr/include/libxml2 \

LIBS    +=  -lqjson

#Add sources below
SOURCES += main.cpp \ 
    ../TrackerDaemon.cpp \
    ../../../http_requests/src/LoginQuery.cpp \
    ../../../common/src/GpsInfo.cpp \
    ../ReportThread.cpp \
    ../../../http_requests/src/DefaultQuery.cpp \
    ../../../http_requests/src/AddNewMarkQuery.cpp \
    ../../../common/src/DataMarks.cpp \
    ../../../common/src/User.cpp \
    ../../../json/src/JsonDataMark.cpp \
    ../../../json/src/JsonChannel.cpp \
    ../../../common/src/Channel.cpp \
    ../../../json/src/LoginResponseJSON.cpp \
    ../../../json/src/LoginRequestJSON.cpp \
    ../../../json/src/JsonSerializer.cpp \
    ../../../json/src/DefaultResponseJSON.cpp \
    ../../../json/src/ChannelListJSON.cpp \
    ../../../json/src/AddNewMarkResponseJSON.cpp \
    ../../../json/src/AddNewMarkRequestJSON.cpp \
    ../../../json/src/AddChannelRequestJSON.cpp \
    ../../../json/src/JsonUser.cpp \
    ../../../common/src/MobilityGps.cpp \
    ../../../common/src/defines.cpp \
    TrackerDaemon_TestObject.cpp
HEADERS += \ 
    TrackerDaemon_Test.h \
    ../TrackerDaemon.h \
    ../../../common/inc/GpsInfo.h \
    ../../../http_requests/inc/LoginQuery.h \
    ../ReportThread.h \
    ../../../http_requests/inc/DefaultQuery.h \
    ../../../http_requests/inc/AddNewMarkQuery.h \
    ../../../common/inc/DataMarks.h \
    ../../../common/inc/User.h \
    ../../../common/inc/Channel.h \
    ../../../json/inc/JsonSerializer.h \
    ../../../json/inc/JsonDataMark.h \
    ../../../json/inc/JsonChannel.h \
    ../../../json/inc/AddNewMarkResponseJSON.h \
    ../../../json/inc/AddNewMarkRequestJSON.h \
    ../../../json/inc/AddChannelResponseJSON.h \
    ../../../json/inc/AddChannelRequestJSON.h \
    ../../../common/inc/defines.h \
    ../../../json/inc/LoginResponseJSON.h \
    ../../../common/inc/MobilityGps.h \
    TrackerDaemon_TestObject.h

CONFIG   += qtestlib mobility

MOBILITY = location

TEMPLATE = app

QT += testlib network

TARGET = test.suite
