TEMPLATE = app

TARGET = wikigpsTracker-ui

target.path = /usr/bin
INSTALLS += target

include(../../../config.pri)

LIBS += -lcommon
QT += network xml


DEPENDPATH += . \

INCLUDEPATH += . \
               ../../common/inc

# Input
SOURCES += main.cpp \
           OptionsWidget.cpp \
           LogWidget.cpp \
           MainWindow.cpp ../../common/src/defines.cpp

HEADERS += tracker.h \
           Status.h \
           OptionsWidget.h \
           LogWidget.h \
           MainWindow.h \
	   ../../common/inc/defines.h

RESOURCES += \
    images.qrc

FORMS += \
    MainWindow.ui
