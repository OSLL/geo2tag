lib_LTLIBRARIES = libboundparam.la 

INCLUDES = -I@top_srcdir@/include 

libboundparam_la_LDFLAGS = \
    -version-info @LIB_VERSION@ \
    -export-dynamic \
	-module

if QNX
libboundparam_la_LIBADD = \
	../autotest/libgtrtstlc.la \
	-L${prefix}/lib -lodbc 
else
libboundparam_la_LIBADD = \
	../autotest/libgtrtstlc.la \
	../DriverManager/libodbc.la 
endif

libboundparam_la_SOURCES = \
	boundparam.c \
	helper.c \
	cboundtimestampparam.c

EXTRA_DIST = \
    cursor.c \
	quiktest.h \
	quiktest.c \
	custom.c \
	custom.h \
	helper.h \
	libboundparam.exp
