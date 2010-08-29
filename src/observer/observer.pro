include(../../config.pri)

INCLUDEPATH += . inc \
    ../common/inc \
    ../common/common/inc \
    ../common/thread/inc \
    ../common/exception/inc \
    ../gui/inc \
    ../fcgiio/inc \
    ../json/inc

#SUBDIRS= ../common

SOURCES += main.cpp \
    observer.cpp \
    CurlInitException.cpp \
    GoogleMapLoader.cpp \
    MapWidget.cpp \
    MapLoaderFactory.cpp \
    ../gui/src/RSSFeedQuery.cpp \
    ../gui/src/RSSFeedJSON.cpp \
    ../gui/src/LoginQuery.cpp \
    ../gui/src/OnLineInformation.cpp \
    ../gui/src/ApplyMarkQuery.cpp\
    ../gui/src/AvailableChannelsListQuery.cpp \
    ../gui/src/SubscribeChannelQuery.cpp \
    ../gui/src/UnsubscribeChannelQuery.cpp \
    ../gui/src/SubscribedChannelsListQuery.cpp \
    ../gui/src/ChannelListJSON.cpp \ 
    ../gui/src/SubscribeChannelJSON.cpp \
    Picture.cpp  
FORMS += ObserverForm.ui
HEADERS += observer.h \
    MapLoaderFactory.h \
    MapLoader.h \
    GoogleMapLoader.h \
    CurlInitException.h \
    Picture.h \
    MapWidget.h \
    ../gui/inc/RSSFeedQuery.h \
    ../gui/inc/RSSFeedJSON.h \
    ../gui/inc/LoginQuery.h \
    ../gui/inc/OnLineInformation.h \
    ../gui/inc/ApplyMarkQuery.h \
    ../gui/inc/AvailableChannelsListQuery.h \
    ../gui/inc/SubscribeChannelQuery.h \
    ../gui/inc/UnsubscribeChannelQuery.h \
    ../gui/inc/SubscribedChannelsListQuery.h \
    ../gui/inc/ChannelListJSON.h \
    ../gui/inc/SubscribeChannelJSON.h \

LIBS+= -lcommon -lqjson -lcurl -lfcgiio -ljson
QT += core gui network
