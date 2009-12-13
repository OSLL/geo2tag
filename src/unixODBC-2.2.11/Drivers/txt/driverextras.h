/**********************************************
 * driverextras.h
 *
 * Purpose:
 *
 * To define driver specifc extras such as structs to be included
 * along side the common ODBC includes.
 *
 * Description:
 *
 * The short-term storage a driver requires as infrastructure varies somewhat from
 * DBMS to DBMS. The ODBC DriverManager requires predictable storage and it is defined
 * in include files such as hstmt.h. The Driver also requires predictable storage and
 * it is defined in driver.h. Storage *specific to a type of driver* is defined here.
 *
 * The three main storage items are the ENV, DBC, and STMT structs. These are defined
 * as type void * in sql.h.
 *
 * So if your driver requires extra storage (and it probably does) then define
 * the storage within these structs, allocate/initialize as required. Cast them
 * to and from the standard names as required.
 *
 * For example;
 *
 * App			DM				|DRV				
 *				(as per hdbc.h)	|(as per driver.h)	
 *====================================================================
 *	hDbc=void*	hDbc=SQLHDBC	hDbc=HDBCEXTRAS
 *--------------------------------------------------------------------
 *
 * DO NOT FORGET TO FREE ANY ALLOCATED MEMORY (at some point)
 *
 **********************************************************************
 *
 * This code is LGPL. Please ensure that this message remains in future
 * distributions and uses of this code (thats about all I get out of it).
 * - Peter Harvey pharvey@codebydesign.com
 *
 **********************************************************************/

#ifndef DRIVEREXTRAS_H
#define DRIVEREXTRAS_H

/**********************************************
 * KEEP IT SIMPLE; PUT ALL DRIVER INCLUDES HERE THEN EACH DRIVER MODULE JUST INCLUDES THIS ONE FILE
 **********************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>

#include <sqp.h>

/**********************************************
 * STRING: FOR READABILITY
 **********************************************/
typedef char * HVALUE;

/**********************************************
 * RESULTSET ROW: ARRAY OF STRINGS
 **********************************************/
typedef HVALUE * HROW;

/**********************************************
 * RESULTSET ROWS: ARRAY OF HROW
 **********************************************/
typedef HROW * HROWS;

/**********************************************
 * RESULTSET COLUMN: DESCRIBES COLUMN
 **********************************************/
#define MAXCOLUMNNAME_LEN FILENAME_MAX
#define DEFAULTCOLUMNNAME "%ld"
#define DEFAULTCOLUMNTYPE SQL_VARCHAR
#define DEFAULTCOLUMNLENGTH 255
#define DEFAULTCOLUMNPRECISION 0
typedef struct
{
    char *          pszTable;               /* TABLE CONTAINING THIS COLUMN (no path)   */
    char *          pszName;                /* USER NAME i.e. COLUMN1, COLUMN2...       */
	short			nType;			        /* C DATA TYPE ie SQL_C_CHAR			    */
    short           nLength;                /* NUMBER OF CHARS (even for NUMERIC)       */
    short           nPrecision;             /* NUMBER OF DECIMALS WHEN NUMERIC          */

} COLUMN, *HCOLUMN;

/**********************************************
 * RESULTSET COLUMNS: ARRAY OF HCOLUMN
 **********************************************/
typedef HCOLUMN * HCOLUMNS;

/**********************************************
 * BOUNDCOLUMN: ONE OF THIS FOR EACH BOUND COLUMN
 **********************************************/
typedef struct tBOUNDCOLUMN
{
    long            nCol;                   /* SOURCE COLUMN                            */
	short			nTargetType;			/* BIND: C DATA TYPE ie SQL_C_CHAR			*/
	char 			*pTargetValue;			/* BIND: POINTER FROM APPLICATION TO COPY TO*/
	long			nTargetValueMax;		/* BIND: MAX SPACE IN pTargetValue			*/
	long			*pnLengthOrIndicator;	/* BIND: TO RETURN LENGTH OR NULL INDICATOR	*/

} BOUNDCOLUMN, *HBOUNDCOLUMN;

/**********************************************
 * BOUNDCOLUMN: ARRAY OF HBOUNDCOLUMN
 **********************************************/
typedef HBOUNDCOLUMN * HBOUNDCOLUMNS;

/**********************************************
 * RESULTSET: RESULTS
 **********************************************/
typedef struct
{
    HROWS           hRows;                  /* ARRAY OF HROW                            */
    long            nRows;                  /* ROWS IN hRows                            */
    long            nRow;                   /* CURRENT HROW (1 based)                   */

    HCOLUMNS        hColumns;               /* ARRAY OF HCOLUMN                         */
    long            nCols;                  /* COLS IN hColumns                         */
    long            nCol;                   /* CURRENT HCOLUMN (1 based)                */
                    
    HBOUNDCOLUMNS   hBoundColumns;          /* ARRAY OF HBOUNDCOLUMN                    */
    long            nBoundColumns;          /* COLS IN hBoundColumns                    */
    long            nBoundColumn;           /* CURRENT HBOUNDCOLUMN (1 based)           */

} RESULTSET, *HRESULTSET;


/*******************************************************************
 *******************************************************************/


/**********************************************
 * ENVIRONMENT: DRIVER SPECIFIC STUFF THAT NEEDS TO BE STORED IN THE DRIVERS ENVIRONMENT
 **********************************************/
typedef struct tENVEXTRAS
{
    int             nDummy;                 /* NO EXTRAS HERE                           */

} ENVEXTRAS, *HENVEXTRAS;

/**********************************************
 * CONNECTION: DRIVER SPECIFIC STUFF THAT NEEDS TO BE STORED IN THE DRIVERS CONNECTIONS
 **********************************************/
#define DEFAULTCOLUMNSEPERATOR '|'
#define CATALOGNAME "TableDetails.catalog"
typedef struct tDBCEXTRAS
{
    char *          pszDatabase;            /* DSN NAME                                 */
    char *          pszDirectory;           /* DIR WHICH HAS TABLE FILES                */
                                            /* (extracted from DSN or default ~)        */
    char            cColumnSeperator;       /* COLUMN VALUES END WITH THIS              */
                                            /* (extracted from DSN or default ',')      */
    int             bCatalog;               /* DIR HAS A CATALOG FILE (CATALOGNAME)     */
    int             bCaseSensitive;         /* DATA IS CASE SENSITIVE (default yes)     */
} DBCEXTRAS, *HDBCEXTRAS;


/**********************************************
 * STATEMENT: DRIVER SPECIFIC STUFF THAT NEEDS TO BE STORED IN THE DRIVERS STATEMENT
 **********************************************/
typedef struct tSTMTEXTRAS
{
    HRESULTSET      hResultSet;             /* RESULT SET (if any)                      */
	HSQPPARSEDSQL   hParsedSQL;			    /* PTR TO STRUCT CONTAINING THE PARSED SQL  */

    HLST            hParams;                /* BOUND PARAMS (if any)                    */

} STMTEXTRAS, *HSTMTEXTRAS;


HCOLUMN CreateColumn_   ( char *szTable, char *szName, short nType, short nLength, short nPrecision );
HROW    CreateRow_      ( int nCols, ... );
void    FreeRow_        ( HROW *phRow, long nCols );
void    FreeRows_       ( HROWS *phRows, long nRows, long nCols );
void    FreeColumn_     ( HCOLUMN *phColumn );
void    FreeColumns_    ( HCOLUMNS *phColumns, long nCols );

#endif


