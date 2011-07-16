#-------------------------------------------------
#
# Project created by QtCreator 2010-11-18T12:10:20
#
#-------------------------------------------------
QT       += core
QT       -= gui
TARGET = hello.wt
LIBS += -L/usr/lib -lwt -lwthttp
QMAKE_CXXFLAGS += -DNDEBUG
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
SOURCES += main.cpp
