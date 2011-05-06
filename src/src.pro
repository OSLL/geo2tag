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
	     tracker \
             service \
	     common/tst
}

symbian: {
}


