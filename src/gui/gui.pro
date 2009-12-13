TEMPLATE = app
TARGET = geo2tag

DEPENDPATH += .
INCLUDEPATH += . ../common/inc/ ../maps/inc ../common/common/inc \
		../common/thread/inc ../common/exception/inc

# Input
HEADERS += defs.h MapForm.h MapWidget.h MarkTableDelegat.h TabWidget.h
SOURCES += main.cpp MapForm.cpp MapWidget.cpp MarkTableDelegat.cpp TabWidget.cpp


LIBS    += -L../internal/lib -lodbc -lcommon -lmaps -lcurl -llocation -lgconf-2 -ldbus-glib-1 -ldbus-1 -lpthread -lgobject-2.0 -lglib-2.0

DESTDIR = ../../

