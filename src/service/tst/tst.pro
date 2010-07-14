######################################################################
# Automatically generated by qmake (2.01a) ?? ????? 6 17:53:01 2010
######################################################################

TEMPLATE = app
TARGET = test.tst
DEPENDPATH += . \
              ../inc \
              ../json \
              ../../src \
              ../../common/inc \
              ../../common/src \
              ../../common/common/inc \
              ../../common/common/src \
              ../../common/exception/inc \
              ../../common/exception/src \
              ../../common/odbc/inc \
              ../../common/odbc/src \
              ../../common/thread/inc \
              ../../common/thread/src
INCLUDEPATH += \
               . \
               ../inc \
               ../../common/inc \
               ../../common/odbc/inc \
               ../../common/common/inc \
               ../../common/exception/inc \
               ../../common/thread/inc \
               ../../common/src

# Input

HEADERS += ../inc/AvailableList.h \
           ../inc/cast.h \
           ../inc/elements.h \
           ../inc/engine.h \
           ../inc/exception.h \
           ../inc/query.h \
           ../inc/quick.h \
           ../inc/reader.h \
           ../inc/reply.h \
           ../inc/server.h \
           ../inc/stream.h \
           ../inc/visitor.h \
           ../inc/writer.h \
           ../../common/inc/Channel.h \
           ../../common/inc/DataMarks.h \
           ../../common/inc/DbSession.h \
           ../../common/inc/DefaultException.h \
           ../../common/inc/Handle.h \
           ../../common/inc/Picture.h \
           ../../common/inc/PngPicture.h \
           ../../common/inc/pointer.h \
           ../../common/inc/User.h \
           ../../common/src/ChannelInternal.h \
           ../../common/src/DataMarkInternal.h \
           ../../common/src/UserInternal.h \
           ../../common/common/inc/BitTools.h \
           ../../common/common/inc/ConstMap.h \
           ../../common/common/inc/Crc.h \
           ../../common/common/inc/Elect.h \
           ../../common/common/inc/Escape.h \
           ../../common/common/inc/Init.h \
           ../../common/common/inc/Interval.h \
           ../../common/common/inc/MemDump.h \
           ../../common/common/inc/PodPair.h \
           ../../common/common/inc/Singleton.h \
           ../../common/common/inc/Sleep.h \
           ../../common/common/inc/Streamable.h \
           ../../common/common/inc/Time.h \
           ../../common/common/inc/toolDefs.h \
           ../../common/common/inc/TypeInfo.h \
           ../../common/exception/inc/Exception.h \
           ../../common/exception/inc/Trace.h \
           ../../common/odbc/inc/Cache.h \
           ../../common/odbc/inc/Db.h \
           ../../common/odbc/inc/DbException.h \
           ../../common/odbc/inc/DbLock.h \
           ../../common/odbc/inc/DbQuery.h \
           ../../common/thread/inc/AtomicCount.h \
           ../../common/thread/inc/DummySync.h \
           ../../common/thread/inc/Event.h \
           ../../common/thread/inc/Lock.h \
           ../../common/thread/inc/Mutex.h \
           ../../common/thread/inc/SpinLock.h \
           ../../common/thread/inc/SwMr.h \
           ../../common/thread/inc/Thread.h \
           ../../common/thread/inc/Threading.h \
           ../../common/thread/inc/WorkerThread.h \
           ../inc/quick.inl \
           ../inc/cast.inl

SOURCES += ../json/elements.cpp \
           ../json/quick.cpp \
           ../json/reader.cpp \
           ../json/writer.cpp \
           ../src/AvailableList.cpp \
           ../src/query.cpp \
           ../src/stream.cpp \
           ../src/server.cpp

SOURCES += available_list.cpp

LIBS += -L../../internal/lib -lfcgi -lodbc -lcommon
