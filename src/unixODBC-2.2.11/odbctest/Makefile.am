if QT

bin_PROGRAMS = odbctest

INCLUDES = -I@top_srcdir@/include @QT_INCLUDES@ -DSYSTEM_FILE_PATH=\"@sysconfdir@\" $(INCLTDL)

odbctest_LDADD = @X_LDFLAGS@ \
	@QT_LDFLAGS@ \
	@USER_LDFLAGS@ \
	@LIB_QT@ \
	../odbcinst/libodbcinst.la \
	../DriverManager/libodbc.la \
	$(LIBLTDL) $(LIBADD_DL)

odbctest_DEPENDANCIES = ../odbcinst/libodbcinst.la ../DriverManager/libodbc.la

CLEANFILES = \
	moc_results.cpp \
	moc_stmt.cpp \
	moc_odbctest.cpp \
	moc_desc.cpp \
	moc_diag.cpp \
	moc_env.cpp \
	moc_catalog.cpp \
	moc_conn.cpp \
	moc_attr.cpp \
	moc_tools.cpp

nodist_odbctest_SOURCES = \
	moc_results.cpp \
	moc_stmt.cpp \
	moc_odbctest.cpp \
	moc_desc.cpp \
	moc_diag.cpp \
	moc_env.cpp \
	moc_catalog.cpp \
	moc_conn.cpp \
	moc_attr.cpp \
	moc_tools.cpp

odbctest_SOURCES = \
	tools.cpp \
	stmt.cpp \
	results.cpp \
	open.cpp \
	odbctest.cpp \
	installer.cpp \
	env.cpp \
	edit.cpp \
	desc.cpp \
	diag.cpp \
	conn.cpp \
	catalog.cpp \
	attr.cpp

moc_results.cpp: @srcdir@/results.h
	@MOC@ @srcdir@/results.h -o moc_results.cpp

moc_stmt.cpp: @srcdir@/stmt.h
	@MOC@ @srcdir@/stmt.h -o moc_stmt.cpp

moc_odbctest.cpp: @srcdir@/odbctest.h
	@MOC@ @srcdir@/odbctest.h -o moc_odbctest.cpp

moc_desc.cpp: @srcdir@/desc.h
	@MOC@ @srcdir@/desc.h -o moc_desc.cpp

moc_diag.cpp: @srcdir@/diag.h
	@MOC@ @srcdir@/diag.h -o moc_diag.cpp

moc_env.cpp: @srcdir@/env.h
	@MOC@ @srcdir@/env.h -o moc_env.cpp

moc_catalog.cpp: @srcdir@/catalog.h
	@MOC@ @srcdir@/catalog.h -o moc_catalog.cpp

moc_conn.cpp: @srcdir@/conn.h
	@MOC@ @srcdir@/conn.h -o moc_conn.cpp

moc_attr.cpp: @srcdir@/attr.h
	@MOC@ @srcdir@/attr.h -o moc_attr.cpp

moc_tools.cpp: @srcdir@/tools.h
	@MOC@ @srcdir@/tools.h -o moc_tools.cpp

endif

EXTRA_DIST = \
	results.h \
	stmt.h \
	odbctest.h \
	desc.h \
	diag.h \
	env.h \
	catalog.h \
	conn.h \
	attr.h \
	tools.h 

