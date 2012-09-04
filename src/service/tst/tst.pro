include(../../../config.pri)

DEPENDDEPATH = . \
              ../inc/ \ 
              ../src/ \
              ../../common/inc/ \
              ../../common/src/ \
              ../../json/inc \
              ../../json/src 

INCLUDEPATH = . ../inc/ ../../common/inc/ \
              ../../json/inc

SOURCES += ../src/QueryExecutor.cpp \
           ../src/Geo2tagDatabase.cpp \
           ../src/UpdateThread.cpp \
           ../src/UserInternal.cpp \
           ../src/DataMarkInternal.cpp \
           ../src/SessionInternal.cpp \
           ../src/ChannelInternal.cpp \
           main.cpp \
           Test_QueryExecutor.cpp

HEADERS += ../inc/QueryExecutor.h \
           ../inc/Geo2tagDatabase.h \
           ../inc/UpdateThread.h \
           ../inc/UserInternal.h \
           ../inc/DataMarkInternal.h \
           ../inc/SessionInternal.h \
           ../inc/ChannelInternal.h \
           Test_QueryExecutor.h \ 
	   UpdateThread_Test.h	   

CONFIG   += qtestlib

TEMPLATE = app

QT += testlib
QT += sql


## GT_779 This is not correct test format for pulse. refactoring is required
TARGET = test.DbInteraction



LIBS -= -L/usr/lib
LIBS += -lcommon -lwikigpsJson
message($$LIBS)
