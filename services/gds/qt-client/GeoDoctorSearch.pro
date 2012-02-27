# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xE6691141

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices Location

CONFIG += mobility
MOBILITY += location

INCLUDEPATH += . inc

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/GDSService.cpp \
    src/LoginWidget.cpp
HEADERS += inc/mainwindow.h \
    inc/GDSService.h \
    inc/LoginWidget.h

# qsjon library
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
    ../../../3rdparty/qjson-0.7.1/src/parser_p.h \
    ../../../3rdparty/qjson-0.7.1/src/json_scanner.h \
    ../../../3rdparty/qjson-0.7.1/src/parserrunnable.h \
    ../../../3rdparty/qjson-0.7.1/src/json_parser.hh \
    ../../../3rdparty/qjson-0.7.1/src/stack.hh \
    ../../../3rdparty/qjson-0.7.1/src/location.hh \
    ../../../3rdparty/qjson-0.7.1/src/position.hh

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()





