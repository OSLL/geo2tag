TEMPLATE = app
include(../../config.pri)
TARGET = fcgi_server

INSTALLS += target conf_lighttpd odbc_configs postgre_conf

target.path = /opt/geo2tag/

conf_lighttpd.files = wikigps-service.conf
conf_lighttpd.path = /etc/lighttpd/conf-enabled

postgre_conf.files = pg_hba.conf
postgre_conf.path = /opt/geo2tag/

db_content.files = base.sql
db_content.path = /opt/geo2tag/

odbc_configs.files = ../../scripts/odbc.ini ../../scripts/odbcinst.ini
odbc_configs.path = /opt/geo2tag/

DEPENDPATH += . \
              inc \
              src \
              ../common/inc \
              ../common/src \
              ../common/common/inc \
              ../common/common/src \
              ../common/exception/inc \
              ../common/exception/src \
              ../common/odbc/inc \
              ../common/odbc/src \
              ../common/thread/inc \
              ../common/thread/src

INCLUDEPATH += \
               . \
               inc \
               ../common/inc \
               ../common/odbc/inc \
               ../common/common/inc \
               ../common/exception/inc \
               ../common/thread/inc \
               ../fcgiio/inc \
               ../json/inc

# Input
HEADERS += \
           inc/DynamicCastFailure.h \
           inc/server.h \
           inc/ChannelInternal.h \
           inc/DataMarkInternal.h \
           inc/UserInternal.h \
           inc/DbSession.h \
      	   inc/UpdateThread.h \
           inc/QueryExecutor.h \
    inc/SessionInternal.h \
    inc/Geo2tagDatabase.h


SOURCES += src/main.cpp \
           src/server.cpp \
           src/ChannelInternal.cpp \
           src/DataMarkInternal.cpp \
           src/DynamicCastFailure.cpp \
           src/UserInternal.cpp \
           src/DbSession.cpp \
           src/UpdateThread.cpp \
           src/QueryExecutor.cpp \
    src/SessionInternal.cpp \
    src/Geo2tagDatabase.cpp

LIBS +=  -lcommon -lfcgi -lwikigpsJson -lpq 
OBJECTS_DIR = .obj

QMAKE_LFLAGS += -L../lib -L/usr/lib/

QT -= gui
QT += sql
