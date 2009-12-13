if QT

bin_PROGRAMS = DataManager

INCLUDES = -I@top_srcdir@/include @QT_INCLUDES@

DataManager_LDADD = @X_LDFLAGS@ \
	@QT_LDFLAGS@ \
	@USER_LDFLAGS@ \
	@LIB_QT@ \
	../odbcinst/libodbcinst.la \
	../DriverManager/libodbc.la 

DataManager_DEPENDANCIES = \
    ../odbcinst/libodbcinst.la \
	../DriverManager/libodbc.la 

DataManager_SOURCES = \
	classCanvas.cpp \
	classColumn.cpp \
	classDataManager.cpp \
	classDataSource.cpp \
	classDataSources.cpp \
	classDriver.cpp \
	classDrivers.cpp \
	classISQL.cpp \
	classIndex.cpp \
	classIndexs.cpp \
	classLogin.cpp \
	classNode.cpp \
	classODBC.cpp \
	classPrimaryKeys.cpp \
	classSpecialColumns.cpp \
	classTable.cpp \
	classTables.cpp \
	classConnectionFrame.cpp \
	classBrowseFrame.cpp \
	classBrowse.cpp \
	main.cpp \
	mclassDataManager.cpp \
	mclassCanvas.cpp \
	mclassLogin.cpp \
	mclassISQL.cpp \
	mclassConnectionFrame.cpp \
	mclassBrowseFrame.cpp \
	mclassBrowse.cpp 

mclassDataManager.cpp: @srcdir@/classDataManager.h
	@MOC@ @srcdir@/classDataManager.h -o mclassDataManager.cpp

mclassCanvas.cpp: @srcdir@/classCanvas.h
	@MOC@ @srcdir@/classCanvas.h -o mclassCanvas.cpp

mclassLogin.cpp: @srcdir@/classLogin.h
	@MOC@ @srcdir@/classLogin.h -o mclassLogin.cpp

mclassISQL.cpp: @srcdir@/classISQL.h
	@MOC@ @srcdir@/classISQL.h -o mclassISQL.cpp

mclassConnectionFrame.cpp: @srcdir@/classConnectionFrame.h
	@MOC@ @srcdir@/classConnectionFrame.h -o mclassConnectionFrame.cpp

mclassBrowseFrame.cpp: @srcdir@/classBrowseFrame.h
	@MOC@ @srcdir@/classBrowseFrame.h -o mclassBrowseFrame.cpp

mclassBrowse.cpp: @srcdir@/classBrowse.h
	@MOC@ @srcdir@/classBrowse.h -o mclassBrowse.cpp

endif

EXTRA_DIST = \
	classCanvas.h \
	classColumn.h \
	classDataManager.h \
	classDataSource.h \
	classDataSources.h \
	classDriver.h \
	classDrivers.h \
	classConnectionFrame.h \
	classBrowseFrame.h \
	classBrowse.h \
	classISQL.h \
	classIndex.h \
	classIndexs.h \
	classLogin.h \
	classNode.h \
	classODBC.h \
	classPrimaryKeys.h \
	classSpecialColumns.h \
	classTable.h \
	classTables.h \
	LinuxODBC.xpm \
	column.xpm \
	computergreen.xpm \
	computerred.xpm \
	datasourceoff.xpm \
	datasourceon.xpm \
	datasourcessystem.xpm \
	datasourcesuser.xpm \
	driver.xpm \
	drivers.xpm \
	exec.xpm \
	id.xpm \
	keygold.xpm \
	keyred.xpm \
	keysilver.xpm \
	keysilver2.xpm \
	new.xpm \
	open.xpm \
	run.xpm \
	save.xpm \
	table.xpm \
	tables.xpm \
	ODBC.xpm
