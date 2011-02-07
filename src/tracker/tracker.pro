TEMPLATE = subdirs

TARGET = daemon/wikigpsTracker ui/wikigpsTracker-ui observer/wikigps-observer

target.path = /usr/bin
INSTALLS += target

include(../../config.pri)

SUBDIRS = daemon ui observer
