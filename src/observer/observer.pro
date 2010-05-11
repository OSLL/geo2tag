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
    MapLoader.cpp \
    MapLoaderFactory.cpp \
    ../gui/src/RSSFeedQuery.cpp \
    ../gui/src/RSSFeedJSON.cpp \
    ../gui/src/LoginQuery.cpp \
    Picture.cpp  
FORMS += ObserverForm.ui
HEADERS += observer.h \
    MapLoaderFactory.h \
    MapLoader.h \
    GoogleMapLoader.h \
    CurlInitException.h \
    Picture.h \
    ../gui/inc/RSSFeedQuery.h \
    ../gui/inc/RSSFeedJSON.h \
    ../gui/inc/LoginQuery.h \
    defs.h

LIBS+= -L/home/vood/geo2tag/trunk/src/lib/ -lcommon -lqjson -lcurl -lfcgiio -ljson

QT += core gui network
