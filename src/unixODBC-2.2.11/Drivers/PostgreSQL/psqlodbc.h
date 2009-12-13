
/* File:            psqlodbc.h
 *
 * Description:     This file contains defines and declarations that are related to
 *                  the entire driver.
 *
 * Comments:        See "notice.txt" for copyright and license information.
 *
 */

#ifndef __PSQLODBC_H__
#define __PSQLODBC_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>	/* for FILE* pointers: see GLOBAL_VALUES */
#include <string.h>     /* for prototype of strlen() etc */ 

#ifndef WIN32
#include <stdlib.h>     /* for prototype of atof() etc */ 

#define Int4 int
#define UInt4 unsigned int
#define Int2 short
#define UInt2 unsigned short

#ifndef UNIXODBC
typedef float SFLOAT;
typedef double SDOUBLE;
#endif

#else
#define Int4 int
#define UInt4 unsigned int
#define Int2 short
#define UInt2 unsigned short
#endif

typedef UInt4 Oid;

/* Driver stuff */
/*#define ODBCVER				0x0200*/
#define DRIVER_ODBC_VER		"02.00"

#define DRIVERNAME             "PostgreSQL ODBC"
#define DBMS_NAME              "PostgreSQL"
#define DBMS_VERSION           "06.40.0007 PostgreSQL 6.4"
#define POSTGRESDRIVERVERSION  "06.40.0007"

#ifdef WIN32
#define DRIVER_FILE_NAME		"PSQLODBC.DLL"
#else

#ifdef UNIXODBC
#define DRIVER_FILE_NAME		"libodbcpsql.so"
#else
#define DRIVER_FILE_NAME		"libpsqlodbc.so"
#endif

#endif

/* Limits */
#define MAX_MESSAGE_LEN				65536
#define MAX_CONNECT_STRING			4096
#define ERROR_MSG_LENGTH			4096
#define FETCH_MAX					100		/* default number of rows to cache for declare/fetch */
#define TUPLE_MALLOC_INC			100
#define SOCK_BUFFER_SIZE			4096	/* default socket buffer size */
#define MAX_CONNECTIONS				128		/* conns per environment (arbitrary)  */
#define MAX_FIELDS					512
#define BYTELEN						8
#define VARHDRSZ					sizeof(Int4)

#define MAX_TABLE_LEN				32
#define MAX_COLUMN_LEN				32
#define MAX_CURSOR_LEN				32

/*	Registry length limits */
#define LARGE_REGISTRY_LEN			4096	/* used for special cases */
#define MEDIUM_REGISTRY_LEN			256		/* normal size for user,database,etc. */
#define SMALL_REGISTRY_LEN			10		/* for 1/0 settings */


/*	These prefixes denote system tables */
#define POSTGRES_SYS_PREFIX	"pg_"
#define KEYS_TABLE			"dd_fkey"

/*	Info limits */
#define MAX_INFO_STRING		128
#define MAX_KEYPARTS		20
#define MAX_KEYLEN			512			/*//	max key of the form "date+outlet+invoice" */
#define MAX_STATEMENT_LEN	MAX_MESSAGE_LEN


#define PG62	"6.2"		/* "Protocol" key setting to force Postgres 6.2 */
#define PG63	"6.3"		/* "Protocol" key setting to force postgres 6.3 */
#define PG64	"6.4"

typedef struct ConnectionClass_ ConnectionClass;
typedef struct StatementClass_ StatementClass;
typedef struct QResultClass_ QResultClass;
typedef struct SocketClass_ SocketClass;
typedef struct BindInfoClass_ BindInfoClass;
typedef struct ParameterInfoClass_ ParameterInfoClass;
typedef struct ColumnInfoClass_ ColumnInfoClass;
typedef struct TupleListClass_ TupleListClass;
typedef struct EnvironmentClass_ EnvironmentClass;
typedef struct TupleNode_ TupleNode;
typedef struct TupleField_ TupleField;

typedef struct col_info COL_INFO;
typedef struct lo_arg LO_ARG;

typedef struct GlobalValues_
{
	int					fetch_max;
	int					socket_buffersize;
	int					unknown_sizes;
	int					max_varchar_size;
	int					max_longvarchar_size;
	char				debug;
	char				commlog;
	char				disable_optimizer;
	char				ksqo;
	char				unique_index;
	char				readonly;
	char				use_declarefetch;
	char				text_as_longvarchar;
	char				unknowns_as_longvarchar;
	char				bools_as_char;
	char				lie;
	char				parse;
	char				cancel_as_freestmt;
	char				extra_systable_prefixes[MEDIUM_REGISTRY_LEN];
	char				conn_settings[LARGE_REGISTRY_LEN];
	char				protocol[SMALL_REGISTRY_LEN];

	FILE*				mylogFP;
	FILE*				qlogFP;	
} GLOBAL_VALUES;

/*
 * rename to avoid colision
 */

#define global      pg_global

typedef struct StatementOptions_ {
	int maxRows;
	int maxLength;
	int rowset_size;
	int keyset_size;
	int cursor_type;
	int scroll_concurrency;
	int retrieve_data;
	int bind_size;		        /* size of each structure if using Row Binding */
	int use_bookmarks;
} StatementOptions;

/*	Used to pass extra query info to send_query */
typedef struct QueryInfo_ {
	int				row_size;
	QResultClass	*result_in;
	char			*cursor;
} QueryInfo;


#define PG_TYPE_LO				-999	/* hack until permanent type available */
#define PG_TYPE_LO_NAME			"lo"
#define OID_ATTNUM				-2		/* the attnum in pg_index of the oid */

/* sizes */
#define TEXT_FIELD_SIZE			65536	/* size of text fields (not including null term) */
#define NAME_FIELD_SIZE			32		/* size of name fields */
#define MAX_VARCHAR_SIZE		254		/* maximum size of a varchar (not including null term) */


#include "misc.h"

RETCODE SQL_API PG__SQLAllocStmt(HDBC hdbc, HSTMT FAR *phstmt);

RETCODE SQL_API PG__SQLBindCol(
        HSTMT      hstmt,
        UWORD      icol,
        SWORD      fCType,
        PTR        rgbValue,
        SQLLEN     cbValueMax,
        SQLLEN     *pcbValue);

RETCODE SQL_API PG__SQLColumns(
                           HSTMT        hstmt,
                           UCHAR FAR *  szTableQualifier,
                           SWORD        cbTableQualifier,
                           UCHAR FAR *  szTableOwner,
                           SWORD        cbTableOwner,
                           UCHAR FAR *  szTableName,
                           SWORD        cbTableName,
                           UCHAR FAR *  szColumnName,
                           SWORD        cbColumnName);

RETCODE SQL_API PG__SQLExecDirect(
        HSTMT     hstmt,
        UCHAR FAR *szSqlStr,
        SDWORD    cbSqlStr);

RETCODE SQL_API PG__SQLExecute(HSTMT hstmt);

RETCODE SQL_API PG__SQLExtendedFetch(
        HSTMT      hstmt,
        UWORD      fFetchType,
        SDWORD     irow,
        UDWORD FAR *pcrow,
        UWORD  FAR *rgfRowStatus);

RETCODE SQL_API PG__SQLFetch(HSTMT hstmt);

RETCODE SQL_API PG__SQLFreeStmt(HSTMT hstmt, UWORD fOption);

RETCODE SQL_API PG__SQLGetData(
        HSTMT      hstmt,
        UWORD      icol,
        SWORD      fCType,
        PTR        rgbValue,
        SQLLEN     cbValueMax,
        SQLLEN     *pcbValue);

#endif
