TEMPLATE = subdirs

TARGET = ui/wikigpsTracker-ui observer/wikigps-observer

target.path = /usr/bin
INSTALLS += target 

include(../../config.pri)


SUBDIRS = ui observer 


maemo5 {
	SUBDIRS += daemon
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
