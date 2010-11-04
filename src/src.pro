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
             gui \
             tracker \
             webside
}

symbian {
}


