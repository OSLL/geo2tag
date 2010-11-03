TEMPLATE = subdirs

TARGET = daemon/tracker ui/tracker-ui

target.path = /usr/bin
INSTALLS += target

include(../../config.pri)

SUBDIRS= ui daemon
