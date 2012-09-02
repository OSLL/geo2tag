include(../../../config.pri)

INCLUDEPATH = ../inc/ ../../common/inc/ \
              ../../json/inc/ \

SOURCES += main.cpp \
    ../src/JsonUser.cpp \
    ../../common/src/BasicUser.cpp \
    ../../common/src/User.cpp \
    ../../json/src/JsonSerializer.cpp \
    ../../json/src/JsonChannel.cpp \
    ../../json/src/RegisterUserRequestJSON.cpp \
    ../../json/src/RegisterUserResponseJSON.cpp \
    ../../json/src/AvailableChannelsResponseJSON.cpp \
    ../../json/src/QuitSessionRequestJSON.cpp \
    ../../json/src/QuitSessionResponseJSON.cpp \
    ../../json/src/RestorePasswordRequestJSON.cpp \
    ../../json/src/DefaultResponseJSON.cpp \
    Test_RegisterUserResponseJSON.cpp \
    Test_RegisterUserRequestJSON.cpp \
    Test_AvailableChannelsResponseJSON.cpp \
    Test_QuitSessionRequestJSON.cpp \
    Test_QuitSessionResponseJSON.cpp \
    Test_RestorePasswordRequestJSON.cpp \
    Test_RestorePasswordResponseJSON.cpp
HEADERS += JsonUser_Test.h \
    ../inc/JsonUser.h \
    ../../common/inc/BasicUser.h \
    ../../common/inc/User.h \
    ../../json/inc/JsonChannel.h \
    ../../json/inc/JsonSerializer.h \
    ../../json/inc/RegisterUserRequestJSON.h \
    ../../json/inc/RegisterUserResponseJSON.h \
    ../../json/inc/AvailableChannelsResponseJSON.h \
    ../../json/inc/QuitSessionRequestJSON.h \
    ../../json/inc/QuitSessionResponseJSON.h \
    ../../json/inc/RestorePasswordRequestJSON.h \
    ../../json/inc/RestorePasswordResponseJSON.h \
    ../../json/inc/DefaultResponseJSON.h \
    Test_RegisterUserResponseJSON.h \
    Test_RegisterUserRequestJSON.h \
    Test_AvailableChannelsResponseJSON.h \
    Test_QuitSessionResponseJSON.h \
    Test_QuitSessionRequestJSON.h \
    Test_RestorePasswordResponseJSON.h \
    Test_RestorePasswordRequestJSON.h

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib

TARGET = test.suite

LIBS    +=  -lcommon -lqjson
