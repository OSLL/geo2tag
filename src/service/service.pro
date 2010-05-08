TEMPLATE = app
include(../../config.pri)
TARGET = fcgi_server
DEPENDPATH += . \
              inc \
              src \
              ../common/inc \
              ../common/src \
              ../common/common/inc \
              ../common/common/src \
              ../common/exception/inc \
              ../common/exception/src \
              ../common/odbc/inc \
              ../common/odbc/src \
              ../common/thread/inc \
              ../common/thread/src

INCLUDEPATH += \
               . \
               inc \
               ../common/inc \
               ../common/odbc/inc \
               ../common/common/inc \
               ../common/exception/inc \
               ../common/thread/inc \
               ../fcgiio/inc \
               ../json/inc

# Input
HEADERS += inc/AvailableListJsonQuery.h \
           inc/UnknownJsonQuery.h \
           inc/SubscribedListJsonQuery.h \
	   inc/SubscribedUsersJsonQuery.h \
           inc/LoginJsonQuery.h \
           inc/SubscribeQuery.h \
           inc/ApplyMarkJsonQuery.h \
           inc/ApplyChannelJsonQuery.h \
           inc/RssFeedJsonQuery.h \
           inc/IJsonQuery.h \
           inc/JsonQueryFactory.h \
           inc/UnsubscribeJsonQuery.h \
           inc/DynamicCastFailure.h \
           inc/engine.h \
           inc/query.h \
           inc/server.h \
           inc/ChannelInternal.h \
           inc/DataMarkInternal.h \
           inc/UserInternal.h \
           inc/DbSession.h 

SOURCES += src/main.cpp \
           src/SubscribedListJsonQuery.cpp \
           src/RssFeedJsonQuery.cpp \
           src/SubscribedUsersJsonQuery.cpp \
           src/AvailableListJsonQuery.cpp \
           src/ApplyMarkJsonQuery.cpp \
           src/ApplyChannelJsonQuery.cpp \
           src/LoginJsonQuery.cpp \
           src/UnknownJsonQuery.cpp \
           src/SubscribeQuery.cpp \
           src/UnsubscribeJsonQuery.cpp \
           src/query.cpp \
           src/server.cpp \
           src/ChannelInternal.cpp \
           src/DataMarkInternal.cpp \
           src/DynamicCastFailure.cpp \
           src/UserInternal.cpp \
           src/DbSession.cpp 
LIBS +=  -lfcgiio -lodbc -lcommon -lfcgi -ljson
OBJECTS_DIR = .obj

QT -= core gui
