AUTOMAKE_OPTIONS=foreign
INCLUDES = @MYSQL_INCLUDES@
lib_LTLIBRARIES=libmyodbc3.la

libmyodbc3_la_SOURCES= \
catalog.c \
connect.c \
dll.c \
execute.c \
handle.c\
info.c \
myodbc3.c \
options.c \
prepare.c \
results.c \
transact.c \
utility.c \
dialogs.h \
myodbc3.h \
resource.h \
cursor.c \
error.c \
misc.c

libmyodbc3_la_LDFLAGS=-release @myodbc_version@
libmyodbc3_la_LIBADD=@MYSQL_LIB_PATH@ -lmysqlclient
LDFLAGS=@EXTRA_LDFLAGS@





