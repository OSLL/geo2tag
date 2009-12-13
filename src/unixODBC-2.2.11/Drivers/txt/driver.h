/**********************************************
 * Driver.h
 *
 * Description:
 *
 * This is all of the stuff that is common among ALL drivers (but not to the DriverManager).
 *
 * Make sure that your driver specific driverextras.h exists!
 *
 **********************************************************************
 *
 * This code was created by Peter Harvey (mostly during Christmas 98/99).
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************/		
#ifndef _H_DRIVER
#define _H_DRIVER

/*****************************************************************************
 * ODBC VERSION (THAT THIS DRIVER COMPLIES WITH)
 *****************************************************************************/
#define ODBCVER 0x0351 /* this is optimistic to say the least but we are heading to full 3.51 compliance */

#include <log.h>
#include <ini.h>
#include <sqp.h>
#include <sqlext.h>
#include <odbcinstext.h>
#include "driverextras.h"

#define SQL_MAX_CURSOR_NAME 100

/*****************************************************************************
 * STATEMENT
 *****************************************************************************/
typedef struct     tDRVSTMT
{
	struct tDRVSTMT	*pPrev;									/* prev struct or null		*/
	struct tDRVSTMT	*pNext;									/* next struct or null		*/
    SQLPOINTER 		hDbc;									/* pointer to DB context    */

    SQLCHAR    		szCursorName[SQL_MAX_CURSOR_NAME];		/* name of cursor           */
    SQLCHAR    		*pszQuery;								/* query string             */
	long			nRowsAffected;							/* is set when SQL Executed	*/
	SQLCHAR     	szSqlMsg[LOG_MSG_MAX];					/* buff to format msgs		*/
	HLOG			hLog;									/* handle to msg logs		*/
    SQLUSMALLINT    *pRowStatusArray;                       /* SQL_ROW_NOROW etc        */

    HSTMTEXTRAS 	hStmtExtras;							/* DRIVER SPECIFIC STORAGE  */

} DRVSTMT, *HDRVSTMT;

/*****************************************************************************
 * CONNECTION
 *****************************************************************************/
typedef struct     tDRVDBC
{
	struct tDRVDBC	*pPrev;								/* prev struct or null		*/
	struct tDRVDBC	*pNext;								/* next struct or null		*/
    SQLPOINTER		hEnv; 	                           	/* pointer to ENV structure */
	HDRVSTMT		hFirstStmt;							/* first in list or null	*/
	HDRVSTMT		hLastStmt;							/* last in list or null		*/

	SQLCHAR     	szSqlMsg[LOG_MSG_MAX];				/* buff to format msgs		*/
	HLOG			hLog;								/* handle to msg logs		*/

    int	        	bConnected;                         /* TRUE on open connection  */
    HDBCEXTRAS  	hDbcExtras;                         /* DRIVER SPECIFIC DATA     */

} DRVDBC, *HDRVDBC;

/*****************************************************************************
 * ENVIRONMENT
 *****************************************************************************/
typedef struct     tDRVENV
{
	HDRVDBC		hFirstDbc;							/* first in list or null	*/
	HDRVDBC		hLastDbc;							/* last in list or null		*/

    SQLCHAR     szSqlMsg[LOG_MSG_MAX];				/* buff to format msgs		*/
	HLOG		hLog;								/* handle to msg logs		*/

    HENVEXTRAS  hEnvExtras;

} DRVENV, *HDRVENV;


/*****************************************************************************
 * API - private 
 *
 * - Do not let these be the same name as any funcs in the libodbc.so. Doing so
 *   will cause enough confusion in ld that it will call the func in libodbc.so when
 *   you really want the func in here. Very bad.
 *
 *****************************************************************************/
SQLRETURN SQLAllocEnv_      ( SQLHENV *phDrvEnv );
SQLRETURN SQLFreeEnv_       ( SQLHENV hDrvEnv );

SQLRETURN SQLAllocConnect_  ( SQLHENV hDrvEnv, SQLHDBC *phDrvDbc );
SQLRETURN SQLFreeConnect_   ( SQLHDBC hDrvDbc );
SQLRETURN FreeDbc_          ( HDRVDBC hDrvDbc );
SQLRETURN FreeDbcList_      ( HDRVENV hDrvEnv );

SQLRETURN SQLAllocStmt_     ( SQLHDBC hDrvDbc, SQLHSTMT *phDrvStmt );
SQLRETURN SQLFreeStmt_      ( SQLHSTMT hDrvStmt, SQLUSMALLINT nOption );
SQLRETURN FreeStmt_         ( HDRVSTMT hDrvStmt );
SQLRETURN FreeStmtList_     ( HDRVDBC hDrvDbc );

SQLRETURN SQLConnect_       ( SQLHDBC hDrvDbc, SQLCHAR *szDataSource, SQLSMALLINT nDataSourceLength, SQLCHAR * szUID, SQLSMALLINT nUIDLength, SQLCHAR * szPWD, SQLSMALLINT nPWDLength );


void      ResetStmt_( HDRVSTMT hStmt );
SQLRETURN FreeResultSet_( HSTMTEXTRAS hStmt );

/**********************************************
 * I/O ENTRY API: REIMPLEMENT THESE FOR EACH DATABASE TYPE WHEN MORE FLEXIBILITY IS NEEDED
 **********************************************/
typedef enum
{
    IOInsert,
    IOUpdate,
    IODelete,
    IOSelect,
    IOCreate,
    IODrop

} IOMode;

typedef SQLPOINTER HTABLE;

SQLRETURN IOUpdateTable( HDRVSTMT hStmt );
SQLRETURN IOSelectTable( HDRVSTMT hStmt );
SQLRETURN IOInsertTable( HDRVSTMT hStmt );
SQLRETURN IODeleteTable( HDRVSTMT hStmt );
SQLRETURN IOCreateTable( HDRVSTMT hStmt );
SQLRETURN IODropTable  ( HDRVSTMT hStmt );

/**********************************************
 * I/O SUPPORT API: REIMPLEMENT THESE FOR EACH DATABASE TYPE (i.e. text, xbase, ms access...)
 **********************************************/
int IOTableOpen( HTABLE *phTable, HDRVSTMT hStmt, char *szTableName, IOMode nMode );
int IOTableClose( HTABLE *phTable );
int IOTableHeaderRead( HTABLE hTable, HCOLUMNS *phColumns, long *pnCols );
int IOTableHeaderWrite( HTABLE hTable, HCOLUMNS hColumns, long nCols );
int IOTableRead( HTABLE hTable, HROW *phRow, long nCols );
int IOTableWrite( HTABLE hTable, HROW hRow, long nCols );
void IOSort( HLST hOrderBy, sqpOrder nOrder, HROWS hRows, long left, long right );
void IOXrefWhere( HSQPCOND hWhere, HCOLUMNS hColumns, long nCols );
int IOLike( char *pRow, char *pVal, char cEscape, HDBCEXTRAS hDbcExtras );
int IOComp( HROW hRow, HSQPCOMPARISON hComp, HDBCEXTRAS hDbcExtras );
int IOWhere( HROW hRow, HSQPCOND hWhere, HDBCEXTRAS hDbcExtras );

#endif


