TEMPLATE = lib
TARGET =  json
DEPENDPATH += . inc src
INCLUDEPATH += . inc

# Input
HEADERS += inc/cast.h \
           inc/elements.h \
           inc/exception.h \
           inc/quick.h \
           inc/reader.h \
           inc/visitor.h \
           inc/writer.h \
           inc/quick.inl \
           inc/cast.inl
SOURCES += src/elements.cpp src/quick.cpp src/reader.cpp src/writer.cpp

DESTDIR = ../internal/lib
OBJECTS_DIR = .obj
MOC_DIR = .moc
CONFIG += DEBUG
