TEMPLATE = lib
include(../../config.pri)

TARGET = ../lib/wikigpsJson
DEPENDPATH += . inc src

target.path = /usr/lib
INSTALLS += target

INCLUDEPATH += . inc \
                 ../common/inc/

# Input
HEADERS += \
           inc/AvailableChannelsResponseJSON.h \
           inc/AvailableChannelsRequestJSON.h \
           inc/LoadTagsResponseJSON.h \
           inc/LoadTagsRequestJSON.h \
           inc/SubscribeChannelJSON.h \
           inc/UnsubscribeChannelRequestJSON.h \
           inc/JsonUser.h \
           inc/JsonDataMark.h \
           inc/JsonChannel.h \
           inc/JsonTimeSlot.h \
           inc/JsonSerializer.h \
           inc/LoginRequestJSON.h \
           inc/LoginResponseJSON.h \
           inc/AddUserRequestJSON.h \
           inc/AddUserResponseJSON.h \
           inc/WriteTagRequestJSON.h \
           inc/WriteTagResponseJSON.h \
           inc/SubscribeChannelResponseJSON.h \
           inc/UnsubscribeChannelResponseJSON.h \
           inc/SubscribedChannelsRequestJSON.h \
           inc/SubscribedChannelsResponseJSON.h \
           inc/AddChannelRequestJSON.h \
           inc/AddChannelResponseJSON.h \
           inc/GetTimeSlotRequestJSON.h \
           inc/GetTimeSlotResponseJSON.h \
           inc/DefaultResponseJSON.h \
           inc/SetTimeSlotRequestJSON.h \
           inc/SetTimeSlotResponseJSON.h \
           inc/GetTimeSlotMarkRequestJSON.h \
           inc/GetTimeSlotMarkResponseJSON.h \
           inc/SetTimeSlotMarkRequestJSON.h \
           inc/SetTimeSlotMarkResponseJSON.h \
           inc/SetDefaultTimeSlotMarkRequestJSON.h \
           inc/SetDefaultTimeSlotMarkResponseJSON.h \
           inc/SetDefaultTimeSlotRequestJSON.h \
           inc/SetDefaultTimeSlotResponseJSON.h \
           inc/FilterRequestJSON.h \
           inc/FilterDefaultResponseJSON.h \
           inc/FilterCircleRequestJSON.h \ 
           inc/FilterCylinderRequestJSON.h \
           inc/FilterPolygonRequestJSON.h \
           inc/FilterRectangleRequestJSON.h \
           inc/FilterBoxRequestJSON.h \
           inc/FilterFenceRequestJSON.h \
           inc/RegisterUserRequestJSON.h \
           inc/RegisterUserResponseJSON.h \
           inc/ErrnoInfoResponseJSON.h \
	   inc/VersionResponseJSON.h 

SOURCES += \
           src/AvailableChannelsResponseJSON.cpp \
           src/LoadTagsResponseJSON.cpp \
           src/LoadTagsRequestJSON.cpp \
           src/SubscribeChannelJSON.cpp \
           src/JsonUser.cpp \
           src/JsonChannel.cpp \
           src/SubscribedChannelsRequestJSON.cpp \
           src/JsonTimeSlot.cpp \
           src/JsonDataMark.cpp \
           src/JsonSerializer.cpp \
           src/LoginRequestJSON.cpp \
           src/LoginResponseJSON.cpp \
           src/WriteTagRequestJSON.cpp \
           src/AddChannelRequestJSON.cpp \
           src/GetTimeSlotRequestJSON.cpp \
           src/GetTimeSlotResponseJSON.cpp \
           src/DefaultResponseJSON.cpp \
           src/SetTimeSlotRequestJSON.cpp \
           src/GetTimeSlotMarkRequestJSON.cpp \
           src/WriteTagResponseJSON.cpp \
           src/GetTimeSlotMarkResponseJSON.cpp \
           src/SetTimeSlotMarkRequestJSON.cpp \
           src/FilterRequestJSON.cpp \
           src/FilterDefaultResponseJSON.cpp \
           src/FilterCircleRequestJSON.cpp \
           src/FilterCylinderRequestJSON.cpp \
           src/FilterPolygonRequestJSON.cpp \
           src/FilterRectangleRequestJSON.cpp \
           src/FilterBoxRequestJSON.cpp \
           src/FilterFenceRequestJSON.cpp \
           src/RegisterUserRequestJSON.cpp \
           src/RegisterUserResponseJSON.cpp \
           src/ErrnoInfoResponseJSON.cpp \
	   src/VersionResponseJSON.cpp

LIBS    +=  -lcommon -lqjson

linux: {
    DEFINES += DESKTOP_STYLE
}

QT += core
QT -= gui
