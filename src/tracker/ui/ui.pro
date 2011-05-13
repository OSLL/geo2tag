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
           src/ByteSpinBox.cpp \
           OptionsWidget.cpp \
           LogWidget.cpp \
	   AboutWidget.cpp \
           MainWindow.cpp ../../common/src/defines.cpp

HEADERS += tracker.h \
           inc/ByteSpinBox.h \
           Status.h \
           OptionsWidget.h \
           LogWidget.h \
           MainWindow.h \
	   AboutWidget.h \
           ../../common/inc/defines.h

RESOURCES += \
    images.qrc
		
!maemo5 {
	FORMS += MainWindow.ui
		}

maemo5 {
	DEFINES += MAEMO5_UI
	FORMS += MainWindow_maemo.ui
}

