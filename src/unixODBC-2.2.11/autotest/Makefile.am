lib_LTLIBRARIES = libgtrtst.la
noinst_LTLIBRARIES = libgtrtstlc.la

INCLUDES = -I@top_srcdir@/include -I@top_srcdir@/odbctest

EXTRA_DIST = \
	gtrtst.exp

libgtrtst_la_LDFLAGS = \
	-version-info @LIB_VERSION@ \
	-export-dynamic \
	-export-symbols @srcdir@/gtrtst.exp 

libgtrtst_la_DEPENDENCIES = \
	../DriverManager/libodbc.la 

libgtrtst_la_SOURCES = \
	gtrtst.c

libgtrtstlc_la_SOURCES = \
	gtrtst.c
