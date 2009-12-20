TEMPLATE = app
TARGET = geo2tag

DEPENDPATH += .
INCLUDEPATH += . ../common/inc/ ../maps/inc ../common/common/inc \
		../common/thread/inc ../common/exception/inc ../internal/include ../common/odbc/inc

# Input
HEADERS += MapWidget.h MarkTableDelegat.h  MainWindow.h CentralWidget.h MarkPane.h MapPane.h
SOURCES += main.cpp MapWidget.cpp MarkTableDelegat.cpp  MainWindow.cpp CentralWidget.cpp MarkPane.cpp MapPane.cpp


LIBS    += -L../internal/lib -lodbc -lcommon -lmaps -lcurl

DESTDIR = ../../

OBJECTS_DIR = .obj

MOC_DIR = .moc

DEFINES += DESKTOP_STYLE
