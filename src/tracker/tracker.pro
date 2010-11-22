TEMPLATE = subdirs

TARGET = daemon/wikigpsTracker ui/wikigpsTracker-ui

target.path = /usr/bin
INSTALLS += target

include(../../config.pri)

SUBDIRS = daemon ui
