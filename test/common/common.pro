TEMPLATE = app
TARGET = common.tst

DEPENDPATH += .
INCLUDEPATH += . ../../src/common/inc/ ../../src/maps/inc ../../src/common/common/inc \
		../../src/common/thread/inc ../../src/common/exception/inc ../../src/common/odbc/inc \
    ../../src/internal/include 

# Input
SOURCES += main.cpp


LIBS    += -L../../src/internal/lib -lodbc -lcommon -lmaps

DESTDIR = ./

OBJECTS_DIR = .obj

MOC_DIR = .moc
