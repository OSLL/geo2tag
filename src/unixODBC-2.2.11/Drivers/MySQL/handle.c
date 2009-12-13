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
 * HANDLE.C                                                                *
 *                                                                         *
 * @description: Allocation and freeing of handles                         *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Nov-07                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

/*************************************************************************** 
 * The following ODBC APIs are implemented in this file:                   *
 *                                                                         *
 *   SQLAllocHandle      (ISO 92)                                          *
 *   SQLFreeHandle       (ISO 92)                                          *
 *   SQLAllocEnv         (ODBC, Deprecated)                                *
 *   SQLAllocConnect     (ODBC, Deprecated)                                *
 *   SQLAllocStmt        (ODBC, Deprecated)                                *
 *   SQLFreeEnv          (ODBC, Deprecated)                                *
 *   SQLFreeConnect      (ODBC, Deprecated)                                *
 *   SQLFreeStmt         (ISO 92)                                          *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"

/*
  @type    : myodbc3 internal
  @purpose : to allocate the environment handle and to maintain
       its list
*/

SQLRETURN SQL_API my_SQLAllocEnv(SQLHENV FAR * phenv)
{
  myodbc_init();
#ifndef _UNIX_
  {
    HGLOBAL henv= GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof (ENV));
    if (henv == NULL || (*phenv = (SQLHENV)GlobalLock (henv)) == NULL)
    {
      GlobalFree (henv);      /* Free it if lock fails */
      *phenv = SQL_NULL_HENV;
      return (SQL_ERROR);
    }
  }
#else
  if (!(*phenv = (SQLHENV) my_malloc(sizeof(ENV),MYF(MY_ZEROFILL))))
  {
    *phenv = SQL_NULL_HENV;
    return (SQL_ERROR);
  }
#endif /* _UNIX_ */
  ((ENV FAR *)*phenv)->odbc_ver = SQL_OV_ODBC2;
  return (SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 API
  @purpose : to allocate the environment handle
*/

SQLRETURN SQL_API SQLAllocEnv(SQLHENV FAR * phenv)
{
  return my_SQLAllocEnv(phenv);
}

/*
  @type    : myodbc3 internal
  @purpose : to free the environment handle
*/

SQLRETURN SQL_API my_SQLFreeEnv(SQLHENV henv)
{
#ifndef _UNIX_
  GlobalUnlock(GlobalHandle((HGLOBAL) henv));
  GlobalFree(GlobalHandle((HGLOBAL) henv));
#else
  myodbc_end();
  my_free((char*) henv,MYF(0));
#endif /* _UNIX_ */
  return (SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 API
  @purpose : to free the environment handle
*/
SQLRETURN SQL_API SQLFreeEnv(SQLHENV henv)
{
  return my_SQLFreeEnv(henv);
}

/*
  @type    : myodbc3 internal
  @purpose : to allocate the connection handle and to
       maintain the connection list
*/

SQLRETURN SQL_API my_SQLAllocConnect(SQLHENV henv, SQLHDBC FAR *phdbc)
{
  DBC FAR *dbc;
  ENV FAR *penv=(ENV FAR*) henv;

#ifndef _UNIX_
  {
    HGLOBAL hdbc = GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof (DBC));
    if (!hdbc || (*phdbc = (SQLHDBC)GlobalLock (hdbc)) == SQL_NULL_HDBC)
    {
      GlobalFree (hdbc);
      *phdbc = SQL_NULL_HENV;
      return (set_handle_error(1,henv,MYERR_S1001,NULL,0));
    }
  }
#else
  if (!(*phdbc = (SQLHDBC) my_malloc(sizeof(DBC),MYF(MY_ZEROFILL))))
  {
    *phdbc = SQL_NULL_HDBC;
    return (set_handle_error(1,henv,MYERR_S1001,NULL,0));
  }
#endif /* _UNIX_ */

  dbc=(DBC FAR*) *phdbc;
  dbc->mysql.net.vio = 0;     /* Marker if open */
  dbc->flag=0;
  dbc->stmt_options.max_rows= dbc->stmt_options.max_length= 0L;
  dbc->stmt_options.bind_type=SQL_BIND_BY_COLUMN;
  dbc->stmt_options.rows_in_set=1;
  dbc->stmt_options.cursor_type=SQL_CURSOR_FORWARD_ONLY;  /* ODBC default */
  dbc->login_timeout=0;
  dbc->last_query_time= (time_t) time((time_t*) 0);
  dbc->txn_isolation=DEFAULT_TXN_ISOLATION;
  dbc->env= penv;
  penv->connections=list_add(penv->connections,&dbc->list);
  dbc->list.data=dbc;
  pthread_mutex_init(&dbc->lock,NULL);
  return (SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 API
  @purpose : to allocate the connection handle and to
       maintain the connection list
*/

SQLRETURN SQL_API SQLAllocConnect(SQLHENV henv, SQLHDBC FAR *phdbc)
{
  return my_SQLAllocConnect(henv, phdbc);
}

/*
  @type    : myodbc3 internal
  @purpose : to allocate the connection handle and to
       maintain the connection list
*/

SQLRETURN SQL_API my_SQLFreeConnect(SQLHDBC hdbc)
{
  DBC FAR *dbc=(DBC FAR*) hdbc;
  DBUG_ENTER("SQLFreeConnect");

  dbc->env->connections=list_delete(dbc->env->connections,&dbc->list);
  my_free(dbc->dsn,MYF(MY_ALLOW_ZERO_PTR));
  my_free(dbc->database,MYF(MY_ALLOW_ZERO_PTR));
  my_free(dbc->server,MYF(MY_ALLOW_ZERO_PTR));
  my_free(dbc->user,MYF(MY_ALLOW_ZERO_PTR));
  my_free(dbc->password,MYF(MY_ALLOW_ZERO_PTR));
  pthread_mutex_destroy(&dbc->lock);

#ifndef _UNIX_
  GlobalUnlock(GlobalHandle((HGLOBAL) hdbc));
  GlobalFree(GlobalHandle((HGLOBAL) hdbc));
#else
  my_free((char*) hdbc,MYF(0));
#endif
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 API
  @purpose : to allocate the connection handle and to
       maintain the connection list
*/
SQLRETURN SQL_API SQLFreeConnect(SQLHDBC hdbc)
{
  return my_SQLFreeConnect(hdbc);
}

/*
  @type    : myodbc3 internal
  @purpose : allocates the statement handle
*/

SQLRETURN SQL_API my_SQLAllocStmt(SQLHDBC hdbc,SQLHSTMT FAR *phstmt)
{
#ifndef _UNIX_
  HGLOBAL  hstmt;
#endif
  STMT FAR* stmt;
  DBC FAR *dbc=(DBC FAR*) hdbc;
  DBUG_ENTER("SQLAllocStmt");

#ifndef _UNIX_
  hstmt = GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof (STMT));
  if (!hstmt || (*phstmt = (SQLHSTMT)GlobalLock (hstmt)) == SQL_NULL_HSTMT)
  {
    GlobalFree (hstmt);
    *phstmt = SQL_NULL_HSTMT;
    DBUG_RETURN(SQL_ERROR);
  }
#else
 *phstmt = (SQLHSTMT) my_malloc(sizeof (STMT), MYF(MY_ZEROFILL | MY_WME));
  if (*phstmt == SQL_NULL_HSTMT)
  {
    *phstmt = SQL_NULL_HSTMT;
    DBUG_RETURN(SQL_ERROR);
  }
#endif /* IS UNIX */
  stmt= (STMT FAR*) *phstmt;
  stmt->dbc= dbc;
  dbc->statements=list_add(dbc->statements,&stmt->list);
  stmt->list.data=stmt;
  stmt->stmt_options=dbc->stmt_options;
  stmt->stmt_options.cursor_scollable=SQL_SCROLLABLE;
  init_dynamic_array(&stmt->params,sizeof(PARAM_BIND),32,64);
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 APO
  @purpose : allocates the statement handle
*/

SQLRETURN SQL_API SQLAllocStmt(SQLHDBC hdbc,SQLHSTMT FAR *phstmt)
{
  return my_SQLAllocStmt(hdbc,phstmt);
}

/*
  @type    : ODBC 1.0 API
  @purpose : stops processing associated with a specific statement,
       closes any open cursors associated with the statement,
       discards pending results, or, optionally, frees all
       resources associated with the statement handle
*/

SQLRETURN SQL_API SQLFreeStmt(SQLHSTMT hstmt,SQLUSMALLINT fOption)
{
  return my_SQLFreeStmt(hstmt,fOption);
}

/*
  @type    : myodbc3 internal
  @purpose : stops processing associated with a specific statement,
       closes any open cursors associated with the statement,
       discards pending results, or, optionally, frees all
       resources associated with the statement handle
*/

SQLRETURN SQL_API my_SQLFreeStmt(SQLHSTMT hstmt,SQLUSMALLINT fOption)
{
  STMT FAR *stmt=(STMT FAR*) hstmt;
  uint i;
  DBUG_ENTER("SQLFreeStmt");
  DBUG_PRINT("enter",("stmt: %lx  option: %d",hstmt,fOption));

  if (fOption == SQL_UNBIND)
  {
    x_free(stmt->bind);
    stmt->bind=0;
    stmt->bound_columns=0;
    DBUG_RETURN(SQL_SUCCESS);
  }
  for (i=0 ; i < stmt->params.elements ; i++)
  {
    PARAM_BIND *param=dynamic_element(&stmt->params,i,PARAM_BIND*);
    if (param->alloced)
    {
      my_free(param->value,MYF(0));
      param->alloced=0;
    }
    if (fOption == SQL_RESET_PARAMS)
    {
      param->used=0;
      param->real_param_done = FALSE;
    }
  }
  if (fOption == SQL_RESET_PARAMS)
  {
    DBUG_RETURN(SQL_SUCCESS);
  }

  mysql_free_result(stmt->result);
  x_free((gptr) stmt->fields);
  x_free((gptr) stmt->array);
  x_free((gptr) stmt->result_array);
  x_free((gptr) stmt->odbc_types);
  stmt->result=0;
  stmt->result_lengths=0;
  stmt->fields=0;
  stmt->array=0;
  stmt->result_array=0;
  stmt->odbc_types=0;
  stmt->current_values=0;   /* For SQLGetData */
  stmt->fix_fields=0;
  stmt->affected_rows=0;
  stmt->current_row=stmt->cursor_row=stmt->rows_found_in_set=0;
  stmt->state=ST_UNKNOWN;

  if (fOption == MYSQL_RESET_BUFFERS)
  {
    DBUG_RETURN(SQL_SUCCESS);
  }

  x_free((gptr) stmt->table_name);
  stmt->table_name = 0;
  stmt->dummy_state = ST_DUMMY_UNKNOWN;
  stmt->cursor.pk_validated = false;        
  
  for (i=stmt->cursor.pk_count; i--;)
    stmt->cursor.pkcol[i].bind_done = 0;
  stmt->cursor.pk_count = 0;

  if (fOption == SQL_CLOSE)
  {    
    DBUG_RETURN(SQL_SUCCESS);
  }

  /* At this point, only MYSQL_RESET and SQL_DROP left out */
  x_free((gptr) stmt->query);
  stmt->query=0;
  stmt->param_count=0;
  if (fOption == MYSQL_RESET)
  {
    DBUG_RETURN(SQL_SUCCESS);
  }
  stmt->stmt_options.rowStatusPtr=0;
  x_free((gptr) stmt->cursor.name);
  x_free((gptr) stmt->bind);
  delete_dynamic(&stmt->params);
  stmt->dbc->statements=list_delete(stmt->dbc->statements,&stmt->list);
#ifndef _UNIX_
  GlobalUnlock (GlobalHandle ((HGLOBAL) hstmt));
  GlobalFree (GlobalHandle((HGLOBAL) hstmt));
#endif /* !_UNIX_ */

#ifdef _UNIX_
  my_free((char*) hstmt,MYF(0));
#endif /* _UNIX_*/
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : ODBC 3.0 API
  @purpose : allocates an environment, connection, statement, or
       descriptor handle
*/

SQLRETURN SQL_API SQLAllocHandle( SQLSMALLINT HandleType,
          SQLHANDLE   InputHandle,
          SQLHANDLE   *OutputHandlePtr)
{
  SQLRETURN error = SQL_ERROR;
  DBUG_ENTER("SQLAllocHandle");
  DBUG_PRINT("enter",("handle type: %d",HandleType));

  switch (HandleType) {
  case SQL_HANDLE_ENV:
    error = my_SQLAllocEnv(OutputHandlePtr);
    break;

  case SQL_HANDLE_DBC:
    error = my_SQLAllocConnect(InputHandle,OutputHandlePtr);
    break;

  case SQL_HANDLE_STMT:
    error = my_SQLAllocStmt(InputHandle,OutputHandlePtr);
    break;

  default:
    DBUG_RETURN(set_handle_error(SQL_HANDLE_DBC,InputHandle,MYERR_S1C00,NULL,
         0));
  }
  DBUG_RETURN(error);
}

/*
  @type    : ODBC 3.0 API
  @purpose : frees resources associated with a specific environment,
       connection, statement, or descriptor handle
*/

SQLRETURN SQL_API SQLFreeHandle(SQLSMALLINT HandleType,
        SQLHANDLE   Handle)
{
  SQLRETURN error = SQL_ERROR;
  DBUG_ENTER("SQLFreeHandle");
  DBUG_PRINT("enter",("handle type: %d",HandleType));

  switch (HandleType) {
  case SQL_HANDLE_ENV:
    error = my_SQLFreeEnv(Handle);
    break;

  case SQL_HANDLE_DBC:
    error = my_SQLFreeConnect(Handle);
    break;

  case SQL_HANDLE_STMT:
    error = my_SQLFreeStmt(Handle, SQL_DROP);
    break;

  default:
    DBUG_RETURN(SQL_ERROR);
  }
  DBUG_RETURN(error);
}
