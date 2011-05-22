TEMPLATE = subdirs
include(../config.pri)


CONFIG += ordered
SUBDIRS = common


maemo5: {
  SUBDIRS += maps \
             json \
	     http_requests \ 
             tracker
	     
}

linux: {
  SUBDIRS += \
             json json/tst \
             http_requests \
             maps \ 
             service \
	     common/tst
}

symbian: {
  SUBDIRS += json \
             http_requests \
             tracker
}


