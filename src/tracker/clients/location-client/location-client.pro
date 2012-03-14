#QML_IMPORT_PATH = qml
TARGET = location-client
TEMPLATE=app
OBJECTS_DIR = obj
MOC_DIR = moc
INCLUDEPATH += $$MOC_DIR
CONFIG += qt-components

QT += declarative network
DEFINES += Q_WS_SYMBIAN


CONFIG += mobility network debug
CONFIG += qdeclarative-boostable
MOBILITY += location


VERSION = 0.0.1
INSTALLS += location-client
DEPENDPATH += .
INCLUDEPATH += . inc \
                 src \
               ../../../http_requests/inc \
               ../../../json/inc \
               ../../../../3rdparty/qjson-0.7.1/src/ \
               ../../../common/inc
RESOURCES += \
     mapviewer.qrc

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/Client.cpp \
    src/MarksHistory.cpp \
    ../../../http_requests/src/LoginQuery.cpp \
    ../../../http_requests/src/DefaultQuery.cpp \
    ../../../http_requests/src/WriteTagQuery.cpp \
    ../../../http_requests/src/AddUserQuery.cpp \
    ../../../http_requests/src/ApplyChannelQuery.cpp \
    ../../../http_requests/src/SubscribeChannelQuery.cpp \
    ../../../common/src/User.cpp \
    ../../../common/src/TimeSlot.cpp \
    ../../../common/src/DataMarks.cpp \
    ../../../common/src/Channel.cpp \
    ../../../common/src/GpsInfo.cpp \
    ../../../common/src/GpsModeller.cpp \
    ../../../common/src/MobilityGps.cpp \
    ../../../common/src/defines.cpp \
    ../../../json/src/JsonSerializer.cpp \
    ../../../json/src/LoginResponseJSON.cpp \
    ../../../json/src/LoginRequestJSON.cpp \
    ../../../json/src/SubscribeChannelJSON.cpp \
    ../../../json/src/AddChannelRequestJSON.cpp \
    ../../../json/src/DefaultResponseJSON.cpp \
    ../../../json/src/JsonUser.cpp \
    ../../../json/src/WriteTagResponseJSON.cpp \
    ../../../json/src/WriteTagRequestJSON.cpp \
    ../../../json/src/JsonDataMark.cpp \
    ../../../json/src/JsonChannel.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/serializerrunnable.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/serializer.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/qobjecthelper.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/parserrunnable.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/parser.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/json_scanner.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/json_parser.cc

HEADERS += \
    inc/MainWindow.h \
    inc/Client.h \
    inc/MarksHistory.h \
    ../../../http_requests/inc/LoginQuery.h \
    ../../../http_requests/inc/DefaultQuery.h \
    ../../../http_requests/inc/WriteTagQuery.h \
    ../../../http_requests/inc/AddUserQuery.h \
    ../../../http_requests/inc/ApplyChannelQuery.h \
    ../../../http_requests/inc/SubscribeChannelQuery.h \
    ../../../json/inc/JsonSerializer.h \
    ../../../json/inc/LoginResponseJSON.h \
    ../../../json/inc/LoginRequestJSON.h \
    ../../../json/inc/AddChannelResponseJSON.h \
    ../../../json/inc/AddChannelRequestJSON.h \
    ../../../json/inc/SubscribeChannelResponseJSON.h \
    ../../../json/inc/SubscribeChannelJSON.h \
    ../../../json/inc/AddUserRequestJSON.h \
    ../../../json/inc/AddUserResponseJSON.h \
    ../../../json/inc/JsonUser.h \
    ../../../json/inc/JsonTimeSlot.h \
    ../../../json/inc/JsonDataMark.h \
    ../../../json/inc/JsonChannel.h \
    ../../../json/inc/WriteTagResponseJSON.h \
    ../../../json/inc/WriteTagRequestJSON.h \
    ../../../json/inc/DefaultResponseJSON.h \
    ../../../common/inc/User.h \
    ../../../common/inc/TimeSlot.h \
    ../../../common/inc/DataMarks.h \
    ../../../common/inc/Channel.h \
    ../../../common/inc/ConcurrentVector.h \
    ../../../common/inc/defines.h \
    ../../../common/inc/DataChannel.h \
    ../../../common/inc/GpsInfo.h \
    ../../../common/inc/GpsModeller.h \
    ../../../common/inc/MobilityGps.h \
    ../../../common/inc/ErrnoTypes.h \
    ../../../common/inc/symbian.h \
    ../../../../3rdparty/qjson-0.7.1/src/parser.h \
    ../../../../3rdparty/qjson-0.7.1/src/serializerrunnable.h \
    ../../../../3rdparty/qjson-0.7.1/src/serializer.h \
    ../../../../3rdparty/qjson-0.7.1/src/qobjecthelper.h \
    ../../../../3rdparty/qjson-0.7.1/src/qjson_export.h \
    ../../../../3rdparty/qjson-0.7.1/src/qjson_debug.h \
    ../../../../3rdparty/qjson-0.7.1/src/parser_p.h \
    ../../../../3rdparty/qjson-0.7.1/src/json_scanner.h \
    ../../../../3rdparty/qjson-0.7.1/src/parserrunnable.h \
    ../../../../3rdparty/qjson-0.7.1/src/json_parser.hh \
    ../../../../3rdparty/qjson-0.7.1/src/stack.hh \
    ../../../../3rdparty/qjson-0.7.1/src/location.hh \
    ../../../../3rdparty/qjson-0.7.1/src/position.hh \

OTHER_FILES += \
    qml/RecButton.qml\
    qml/LoginView.qml \
    qml/MapViewer.qml \
    qml/main.qml \
    qml/NotifyDialog.qml \
    qml/RightPanel.qml \
    images/strip.png \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

maemo5 {
    target.path = /opt/bin
    INSTALLS += target
INCLUDEPATH += . inc \
                 src \
               ../../../http_requests/inc \
               ../../../json/inc \
               ../../../../3rdparty/qjson-0.7.1/src/ \
               ../../../common/inc
}
symbian {

  #  TARGET.UID3=0x2004234A
    DEFINES+= Q_WS_SYMBIAN
    TARGET.CAPABILITY = NetworkServices \
                    Location \
                    ReadUserData \
                    WriteUserData \
                    LocalServices \
                    UserEnvironment

#    vendorinfo = \
#    "%{\"Regina Dorokhova\"}" \
#    ":\"\""
#    baseFiles.sources = qml\\*.qml
#    baseFiles.path = !:\\private\\2004234A
#    DEPLOYMENT+=baseFiles

}
contains(TEMPLATE,.*lib):DEFINES += QT_SHARED
contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/bin
    INSTALLS += target
}


