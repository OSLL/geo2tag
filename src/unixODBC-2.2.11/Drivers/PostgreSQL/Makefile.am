lib_LTLIBRARIES = libodbcpsql.la

INCLUDES = -I@top_srcdir@/include

libodbcpsql_la_LDFLAGS = \
	-version-info 1:0:0 \
	$(LIBSOCKET) $(LIBNSL) \
	-module

DEFS = -DUNIXODBC @DEFS@

EXTRA_DIST = \
	bind.h \
	columninfo.h \
	connection.h \
	convert.h \
	dlg_specific.h \
	environ.h \
	lobj.h \
	misc.h \
	pgtypes.h \
	psqlodbc.h \
	qresult.h \
	resource.h \
	socket.h \
	statement.h \
	tuple.h \
	tuplelist.h 

libodbcpsql_la_LIBADD = \
	../../lst/liblstlc.la \
	../../log/libloglc.la \
	../../ini/libinilc.la \
	../../odbcinst/libodbcinstlc.la \
	$(LIBLTDL) $(LIBADD_DL) \
    $(LIBADD_CRYPT) $(LIBADD_POW)

libodbcpsql_la_DEPENDENCIES = \
	../../lst/liblstlc.la \
	../../log/libloglc.la \
	../../ini/libinilc.la \
	../../odbcinst/libodbcinstlc.la \
    ../../extras/libodbcextraslc.la

libodbcpsql_la_SOURCES = \
	bind.c \
	columninfo.c \
	connection.c \
	convert.c \
	dlg_specific.c \
	drvconn.c \
	environ.c \
	execute.c \
	info.c \
	lobj.c \
	misc.c \
	options.c \
	parse.c \
	pgtypes.c \
	psqlodbc.c \
	qresult.c \
	results.c \
	socket.c \
	statement.c \
	tuple.c \
	tuplelist.c
