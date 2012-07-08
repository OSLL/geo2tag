#QML_IMPORT_PATH = qml
TARGET = ThereAndHere
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
    ../../../http_requests/src/RegisterUserQuery.cpp \
    ../../../http_requests/src/LoadTagsQuery.cpp \
    ../../../http_requests/src/ApplyChannelQuery.cpp \
    ../../../http_requests/src/SubscribeChannelQuery.cpp \
    ../../../http_requests/src/UnsubscribeChannelQuery.cpp \
    ../../../http_requests/src/SubscribedChannelsQuery.cpp \
    ../../../common/src/User.cpp \
    ../../../common/src/DataMarks.cpp \
    ../../../common/src/Channel.cpp \
    ../../../common/src/GpsInfo.cpp \
    ../../../common/src/GpsModeller.cpp \
    ../../../common/src/MobilityGps.cpp \
    ../../../common/src/defines.cpp \
    ../../../common/src/SettingsStorage.cpp \
    ../../../json/src/JsonSerializer.cpp \
    ../../../json/src/LoginResponseJSON.cpp \
    ../../../json/src/LoginRequestJSON.cpp \
    ../../../json/src/SubscribedChannelsRequestJSON.cpp \
    ../../../json/src/SubscribeChannelJSON.cpp \
    ../../../json/src/AvailableChannelsResponseJSON.cpp \
    ../../../json/src/AddChannelRequestJSON.cpp \
    ../../../json/src/DefaultResponseJSON.cpp \
    ../../../json/src/JsonUser.cpp \
    ../../../json/src/WriteTagResponseJSON.cpp \
    ../../../json/src/WriteTagRequestJSON.cpp \
    ../../../json/src/RegisterUserResponseJSON.cpp \
    ../../../json/src/RegisterUserRequestJSON.cpp \
    ../../../json/src/LoadTagsResponseJSON.cpp \
    ../../../json/src/LoadTagsRequestJSON.cpp \
    ../../../json/src/JsonDataMark.cpp \
    ../../../json/src/JsonChannel.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/serializerrunnable.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/serializer.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/qobjecthelper.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/parserrunnable.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/parser.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/json_scanner.cpp \
    ../../../../3rdparty/qjson-0.7.1/src/json_parser.cc \
    src/Settings.cpp \
    src/Contact.cpp \
    src/ContactModel.cpp

HEADERS += \
    inc/MainWindow.h \
    inc/Client.h \
    inc/MarksHistory.h \
    ../../../http_requests/inc/LoginQuery.h \
    ../../../http_requests/inc/DefaultQuery.h \
    ../../../http_requests/inc/WriteTagQuery.h \
    ../../../http_requests/inc/RegisterUserQuery.h \
    ../../../http_requests/inc/LoadTagsQuery.h \
    ../../../http_requests/inc/ApplyChannelQuery.h \
    ../../../http_requests/inc/SubscribeChannelQuery.h \
    ../../../http_requests/inc/UnsubscribeChannelQuery.h \
    ../../../http_requests/inc/SubscribedChannelsQuery.h \
    ../../../json/inc/JsonSerializer.h \
    ../../../json/inc/LoginResponseJSON.h \
    ../../../json/inc/LoginRequestJSON.h \
    ../../../json/inc/AddChannelResponseJSON.h \
    ../../../json/inc/AddChannelRequestJSON.h \
    ../../../json/inc/SubscribeChannelResponseJSON.h \
    ../../../json/inc/SubscribeChannelJSON.h \
    ../../../json/inc/UnsubscribeChannelRequestJSON.h \
    ../../../json/inc/UnsubscribeChannelResponseJSON.h \
    ../../../json/inc/SubscribedChannelsResponseJSON.h \
    ../../../json/inc/SubscribedChannelsRequestJSON.h \
    ../../../json/inc/AvailableChannelsResponseJSON.h \
    ../../../json/inc/RegisterUserRequestJSON.h \
    ../../../json/inc/RegisterUserResponseJSON.h \
    ../../../json/inc/LoadTagsRequestJSON.h \
    ../../../json/inc/LoadTagsResponseJSON.h \
    ../../../json/inc/JsonUser.h \
    ../../../json/inc/JsonDataMark.h \
    ../../../json/inc/JsonChannel.h \
    ../../../json/inc/WriteTagResponseJSON.h \
    ../../../json/inc/WriteTagRequestJSON.h \
    ../../../json/inc/DefaultResponseJSON.h \
    ../../../common/inc/User.h \
    ../../../common/inc/Channel.h \
    ../../../common/inc/ConcurrentVector.h \
    ../../../common/inc/defines.h \
    ../../../common/inc/DataChannel.h \
    ../../../common/inc/GpsInfo.h \
    ../../../common/inc/GpsModeller.h \
    ../../../common/inc/MobilityGps.h \
    ../../../common/inc/ErrnoTypes.h \
    ../../../common/inc/SettingsStorage.h \
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
    inc/Settings.h \
    inc/Contact.h \
    inc/ContactModel.h

OTHER_FILES += \
    qml/RecButton.qml\
    qml/LoginView.qml \
    qml/MapViewer.qml \
    qml/main.qml \
    qml/NotifyDialog.qml \
    qml/RightPanel.qml \
    qml/TrackingSettingsView.qml \
    images/strip.png \
    images/TH15.svg \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/TrackingSettingsView.qml \
    qml/ContactList.qml \
    qml/ContactEditDialog.qml

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
    ICON= \
    images/TH15.svg
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


