TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS =  gui \
    lib-client
libs_targ.files += lib-client/libclient.so.0.0.1
libs_targ.path += /usr/lib

ui_targ.files += gui/geo2tag
ui_targ.path += /opt/usr/bin

INSTALLS += libs_targ ui_targ
contains(TEMPLATE,.*lib):DEFINES += QT_SHARED


