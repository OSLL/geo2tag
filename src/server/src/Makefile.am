SUBDIRS = json service

bin_PROGRAMS = g2t_fcgi

g2t_fcgi_SOURCES = main.cpp

g2t_fcgi_LDADD = -lfcgi json/libjson.la service/libg2tservice.la
g2t_fcgi_CPPFLAGS = -Iinterface

AM_CFLAGS = -Wall
