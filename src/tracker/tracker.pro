TEMPLATE = subdirs



include(../../config.pri)

linux {
       SUBDIRS = ui observer
       TARGET =  ui/wikigpsTracker-ui observer/wikigps-observer


}

symbian {
	SUBDIRS = ui
}

maemo5 {
	SUBDIRS = ui daemon
        TARGET =  ui/wikigpsTracker-ui daemon/wikigpsTracker
}

target.path = /usr/bin
INSTALLS += target
