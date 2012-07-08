TEMPLATE = lib

TARGET = meego-client

CONFIG += mobility debug network
MOBILITY += location

QT += network
QT -= gui
DEFINES += Q_WS_SYMBIAN

VERSION = 0.0.1
LIBS+= -lQtLocation
DEPENDPATH += .
INCLUDEPATH += . \
               ../../../../../3rdparty/qjson-0.7.1/src/ \
	       ../../../../http_requests/inc \
               ../../../../json/inc \             
               ../../../../common/inc

HEADERS += MeegoClient.h \
../../../../http_requests/inc/LoginQuery.h \
../../../../http_requests/inc/DefaultQuery.h \
../../../../http_requests/inc/WriteTagQuery.h \
../../../../json/inc/JsonSerializer.h \
../../../../json/inc/LoginResponseJSON.h \
../../../../json/inc/LoginRequestJSON.h \
../../../../json/inc/JsonUser.h \
../../../../json/inc/JsonDataMark.h \
../../../../json/inc/JsonChannel.h \
../../../../json/inc/WriteTagResponseJSON.h \
../../../../json/inc/WriteTagRequestJSON.h \
../../../../json/inc/DefaultResponseJSON.h \
../../../../common/inc/User.h \
../../../../common/inc/DataMarks.h \
../../../../common/inc/Channel.h \
../../../../common/inc/ConcurrentVector.h \
../../../../common/inc/defines.h \
../../../../common/inc/DataChannel.h \
../../../../common/inc/GpsInfo.h \
../../../../common/inc/MobilityGps.h \ 
../../../../../3rdparty/qjson-0.7.1/src/parser.h \
../../../../../3rdparty/qjson-0.7.1/src/serializerrunnable.h \
../../../../../3rdparty/qjson-0.7.1/src/serializer.h \
../../../../../3rdparty/qjson-0.7.1/src/qobjecthelper.h \
../../../../../3rdparty/qjson-0.7.1/src/qjson_export.h \
../../../../../3rdparty/qjson-0.7.1/src/qjson_debug.h \
../../../../../3rdparty/qjson-0.7.1/src/parser_p.h \
../../../../../3rdparty/qjson-0.7.1/src/json_scanner.h \
../../../../../3rdparty/qjson-0.7.1/src/parserrunnable.h \
../../../../../3rdparty/qjson-0.7.1/src/json_parser.hh \
../../../../../3rdparty/qjson-0.7.1/src/stack.hh \
../../../../../3rdparty/qjson-0.7.1/src/location.hh \
../../../../../3rdparty/qjson-0.7.1/src/position.hh \
    markshistory.h

SOURCES += MeegoClient.cpp \
../../../../http_requests/src/LoginQuery.cpp \
../../../../http_requests/src/DefaultQuery.cpp \
../../../../http_requests/src/WriteTagQuery.cpp \
../../../../common/src/User.cpp \
../../../../common/src/DataMarks.cpp \
../../../../common/src/Channel.cpp \
../../../../common/src/GpsInfo.cpp \
../../../../common/src/MobilityGps.cpp \
../../../../common/src/defines.cpp \
../../../../json/src/JsonSerializer.cpp \
../../../../json/src/LoginResponseJSON.cpp \
../../../../json/src/LoginRequestJSON.cpp \
../../../../json/src/JsonUser.cpp \
../../../../json/src/WriteTagResponseJSON.cpp \
../../../../json/src/WriteTagRequestJSON.cpp \
../../../../json/src/JsonDataMark.cpp \
../../../../json/src/JsonChannel.cpp \
../../../../../3rdparty/qjson-0.7.1/src/serializerrunnable.cpp \
../../../../../3rdparty/qjson-0.7.1/src/serializer.cpp \
../../../../../3rdparty/qjson-0.7.1/src/qobjecthelper.cpp \
../../../../../3rdparty/qjson-0.7.1/src/parserrunnable.cpp \
../../../../../3rdparty/qjson-0.7.1/src/parser.cpp \
../../../../../3rdparty/qjson-0.7.1/src/json_scanner.cpp \
../../../../../3rdparty/qjson-0.7.1/src/json_parser.cc \
    markshistory.cpp

unix:!symbian {

    target.path = /usr/lib
    INSTALLS += target
}

