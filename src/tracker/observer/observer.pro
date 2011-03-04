TEMPLATE = app

TARGET = wikigps-observer

target.path = /usr/bin
INSTALLS += target

include(../../../config.pri)


QT += network


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
