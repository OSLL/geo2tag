TEMPLATE = subdirs

include(../config.pri)

SUBDIRS = common

maemo5: {
  SUBDIRS += maps \
             gui \
             tracker
}

linux: {
  SUBDIRS += \
             json \
             http_requests \
             service \
             maps \
             tracker
# Mark, you have to check that trunk is able to build before commit such invalid webside 
}

symbian: {
}


