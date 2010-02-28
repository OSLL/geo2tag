TEMPLATE = app
TARGET = wikigps

DEPENDPATH += .
INCLUDEPATH += . \
    ../common/inc/ \
    ../maps/inc \
    ../common/common/inc \
    ../common/thread/inc \
    ../common/exception/inc \
    ../common/src/ \
    ../internal/include \
    ../common/odbc/inc \
    ../internal/include\
    inc/

# Input
HEADERS += inc/MarkTableDelegat.h \
    inc/MarkPane.h \
    inc/MarkEditor.h \
    inc/MarkDetailsDialog.h \
    inc/MapWidget.h \
    inc/MapPane.h \
    inc/MainWindow.h \
    inc/ListView.h \
    inc/ListModel.h \
    inc/RadiusEditor.h \
    inc/ChannelPane.h \
    inc/ChannelModel.h \
    inc/CentralWidget.h
SOURCES += src/RadiusEditor.cpp \
    src/MarkTableDelegat.cpp \
    src/MarkPane.cpp \
    src/MarkEditor.cpp \
    src/MarkDetailsDialog.cpp \
    src/MapWidget.cpp \
    src/MapPane.cpp \
    src/MainWindow.cpp \
    src/main.cpp \
    src/ListView.cpp \
    src/ListModel.cpp \
    src/ChannelPane.cpp \
    src/ChannelModel.cpp \
    src/CentralWidget.cpp
LIBS += -L../internal/lib \
    -lodbc \
    -lcommon \
    -lmaps \
    -lcurl
DESTDIR = ../../
OBJECTS_DIR = .obj
MOC_DIR = .moc
DEFINES += DESKTOP_STYLE
QT += webkit \
    phonon
