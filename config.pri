CONFIG += debug

#CONFIG += linux
#CONFIG += linux_profiling
CONFIG += maemo5
#CONFIG += symbian
SOURCE_DIR = $$PWD

#contains(QT_CONFIG, reduce_exports):CONFIG+=hide_symbols

contains(TEMPLATE,.*lib):DEFINES += QT_SHARED
    
QMAKE_CXXFLAGS += -Werror

maemo5: {
    DEFINES+= Q_WS_MAEMO_5
    QMAKE_CXXFLAGS += -I/usr/include/glib-2.0 \
                      -I/usr/lib/glib-2.0/include \
                      -I/usr/include/gconf/2 \
                      -I/usr/include/dbus-1.0 \
                      -I/usr/lib/dbus-1.0/include
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
}

symbian: {
    #For some reason the default include path doesn't include MOC_DIR on symbian
    OBJECTS_DIR = obj
    MOC_DIR = moc
    INCLUDEPATH += $$MOC_DIR
    DEFINES+= Q_WS_SYMBIAN
}

linux: {
    QMAKE_CXX = g++-4.5
    QMAKE_LINK = g++-4.5
    QMAKE_CXXFLAGS += -fpermissive -g3 -Werror -DREDEFINE_QT_SIGNALS
		INCLUDEPATH += /usr/include/libxml2/
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
}

linux_profiling: {
	QMAKE_CXXFLAGS_DEBUG += -pg
	QMAKE_LFLAGS_DEBUG += -pg
}

LIBS += -L$$SOURCE_DIR/src/lib

UI_HEADERS_DIR = inc
UI_SOURCES_DIR = src

