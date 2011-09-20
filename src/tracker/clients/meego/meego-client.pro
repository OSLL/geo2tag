TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib-meego-client ui

libs_targ.files += lib-meego-client/libmeego-client.so.0.0.1
libs_targ.path += /usr/lib

ui_targ.files += ui/meego-client-ui
ui_targ.path += /opt/usr/bin

INSTALLS += libs_targ ui_targ

