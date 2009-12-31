TEMPLATE = app
TARGET = wikigps

DEPENDPATH += .
INCLUDEPATH += . ../common/inc/ ../maps/inc ../common/common/inc \
		../common/thread/inc ../common/exception/inc ../common/src/ ../internal/include ../common/odbc/inc \
    ../internal/include

# Input
HEADERS += defs.h MapWidget.h MarkTableDelegat.h  MainWindow.h CentralWidget.h MarkPane.h MapPane.h ChannelPane.h ChannelModel.h MarkEditor.h
SOURCES += main.cpp MapWidget.cpp MarkTableDelegat.cpp  MainWindow.cpp CentralWidget.cpp MarkPane.cpp MapPane.cpp ChannelPane.cpp ChannelModel.cpp MarkEditor.cpp


LIBS    += -L../internal/lib -lodbc -lcommon -lmaps -lcurl

DESTDIR = ../../

OBJECTS_DIR = .obj

MOC_DIR = .moc

DEFINES += DESKTOP_STYLE
