TEMPLATE = lib
TARGET = ../lib/common

target.path = /usr/lib
INSTALLS += target

include(../../config.pri)

DEPENDPATH += . \
    inc \
    src \
    common/inc \
    common/src \
    exception/inc \
    exception/src

INCLUDEPATH += . \
    inc \
    common/inc \
    exception/inc \
    /usr/include/libxml2 \
    thread/inc

# Input
HEADERS += inc/DataMarks.h \
    inc/DefaultException.h \
    inc/GpsInfo.h \
    inc/Handle.h \
    inc/User.h \
    inc/Picture.h \
    inc/PngPicture.h \
    inc/pointer.h \
    inc/Channel.h \
    inc/Ptr.h \
    inc/defines.h \
    common/inc/BitTools.h \
    common/inc/ConstMap.h \
    common/inc/Crc.h \
    common/inc/Elect.h \
    common/inc/Escape.h \
    common/inc/Init.h \
    common/inc/Interval.h \
    common/inc/MemDump.h \
    common/inc/PodPair.h \
    common/inc/Singleton.h \
    common/inc/Sleep.h \
    common/inc/Streamable.h \
    common/inc/Time.h \
    common/inc/toolDefs.h \
    common/inc/TypeInfo.h \
    exception/inc/Exception.h \
    exception/inc/ExceptionSource.h \
    exception/inc/Trace.h \ 
    inc/Parameters.h

SOURCES += src/DataMarks.cpp \
    src/DefaultException.cpp \
    src/GpsInfo.cpp \
    src/Picture.cpp \
    src/PngPicture.cpp \
    src/Channel.cpp \
    src/User.cpp \
    src/pointer.cpp \
    common/src/BitTools.cpp \
    common/src/Escape.cpp \
    common/src/MemDump.cpp \
    common/src/Time.cpp \
    exception/src/Exception.cpp \
    exception/src/ExceptionSource.cpp \
    exception/src/Trace.cpp \
    common/src/Init.cpp \
    common/src/TypeInfo.cpp \ 
    src/Parameters.cpp



linux: {
    DEPENDPATH += odbc/inc \
                  odbc/src \
                  thread/inc \
                  thread/src

    INCLUDEPATH +=  thread/inc \
                    odbc/inc

    HEADERS += odbc/inc/Cache.h \
               odbc/inc/Db.h \
               odbc/inc/DbException.h \
               odbc/inc/DbLock.h \
               odbc/inc/DbQuery.h \
               thread/inc/AtomicCount.h \
               thread/inc/DummySync.h \
               thread/inc/Event.h \
               thread/inc/Lock.h \
               thread/inc/Mutex.h \
               thread/inc/SpinLock.h \
               thread/inc/SwMr.h \
               thread/inc/Thread.h \
               thread/inc/Threading.h \
               thread/inc/WorkerThread.h \
               common/inc/CrcTable.h \
               common/inc/ElectSeq.h \
               inc/GpsModeller.h \
               odbc/inc/DbConn.h \
               odbc/inc/Transaction.h \
               odbc/inc/DbStmt.h \
               odbc/inc/ExecuteClose.h \
               odbc/inc/DbQueryX.h

    SOURCES += src/GpsModeller.cpp \
               odbc/src/Db.cpp \
               odbc/src/DbLock.cpp \
               odbc/src/DbQuery.cpp \
               thread/src/Event.cpp \
               thread/src/Mutex.cpp \
               thread/src/SwMr.cpp \
               thread/src/Thread.cpp \
               odbc/src/DbException.cpp \
               odbc/src/DbConn.cpp \
               odbc/src/Transaction.cpp \
               odbc/src/DbStmt.cpp \
               odbc/src/ExecuteClose.cpp \
               odbc/src/DbQueryX.cpp 

    LIBS += -lxml2 -lodbc
    
    DEFINES += NO_GPS SIMULATE_GPS
}

maemo5: {
    CONFIG += mobility
    MOBILITY = location

    DEPENDPATH += \
                  thread/inc \
                  thread/src
    INCLUDEPATH += thread/inc/
    HEADERS += thread/inc/AtomicCount.h \
               thread/inc/DummySync.h \
               thread/inc/Event.h \
               thread/inc/Lock.h \
               inc/MobilityGps.h\
               thread/inc/Mutex.h \
               thread/inc/SpinLock.h \
               thread/inc/SwMr.h \
               thread/inc/Thread.h \
               thread/inc/Threading.h \
               thread/inc/WorkerThread.h
	       
    SOURCES += thread/src/Event.cpp \
               src/MobilityGps.cpp\
               thread/src/Mutex.cpp \
               thread/src/SwMr.cpp \
               thread/src/Thread.cpp
    
 LIBS += -lQtLocation
}

symbian: {
}

webside: {
    DEPENDPATH += odbc/inc \
                  odbc/src \
                  thread/inc \
                  thread/src

    INCLUDEPATH +=  thread/inc \
                    odbc/inc

    HEADERS += odbc/inc/Cache.h \
               odbc/inc/Db.h \
               odbc/inc/DbException.h \
               odbc/inc/DbLock.h \
               odbc/inc/DbQuery.h \
               thread/inc/AtomicCount.h \
               thread/inc/DummySync.h \
               thread/inc/Event.h \
               thread/inc/Lock.h \
               thread/inc/Mutex.h \
               thread/inc/SpinLock.h \
               thread/inc/SwMr.h \
               thread/inc/Thread.h \
               thread/inc/Threading.h \
               thread/inc/WorkerThread.h \
               common/inc/CrcTable.h \
               common/inc/ElectSeq.h \
               inc/GpsModeller.h \
               odbc/inc/DbConn.h \
               odbc/inc/Transaction.h \
               odbc/inc/DbStmt.h \
               odbc/inc/ExecuteClose.h \
               odbc/inc/DbQueryX.h

    SOURCES += src/GpsModeller.cpp \
               odbc/src/Db.cpp \
               odbc/src/DbLock.cpp \
               odbc/src/DbQuery.cpp \
               thread/src/Event.cpp \
               thread/src/Mutex.cpp \
               thread/src/SwMr.cpp \
               thread/src/Thread.cpp \
               odbc/src/DbException.cpp \
               odbc/src/DbConn.cpp \
               odbc/src/Transaction.cpp \
               odbc/src/DbStmt.cpp \
               odbc/src/ExecuteClose.cpp \
               odbc/src/DbQueryX.cpp

    LIBS += -lxml2 -lodbc

    DEFINES += NO_GPS SIMULATE_GPS
}

OBJECTS_DIR = .obj
MOC_DIR = .moc
