TEMPLATE = lib
include(../../config.pri)

TARGET = ../lib/wikigpsHttp
DEPENDPATH += . inc src

target.path = /usr/lib
INSTALLS += target

INCLUDEPATH += . inc \
                 ../common/inc/ \
                 ../json/inc

# Input
HEADERS += \
           inc/LoginQuery.h \
           inc/DefaultQuery.h \
           inc/LoadTagsQuery.h \
           inc/AddNewMarkQuery.h

#            inc/ApplyMarkQuery.h \
#           inc/AvailableChannelsListQuery.h \
#
#           inc/LoadTagsQuery.h \
#           inc/SubscribeChannelQuery.h \
#           inc/SubscribedChannelsListQuery.h \
#           inc/UnsubscribeChannelQuery.h \


SOURCES += \
           src/LoginQuery.cpp \
           src/DefaultQuery.cpp \
           src/LoadTagsQuery.cpp \
           src/AddNewMarkQuery.cpp

# src/ApplyMarkQuery.cpp \
#           src/AvailableChannelsListQuery.cpp \
#           src/LoginQuery.cpp \
#           src/LoadTagsQuery.cpp \
#           src/UnsubscribeChannelQuery.cpp \
#           src/SubscribeChannelQuery.cpp \
#           src/SubscribedChannelsListQuery.cpp \


LIBS    +=  -lcommon -lqjson

linux: {
    DEFINES += DESKTOP_STYLE
}
config+=network
QT += core network
QT -= gui
