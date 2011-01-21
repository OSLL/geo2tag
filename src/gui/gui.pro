TEMPLATE = app
include(../../config.pri)

TARGET = wikigps
DEPENDPATH += . inc src

target.path = /usr/bin
INSTALLS += target

INCLUDEPATH += . inc \
                 /usr/include/jqson/ \
                 ../common/inc/ \
                 ../maps/inc \
		 ../http_requests/inc \
                 ../common/common/inc \
                 ../common/thread/inc \
                 ../common/exception/inc \
                 ../internal/include

# Input
HEADERS += inc/ApplyMarkQuery.h \
           inc/AvailableChannelsListQuery.h \     
           inc/CentralWidget.h \
           inc/ChannelListJSON.h \
           inc/ChannelModel.h \
           inc/ChannelPane.h \
           inc/ListModel.h \
           inc/ListView.h \
           inc/LoginQuery.h \
           inc/MainWindow.h \
           inc/MapPane.h \
           inc/MapWidget.h \
           inc/MarkDetailsDialog.h \
           inc/MarkEditor.h \
           inc/MarkPane.h \
           inc/MarkTableDelegat.h \
           inc/OnLineInformation.h \
           inc/OptionsPane.h \
           inc/RadiusEditor.h \
	         inc/LoginWindow.h \
           inc/RSSFeedJSON.h \
           inc/RSSFeedQuery.h \
           inc/SubscribeChannelJSON.h \
           inc/SubscribeChannelQuery.h \
           inc/SubscribedChannelsListQuery.h \
           inc/UnsubscribeChannelQuery.h
SOURCES += src/ChannelModel.cpp \
           src/ChannelPane.cpp \
           src/ListModel.cpp \
           src/ListView.cpp \
           src/main.cpp \
           src/MainWindow.cpp \
           src/MapPane.cpp \
           src/MapWidget.cpp \
      	   src/LoginWindow.cpp \
           src/MarkDetailsDialog.cpp \
           src/MarkEditor.cpp \
           src/MarkPane.cpp \
           src/MarkTableDelegat.cpp \
           src/OnLineInformation.cpp \
           src/OptionsPane.cpp \
           src/RadiusEditor.cpp \
           src/CentralWidget.cpp
#           src/RSSFeedJSON.cpp \
#           src/RSSFeedQuery.cpp \
#           src/UnsubscribeChannelQuery.cpp \
#           src/SubscribeChannelJSON.cpp \
#           src/SubscribeChannelQuery.cpp \
#           src/SubscribedChannelsListQuery.cpp \
#
FORMS  +=  src/LoginForm.ui

LIBS    +=  -lcommon  -lmaps -lqjson -lwikigpsHttp -lwikigpsJson

OBJECTS_DIR = .obj
MOC_DIR = .moc

linux: {
    DEFINES += DESKTOP_STYLE
}

QT += core gui webkit network
