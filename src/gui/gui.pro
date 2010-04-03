TEMPLATE = app
TARGET = wikigps
DEPENDPATH += . inc src \
              ../fcgiio/inc \
              ../json/inc

INCLUDEPATH += . inc \
                 ../common/inc/ \
                 ../maps/inc \
                 ../common/common/inc \
                 ../common/thread/inc \
                 ../common/exception/inc \
                 ../json/inc \
                 ../fcgiio/inc

# Input
HEADERS += inc/ApplyMarkQuery.h \
           inc/AvailableChannelsListQuery.h \     
           inc/CentralWidget.h \
           inc/ChannelModel.h \
           inc/ChannelPane.h \
           inc/ListModel.h \
           inc/ListView.h \
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
           inc/RSSFeed.h \
           inc/RSSFeedQuery.h \
           inc/SubscribeChannelQuery.h \
           inc/SubscribedChannelsListQuery.h \
           inc/defines.h
SOURCES += src/ApplyMarkQuery.cpp \
           src/AvailableChannelsListQuery.cpp \
           src/ChannelModel.cpp \
           src/ChannelPane.cpp \
           src/ListModel.cpp \
           src/ListView.cpp \
           src/main.cpp \
           src/MainWindow.cpp \
           src/MapPane.cpp \
           src/MapWidget.cpp \
           src/MarkDetailsDialog.cpp \
           src/MarkEditor.cpp \
           src/MarkPane.cpp \
           src/MarkTableDelegat.cpp \
           src/OnLineInformation.cpp \
           src/OptionsPane.cpp \
           src/RadiusEditor.cpp \
           src/RSSFeed.cpp \
           src/RSSFeedQuery.cpp \
           src/SubscribeChannelQuery.cpp \
           src/SubscribedChannelsListQuery.cpp \
           src/CentralWidget.cpp

LIBS    += -L../internal/lib -lodbc -lcommon -lmaps -lfcgiio -ljson
OBJECTS_DIR = .obj
MOC_DIR = .moc
DEFINES += DESKTOP_STYLE

QT += webkit phonon network
