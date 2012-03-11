TEMPLATE = subdirs

include(config.pri)

SUBDIRS = src  

CONFIG += mobility ordered
MOBILITY = location

QMAKE_CFLAGS_DEBUG += "-g3 -Werror"
QMAKE_CXXFLAGS_DEBUG +="-g3 -Werror"

