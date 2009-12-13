if QT

bin_PROGRAMS = ODBCConfig

INCLUDES = -I@top_srcdir@/include @QT_INCLUDES@ \
    -DSYSTEM_FILE_PATH=\"@sysconfdir@\" -DDEFLIB_PATH=\"@libdir@\" $(INCLTDL)

ODBCConfig_LDADD = @X_LDFLAGS@ \
	@QT_LDFLAGS@ \
	@USER_LDFLAGS@ \
	@LIB_QT@ \
	../odbcinst/libodbcinst.la \
	../extras/libodbcextraslc.la \
	../DriverManager/libodbc.la 

ODBCConfig_DEPENDANCIES = ../odbcinst/libodbcinst.la ../extras/libodbcextraslc.la ../DriverManager/libodbc.la

ODBCConfig_SOURCES = \
	main.cpp

endif
