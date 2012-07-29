TEMPLATE = subdirs
include(../config.pri)


CONFIG += ordered

message($$CONFIG)

symbian: {
	message(symbian target)
  SUBDIRS += tracker
}

maemo5: {
  SUBDIRS += common
  SUBDIRS += maps \
             json \
	     http_requests \ 
             tracker
	     
}

linux: {
  SUBDIRS += common
  SUBDIRS += \
             json \
             json/tst \
             http_requests \
             service \
             service/tst \
             #wikigps_http_standalone \
             http_requests/tst_http_requests \
             #wikigps_http_standalone/tst \
             tracker/clients/meego/tst \
             common/tst \
	     maps \
             #tracker/observer
}

#SUBDIRS += \
#    http_requests/tst_http_requests

