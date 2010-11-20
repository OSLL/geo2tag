TEMPLATE = subdirs

TARGET = daemon/wikigpsTracker

target.path = /usr/bin
INSTALLS += target

include(../../config.pri)

SUBDIRS = daemon
