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
           inc/WriteTagQuery.h \
           inc/AddUserQuery.h \
           inc/ApplyChannelQuery.h \
           inc/SubscribeChannelQuery.h \
           inc/RegisterUserQuery.h \
           inc/AvailableChannelsQuery.h \
           inc/SubscribedChannelsQuery.h \
	   inc/VersionQuery.h 


#            inc/ApplyMarkQuery.h \
#           inc/SubscribeChannelQuery.h \
#           inc/UnsubscribeChannelQuery.h \


SOURCES += \
           src/LoginQuery.cpp \
           src/DefaultQuery.cpp \
           src/LoadTagsQuery.cpp \
           src/WriteTagQuery.cpp \
           src/AddUserQuery.cpp \
           src/ApplyChannelQuery.cpp \
           src/SubscribeChannelQuery.cpp \
           src/SubscribedChannelsQuery.cpp \
           src/RegisterUserQuery.cpp \ 
	   src/VersionQuery.cpp


# src/ApplyMarkQuery.cpp \
#           src/AvailableChannelsListQuery.cpp \
#           src/LoginQuery.cpp \
#           src/LoadTagsQuery.cpp \
#           src/UnsubscribeChannelQuery.cpp \
#           src/SubscribeChannelQuery.cpp \


LIBS    +=  -lcommon -lqjson

linux: {
    DEFINES += DESKTOP_STYLE
}
config+=network
QT += core network
QT -= gui
