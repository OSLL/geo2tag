TEMPLATE = app

TARGET = wikigpsTracker-ui

target.path = /usr/bin
INSTALLS += target

include(../../../config.pri)

LIBS += -lcommon
QT += network xml



DEPENDPATH += . \

INCLUDEPATH += . \
               ../../common/inc \
							 ../daemon/


maemo5 {
SOURCES += MaemoDaemon.cpp
HEADERS += MaemoDaemon.h
}

linux {
SOURCES += MaemoDaemon.cpp
HEADERS += MaemoDaemon.h
}

symbian {
SOURCES += SymbianDaemon.cpp \
					 ../daemon/TrackerDaemon.cpp \
HEADERS += SymbianDaemon.h \
           ../daemon/TrackerDaemon.h \
}

# Input
SOURCES += main.cpp \
           src/ByteSpinBox.cpp \
           OptionsWidget.cpp \
           LogWidget.cpp \
				   AboutWidget.cpp \
           MainWindow.cpp \
					 DaemonManager.cpp \
					 ../../common/src/defines.cpp

HEADERS += tracker.h \
           inc/ByteSpinBox.h \
           Status.h \
           OptionsWidget.h \
           LogWidget.h \
           MainWindow.h \
				   AboutWidget.h \
					 DaemonManager.h \
           ../../common/inc/defines.h

RESOURCES += \
    images.qrc

FORMS += \
    MainWindow.ui
