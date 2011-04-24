TEMPLATE = app

TARGET = wikigps-observer
#CONFIG += linux_profiling

target.path = /usr/bin
INSTALLS += target

include(../../../config.pri)

QT += network

LIBS += -lcommon

DEPENDPATH += . \

INCLUDEPATH += . \
               ../../common/inc \
               ../../maps/inc \
               ../../json/inc \
               ../ui/ \
               ../../http_requests/inc

LIBS += -lmaps -lqjson -lcommon -lwikigpsHttp -lwikigpsJson

# Input
SOURCES += main.cpp \
    MainWindow.cpp \ 
    ../ui/OptionsWidget.cpp \
    DataThread.cpp \
    ViewOptsWidget.cpp

HEADERS += ../ui/OptionsWidget.h \
    MainWindow.h \
    ViewOptsWidget.h \
    DataThread.h
