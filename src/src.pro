TEMPLATE = subdirs

include(../config.pri)

SUBDIRS = common

maemo5: {
  SUBDIRS += maps \
             gui \
             tracker
}

linux: {
  SUBDIRS += service
}

symbian: {
}


