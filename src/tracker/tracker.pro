TEMPLATE = subdirs

TARGET = ui/wikigpsTracker-ui
INSTALLS += target 

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
	TARGET += daemon/wikigpsTracker 
	eventd_script.files = wikigpsTracker
	eventd_script.path = /etc/event.d

	logrotate_script.files = wikigpsTracker-logrotate
	logrotate_script.path = /etc/logrotate.d

	icon.files = ../data/wikigpsTracker-ui.png
	icon.path = /usr/share/icons/hicolor/48x48/apps/

        desktop_file.files = ../data/tracker.desktop
	desktop_file.path = /usr/share/applications/hildon/

  INSTALLS += eventd_script logrotate_script icon desktop_file
}

!symbian{
    target.path = /usr/bin

}
