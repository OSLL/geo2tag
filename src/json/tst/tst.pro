INCLUDEPATH = ../inc/ ../../common/inc/ \
              ../../json/inc/ \

SOURCES += main.cpp \
    ../src/JsonUser.cpp \
    ../../common/src/User.cpp \
    ../../json/src/JsonSerializer.cpp \
    ../../json/src/JsonChannel.cpp \
    ../../json/src/RegisterUserRequestJSON.cpp \
    ../../json/src/RegisterUserResponseJSON.cpp \
    ../../json/src/AvailableChannelsResponseJSON.cpp \
    Test_RegisterUserResponseJSON.cpp \
    Test_RegisterUserRequestJSON.cpp \
    Test_AvailableChannelsResponseJSON.cpp
HEADERS += JsonUser_Test.h \
    ../inc/JsonUser.h \
    ../../common/inc/User.h \
    ../../json/inc/JsonChannel.h \
    ../../json/inc/JsonSerializer.h \
    ../../json/inc/RegisterUserRequestJSON.h \
    ../../json/inc/RegisterUserResponseJSON.h \
    ../../json/inc/AvailableChannelsResponseJSON.h \
    Test_RegisterUserResponseJSON.h \
    Test_RegisterUserRequestJSON.h \
    Test_AvailableChannelsResponseJSON.h

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib

TARGET = test.suite

LIBS    +=  -lcommon -lqjson
