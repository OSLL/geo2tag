#include(../../../config.pri)

#INCLUDEPATH +=

#DEFINES +=
#LIBS += 

SOURCES += ${TESTFILE}

CONFIG +=qtestlib console
CONFIG -=app_bundle

TARGET = utest.${PACKAGE}_${CLASS}

#HEADERS += 

