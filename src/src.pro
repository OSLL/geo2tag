TEMPLATE = subdirs
include(../config.pri)


CONFIG += ordered
message($$LIBS)

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
             http_requests \
             service \
	     maps \ 
             json/tst \
             http_requests/tst_http_requests \
             tracker/clients/meego/tst \
             common/tst \
             service/tst 
             #tracker/observer
}

#SUBDIRS += \
#    http_requests/tst_http_requests

