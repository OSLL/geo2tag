TEMPLATE = subdirs

include(../config.pri)

SUBDIRS = common

maemo5 {
  SUBDIRS += maps \
             gui \
	     tracker
}

linux {
  SUBDIRS += json \
             fcgiio \
             service \
             maps \
             gui
}

symbian {
}

webside {
  SUBDIRS += webside
}

