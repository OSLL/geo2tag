TEMPLATE = app

TARGET = wikigpsTracker-ui
include(../../../config.pri)


QT += network

DEPENDPATH += . \

INCLUDEPATH += . \
               ../../common/inc \
               ../daemon/


!symbian {
LIBS += -lcommon
target.path = /usr/bin
INSTALLS += target
SOURCES += MaemoDaemon.cpp
HEADERS += MaemoDaemon.h
}


symbian {
VERSION = 0.16.1
#TARGET.UID3 = 0x20041E3E
LIBS+= -lQtLocation
CONFIG += mobility
MOBILITY += location

INCLUDEPATH += ../../../../http_requests/inc \
               inc \
               ../../json/inc \
               ../../../3rdparty/qjson-0.7.1/src/

TARGET.CAPABILITY = NetworkServices \
                    Location \
                    ReadUserData \
                    WriteUserData \
                    LocalServices \
                    UserEnvironment \
                    ReadDeviceData \
                    WriteDeviceData

DEFINES += NO_DAEMON
SOURCES += SymbianDaemon.cpp \
../daemon/TrackerDaemon.cpp \
../../http_requests/src/LoginQuery.cpp \
../../http_requests/src/DefaultQuery.cpp \
../../http_requests/src/AddNewMarkQuery.cpp \
../../common/src/User.cpp \
../../common/src/DataMarks.cpp \
../../common/src/Channel.cpp \
../../common/src/GpsInfo.cpp \
../../common/src/GpsModeller.cpp \
../../common/src/MobilityGps.cpp \
../../json/src/JsonSerializer.cpp \
../../json/src/LoginResponseJSON.cpp \
../../json/src/LoginRequestJSON.cpp \
../../json/src/JsonUser.cpp \
../../json/src/AddNewMarkResponseJSON.cpp \
../../json/src/AddNewMarkRequestJSON.cpp \
../../json/src/JsonDataMark.cpp \
../../json/src/JsonChannel.cpp \
../../../3rdparty/qjson-0.7.1/src/serializerrunnable.cpp \
../../../3rdparty/qjson-0.7.1/src/serializer.cpp \
../../../3rdparty/qjson-0.7.1/src/qobjecthelper.cpp \
../../../3rdparty/qjson-0.7.1/src/parserrunnable.cpp \
../../../3rdparty/qjson-0.7.1/src/parser.cpp \
../../../3rdparty/qjson-0.7.1/src/json_scanner.cpp \
../../../3rdparty/qjson-0.7.1/src/json_parser.cc

HEADERS += SymbianDaemon.h \
../../http_requests/inc/LoginQuery.h \
../../http_requests/inc/DefaultQuery.h \
../../http_requests/inc/AddNewMarkQuery.h \
../../json/inc/JsonSerializer.h \
../../json/inc/LoginResponseJSON.h \
../../json/inc/LoginRequestJSON.h \
../../json/inc/JsonUser.h \
../../json/inc/JsonDataMark.h \
../../json/inc/JsonChannel.h \
../../json/inc/AddNewMarkResponseJSON.h \
../../json/inc/AddNewMarkRequestJSON.h \
../../json/inc/DefaultResponseJSON.h \
../../common/inc/User.h \
../../common/inc/DataMarks.h \
../../common/inc/Channel.h \
../../common/inc/ConcurrentVector.h \
../../common/inc/defines.h \
../../common/inc/DataChannel.h \
../../common/inc/symbian.h \
../../common/inc/GpsInfo.h \
../../common/inc/GpsModeller.h \
../../common/inc/MobilityGps.h \
../../../3rdparty/qjson-0.7.1/src/parser.h \
../../../3rdparty/qjson-0.7.1/src/serializerrunnable.h \
../../../3rdparty/qjson-0.7.1/src/serializer.h \
../../../3rdparty/qjson-0.7.1/src/qobjecthelper.h \
../../../3rdparty/qjson-0.7.1/src/qjson_export.h \
../../../3rdparty/qjson-0.7.1/src/qjson_debug.h \
../../../3rdparty/qjson-0.7.1/src/parser_p.h \
../../../3rdparty/qjson-0.7.1/src/json_scanner.h \
../../../3rdparty/qjson-0.7.1/src/parserrunnable.h \
../../../3rdparty/qjson-0.7.1/src/json_parser.hh \
../../../3rdparty/qjson-0.7.1/src/stack.hh \
../../../3rdparty/qjson-0.7.1/src/location.hh \
../../../3rdparty/qjson-0.7.1/src/position.hh \
../daemon/TrackerDaemon.h
#TARGET.UID3 = 0xA000C606

}

# Input
SOURCES += main.cpp \
           src/ByteSpinBox.cpp \
           OptionsWidget.cpp \
           LogWidget.cpp \
           AboutWidget.cpp \
           MainWindow.cpp \
           DaemonManager.cpp \
           ../../common/src/defines.cpp

HEADERS += tracker.h \
           inc/ByteSpinBox.h \
           Status.h \
           OptionsWidget.h \
           LogWidget.h \
           MainWindow.h \
           AboutWidget.h \
           DaemonManager.h \
           ../../common/inc/defines.h

RESOURCES += \
    images.qrc
		
!maemo5 {
	FORMS += MainWindow.ui
}

maemo5 {
	DEFINES += MAEMO5_UI
	FORMS += MainWindow_maemo.ui
}

