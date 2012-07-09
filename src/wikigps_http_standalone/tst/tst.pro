include(../../../config.pri)
INCLUDEPATH =  .. ../inc/ ../../common/inc

SOURCES += \
    main.cpp \
    ../src/AddUserQuery.cpp \
    ../src/RegisterUserQuery.cpp \
    ../src/LoginQuery.cpp \
    ../src/DefaultQuery.cpp \
    ../src/VersionQuery.cpp \
    ../src/ApplyChannelQuery.cpp \
    ../src/SubscribedChannelsQuery.cpp \
    ../src/SubscribeChannelQuery.cpp \
    ../src/WriteTagQuery.cpp \
    ../src/LoadTagsQuery.cpp \
    ../json/src/JsonUser.cpp \
    ../json/src/JsonChannel.cpp \
    ../json/src/JsonSession.cpp \
    ../json/src/JsonDataMark.cpp \
    ../json/src/JsonSerializer.cpp \
    ../json/src/AddUserRequestJSON.cpp \
    ../json/src/LoginRequestJSON.cpp \
    ../json/src/LoginResponseJSON.cpp \
    ../json/src/RegisterUserRequestJSON.cpp \
    ../json/src/RegisterUserResponseJSON.cpp \
    ../json/src/AvailableChannelsResponseJSON.cpp \
    ../json/src/SubscribedChannelsRequestJSON.cpp \
    ../json/src/SubscribeChannelJSON.cpp \
    ../json/src/AddChannelRequestJSON.cpp \
    ../json/src/WriteTagRequestJSON.cpp \
    ../json/src/WriteTagResponseJSON.cpp \
    ../json/src/LoadTagsRequestJSON.cpp \
    ../json/src/LoadTagsResponseJSON.cpp \
    ../json/src/DefaultResponseJSON.cpp \
    ../json/src/VersionResponseJSON.cpp \
    ../common/src/signals.cpp \
    ../common/src/session.cpp \
    ../common/src/User.cpp \
    ../common/src/Channel.cpp \
    ../common/src/DataMarks.cpp \
    Test_RegisterUserQuery.cpp \
    Test_AddUserQuery.cpp \
    Test_ApplyChannelQuery.cpp \
    Test_AvailableChannelsQuery.cpp \
    Test_SubscribeChannelQuery.cpp \
    Test_SubscribedChannelsQuery.cpp \
    Test_WriteTagQuery.cpp \
    Test_LoginQuery.cpp \
    Test_LoadTagsQuery.cpp

HEADERS += \
    ../inc/AddUserQuery.h \
    ../inc/RegisterUserQuery.h \
    ../inc/LoginQuery.h \
    ../inc/DefaultQuery.h \
    ../inc/VersionQuery.h \
    ../inc/ApplyChannelQuery.h \
    ../inc/AvailableChannelsQuery.h \
    ../inc/SubscribedChannelsQuery.h \
    ../inc/SubscribeChannelQuery.h \
    ../inc/WriteTagQuery.h \
    ../inc/LoadTagsQuery.h \
    ../json/inc/JsonUser.h \
    ../json/inc/JsonChannel.h \
    ../json/inc/JsonSession.h \
    ../json/inc/JsonDataMark.h \
    ../json/inc/JsonSerializer.h \
    ../json/inc/AddUserRequestJSON.h \
    ../json/inc/AddUserResponseJSON.h \
    ../json/inc/LoginRequestJSON.h \
    ../json/inc/LoginResponseJSON.h \
    ../json/inc/RegisterUserRequestJSON.h \
    ../json/inc/RegisterUserResponseJSON.h \
    ../json/inc/AddChannelRequestJSON.h \
    ../json/inc/AddChannelResponseJSON.h \
    ../json/inc/AvailableChannelsRequestJSON.h \
    ../json/inc/AvailableChannelsResponseJSON.h \
    ../json/inc/SubscribedChannelsRequestJSON.h \
    ../json/inc/SubscribeChannelResponseJSON.h \
    ../json/inc/WriteTagRequestJSON.h \
    ../json/inc/WriteTagResponseJSON.h \
    ../json/inc/LoadTagsRequestJSON.h \
    ../json/inc/LoadTagsResponseJSON.h \
    ../json/inc/VersionResponseJSON.h \
    ../json/inc/DefaultResponseJSON.h \
    ../common/inc/signals.h \
    ../common/inc/session.h \
    ../common/inc/User.h \
    ../common/inc/Channel.h \
    ../common/inc/DataMarks.h \
    VersionQuery_Test.h \
    Test_RegisterUserQuery.h \
    Test_AddUserQuery.h \
    Test_ApplyChannelQuery.h \
    Test_AvailableChannelsQuery.h \
    Test_SubscribeChannelQuery.h \
    Test_SubscribedChannelsQuery.h \
    Test_WriteTagQuery.h \
    Test_LoginQuery.h \
    Test_LoadTagsQuery.h

CONFIG += qtestlib debug

TEMPLATE = app

QT += testlib network
Qt -= gui

TARGET = test.suite

LIBS += -lcommon -lqjson
