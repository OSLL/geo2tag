lib_LTLIBRARIES = libodbc.la

INCLUDES = -I@top_srcdir@/include \
		$(INCLTDL) \
		-DSHLIBEXT=\"@SHLIBEXT@\" \
		-DSYSTEM_FILE_PATH=\"@sysconfdir@\" \
		-DSYSTEM_LIB_PATH=\"@libdir@\" \
		-DUNICODE_ENCODING=\"@ICONV_UNICODE_ENCODING@\" \
		-DASCII_ENCODING=\"@ICONV_CHAR_ENCODING@\" 

EXTRA_DIST = \
	drivermanager.h \
	DriverManager.exp \
    __stats.h \
	drivermanager_axp.opt

libodbc_la_LIBADD = \
	../lst/liblstlc.la \
    ../log/libloglc.la \
	../ini/libinilc.la \
	../odbcinst/libodbcinstlc.la \
	$(LIBLTDL) $(LIBADD_DL) \
	$(LIBICONV)

libodbc_la_LDFLAGS = \
	-version-info @LIB_VERSION@ \
	-export-dynamic \
	-export-symbols @srcdir@/DriverManager.exp

libodbc_la_DEPENDENCIES = $(LIBLTDL) \
	../lst/liblstlc.la \
    ../log/libloglc.la \
    ../ini/libinilc.la \
    ../odbcinst/libodbcinstlc.la 

libodbc_la_SOURCES = \
	SQLAllocConnect.c \
	SQLAllocEnv.c \
	SQLAllocHandle.c \
	SQLAllocHandleStd.c \
	SQLAllocStmt.c \
	SQLBindCol.c \
	SQLBindParam.c \
	SQLBindParameter.c \
	SQLBrowseConnect.c \
	SQLBulkOperations.c \
	SQLCancel.c \
	SQLCloseCursor.c \
	SQLColAttribute.c \
	SQLColAttributes.c \
	SQLColumnPrivileges.c \
	SQLColumns.c \
	SQLConnect.c \
	SQLCopyDesc.c \
	SQLDataSources.c \
	SQLDescribeCol.c \
	SQLDescribeParam.c \
	SQLDisconnect.c \
	SQLDriverConnect.c \
	SQLDrivers.c \
	SQLEndTran.c \
	SQLError.c \
	SQLExecDirect.c \
	SQLExecute.c \
	SQLExtendedFetch.c \
	SQLFetch.c \
	SQLFetchScroll.c \
	SQLForeignKeys.c \
	SQLFreeConnect.c \
	SQLFreeEnv.c \
	SQLFreeHandle.c \
	SQLFreeStmt.c \
	SQLGetConnectAttr.c \
	SQLGetConnectOption.c \
	SQLGetCursorName.c \
	SQLGetData.c \
	SQLGetDescField.c \
	SQLGetDescRec.c \
	SQLGetDiagField.c \
	SQLGetDiagRec.c \
	SQLGetEnvAttr.c \
	SQLGetFunctions.c \
	SQLGetInfo.c \
	SQLGetStmtAttr.c \
	SQLGetStmtOption.c \
	SQLGetTypeInfo.c \
	SQLMoreResults.c \
	SQLNativeSql.c \
	SQLNumParams.c \
	SQLNumResultCols.c \
	SQLParamData.c \
	SQLParamOptions.c \
	SQLPrepare.c \
	SQLPrimaryKeys.c \
	SQLProcedureColumns.c \
	SQLProcedures.c \
	SQLPutData.c \
	SQLRowCount.c \
	SQLSetConnectAttr.c \
	SQLSetConnectOption.c \
	SQLSetCursorName.c \
	SQLSetDescField.c \
	SQLSetDescRec.c \
	SQLSetEnvAttr.c \
	SQLSetParam.c \
	SQLSetPos.c \
	SQLSetScrollOptions.c \
	SQLSetStmtAttr.c \
	SQLSetStmtOption.c \
	SQLSpecialColumns.c \
	SQLStatistics.c \
	SQLTablePrivileges.c \
	SQLTables.c \
	SQLTransact.c \
	SQLBrowseConnectW.c \
	SQLColAttributeW.c \
	SQLColAttributesW.c \
	SQLColumnPrivilegesW.c \
	SQLColumnsW.c \
	SQLConnectW.c \
	SQLDataSourcesW.c \
	SQLDescribeColW.c \
	SQLDriverConnectW.c \
	SQLDriversW.c \
	SQLErrorW.c \
	SQLExecDirectW.c \
	SQLForeignKeysW.c \
	SQLGetConnectAttrW.c \
	SQLGetConnectOptionW.c \
	SQLGetCursorNameW.c \
	SQLGetDescFieldW.c \
	SQLGetDescRecW.c \
	SQLGetDiagFieldW.c \
	SQLGetDiagRecW.c \
	SQLGetInfoW.c \
	SQLGetStmtAttrW.c \
	SQLGetTypeInfoW.c \
	SQLNativeSqlW.c \
	SQLPrepareW.c \
	SQLPrimaryKeysW.c \
	SQLProcedureColumnsW.c \
	SQLProceduresW.c \
	SQLSetConnectAttrW.c \
	SQLSetConnectOptionW.c \
	SQLSetCursorNameW.c \
	SQLSetDescFieldW.c \
	SQLSetStmtAttrW.c \
	SQLSpecialColumnsW.c \
	SQLStatisticsW.c \
	SQLTablePrivilegesW.c \
	SQLTablesW.c \
	__connection.c \
	__handles.c \
	__info.c \
	__stats.c \
	__attribute.c
