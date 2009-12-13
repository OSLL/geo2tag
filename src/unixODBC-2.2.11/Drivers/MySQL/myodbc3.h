/***************************************************************************  
 * Copyright (C) 1995-2002 MySQL AB, www.mysql.com                         *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program; if not, write to the Free Software Foundation  *
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA           *
****************************************************************************/

/*************************************************************************** 
 * MYODBC3.H                                                               *
 *                                                                         *
 * @description: Definations neaded by the myodbc3 driver                  *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

#ifndef __MYODBC3_H__
#define __MYODBC3_H__


#define ODBCVER 0x0351
#ifdef APSTUDIO_READONLY_SYMBOLS
#define WIN32   /* Hack for rc files */
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef RC_INVOKED
#define stdin
#endif

#ifdef HAVE_CONFIG_H
#include "myconf.h"
#undef PACKAGE
#undef VERSION
#endif

#include <my_global.h>
#include <my_sys.h>
#include <mysql.h>
#include <my_list.h>
#include <m_string.h>
#ifdef THREAD
#include <my_pthread.h>
#else
#define pthread_mutex_lock(A)
#define pthread_mutex_unlock(A)
#define pthread_mutex_init(A,B)
#define pthread_mutex_destroy(A)
#endif

#ifdef __cplusplus
}
#endif

#ifndef _UNIX_
#include <windowsx.h>
#ifndef RC_INVOKED
#pragma pack(1)
#endif

#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#endif

#ifdef _UNIX_

/**
** if iODBC/unixODBC exits include their headers
**/
#if defined(HAVE_ISQL_H) && defined(HAVE_ISQLEXT_H)
# include <isql.h>
# include <isqlext.h>
#elif defined(HAVE_SQL_H) && defined(HAVE_SQLEXT_H)
# include <sql.h>
# include <sqlext.h>
#endif

#if defined(HAVE_SQLGETPRIVATEPROFILESTRING)
# if defined(HAVE_IODBCINST_H)
#  include <iodbcinst.h>
# elif defined(HAVE_ODBCINST_H)
#  include <odbcinst.h>
# endif
#else

#define SQLGetPrivateProfileString _myodbc_SQLGetPrivateProfileString
#endif /* HAVE_IODBCINST */

/*
  If SQL_API is not defined, define it, unixODBC doesn't have this
*/
#if !defined(SQL_API)
# define SQL_API
#endif

#endif /* IS UNIX */

#include "error.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#if defined(_WIN32) || defined(WIN32)
#define INTFUNC  __stdcall
#define EXPFUNC  __stdcall
#else
#define INTFUNC PASCAL
#define EXPFUNC __export CALLBACK
#endif

#ifdef SQL_SPEC_STRING
#undef SQL_SPEC_STRING
#endif

#define SQL_SPEC_STRING   "03.51"
#define DRIVER_VERSION    "03.51.02"
#define ODBC_DRIVER       "ODBC 3.51 Driver"
#define DRIVER_NAME       "MySQL ODBC 3.51 Driver"

/*
   Internal driver definations
*/
#define MYSQL_RESET_BUFFERS 1000  /* param to SQLFreeStmt */
#define MYSQL_RESET 1001    /* param to SQLFreeStmt */
#define MYSQL_3_21_PROTOCOL 10
#define CHECK_IF_ALIVE      3600  /* Seconds between queries for ping */

#define MYSQL_MAX_CURSOR_LEN 18
#define MYSQL_STMT_LEN 1024
#define MYSQL_STRING_LEN 1024
#define MYSQL_MAX_SEARCH_STRING_LEN NAME_LEN+10
#define MY_MAX_PK_PARTS 32

#define x_free(A) { gptr tmp=(gptr) (A); if (tmp) my_free(tmp,MYF(MY_WME+MY_FAE)); }

/*
  Connection parameters, that affects the driver behaviour
*/
#define FLAG_FIELD_LENGTH       1   /* field_length instead of max_length */
#define FLAG_FOUND_ROWS         2   /* Access wants can't handle affected_rows */
#define FLAG_DEBUG              4   /* Put a debug log on C:\myodbc.log */
#define FLAG_BIG_PACKETS        8   /* Allow BIG packets. */
#define FLAG_NO_PROMPT          16  /* Don't prompt on connection */
#define FLAG_DYNAMIC_CURSOR     32  /* Enables the dynamic cursor */
#define FLAG_NO_SCHEMA          64
#define FLAG_NO_DEFAULT_CURSOR  128
#define FLAG_NO_LOCALE          256
#define FLAG_PAD_SPACE          512  /* Pad CHAR:s with space to max length */
#define FLAG_FULL_COLUMN_NAMES  1024 /* Extends SQLDescribeCol */
#define FLAG_COMPRESSED_PROTO   2048 /* Use compressed protocol */
#define FLAG_IGNORE_SPACE       4096
#define FLAG_NAMED_PIPE         8192
#define FLAG_NO_BIGINT          16384
#define FLAG_NO_CATALOG         32768
#define FLAG_USE_MYCNF          65536L/* Read my.cnf at start */
#define FLAG_SAFE               131072L/* Try to be as safe as possible */
#define FLAG_NO_TRANSACTIONS  (FLAG_SAFE*2)

#ifndef DEFAULT_TXN_ISOLATION
#define DEFAULT_TXN_ISOLATION SQL_TXN_READ_COMMITTED
#endif

/*
  Statement attributes
*/
typedef struct stmt_options {

  SQLUINTEGER      bind_type,rows_in_set,cursor_type;
  SQLINTEGER       max_length,max_rows, *bind_offset;
  SQLUINTEGER      cursor_scollable;
  SQLUINTEGER      cursor_sensitivity;
  SQLUSMALLINT     *paramStatusPtr;
  SQLUINTEGER      *paramProcessedPtr;
  my_bool          retrieve_data;
  SQLUSMALLINT     *rowStatusPtr;
  SQLUSMALLINT     *rowOperationPtr;
  SQLUINTEGER      *rowsFetchedPtr;
  SQLUINTEGER      simulateCursor;

} STMT_OPTIONS;

/*
  Environment handler
*/
typedef struct  tagENV {
  
  uint        odbc_ver;
  LIST        *connections;
  MYERROR     error;

} ENV;

/*
  Connection handler
*/
typedef struct tagDBC {

  ENV           *env;
  MYSQL         mysql;
  char          *dsn,*database,
                *user,*password,*server;
  uint          port;
  ulong         flag,login_timeout;
  LIST          *statements;
  LIST          *descriptors;
  LIST          list;
  STMT_OPTIONS  stmt_options;
  time_t        last_query_time; 
  int           txn_isolation;
#ifdef THREAD
  pthread_mutex_t lock;
#endif
  MYERROR       error; 
  uint          cursor_count;
} DBC;

/*
  Statement row binding handler
*/
typedef struct st_bind {

  MYSQL_FIELD     *field;
  SQLSMALLINT     fCType;
  SQLPOINTER      rgbValue;
  SQLINTEGER      cbValueMax;
  SQLINTEGER FAR  *pcbValue;
  LIST            list;

} BIND;

/*
  Statement param binding handler
*/
typedef struct st_param_bind {

  SQLSMALLINT SqlType,CType;
  gptr        buffer;
  char        *pos_in_query,*value;
  SQLINTEGER  ValueMax,*actual_len,value_length;
  bool        alloced,used;
  bool        real_param_done;

} PARAM_BIND;

/*
  Statement states
*/
enum MY_STATE { ST_UNKNOWN, ST_PREPARED, ST_PRE_EXECUTED, ST_EXECUTED };
enum MY_DUMMY_STATE { ST_DUMMY_UNKNOWN, ST_DUMMY_PREPARED, ST_DUMMY_EXECUTED };

/*
  Statement primary key handler for cursors
*/
typedef struct pk_column {

  char        name[NAME_LEN+1];
  my_bool     bind_done;

} MY_PK_COLUMN;

/*
  Statement cursor handler
*/
typedef struct cursor {

  const char  *name;
  uint         pk_count;
  my_bool      pk_validated;
  MY_PK_COLUMN pkcol[MY_MAX_PK_PARTS];

} MYCURSOR;

/*
  IRD decriptor
*/
typedef struct irdDESC {

  BIND *bind;
  uint count;

} IRD_DESC;

/*
  ARD descriptor
*/
typedef struct ardDESC {

  DYNAMIC_ARRAY param;
  uint          count;

} ARD_DESC;


/*
  Descriptor handler
*/
typedef struct tagDESC {

  DBC FAR  *dbc;
  IRD_DESC *ird;
  ARD_DESC *ard;
  LIST     list;

} DESC;

/*
  Implementation descriptors
*/
typedef struct tagSTMT MY_STMT;
typedef struct impDESC {

  MY_STMT *stmt;
  DESC    *desc;

} IMPDESC;


/*
  Main statement handler
*/
typedef struct  tagSTMT {

  DBC FAR       *dbc;
  MYSQL_RES     *result;
  uint          last_getdata_col;
  long          current_row;
  long          cursor_row;
  ulong         getdata_offset;
  ulong         *result_lengths;
  uint          *order,order_count,param_count,current_param,
                rows_found_in_set,bound_columns;
  my_ulonglong  affected_rows;
  STMT_OPTIONS  stmt_options;
  enum MY_STATE state;
  enum MY_DUMMY_STATE dummy_state;
  MYSQL_ROW     array,result_array,current_values;
  MYSQL_ROW     (*fix_fields)(struct tagSTMT FAR* stmt,MYSQL_ROW row);
  MYSQL_FIELD   *fields;
  MYSQL_ROW_OFFSET  end_of_set;  
  DYNAMIC_ARRAY params;     
  BIND          *bind;      
  SQLSMALLINT   *odbc_types; 
  char          *query,*query_end;
  const char    *table_name;
  LIST          list;
  MYCURSOR      cursor;
  MYERROR       error;

  /* DESC PART..not yet supported....., to make MS ODBC DM to work 
     without crashing, just return the dummy pointers, when it 
     internally calls SQLGetStmtAttr at the time of stmt allocation
  */
  IMPDESC       ird,ard;
  IMPDESC       ipd,apd;
  
} STMT;

#ifndef _UNIX_
extern HINSTANCE NEAR s_hModule;  /* DLL handle. */
#endif
extern char *default_locale, *decimal_point, *thousands_sep;
extern uint decimal_point_length,thousands_sep_length;
#ifdef THREAD
extern pthread_mutex_t myodbc_lock;
#endif

/*
  Resource defines for "SQLDriverConnect" dialog box
*/
#define ID_LISTBOX  100
#define CONFIGDSN 1001
#define CONFIGDEFAULT 1002
#define EDRIVERCONNECT  1003

#include "myutil.h"

#endif /* __MYODBC3_H__ */
