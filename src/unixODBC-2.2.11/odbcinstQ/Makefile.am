libodbcinstQ_la_LDFLAGS = \
        -version-info 1:0:0 \
		-export-dynamic \
		-export-symbols @srcdir@/odbcinstQ.exp \
	    -module

if QT

lib_LTLIBRARIES = libodbcinstQ.la 

INCLUDES = -I@top_srcdir@/include @QT_INCLUDES@ \
    -DSYSTEM_FILE_PATH=\"@sysconfdir@\" -DDEFLIB_PATH=\"@libdir@\" $(INCLTDL)

libodbcinstQ_la_LIBADD = @X_LDFLAGS@ \
	@QT_LDFLAGS@ \
	@USER_LDFLAGS@ \
	@LIB_QT@ \
	@LIBLTDL@ \
	../odbcinst/libodbcinst.la \
	../extras/libodbcextraslc.la \
	../DriverManager/libodbc.la 

libodbcinstQ_la_DEPENDENCIES = \
	../odbcinst/libodbcinst.la \
	../extras/libodbcextraslc.la \
	../DriverManager/libodbc.la

CLEANFILES = \
	mCAbout.cpp \
	mCCredits.cpp \
	mCDriverPrompt.cpp \
	mCDrivers.cpp \
	mCDSNList.cpp \
	mCFileDSN.cpp \
	mCFileList.cpp \
	mCFileSelector.cpp \
	mCODBCConfig.cpp \
	mCProperties.cpp \
	mCPropertiesFrame.cpp \
	mCStatDetails.cpp \
	mCStatSummary.cpp \
	mCStats.cpp \
	mCSystemDSN.cpp \
	mCTracing.cpp \
	mCUserDSN.cpp

nodist_libodbcinstQ_la_SOURCES = \
	mCAbout.cpp \
	mCCredits.cpp \
	mCDriverPrompt.cpp \
	mCDrivers.cpp \
	mCDSNList.cpp \
	mCFileDSN.cpp \
	mCFileList.cpp \
	mCFileSelector.cpp \
	mCODBCConfig.cpp \
	mCProperties.cpp \
	mCPropertiesFrame.cpp \
	mCStatDetails.cpp \
	mCStatSummary.cpp \
	mCStats.cpp \
	mCSystemDSN.cpp \
	mCTracing.cpp \
	mCUserDSN.cpp

libodbcinstQ_la_SOURCES = \
	CAbout.cpp \
	CCredits.cpp \
	CDLL.cpp \
	CDSNList.cpp \
	CDriverPrompt.cpp \
	CDrivers.cpp \
	CFileDSN.cpp \
	CFileList.cpp \
	CFileSelector.cpp \
	CODBCConfig.cpp \
	CProperties.cpp \
	CPropertiesFrame.cpp \
	CSplashDialog.cpp \
	CStatDetails.cpp \
	CStatSummary.cpp \
	CStats.cpp \
	CSystemDSN.cpp \
	CTracing.cpp \
	CUserDSN.cpp \
	SQLManageDataSources.cpp

mCDrivers.cpp: @srcdir@/CDrivers.h
	@MOC@ @srcdir@/CDrivers.h -o mCDrivers.cpp

mCODBCConfig.cpp: @srcdir@/CODBCConfig.h
	@MOC@ @srcdir@/CODBCConfig.h -o mCODBCConfig.cpp

mCSystemDSN.cpp: @srcdir@/CSystemDSN.h
	@MOC@ @srcdir@/CSystemDSN.h -o mCSystemDSN.cpp

mCFileDSN.cpp: @srcdir@/CFileDSN.h
	@MOC@ @srcdir@/CFileDSN.h -o mCFileDSN.cpp

mCUserDSN.cpp: @srcdir@/CUserDSN.h
	@MOC@ @srcdir@/CUserDSN.h -o mCUserDSN.cpp

mCTracing.cpp: @srcdir@/CTracing.h
	@MOC@ @srcdir@/CTracing.h -o mCTracing.cpp

mCStats.cpp: @srcdir@/CStats.h
	@MOC@ @srcdir@/CStats.h -o mCStats.cpp

mCStatSummary.cpp: @srcdir@/CStatSummary.h
	@MOC@ @srcdir@/CStatSummary.h -o mCStatSummary.cpp

mCStatDetails.cpp: @srcdir@/CStatDetails.h
	@MOC@ @srcdir@/CStatDetails.h -o mCStatDetails.cpp

mCAbout.cpp: @srcdir@/CAbout.h
	@MOC@ @srcdir@/CAbout.h -o mCAbout.cpp

mCDriverPrompt.cpp: @srcdir@/CDriverPrompt.h
	@MOC@ @srcdir@/CDriverPrompt.h -o mCDriverPrompt.cpp

mCPropertiesFrame.cpp: @srcdir@/CPropertiesFrame.h
	@MOC@ @srcdir@/CPropertiesFrame.h -o mCPropertiesFrame.cpp

mCProperties.cpp: @srcdir@/CProperties.h
	@MOC@ @srcdir@/CProperties.h -o mCProperties.cpp

mCFileSelector.cpp: @srcdir@/CFileSelector.h
	@MOC@ @srcdir@/CFileSelector.h -o mCFileSelector.cpp

mCDSNList.cpp: @srcdir@/CDSNList.h
	@MOC@ @srcdir@/CDSNList.h -o mCDSNList.cpp

mCFileList.cpp: @srcdir@/CFileList.h
	@MOC@ @srcdir@/CFileList.h -o mCFileList.cpp

mCCredits.cpp: @srcdir@/CCredits.h
	@MOC@ @srcdir@/CCredits.h -o mCCredits.cpp

endif

EXTRA_DIST = \
	CAbout.h \
	CTracing.h \
	CStats.h \
	CStatSummary.h \
	CSplashDialog.h \
	CStatDetails.h \
	CDLL.h \
	CDSNList.h \
	CFileList.h \
	CDriverPrompt.h \
	CDrivers.h \
	CFileSelector.h \
	CODBCConfig.h \
	CPropertiesFrame.h \
	CProperties.h \
	CSystemDSN.h \
	CFileDSN.h \
	CUserDSN.h \
	CCredits.h \
	Splash.xpm \
	ODBC.xpm \
	computer.xpm \
	driver.xpm \
	info.xpm \
	checkOk.xpm \
	checkCancel.xpm \
	person.xpm \
	server.xpm \
	smallLinuxODBC.xpm \
	about.xpm \
	dsn-file.xpm \
	dsn-system.xpm \
	dsn-user.xpm \
	stats.xpm \
	ODBC.xpm \
	Canada.xpm \
	GreatBritain.xpm \
	trace.xpm \
	NoFlag.xpm \
	advanced.xpm \
	odbcinstQ.exp


