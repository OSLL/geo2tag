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
 * OPTIONS.C                                                               *
 *                                                                         *
 * @description: For handling attribute APIs                               *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

/*************************************************************************** 
 * The following ODBC APIs are implemented in this file:                   *
 *                                                                         *
 *   SQLSetEnvAttr       (ISO 92)                                          *
 *   SQLGetEnvAttr       (ISO 92)                                          *
 *   SQLSetConnectAttr   (ISO 92)                                          *
 *   SQLGetConnecyAttr   (ISO 92)                                          *
 *   SQLSetStmtAttr      (ISO 92)                                          *
 *   SQLGetStmtAttr      (ISO 92)                                          *
 *   SQLSetConnectOption (ODBC, Deprecated)                                *
 *   SQLGetConnectOption (ODBC, Deprecated)                                *
 *   SQLSetStmtOption    (ODBC, Deprecated)                                *
 *   SQLGetStmtOption    (ODBC, Deprecated)                                *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"

/*
  @type    : myodbc3 internal
  @purpose : sets the common connection/stmt attributes
*/

SQLRETURN set_constmt_attr(SQLSMALLINT  HandleType,
         SQLHANDLE   Handle,
         STMT_OPTIONS *options,
         SQLINTEGER  Attribute,
         SQLPOINTER  ValuePtr)
{
  DBUG_ENTER("set_constmt_attr");

  switch(Attribute){

  case SQL_ATTR_ASYNC_ENABLE:
    if( (SQLUINTEGER)ValuePtr == SQL_ASYNC_ENABLE_ON )
      DBUG_RETURN(set_handle_error(HandleType,Handle,MYERR_01S02,
           "Doesn't support asynchronous, changed to default",0));
    break;

  case SQL_ATTR_CURSOR_SENSITIVITY:
    if ((SQLUINTEGER)ValuePtr != SQL_UNSPECIFIED )
    {
      options->cursor_sensitivity = SQL_UNSPECIFIED;
      DBUG_RETURN(set_handle_error(HandleType,Handle,MYERR_01S02,
           "Option value changed to default cursor sensitivity",0));
    }
    options->cursor_sensitivity = (SQLUINTEGER)ValuePtr;
    break;

  case SQL_ATTR_CURSOR_TYPE:
    if (((STMT FAR*)Handle)->dbc->flag & FLAG_DYNAMIC_CURSOR)
    {
      if ((SQLUINTEGER) ValuePtr != SQL_CURSOR_KEYSET_DRIVEN)
  options->cursor_type= (SQLUINTEGER)ValuePtr;

      else
      {
  options->cursor_type=SQL_CURSOR_STATIC;
  DBUG_RETURN(set_handle_error(HandleType,Handle,MYERR_01S02,
             "Option value changed to default static cursor",0));
      }
    }
    else
    {
      if ((SQLUINTEGER) ValuePtr == SQL_CURSOR_FORWARD_ONLY ||
   (SQLUINTEGER) ValuePtr == SQL_CURSOR_STATIC)
  options->cursor_type= (SQLUINTEGER)ValuePtr;

      else
      {
  options->cursor_type=SQL_CURSOR_STATIC;
  DBUG_RETURN(set_handle_error(HandleType,Handle,MYERR_01S02,
             "Option value changed to default static cursor",0));
      }
    }
    break;

  case SQL_ATTR_MAX_LENGTH:
    options->max_length = (SQLUINTEGER) ValuePtr;
    break;

  case SQL_ATTR_MAX_ROWS:
    options->max_rows = (SQLUINTEGER) ValuePtr;
    break;

  case SQL_ATTR_METADATA_ID:
    if ((SQLUINTEGER)ValuePtr == SQL_TRUE)
      DBUG_RETURN(set_handle_error(HandleType,Handle,MYERR_01S02,
           "Doesn't support SQL_ATTR_METADATA_ID to true, changed to default",0));
    break;

  case SQL_ATTR_RETRIEVE_DATA:
    DBUG_PRINT("info",("SQL_ATTR_RETRIEVE_DATA value is ignored"));
    break;

  case SQL_ATTR_ROW_BIND_TYPE:
    options->bind_type = (SQLUINTEGER )ValuePtr;
    break;

  case SQL_ATTR_SIMULATE_CURSOR:
    if ((SQLUINTEGER)ValuePtr != SQL_SC_TRY_UNIQUE )
      DBUG_RETURN(set_handle_error(HandleType,Handle,MYERR_01S02,
           "Option value changed to default cursor simulation",0));
    break;

  case SQL_ATTR_ROW_BIND_OFFSET_PTR:
    options->bind_offset = (SQLINTEGER *)ValuePtr;
    break;

  case SQL_ATTR_QUERY_TIMEOUT:
  case SQL_ATTR_KEYSET_SIZE:
  case SQL_ATTR_CONCURRENCY:
  case SQL_ATTR_NOSCAN:
  case SQL_ATTR_ROW_OPERATION_PTR: /* need to support this ....*/
    DBUG_PRINT("info",("Attribute, %d is ignored", Attribute));
    break;

  case SQL_ATTR_FETCH_BOOKMARK_PTR:
  case SQL_ATTR_USE_BOOKMARKS:
    DBUG_RETURN(set_handle_error(HandleType,Handle,MYERR_S1C00,NULL,0));
    break;

  default:
    DBUG_PRINT("info",("Attribute, %d is ignored", Attribute));
    break;
  }
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : myodbc3 internal
  @purpose : returns the common connection/stmt attributes
*/

SQLRETURN get_constmt_attr(SQLSMALLINT  HandleType,
         SQLHANDLE   Handle,
         STMT_OPTIONS *options,
         SQLINTEGER  Attribute,
         SQLPOINTER  ValuePtr,
         SQLINTEGER  *StringLengthPtr)
{
  DBUG_ENTER("get_constmt_attr");

  switch(Attribute) {

  case SQL_ATTR_ASYNC_ENABLE:
    *((SQLUINTEGER *) ValuePtr)= SQL_ASYNC_ENABLE_OFF;
    break;

  case SQL_ATTR_CURSOR_SENSITIVITY:
    *((SQLUINTEGER *) ValuePtr)= options->cursor_sensitivity;
    break;

  case SQL_ATTR_CURSOR_TYPE:
    *((SQLUINTEGER *) ValuePtr)= options->cursor_type;
    break;

  case SQL_ATTR_MAX_LENGTH:
    *((SQLUINTEGER *) ValuePtr)= options->max_length;
    break;

  case SQL_ATTR_MAX_ROWS:
    *((SQLUINTEGER *) ValuePtr)= options->max_rows;
    break;

  case SQL_ATTR_METADATA_ID:
    *((SQLUINTEGER *) ValuePtr)= SQL_FALSE;
    break;

  case SQL_ATTR_QUERY_TIMEOUT:
    *((SQLUINTEGER *) ValuePtr)= SQL_QUERY_TIMEOUT_DEFAULT;
    break;

  case SQL_ATTR_RETRIEVE_DATA:
    *((SQLUINTEGER *) ValuePtr)= SQL_RD_DEFAULT;
    break;

  case SQL_ATTR_ROW_BIND_TYPE:
    *((SQLUINTEGER *) ValuePtr)= options->bind_type;
    break;

  case SQL_ATTR_SIMULATE_CURSOR:
    *((SQLUINTEGER *) ValuePtr)= SQL_SC_TRY_UNIQUE;
    break;

  case SQL_ATTR_CONCURRENCY:
    *((SQLUINTEGER *) ValuePtr)= SQL_CONCUR_ROWVER;
    break;

  case SQL_KEYSET_SIZE:
    *((SQLUINTEGER *) ValuePtr)=0L;
    break;

  case SQL_ROWSET_SIZE:
    *(SQLUINTEGER *)ValuePtr = options->rows_in_set;
    break;

  case SQL_NOSCAN:
    *((SQLUINTEGER *) ValuePtr)=SQL_NOSCAN_ON;
    break;

  case SQL_ATTR_ROW_BIND_OFFSET_PTR:
    *((SQLINTEGER *) ValuePtr) = options->bind_offset ?
      *(options->bind_offset):
      0;
    break;

  case SQL_ATTR_ROW_OPERATION_PTR: /* need to support this ....*/
    DBUG_PRINT("info",("Attribute, %d is ignored", Attribute));
    DBUG_RETURN(SQL_SUCCESS_WITH_INFO);

  case SQL_ATTR_FETCH_BOOKMARK_PTR:
  case SQL_ATTR_USE_BOOKMARKS:
    DBUG_RETURN(set_handle_error(HandleType,Handle,MYERR_S1C00,NULL,0));

  default:
    DBUG_PRINT("error",("Invalid attribute/option identifier:%d", Attribute));
  }
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : myodbc3 internal
  @purpose : sets the connection attributes
*/

SQLRETURN set_con_attr(SQLHDBC    hdbc,
           SQLINTEGER Attribute,
           SQLPOINTER ValuePtr,
           SQLINTEGER StringLengthPtr)
{
  DBC FAR *dbc=(DBC FAR*) hdbc;
  DBUG_ENTER("SQLSetConnectAttr");
  DBUG_PRINT("enter",("Atrr: %d,%x,%d",Attribute,ValuePtr,StringLengthPtr));

  switch(Attribute) {

  case SQL_ATTR_ACCESS_MODE:
    DBUG_PRINT("info",("SQL_ATTR_ACCESS_MODE %ld ignored",
           (SQLUINTEGER)ValuePtr));
    break;

  case SQL_ATTR_AUTOCOMMIT:
    if ((SQLUINTEGER)ValuePtr != SQL_AUTOCOMMIT_ON)
    {
      if (!(dbc->mysql.server_capabilities & CLIENT_TRANSACTIONS) ||
    (dbc->flag & FLAG_NO_TRANSACTIONS))
  DBUG_RETURN(set_handle_error(SQL_HANDLE_DBC,dbc,MYERR_S1C00,
             "Transactions are not enabled", 4000));

      if (dbc->mysql.server_status & SERVER_STATUS_AUTOCOMMIT)
  DBUG_RETURN(odbc_stmt(dbc,"SET AUTOCOMMIT=0"));
    }
    else if ((dbc->mysql.server_capabilities & CLIENT_TRANSACTIONS) &&
       !(dbc->mysql.server_status & SERVER_STATUS_AUTOCOMMIT))
      DBUG_RETURN(odbc_stmt(dbc,"SET AUTOCOMMIT=1"));

    break;

  case SQL_ATTR_CONNECTION_TIMEOUT:
    DBUG_RETURN(mysql_options(&dbc->mysql, MYSQL_OPT_CONNECT_TIMEOUT,
            (const char *)((SQLUINTEGER)ValuePtr)));
    break;

  case SQL_ATTR_CURRENT_CATALOG:
    pthread_mutex_lock(&dbc->lock);
    if (mysql_select_db(&dbc->mysql,(char*) ValuePtr))
    {
      set_handle_error(SQL_HANDLE_DBC,dbc,MYERR_S1000,mysql_error(&dbc->mysql),
           mysql_errno(&dbc->mysql));
      pthread_mutex_unlock(&dbc->lock);
      DBUG_RETURN(SQL_ERROR);
    }
    my_free((gptr) dbc->database,MYF(0));
    dbc->database=my_strdup((char*) ValuePtr,MYF(MY_WME));
    pthread_mutex_unlock(&dbc->lock);
    break;

  case SQL_ATTR_LOGIN_TIMEOUT:
    dbc->login_timeout=(SQLUINTEGER)ValuePtr;
    break;

  case SQL_ATTR_ODBC_CURSORS:
  case SQL_OPT_TRACE:
  case SQL_OPT_TRACEFILE:
  case SQL_QUIET_MODE:
  case SQL_TRANSLATE_DLL:
  case SQL_TRANSLATE_OPTION:
  {
    char buff[100];
    sprintf(buff,"Suppose to set this attribute '%d' through driver manager, not by the driver",Attribute);
    DBUG_RETURN(set_handle_error(SQL_HANDLE_DBC,hdbc,MYERR_01S02,buff,0));
  }

  case SQL_ATTR_PACKET_SIZE:
    DBUG_PRINT("info",("SQL_ATTR_PACKET_SIZE %ld ignored",
           (SQLUINTEGER)ValuePtr));
    break;

  case SQL_ATTR_TXN_ISOLATION:
    if (dbc->mysql.server_capabilities & CLIENT_TRANSACTIONS)
    {
      char buff[80];
      const char *level;

      if ((SQLINTEGER)ValuePtr & SQL_TXN_SERIALIZABLE)
  level="SERIALIZABLE";
      else if ((SQLINTEGER)ValuePtr & SQL_TXN_REPEATABLE_READ)
  level="REPEATABLE READ";
      else if ((SQLINTEGER)ValuePtr & SQL_TXN_REPEATABLE_READ)
  level="READ COMMITTED";
      else
  level="READ UNCOMMITTED";
      sprintf(buff,"SET SESSION TRANSACTION ISOLATION LEVEL %s",level);
      if (odbc_stmt(dbc,buff) == SQL_SUCCESS)
  dbc->txn_isolation=(SQLINTEGER)ValuePtr;
    }
    else
      DBUG_PRINT("info",("SQL_ATTR_TXN_ISOLATION %ld ignored",
       (SQLINTEGER)ValuePtr));
    break;

    /* 3.x driver doesn't support any statement attributes
       at connection level, but to make sure all 2.x apps
       works fine...lets support it..nothing to loose..
    */
  default:
    DBUG_RETURN(set_constmt_attr(2,dbc,&dbc->stmt_options,
         Attribute,ValuePtr));
  }
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : myodbc3 internal
  @purpose : returns the connection attribute values
*/

SQLRETURN get_con_attr(SQLHDBC    hdbc,
           SQLINTEGER Attribute,
           SQLPOINTER ValuePtr,
           SQLINTEGER BufferLength,
           SQLINTEGER *StringLengthPtr)
{
  DBC FAR *dbc=(DBC FAR*) hdbc;
  SQLRETURN result=SQL_SUCCESS;
  SQLINTEGER strlen;
  SQLPOINTER vparam=0;
  DBUG_ENTER("SQLGetConnectAttr");
  DBUG_PRINT("enter",("Atrr: %d,%x,%d,%x",Attribute,ValuePtr,BufferLength,
          StringLengthPtr));

  if (!ValuePtr)
    ValuePtr = vparam;

  if (!StringLengthPtr)
    StringLengthPtr = &strlen;

  switch(Attribute){

  case SQL_ATTR_ACCESS_MODE:
    *((SQLUINTEGER *) ValuePtr)= SQL_MODE_READ_WRITE;
    break;

  case SQL_ATTR_AUTO_IPD:
    *((SQLUINTEGER *) ValuePtr)= SQL_FALSE;
    break;

  case SQL_ATTR_AUTOCOMMIT:
    *((SQLUINTEGER *)ValuePtr)= (((dbc->mysql.server_status &
          SERVER_STATUS_AUTOCOMMIT) ||
         !(dbc->mysql.server_capabilities &
           CLIENT_TRANSACTIONS)) ?
         SQL_AUTOCOMMIT_ON :
         SQL_AUTOCOMMIT_OFF);
    break;

  case SQL_ATTR_CONNECTION_DEAD:
    if (check_if_server_is_alive(dbc))
      *((SQLUINTEGER *) ValuePtr)= SQL_CD_TRUE;
    else
      *((SQLUINTEGER *) ValuePtr)= SQL_CD_FALSE;
    break;

  case SQL_ATTR_CONNECTION_TIMEOUT:
    *((SQLUINTEGER *) ValuePtr)= dbc->mysql.net.timeout;
    break;

  case SQL_ATTR_CURRENT_CATALOG:
    *StringLengthPtr=(SQLSMALLINT) (strmake((char*)ValuePtr,dbc->database,
              BufferLength) - (char*) ValuePtr);
    break;

  case SQL_ATTR_LOGIN_TIMEOUT:
    *((SQLUINTEGER *) ValuePtr)= dbc->login_timeout;
    break;

  case SQL_ATTR_ODBC_CURSORS:
  case SQL_OPT_TRACE:
  case SQL_OPT_TRACEFILE:
  case SQL_QUIET_MODE:
  case SQL_TRANSLATE_DLL:
  case SQL_TRANSLATE_OPTION:
  {
    char buff[100];
    sprintf(buff,"Suppose to get this attribute '%d' through driver manager, not by the driver",Attribute);
    result = set_handle_error(SQL_HANDLE_DBC,hdbc,MYERR_01S02,buff,0);
    break;
  }

  case SQL_ATTR_PACKET_SIZE:
    *((SQLUINTEGER *) ValuePtr)= dbc->mysql.net.max_packet;
    break;

  case SQL_ATTR_TXN_ISOLATION:
    *((SQLINTEGER *) ValuePtr)= dbc->txn_isolation;
    break;

    /*
      3.x driver doesn't support any statement attributes
      at connection level, but to make sure all 2.x apps
      works fine...lets support it..nothing to loose..
    */
  default:
    result=get_constmt_attr(2,dbc,&dbc->stmt_options,
          Attribute,ValuePtr,
          StringLengthPtr);
  }
  DBUG_RETURN(result);
}

/*
  @type    : myodbc3 internal
  @purpose : sets the statement attributes
*/

SQLRETURN set_stmt_attr(SQLHSTMT   hstmt,
      SQLINTEGER Attribute,
      SQLPOINTER ValuePtr,
      SQLINTEGER StringLengthPtr)
{
  STMT FAR *stmt=(STMT FAR*) hstmt;
  SQLRETURN result=SQL_SUCCESS;
  STMT_OPTIONS *options = &stmt->stmt_options;
  DBUG_ENTER("SQLSetStmtAttr");
  DBUG_PRINT("enter",("Atrr: %d,%x,%d",Attribute,ValuePtr,StringLengthPtr));

  switch(Attribute){

  case SQL_ATTR_CURSOR_SCROLLABLE:
    options->cursor_scollable = (SQLUINTEGER)ValuePtr;
    break;

  case SQL_ATTR_AUTO_IPD:
    if ((SQLUINTEGER) ValuePtr != SQL_FALSE )
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,hstmt,MYERR_01S02,
           "Option value changed to default auto ipd",
           0));
    break;

  case SQL_ATTR_PARAM_STATUS_PTR: /* need to support this ....*/
    options->paramStatusPtr = (SQLUSMALLINT *)ValuePtr;
    break;

  case SQL_ATTR_PARAMS_PROCESSED_PTR: /* need to support this ....*/
    options->paramProcessedPtr = (SQLUINTEGER *)ValuePtr;
    break;

  case SQL_ATTR_PARAMSET_SIZE:
    if ((SQLUINTEGER)ValuePtr != 1 )
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,hstmt,MYERR_01S02,
           "Option value changed to default parameter size",0));
    break;

  case SQL_ATTR_ROW_ARRAY_SIZE:
  case SQL_ROWSET_SIZE:
    options->rows_in_set = (SQLUINTEGER)ValuePtr;
    break;

  case SQL_ATTR_ROW_NUMBER:
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,hstmt,MYERR_S1000,
         "Trying to set read-only attribute",0));
    break;

  case SQL_ATTR_ROW_OPERATION_PTR:
    options->rowOperationPtr = (SQLUSMALLINT *)ValuePtr;
    break;

  case SQL_ATTR_ROW_STATUS_PTR:
    options->rowStatusPtr = (SQLUSMALLINT *)ValuePtr;
    break;

  case SQL_ATTR_ROWS_FETCHED_PTR:
    options->rowsFetchedPtr = (SQLUINTEGER *)ValuePtr;
    break;

  case SQL_ATTR_SIMULATE_CURSOR:
    options->simulateCursor = (SQLUINTEGER)ValuePtr;
    break;

    /*
      3.x driver doesn't support any statement attributes
       at connection level, but to make sure all 2.x apps
       works fine...lets support it..nothing to loose..
    */
  default:
    result = set_constmt_attr(3,hstmt,options,
            Attribute,ValuePtr);
  }
  DBUG_RETURN(result);
}

/*
  @type    : myodbc3 internal
  @purpose : returns the statement attribute values
*/

SQLRETURN get_stmt_attr(SQLHSTMT   hstmt,
      SQLINTEGER Attribute,
      SQLPOINTER ValuePtr,
      SQLINTEGER BufferLength,
      SQLINTEGER *StringLengthPtr)
{
  SQLRETURN result=SQL_SUCCESS;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  STMT_OPTIONS *options = &stmt->stmt_options;
  SQLPOINTER vparam;
  SQLINTEGER strlen;
  DBUG_ENTER("SQLGetStmtAttr");
  DBUG_PRINT("enter",("Atrr: %d,%x,%d,%x",Attribute,ValuePtr,BufferLength,
          StringLengthPtr));

  if (!ValuePtr)
    ValuePtr = &vparam;

  if (!StringLengthPtr)
    StringLengthPtr = &strlen;

  switch(Attribute) {

  case SQL_ATTR_CURSOR_SCROLLABLE:
    *(SQLUINTEGER *)ValuePtr = options->cursor_scollable;
    break;

  case SQL_ATTR_AUTO_IPD:
    *(SQLUINTEGER *)ValuePtr = SQL_FALSE;
    break;

  case SQL_ATTR_PARAM_STATUS_PTR: /* need to support this ....*/
    (SQLUSMALLINT *)ValuePtr = options->paramStatusPtr;
    break;

  case SQL_ATTR_PARAMS_PROCESSED_PTR: /* need to support this ....*/
    (SQLUINTEGER *)ValuePtr = options->paramProcessedPtr;
    break;

  case SQL_ATTR_PARAMSET_SIZE:
    *(SQLUINTEGER *)ValuePtr = 1;
    break;

  case SQL_ATTR_ROW_ARRAY_SIZE:
    *(SQLUINTEGER *)ValuePtr = options->rows_in_set;
    break;

  case SQL_ATTR_ROW_NUMBER:
    *(SQLUINTEGER *)ValuePtr = stmt->current_row+1;
    break;

  case SQL_ATTR_ROW_OPERATION_PTR: /* need to support this ....*/
    (SQLUSMALLINT *)ValuePtr = options->rowOperationPtr;
    break;

  case SQL_ATTR_ROW_STATUS_PTR:
    (SQLUSMALLINT *)ValuePtr = options->rowStatusPtr;
    break;

  case SQL_ATTR_ROWS_FETCHED_PTR:
    (SQLUINTEGER *)ValuePtr = options->rowsFetchedPtr;
    break;

  case SQL_ATTR_SIMULATE_CURSOR:
    *(SQLUINTEGER *)ValuePtr = options->simulateCursor;
    break;

    /*
      To make iODBC and MS ODBC DM to work, return the following cases
      as success, by just allocating...else
      - iODBC is hanging at the time of stmt allocation
      - MS ODB DM is crashing at the time of stmt allocation
    */
  case SQL_ATTR_APP_ROW_DESC:
    *(SQLPOINTER *)ValuePtr = &stmt->ard;
    *StringLengthPtr = sizeof(SQL_IS_POINTER);
    break;

  case SQL_ATTR_IMP_ROW_DESC:
    *(SQLPOINTER *)ValuePtr = &stmt->ird;
    *StringLengthPtr = sizeof(SQL_IS_POINTER);
    break;

  case SQL_ATTR_APP_PARAM_DESC:
    *(SQLPOINTER *)ValuePtr = &stmt->apd;
    *StringLengthPtr = sizeof(SQL_IS_POINTER);
    break;

  case SQL_ATTR_IMP_PARAM_DESC:
    *(SQLPOINTER *)ValuePtr = &stmt->ipd;
    *StringLengthPtr = sizeof(SQL_IS_POINTER);
    break;

    /*
      3.x driver doesn't support any statement attributes
       at connection level, but to make sure all 2.x apps
       works fine...lets support it..nothing to loose..
    */
  default:
    result=get_constmt_attr(3,hstmt,options,
          Attribute,ValuePtr,
          StringLengthPtr);
  }

  DBUG_RETURN(result);
}

/*
  @type    : ODBC 1.0 API
  @purpose : sets the connection options
*/

SQLRETURN SQL_API SQLSetConnectOption(SQLHDBC hdbc, SQLUSMALLINT fOption,
              SQLUINTEGER  vParam)
{
  SQLRETURN result=SQL_SUCCESS;
  DBUG_ENTER("SQLSetConnectOption");

  result=set_con_attr(hdbc,fOption,(SQLPOINTER)vParam,SQL_NTS);
  DBUG_RETURN(result);
}

/*
  @type    : ODBC 1.0 API
  @purpose : returns the connection options
*/

SQLRETURN SQL_API SQLGetConnectOption(SQLHDBC hdbc,SQLUSMALLINT fOption,
              SQLPOINTER vParam)
{
  SQLRETURN result=SQL_SUCCESS;
  DBUG_ENTER("SQLGetConnectOption");

  result=get_con_attr(hdbc,fOption,vParam,SQL_NTS,(SQLINTEGER *)NULL);
  DBUG_RETURN(result);
}

/*
  @type    : ODBC 1.0 API
  @purpose : sets the statement options
*/

SQLRETURN SQL_API SQLSetStmtOption(SQLHSTMT hstmt,SQLUSMALLINT fOption,
           SQLUINTEGER vParam)
{
  SQLRETURN result=SQL_SUCCESS;
  DBUG_ENTER("SQLSetStmtOption");

  result=set_stmt_attr(hstmt,fOption,(SQLPOINTER)vParam,SQL_NTS);
  DBUG_RETURN(result);
}

/*
  @type    : ODBC 1.0 API
  @purpose : returns the statement options
*/

SQLRETURN SQL_API SQLGetStmtOption(SQLHSTMT hstmt,SQLUSMALLINT fOption,
           SQLPOINTER vParam)
{
  SQLRETURN result=SQL_SUCCESS;
  DBUG_ENTER("SQLGetStmtOption");

  result=get_stmt_attr(hstmt,fOption,vParam,SQL_NTS,(SQLINTEGER *)NULL);
  DBUG_RETURN(result);
}

/*
  @type    : ODBC 3.0 API
  @purpose : sets the environment attributes
*/

SQLRETURN SQL_API SQLSetEnvAttr(SQLHENV    henv,
        SQLINTEGER Attribute,
        SQLPOINTER ValuePtr,
        SQLINTEGER StringLength)
{
  DBUG_ENTER("SQLSetEnvAttr");
  DBUG_PRINT("enter",("Atrr: %d,value:%d",Attribute,ValuePtr));

  if (((ENV FAR *)henv)->connections)
    DBUG_RETURN(set_handle_error(SQL_HANDLE_ENV,henv,
                                 MYERR_S1010,NULL,0));

  switch (Attribute) {

  case SQL_ATTR_ODBC_VERSION:
    ((ENV FAR *)henv)->odbc_ver = (SQLINTEGER)ValuePtr;
    break;

  case SQL_ATTR_OUTPUT_NTS:
    if ((SQLINTEGER)ValuePtr == SQL_TRUE )
      break;

  default:
    DBUG_RETURN(set_handle_error(SQL_HANDLE_ENV,henv,
                                 MYERR_S1C00,NULL,0));
  }
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : ODBC 3.0 API
  @purpose : returns the environment attributes
*/

SQLRETURN SQL_API SQLGetEnvAttr(SQLHENV    henv,
        SQLINTEGER Attribute,
        SQLPOINTER ValuePtr,
        SQLINTEGER BufferLength,
        SQLINTEGER *StringLengthPtr)
{
  DBUG_ENTER("SQLGetEnvAttr");
  DBUG_PRINT("enter",("Atrr: %d",Attribute));

  switch(Attribute) {

  case SQL_ATTR_ODBC_VERSION:
    *(SQLINTEGER*)ValuePtr = ((ENV FAR *)henv)->odbc_ver;
    break;

  case SQL_ATTR_OUTPUT_NTS:
    *((SQLINTEGER*)ValuePtr) = SQL_TRUE;
    break;

  default:
    DBUG_RETURN(set_handle_error(SQL_HANDLE_ENV,henv,
                                 MYERR_S1C00,NULL,0));
  }
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : ODBC 3.0 API
  @purpose : sets the connection attributes
*/

SQLRETURN SQL_API SQLSetConnectAttr(SQLHDBC hdbc,
            SQLINTEGER Attribute,
            SQLPOINTER ValuePtr,
            SQLINTEGER StringLength)
{
  return set_con_attr(hdbc,Attribute,
          ValuePtr,StringLength);
}

/*
  @type    : ODBC 3.0 API
  @purpose : returns the connection attribute values
*/

SQLRETURN SQL_API SQLGetConnectAttr(SQLHDBC hdbc,
            SQLINTEGER Attribute,
            SQLPOINTER ValuePtr,
            SQLINTEGER BufferLength,
            SQLINTEGER *StringLengthPtr)
{
  return get_con_attr(hdbc,Attribute,
          ValuePtr,BufferLength,
          StringLengthPtr);
}

/*
  @type    : ODBC 3.0 API
  @purpose : sets the statement attributes
*/

SQLRETURN SQL_API SQLSetStmtAttr(SQLHSTMT   hstmt,
         SQLINTEGER Attribute,
         SQLPOINTER ValuePtr,
         SQLINTEGER StringLength)
{

  return set_stmt_attr(hstmt, Attribute, ValuePtr, StringLength);
}

/*
  @type    : ODBC 3.0 API
  @purpose : returns the statement attribute values
*/
SQLRETURN SQL_API SQLGetStmtAttr(SQLHSTMT   hstmt,
         SQLINTEGER Attribute,
         SQLPOINTER ValuePtr,
         SQLINTEGER BufferLength,
         SQLINTEGER *StringLengthPtr)
{
  return get_stmt_attr(hstmt,Attribute, ValuePtr,BufferLength,
           StringLengthPtr);
}
