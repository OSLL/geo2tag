TEMPLATE = lib
include(../../config.pri)

TARGET = ../lib/wikigpsHttp-standalone
DEPENDPATH += . inc src ./json/inc ./json/src ./common/inc ./common/src

target.path = /usr/lib
INSTALLS += target

INCLUDEPATH += . inc ../common/inc

# Input
HEADERS += \
           inc/LoginQuery.h \
           inc/DefaultQuery.h \
           inc/LoadTagsQuery.h \
           inc/WriteTagQuery.h \
           inc/AddUserQuery.h \
           inc/ApplyChannelQuery.h \
           inc/SubscribeChannelQuery.h \
           inc/RegisterUserQuery.h \
           inc/AvailableChannelsQuery.h \
           inc/SubscribedChannelsQuery.h \
           inc/VersionQuery.h \
           json/inc/AvailableChannelsResponseJSON.h \
           json/inc/AvailableChannelsRequestJSON.h \
           json/inc/LoadTagsResponseJSON.h \
           json/inc/LoadTagsRequestJSON.h \
           json/inc/SubscribeChannelJSON.h \
           json/inc/UnsubscribeChannelRequestJSON.h \
           json/inc/JsonUser.h \
           json/inc/JsonDataMark.h \
           json/inc/JsonChannel.h \
           json/inc/JsonSerializer.h \
           json/inc/LoginRequestJSON.h \
           json/inc/LoginResponseJSON.h \
           json/inc/AddUserRequestJSON.h \
           json/inc/AddUserResponseJSON.h \
           json/inc/WriteTagRequestJSON.h \
           json/inc/WriteTagResponseJSON.h \
           json/inc/SubscribeChannelResponseJSON.h \
           json/inc/UnsubscribeChannelResponseJSON.h \
           json/inc/SubscribedChannelsRequestJSON.h \
           json/inc/SubscribedChannelsResponseJSON.h \
           json/inc/AddChannelRequestJSON.h \
           json/inc/AddChannelResponseJSON.h \
           json/inc/DefaultResponseJSON.h \
           json/inc/FilterRequestJSON.h \
           json/inc/FilterDefaultResponseJSON.h \
           json/inc/FilterCircleRequestJSON.h \
           json/inc/FilterCylinderRequestJSON.h \
           json/inc/FilterPolygonRequestJSON.h \
           json/inc/FilterRectangleRequestJSON.h \
           json/inc/FilterBoxRequestJSON.h \
           json/inc/FilterFenceRequestJSON.h \
           json/inc/RegisterUserRequestJSON.h \
           json/inc/RegisterUserResponseJSON.h \
           json/inc/ErrnoInfoResponseJSON.h \
           json/inc/VersionResponseJSON.h \
           json/inc/FilterChannelRequestJSON.h \
           json/inc/FilterChannelResponseJSON.h \
           json/inc/DeleteUserRequestJSON.h \
           json/inc/DeleteUserResponseJSON.h \
           json/inc/BuildResponseJSON.h \
           json/inc/QuitSessionRequestJSON.h \
           json/inc/QuitSessionResponseJSON.h \
           json/inc/JsonSession.h \
           common/inc/ConcurrentVector.h \
           common/inc/Channel.h \
           common/inc/User.h \
           common/inc/DataMarks.h \
           common/inc/defines.h \
           common/inc/DataChannel.h\
           common/inc/FShape.h \
           common/inc/FShapeCircle.h \
           common/inc/FShapePolygon.h \
           common/inc/FShapeRectangle.h \
           common/inc/SettingsStorage.h \
           common/inc/Session.h \
           common/inc/signals.h \

SOURCES += \
           src/LoginQuery.cpp \
           src/DefaultQuery.cpp \
           src/LoadTagsQuery.cpp \
           src/WriteTagQuery.cpp \
           src/AddUserQuery.cpp \
           src/ApplyChannelQuery.cpp \
           src/SubscribeChannelQuery.cpp \
           src/SubscribedChannelsQuery.cpp \
           src/RegisterUserQuery.cpp \
           src/VersionQuery.cpp \
           json/src/AvailableChannelsResponseJSON.cpp \
           json/src/LoadTagsResponseJSON.cpp \
           json/src/LoadTagsRequestJSON.cpp \
           json/src/SubscribeChannelJSON.cpp \
           json/src/JsonUser.cpp \
           json/src/JsonChannel.cpp \
           json/src/SubscribedChannelsRequestJSON.cpp \
           json/src/JsonDataMark.cpp \
           json/src/JsonSerializer.cpp \
           json/src/LoginRequestJSON.cpp \
           json/src/LoginResponseJSON.cpp \
           json/src/WriteTagRequestJSON.cpp \
           json/src/AddChannelRequestJSON.cpp \
           json/src/DefaultResponseJSON.cpp \
           json/src/WriteTagResponseJSON.cpp \
           json/src/FilterRequestJSON.cpp \
           json/src/FilterDefaultResponseJSON.cpp \
           json/src/FilterCircleRequestJSON.cpp \
           json/src/FilterCylinderRequestJSON.cpp \
           json/src/FilterPolygonRequestJSON.cpp \
           json/src/FilterRectangleRequestJSON.cpp \
           json/src/FilterBoxRequestJSON.cpp \
           json/src/FilterFenceRequestJSON.cpp \
           json/src/RegisterUserRequestJSON.cpp \
           json/src/RegisterUserResponseJSON.cpp \
           json/src/ErrnoInfoResponseJSON.cpp \
           json/src/VersionResponseJSON.cpp \
           json/src/AddUserRequestJSON.cpp \
           json/src/FilterChannelRequestJSON.cpp \
           json/src/FilterChannelResponseJSON.cpp \
           json/src/QuitSessionRequestJSON.cpp \
           json/src/QuitSessionResponseJSON.cpp \
           json/src/JsonSession.cpp \
           common/src/Channel.cpp \
           common/src/User.cpp \
           common/src/DataMarks.cpp \
           common/src/defines.cpp \
           common/src/FShapeCircle.cpp \
           common/src/FShapePolygon.cpp \
           common/src/FShapeRectangle.cpp \
           common/src/SettingsStorage.cpp \
           common/src/Session.cpp \
           common/src/signals.cpp \

linux: {
    DEFINES += DESKTOP_STYLE
}
config+=network
QT += core network
QT += gui
