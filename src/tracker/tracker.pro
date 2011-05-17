TEMPLATE = subdirs

TARGET = daemon/wikigpsTracker ui/wikigpsTracker-ui observer/wikigps-observer

target.path = /usr/bin
INSTALLS += target

include(../../config.pri)

linux {
	SUBDIRS = ui observer 
}

symbian {
	SUBDIRS = ui
}

maemo5 {
	SUBDIRS = ui daemon
}
