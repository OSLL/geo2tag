# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir

VERSION = 0.16.1

INCLUDEPATH += ../../../../common/inc \
               ../../../../maps/inc \
               ../../../../http_requests/inc \
               ../../../../tracker/ui/ \
               ../../../../tracker/ui/inc \
               ../../../../json/inc \
               ../../../../../3rdparty/qjson-0.7.1/src/


DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0x20041E3E

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += location

QT += network
# LIBS +=

SOURCES += \
    ../../../observer/ViewOptsWidget.cpp \
    ../../../observer/MainWindow.cpp \
    ../../../observer/main.cpp \
    ../../../observer/DataThread.cpp \
    ../../../../common/src/defines.cpp \
    ../../../../maps/src/MapScene.cpp \
    ../../../ui/OptionsWidget.cpp \
    ../../../../http_requests/src/RSSFeedQuery.cpp \
    ../../../../http_requests/src/LoginQuery.cpp \
    ../../../../http_requests/src/DefaultQuery.cpp \
    ../../../../http_requests/src/ApplyMarkQuery.cpp \
    ../../../../http_requests/src/AddNewMarkQuery.cpp \
    ../../../../common/src/User.cpp \
    ../../../../maps/src/Preloading.cpp \
    ../../../../maps/src/OSMCoordinatesConverter.cpp \
    ../../../../maps/src/MapsUploadThread.cpp \
    ../../../../maps/src/MapsUploader.cpp \
    ../../../../common/src/DataMarks.cpp \
    ../../../../common/src/Channel.cpp \
    ../../../../json/src/RSSFeedRequestJSON.cpp \
    ../../../../json/src/RSSFeedJSON.cpp \
    ../../../../json/src/JsonSerializer.cpp \
    ../../../../json/src/LoginResponseJSON.cpp \
    ../../../../json/src/LoginRequestJSON.cpp \
    ../../../../json/src/JsonUser.cpp \
    ../../../../json/src/AddChannelRequestJSON.cpp \
    ../../../../json/src/AddNewMarkResponseJSON.cpp \
    ../../../../json/src/AddNewMarkRequestJSON.cpp \
    ../../../../json/src/JsonDataMark.cpp \
    ../../../../json/src/JsonChannel.cpp \
    ../../../ui/src/ByteSpinBox.cpp \
    ../../../../common/src/GpsInfo.cpp \
    ../../../../common/src/GpsModeller.cpp \
    ../../../../common/src/MobilityGps.cpp \
    ../../../../../3rdparty/qjson-0.7.1/src/serializerrunnable.cpp \
    ../../../../../3rdparty/qjson-0.7.1/src/serializer.cpp \
    ../../../../../3rdparty/qjson-0.7.1/src/qobjecthelper.cpp \
    ../../../../../3rdparty/qjson-0.7.1/src/parserrunnable.cpp \
    ../../../../../3rdparty/qjson-0.7.1/src/parser.cpp \
    ../../../../../3rdparty/qjson-0.7.1/src/json_scanner.cpp \
    ../../../../../3rdparty/qjson-0.7.1/src/json_parser.cc \
    ../../../../maps/src/LightMap.cpp \
    ../../../../maps/src/SlippyMap.cpp
HEADERS += \
    ../../../observer/ViewOptsWidget.h \
    ../../../observer/MainWindow.h \
    ../../../observer/main.h \
    ../../../observer/DataThread.h \
    ../../../../common/src/defines.h \
    ../../../../maps/inc/MapScene.h \
    ../../../ui/OptionsWidget.h \
    ../../../../http_requests/src/RSSFeedQuery.h \
    ../../../../http_requests/src/LoginQuery.h \
    ../../../../http_requests/src/DefaultQuery.h \
    ../../../../http_requests/src/ApplyMarkQuery.h \
    ../../../../http_requests/src/AddNewMarkQuery.h \
    ../../../../common/src/User.h \
    ../../../../maps/inc/Preloading.h \
    ../../../../maps/inc/OSMCoordinatesConverter.h \
    ../../../../maps/inc/MapsUploadThread.h \
    ../../../../maps/inc/MapsUploader.h \
    ../../../../common/inc/DataMarks.h \
    ../../../../common/inc/Channel.h \
    ../../../../json/inc/RSSFeedRequestJSON.h \
    ../../../../json/inc/RSSFeedJSON.h \
    ../../../../json/inc/JsonSerializer.h \
    ../../../../json/inc/LoginResponseJSON.h \
    ../../../../json/inc/LoginRequestJSON.h \
    ../../../../json/inc/JsonUser.h \
    ../../../../json/inc/AddChannelRequestJSON.h \
    ../../../../http_requests/inc/RSSFeedQuery.h \
    ../../../../http_requests/inc/LoginQuery.h \
    ../../../../http_requests/inc/DefaultQuery.h \
    ../../../../http_requests/inc/ApplyMarkQuery.h \
    ../../../../http_requests/inc/AddNewMarkQuery.h \
    ../../../../common/inc/User.h \
    ../../../../json/inc/JsonDataMark.h \
    ../../../../json/inc/JsonChannel.h \
    ../../../../json/inc/AddNewMarkResponseJSON.h \
    ../../../../json/inc/AddNewMarkRequestJSON.h \
    ../../../../json/inc/AddChannelResponseJSON.h \
    ../../../../common/inc/ConcurrentVector.h \
    ../../../../common/inc/defines.h \
    ../../../../common/inc/DataChannel.h \
    ../../../../common/inc/symbian.h \
    ../../../../../3rdparty/qjson-0.7.1/src/parser.h \
    ../../../../json/inc/DefaultResponseJSON.h \
    ../../../../../3rdparty/qjson-0.7.1/src/serializerrunnable.h \
    ../../../../../3rdparty/qjson-0.7.1/src/serializer.h \
    ../../../../../3rdparty/qjson-0.7.1/src/qobjecthelper.h \
    ../../../../../3rdparty/qjson-0.7.1/src/qjson_export.h \
    ../../../../../3rdparty/qjson-0.7.1/src/qjson_debug.h \
    ../../../../../3rdparty/qjson-0.7.1/src/parser_p.h \
    ../../../../../3rdparty/qjson-0.7.1/src/json_scanner.h \
    ../../../../maps/inc/LightMap.h \
    ../../../../maps/inc/SlippyMap.h \
    ../../../ui/tracker.h \
    ../../../ui/inc/ByteSpinBox.h \
    ../../../../common/inc/GpsInfo.h \
    ../../../../common/inc/GpsModeller.h \
    ../../../../common/inc/MobilityGps.h \
    ../../../../../3rdparty/qjson-0.7.1/src/parserrunnable.h \
    ../../../../../3rdparty/qjson-0.7.1/src/json_parser.hh \
    ../../../../../3rdparty/qjson-0.7.1/src/stack.hh \
    ../../../../../3rdparty/qjson-0.7.1/src/location.hh \
    ../../../../../3rdparty/qjson-0.7.1/src/position.hh
FORMS +=

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    ../../../../../../../tmp/OVI_2011-04-26.crt
