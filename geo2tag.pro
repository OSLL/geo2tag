TEMPLATE = subdirs
include(config.pri)

SUBDIRS = src  

CONFIG += mobility
MOBILITY = location

QMAKE_CFLAGS_DEBUG += "-g3"
QMAKE_CXXFLAGS_DEBUG +="-g3"
