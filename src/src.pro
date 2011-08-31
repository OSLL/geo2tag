TEMPLATE = subdirs
include(../config.pri)


CONFIG += ordered
!symbian{
  SUBDIRS += common
}


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
             tracker/observer \
             common/tst
}

symbian: {
  SUBDIRS += tracker
}
