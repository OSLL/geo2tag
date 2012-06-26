include(../../../config.pri)

INCLUDEPATH = ../inc/ ../../common/inc/ \
              ../../service/inc/ \
              ../../json/inc

SOURCES += ../src/QueryExecutor.cpp \
           #../src/UpdateThread.cpp \
           ../src/Geo2tagDatabase.cpp \
           ../../common/src/User.cpp \
           ../../common/src/Channel.cpp \
           ../../common/src/Datamarks.cpp \
           ../../common/src/Session.cpp \
           ../src/UpdateThread.cpp \
           ../src/UserInternal.cpp \
           ../src/DataMarkInternal.cpp \
           ../src/SessionInternal.cpp \
           ../src/ChannelInternal.cpp \
           main.cpp \
           Test_QueryExecutor.cpp

HEADERS += ../inc/QueryExecutor.h \
           #../inc/UpdateThread.h \
           ../inc/Geo2tagDatabase.h \
           ../../common/inc/User.h \
           ../../common/inc/Channel.h \
           ../../common/inc/Datamarks.h \
           ../../common/inc/Session.h \
           ../inc/UpdateThread.h \
           ../inc/UserInternal.h \
           ../inc/DataMarkInternal.h \
           ../inc/SessionInternal.h \
           ../inc/ChannelInternal.h \
           Test_QueryExecutor.h

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib
QT += sql

TARGET = test.suite

LIBS    +=  -lcommon -lqjson
