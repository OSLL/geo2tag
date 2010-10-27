TEMPLATE = app
TARGET = tracker-ui

include(../../../config.pri)

target.path = /usr/bin
INSTALLS += target

QT += network xml


DEPENDPATH += . \
              ../../gui/inc \
              ../../gui/src
	      
INCLUDEPATH += . \
               ../../gui/inc \
               ../../common/inc

# Input
SOURCES += main.cpp \
           RequestSender.cpp \
           TrackerGUI.cpp \
           TitleWidget.cpp \
           OptionsWidget.cpp \
           LogWidget.cpp

HEADERS += tracker.h \
           ../../common/inc/defines.h \
           RequestSender.h \
           Status.h \
           TrackerGUI.h \
           TitleWidget.h \
           OptionsWidget.h \
           LogWidget.h

RESOURCES += \
    images.qrc
