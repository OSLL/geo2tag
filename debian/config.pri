CONFIG += debug


#CONFIG += maemo5

SOURCE_DIR = $$PWD

#contains(QT_CONFIG, reduce_exports):CONFIG+=hide_symbols

contains(TEMPLATE,.*lib):DEFINES += QT_SHARED

maemo5 {
    DEFINES+= Q_WS_MAEMO_5
    QMAKE_CXXFLAGS += -I/usr/include/glib-2.0 \
                      -I/usr/lib/glib-2.0/include \
                      -I/usr/include/gconf/2 \
                      -I/usr/include/dbus-1.0 \
                      -I/usr/lib/dbus-1.0/include
}

symbian {
    #For some reason the default include path doesn't include MOC_DIR on symbian
    INCLUDEPATH += $$MOC_DIR
    DEFINES+= Q_WS_SYMBIA
}

linux{
#		QMAKE_CXX = g++-4.5
#		QMAKE_LINK = g++-4.5
                QMAKE_CXXFLAGS += -fpermissive -Werror  -DREDEFINE_QT_SIGNALS
		INCLUDEPATH += /usr/include/libxml2/
}

LIBS += -L$$SOURCE_DIR/build/lib
QMAKE_LIBDIR_QT = $$SOURCE_DIR/src/lib $$QMAKE_LIBDIR_QT

OBJECTS_DIR = .obj
UI_HEADERS_DIR = inc
UI_SOURCES_DIR = src

