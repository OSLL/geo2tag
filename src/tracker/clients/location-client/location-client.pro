TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += gui
SUBDIRS += lib-client
SOURCE_DIR = $$PWD

LIBS += -L$$SOURCE_DIR/src/lib

libs_targ.files += lib-client/libclient.so.0.0.1
libs_targ.path += /usr/lib

ui_targ.files += gui/geo2tag
ui_targ.path += /opt/usr/bin

INSTALLS += libs_targ ui_targ
contains(TEMPLATE,.*lib):DEFINES += QT_SHARED



