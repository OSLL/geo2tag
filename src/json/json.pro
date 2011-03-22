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
           inc/ChannelListJSON.h \
           inc/RSSFeedJSON.h \
           inc/RSSFeedRequestJSON.h \
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
	   inc/AddNewMarkRequestJSON.h \
	   inc/AddNewMarkResponseJSON.h \
	   inc/SubscribeChannelResponseJSON.h \
	   inc/UnsubscribeChannelResponse.h \
		 inc/AddChannelRequestJSON.h \
		 inc/AddChannelResponseJSON.h \
	   inc/DefaultResponseJSON.h

SOURCES += \
           src/ChannelListJSON.cpp \
           src/RSSFeedJSON.cpp \
           src/RSSFeedRequestJSON.cpp \
           src/SubscribeChannelJSON.cpp \
           src/JsonUser.cpp \
           src/JsonChannel.cpp \
           src/JsonDataMark.cpp \
           src/JsonSerializer.cpp \
           src/LoginRequestJSON.cpp \
           src/LoginResponseJSON.cpp \
           src/AddNewMarkRequestJSON.cpp \
					 src/AddChannelRequestJSON.cpp \
           src/DefaultResponseJSON.cpp

LIBS    +=  -lcommon -lqjson

linux: {
    DEFINES += DESKTOP_STYLE
}

QT += core
QT -= gui
