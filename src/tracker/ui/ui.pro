TEMPLATE = app

TARGET = wikigpsTracker-ui

target.path = /usr/bin
INSTALLS += target

include(../../../config.pri)


QT += network xml


DEPENDPATH += . \

INCLUDEPATH += . \
               ../../common/inc

# Input
SOURCES += main.cpp \
           OptionsWidget.cpp \
           LogWidget.cpp \
           MainWindow.cpp

HEADERS += tracker.h \
           Status.h \
           OptionsWidget.h \
           LogWidget.h \
           MainWindow.h

RESOURCES += \
    images.qrc

FORMS += \
    MainWindow.ui
