# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

QT += network phonon

symbian:TARGET.UID3 = 0xE6691141

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices Location

symbian:ICON = gds_icon

CONFIG += mobility
MOBILITY += location multimedia

INCLUDEPATH += . inc

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/GDSService.cpp \
    src/LoginWidget.cpp \
    src/MainWidget.cpp \
    src/CreateAccountWidget.cpp \
    src/Settings.cpp \
    src/SettingsWidget.cpp \
    src/TrackingWidget.cpp \
    src/SosWidget.cpp \
    src/TrackingService.cpp \
    src/LocationManager.cpp \
    src/MapWidget.cpp \
    src/EventsWidget.cpp \
    src/EventsService.cpp \
    src/utils.cpp
HEADERS += inc/defines.h \
    inc/mainwindow.h \
    inc/GDSService.h \
    inc/LoginWidget.h \
    inc/MainWidget.h \
    inc/CreateAccountWidget.h \
    inc/Settings.h \
    inc/SettingsWidget.h \
    inc/TrackingWidget.h \
    inc/SosWidget.h \
    inc/TrackingService.h \
    inc/LocationManager.h \
    inc/MapWidget.h \
    inc/EventsWidget.h \
    inc/EventsService.h \
    inc/utils.h

# qsjon library
INCLUDEPATH += ../../../3rdparty/qjson-0.7.1/src
SOURCES += ../../../3rdparty/qjson-0.7.1/src/serializerrunnable.cpp \
    ../../../3rdparty/qjson-0.7.1/src/serializer.cpp \
    ../../../3rdparty/qjson-0.7.1/src/qobjecthelper.cpp \
    ../../../3rdparty/qjson-0.7.1/src/parserrunnable.cpp \
    ../../../3rdparty/qjson-0.7.1/src/parser.cpp \
    ../../../3rdparty/qjson-0.7.1/src/json_scanner.cpp \
    ../../../3rdparty/qjson-0.7.1/src/json_parser.cc
HEADERS += ../../../3rdparty/qjson-0.7.1/src/parser.h \
    ../../../3rdparty/qjson-0.7.1/src/serializerrunnable.h \
    ../../../3rdparty/qjson-0.7.1/src/serializer.h \
    ../../../3rdparty/qjson-0.7.1/src/qobjecthelper.h \
    ../../../3rdparty/qjson-0.7.1/src/qjson_export.h \
    ../../../3rdparty/qjson-0.7.1/src/qjson_debug.h \
    ../../../3rdparty/qjson-0.7.1/src/parser.h \
    ../../../3rdparty/qjson-0.7.1/src/parser_p.h \
    ../../../3rdparty/qjson-0.7.1/src/json_scanner.h \
    ../../../3rdparty/qjson-0.7.1/src/parserrunnable.h \
    ../../../3rdparty/qjson-0.7.1/src/json_parser.hh \
    ../../../3rdparty/qjson-0.7.1/src/stack.hh \
    ../../../3rdparty/qjson-0.7.1/src/location.hh \
    ../../../3rdparty/qjson-0.7.1/src/position.hh

# geo2tag requests library
INCLUDEPATH += http_requests/inc ../../../src/json/inc ../../../src/common/inc
SOURCES += http_requests/src/DefaultQuery.cpp \
    http_requests/src/LoginQuery.cpp \
    http_requests/src/WriteTagQuery.cpp \
    http_requests/src/RegisterUserQuery.cpp \
    http_requests/src/AddChannelQuery.cpp \
    http_requests/src/SubscribeChannelQuery.cpp \
    http_requests/src/LoadTagsQuery.cpp \
    http_requests/src/FilterCircleQuery.cpp \
    ../../../src/json/src/JsonSerializer.cpp \
    ../../../src/json/src/LoginResponseJSON.cpp \
    ../../../src/json/src/LoginRequestJSON.cpp \
    ../../../src/json/src/JsonUser.cpp \
    ../../../src/json/src/WriteTagResponseJSON.cpp \
    ../../../src/json/src/WriteTagRequestJSON.cpp \
    ../../../src/json/src/AddChannelRequestJSON.cpp \
    ../../../src/json/src/DefaultResponseJSON.cpp \
    ../../../src/json/src/JsonDataMark.cpp \
    ../../../src/json/src/JsonChannel.cpp \
    ../../../src/json/src/RegisterUserResponseJSON.cpp \
    ../../../src/json/src/RegisterUserRequestJSON.cpp \
    ../../../src/json/src/SubscribeChannelJSON.cpp \
    ../../../src/json/src/LoadTagsRequestJSON.cpp \
    ../../../src/json/src/LoadTagsResponseJSON.cpp \
    ../../../src/json/src/FilterRequestJSON.cpp \
    ../../../src/json/src/FilterCircleRequestJSON.cpp \
    ../../../src/json/src/FilterDefaultResponseJSON.cpp \
    ../../../src/common/src/User.cpp \
    ../../../src/common/src/TimeSlot.cpp \
    ../../../src/common/src/DataMarks.cpp \
    ../../../src/common/src/Channel.cpp \
    ../../../src/common/src/FShapeCircle.cpp
#./../common/src/GpsInfo.cpp \
#../../common/src/GpsModeller.cpp \
#./../common/src/MobilityGps.cpp \

HEADERS += http_requests/inc/DefaultQuery.h \
    http_requests/inc/LoginQuery.h \
    http_requests/inc/WriteTagQuery.h \
    http_requests/inc/RegisterUserQuery.h \
    http_requests/inc/AddChannelQuery.h \
    http_requests/inc/SubscribeChannelQuery.h \
    http_requests/inc/LoadTagsQuery.h \
    http_requests/inc/FilterCircleQuery.h \
    ../../../src/json/inc/JsonSerializer.h \
    ../../../src/json/inc/LoginResponseJSON.h \
    ../../../src/json/inc/LoginRequestJSON.h \
    ../../../src/json/inc/JsonUser.h \
    ../../../src/json/inc/JsonDataMark.h \
    ../../../src/json/inc/JsonChannel.h \
    ../../../src/json/inc/WriteTagResponseJSON.h \
    ../../../src/json/inc/WriteTagRequestJSON.h \
    ../../../src/json/inc/AddChannelResponseJSON.h \
    ../../../src/json/inc/AddChannelRequestJSON.h \
    ../../../src/json/inc/DefaultResponseJSON.h \
    ../../../src/json/inc/RegisterUserResponseJSON.h \
    ../../../src/json/inc/RegisterUserRequestJSON.h \
    ../../../src/json/inc/SubscribeChannelJSON.h \
    ../../../src/json/inc/SubscribeChannelResponseJSON.h \
    ../../../src/json/inc/LoadTagsRequestJSON.h \
    ../../../src/json/inc/LoadTagsResponseJSON.h \
    ../../../src/json/inc/FilterRequestJSON.h \
    ../../../src/json/inc/FilterCircleRequestJSON.h \
    ../../../src/json/inc/FilterDefaultResponseJSON.h \
    ../../../src/common/inc/ErrnoTypes.h \
    ../../../src/common/inc/User.h \
    ../../../src/common/inc/TimeSlot.h \
    ../../../src/common/inc/DataMarks.h \
    ../../../src/common/inc/Channel.h \
    ../../../src/common/inc/FShape.h \
    ../../../src/common/inc/FShapeCircle.h
#./../common/inc/ConcurrentVector.h \
#./../common/inc/defines.h \
#../../common/inc/DataChannel.h \
#../../common/inc/symbian.h \
#../../common/inc/GpsInfo.h \
#../../common/inc/GpsModeller.h \
#../../common/inc/MobilityGps.h \


# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES += \
    GeoDoctorSearch.qrc
























