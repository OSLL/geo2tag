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
           inc/DefaultResponseJSON.h \
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
           inc/VersionResponseJSON.h \
           inc/FilterChannelRequestJSON.h \
           inc/FilterChannelResponseJSON.h \
	   inc/DeleteUserRequestJSON.h \
	   inc/DeleteUserResponseJSON.h \
	   inc/BuildResponseJSON.h \ 
    inc/QuitSessionRequestJSON.h \
    inc/QuitSessionResponseJSON.h \
    inc/JsonSession.h \
    inc/OwnedChannelsRequest.h \
    inc/OwnedChannelsResponse.h \
    inc/RestorePasswordRequestJSON.h \
    inc/RestorePasswordResponseJSON.h

SOURCES += \
           src/AvailableChannelsResponseJSON.cpp \
           src/LoadTagsResponseJSON.cpp \
           src/LoadTagsRequestJSON.cpp \
           src/SubscribeChannelJSON.cpp \
           src/JsonUser.cpp \
           src/JsonChannel.cpp \
           src/SubscribedChannelsRequestJSON.cpp \
           src/JsonDataMark.cpp \
           src/JsonSerializer.cpp \
           src/LoginRequestJSON.cpp \
           src/LoginResponseJSON.cpp \
           src/WriteTagRequestJSON.cpp \
           src/AddChannelRequestJSON.cpp \
           src/DefaultResponseJSON.cpp \
           src/WriteTagResponseJSON.cpp \
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
           src/VersionResponseJSON.cpp \
           src/AddUserRequestJSON.cpp \
           src/FilterChannelRequestJSON.cpp \
           src/FilterChannelResponseJSON.cpp \             
    src/QuitSessionRequestJSON.cpp \
    src/QuitSessionResponseJSON.cpp \
    src/JsonSession.cpp \
    src/RestorePasswordRequestJSON.cpp
           

LIBS    +=  -lcommon -lqjson

linux: {
    DEFINES += DESKTOP_STYLE
}

QT += core
QT -= gui
