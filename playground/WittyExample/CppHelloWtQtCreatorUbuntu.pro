#-------------------------------------------------
#
# Project created by QtCreator 2010-11-18T12:10:20
#
#-------------------------------------------------
QT       += core
QT       -= gui
TARGET = hello.wt
LIBS += -L/usr/lib -lwt -lwtfcgi 
QMAKE_CXXFLAGS += -DNDEBUG -g3
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
SOURCES += main.cpp
#QMAKE_CXX = g++-4.5
#QMAKE_LINK = g++-4.5
