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
 * ERROR.C                                                                 *
 *                                                                         *
 * @description: MyODBC 3.51 error handling                                *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

/*************************************************************************** 
 * The following ODBC APIs are implemented in this file:                   *
 *                                                                         *
 *   SQLGetDiagField     (ISO 92)                                          *
 *   SQLGetDiagRec       (ISO 92)                                          *
 *   SQLError            (ODBC, Deprecated)                                *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"
#include "mysqld_error.h"
#include "errmsg.h"

/*
  @type    : myodbc3 internal error structure
  @purpose : set of internal errors, in the following order
       - SQLSTATE2 (when version is SQL_OV_ODBC2)
       - SQLSTATE3 (when version is SQL_OV_ODBC3)
       - error message text
       - return code
*/

static const MYODBC3_ERR_STR myodbc3_errors[]={

  {"01000","01000","General warning",1 },
  {"01004","01004","String data, right truncated",1 },
  {"01S02","01S02","Option value changed",1 },
  {"01S03","01S03","No rows updated/deleted",1 },
  {"01S04","01S04","More than one row updated/deleted",1 },
  {"01S06","01S06","Attempt to fetch before the result set returned the first rowset",1 },
  {"07001","07002","SQLBindParameter not used for all parameters",-1 },
  {"07005","07005","Prepared statement not a cursor-specification",-1 },
  {"07009","07009","Invalid descriptor index",-1 },
  {"08002","08002","Connection name in use",-1 },
  {"08003","08003","Connection does not exist",-1 },
  {"24000","24000","Invalid cursor state",-1 },
  {"25000","25000","Invalid transaction state",-1 },
  {"25S01","25S01","Transaction state unknown",-1 },
  {"34000","34000","Invalid cursor name",-1 },
  {"S1000","HY000","General driver defined error",-1 },
  {"S1001","HY001","Memory allocation error",-1 },
  {"S1002","HY002","Invalid column number",-1 },
  {"S1003","HY003","Invalid application buffer type",-1 },
  {"S1004","HY004","Invalid SQL data type",-1 },
  {"S1009","HY009","Invalid use of null pointer",-1 },
  {"S1010","HY010","Function sequence error",-1 },
  {"S1011","HY011","Attribute can not be set now",-1 },
  {"S1012","HY012","Invalid transaction operation code",-1 },
  {"S1013","HY013","Memory management error",-1 },
  {"S1015","HY015","No cursor name available",-1 },
  {"S1024","HY024","Invalid attribute value",-1 },
  {"S1090","HY090","Invalid string or buffer length",-1 },
  {"S1091","HY091","Invalid descriptor field identifier",-1 },
  {"S1092","HY092","Invalid attribute/option identifier",-1 },
  {"S1093","HY093","Invalid parameter number",-1 },
  {"S1095","HY095","Function type out of range",-1 },
  {"S1106","HY106","Fetch type out of range",-1 },
  {"S1107","HY107","Row value out of range",-1 },
  {"S1109","HY109","Invalid cursor position",-1 },
  {"S1C00","HYC00","Optional feature not implemented",-1 },
  /* server related..*/
  {"21S01","21S01","Column count does not match value count",-1},
  {"23000","23000","Integrity constraint violation",-1},
  {"42000","42000","Syntax error or access violation",-1},
  {"42S02","42S02","Base table or view not found",-1},
  {"42S12","42S12","Index not found",-1},
  {"42S21","42S21","Column already exists",-1},
  {"42S22","42S22","Column not found",-1},
  {"08S01","08S01","Communication link failure",-1},
};

/*
  @type    : myodbc3 internal
  @purpose : copies error from one handdle to other
*/

SQLRETURN copy_stmt_error(STMT FAR *dst,STMT FAR *src)
{
  strmov(dst->error.sqlstate,src->error.sqlstate);
  strmov(dst->error.message, src->error.message);
  dst->error.native_error = src->error.native_error;
  dst->error.retcode = src->error.retcode;

  return(SQL_SUCCESS);
}

/*
  @type    : myodbc3 internal
  @purpose : sets the connection level errors, which doesn't need any
       conversion
*/

SQLRETURN set_dbc_error(DBC FAR *dbc,char *state,char *message,uint errcode)
{
  DBUG_ENTER("set_dbc_error");
  DBUG_PRINT("error",("message: %s",message));
  strmov(dbc->error.sqlstate,state);
  strxmov(dbc->error.message,MYODBC3_ERROR_PREFIX,message,NullS);
  dbc->error.native_error=errcode;
  DBUG_RETURN(SQL_ERROR);
}

/*
  @type    : myodbc3 internal
  @purpose : sets the statement level errors, which doesn't need any
       conversion
*/

SQLRETURN set_stmt_error(STMT FAR *stmt,char *state,char *message,uint errcode)
{
  DBUG_ENTER("set_stmt_error");
  DBUG_PRINT("error",("message: %s",message));
  strmov(stmt->error.sqlstate,state);
  strxmov(stmt->error.message,MYODBC3_ERROR_PREFIX,
          "[mysqld-",stmt->dbc->mysql.server_version,"]",
          message,NullS);
  stmt->error.native_error=errcode;
  DBUG_RETURN(SQL_ERROR);
}

/*
  @type    : myodbc3 internal
  @purpose : translates SQL error to ODBC error
*/

void translate_error(char *save_state,char *default_state,uint mysql_err)
{
  char *state=default_state;
  DBUG_ENTER("translate_error");
  switch (mysql_err) {
  case ER_WRONG_VALUE_COUNT:    state= "21S01"; break;
  case ER_DUP_KEY:      state= "23000"; break;
  case ER_PARSE_ERROR:      state= "42000"; break;
  case ER_FILE_NOT_FOUND:
  case ER_CANT_OPEN_FILE:   state= "42S02"; break;
  case ER_CANT_DROP_FIELD_OR_KEY: state= "42S12"; break;
  case ER_DUP_FIELDNAME:    state= "42S21"; break;
  case ER_BAD_FIELD_ERROR:    state= "42S22"; break;
  case CR_SERVER_HANDSHAKE_ERR:
  case CR_CONNECTION_ERROR:   state= "08S01"; break;
  default: break;
  }
  strmov(save_state,state);
  DBUG_VOID_RETURN;
}

/*
  @type    : myodbc3 internal
  @purpose : sets the error information to appropriate handle.
       it also sets the SQLSTATE based on the ODBC VERSION
*/

SQLRETURN
set_handle_error(SQLSMALLINT HandleType, SQLHANDLE handle,
     myodbc_errid errid, SQLCHAR *errtext,
     SQLINTEGER mysql_err)
{
  SQLRETURN  sqlreturn = SQL_ERROR;
  SQLINTEGER my_version;
  MYERROR    *error;
  SQLCHAR    mystate[6];
  SQLCHAR    *errmsg;
  SQLINTEGER native_code;
  SQLCHAR    my_error_prefix[256];

  DBUG_ENTER("set_handle_error");
  DBUG_PRINT("values",("type:%d, handle:%x, errid:%d, err:%s,myerr:%d",
          HandleType,handle,errid,errtext,mysql_err));

  switch (HandleType) {

  case SQL_HANDLE_STMT:
    my_version = ((STMT FAR *) handle)->dbc->env->odbc_ver;
    error      = &((STMT FAR *) handle)->error;

    /*
      If stmt handle, include server version also in the error prefix
      ex: [MySQL][ODBC 3.51 Driver][mysqld-3.23.45-max], this helps
      a lot for users..
    */
    strxmov(my_error_prefix,MYODBC3_ERROR_PREFIX,"[mysqld-",
      ((STMT FAR *)handle)->dbc->mysql.server_version,"]",NullS);
    break;

  case SQL_HANDLE_DBC:
    my_version = ((DBC FAR *)handle)->env->odbc_ver;
    error      = &((DBC FAR *)handle)->error;
    strxmov(my_error_prefix,MYODBC3_ERROR_PREFIX,NullS);
    break;

  default:
    my_version = ((ENV FAR *)handle)->odbc_ver;
    error      = &((ENV FAR *)handle)->error;
    strxmov(my_error_prefix,MYODBC3_ERROR_PREFIX,NullS);
    break;
  }
  errmsg = errtext ? errtext : (SQLCHAR *)myodbc3_errors[errid].message;

  if (my_version == SQL_OV_ODBC2)
    strmov(mystate, myodbc3_errors[errid].sqlstate2);
  else
    strmov(mystate, myodbc3_errors[errid].sqlstate3);

  native_code = mysql_err ? mysql_err : errid + 500;

  DBUG_PRINT("intern-error",("[%d@%s:%s]",native_code,mystate,errmsg));
  sqlreturn = error->retcode = myodbc3_errors[errid].retcode;
  strxmov(error->message, my_error_prefix, errmsg, NullS);
  error->native_error = native_code;
  strmov(error->sqlstate , mystate);

  DBUG_RETURN(sqlreturn);
}

/*
  @type    : myodbc3 internal
  @purpose : returns the current values of multiple fields of a diagnostic
       record that contains error, warning, and status information.
       Unlike SQLGetDiagField, which returns one diagnostic field
       per call, SQLGetDiagRec returns several commonly used fields
       of a diagnostic record, including the SQLSTATE, the native
       error code, and the diagnostic message text
*/

SQLRETURN
my_SQLGetDiagRec(SQLSMALLINT HandleType,
      SQLHANDLE Handle,
      SQLSMALLINT RecNumber,
      SQLCHAR *Sqlstate,
      SQLINTEGER  *NativeErrorPtr,
      SQLCHAR *MessageText,
      SQLSMALLINT BufferLength,
      SQLSMALLINT *TextLengthPtr)
{
  SQLCHAR     *errmsg;
  SQLRETURN   result = SQL_SUCCESS;
  SQLSMALLINT tmp_size;
  SQLCHAR     tmp_state[6];
  SQLINTEGER  tmp_error;
  DBUG_ENTER("SQLGetDiagRec");
  DBUG_PRINT("values",("%d,%x,%d,%x,%x,%x,%d,%x",HandleType,Handle,
           RecNumber,Sqlstate,NativeErrorPtr,MessageText,
           BufferLength,TextLengthPtr));

  if (!TextLengthPtr)
    TextLengthPtr= &tmp_size;

  if (!Sqlstate)
    Sqlstate= tmp_state;

  if (!NativeErrorPtr)
    NativeErrorPtr= &tmp_error;

  if (RecNumber <= 0 || BufferLength < 0 || !Handle)
    DBUG_RETURN(SQL_ERROR);

  /*
    Currently we are not supporting error list, so
    if RecNumber > 1, return no data found
  */

  if (RecNumber > 1)
    DBUG_RETURN(SQL_NO_DATA_FOUND);

  switch(HandleType) {
  case SQL_HANDLE_STMT:
    errmsg=((STMT FAR*) Handle)->error.message;
    strmov((char*) Sqlstate,((STMT FAR*) Handle)->error.sqlstate);
    *NativeErrorPtr=((STMT FAR*) Handle)->error.native_error;
    break;

  case SQL_HANDLE_DBC:
    errmsg=((DBC FAR*) Handle)->error.message;
    strmov((char*) Sqlstate,((DBC FAR*) Handle)->error.sqlstate);
    *NativeErrorPtr=((DBC FAR*) Handle)->error.native_error;

    break;

  case SQL_HANDLE_ENV:
    errmsg=((ENV FAR*) Handle)->error.message;
    strmov((char*) Sqlstate,((ENV FAR*) Handle)->error.sqlstate);
    *NativeErrorPtr=((ENV FAR*) Handle)->error.native_error;
    break;

  default:
    DBUG_RETURN(SQL_INVALID_HANDLE);
  }
  if (!errmsg || !errmsg[0])
  {
    *TextLengthPtr=0;
    strmov(Sqlstate,"00000");
    DBUG_RETURN(SQL_NO_DATA_FOUND);
  }
  DBUG_RETURN(copy_str_data(HandleType,Handle,MessageText,BufferLength,
                            TextLengthPtr, errmsg));
}

/*
  @type    : ODBC 3.0 API
  @purpose : returns the current value of a field of a record of the
       diagnostic data structure (associated with a specified handle)
       that contains error, warning, and status information
*/

SQLRETURN SQL_API
SQLGetDiagField(SQLSMALLINT HandleType,
    SQLHANDLE   Handle,
    SQLSMALLINT RecNumber,
    SQLSMALLINT DiagIdentifier,
    SQLPOINTER  DiagInfoPtr,
    SQLSMALLINT BufferLength,
    SQLSMALLINT *StringLengthPtr)
{
  SQLRETURN   error = SQL_SUCCESS;
  SQLPOINTER  szDiagInfo=NULL;
  SQLSMALLINT tmp_size;
  DBUG_ENTER("SQLGetDiagField");
  DBUG_PRINT("values",("%d,%x,%d,%d,%x,%d,%x",HandleType,Handle,
           RecNumber,DiagIdentifier,DiagInfoPtr,
           BufferLength,StringLengthPtr));

  if (!StringLengthPtr)
    StringLengthPtr= &tmp_size;

  if (!DiagInfoPtr)
    DiagInfoPtr= szDiagInfo;

  if (!Handle ||
      !(HandleType == SQL_HANDLE_STMT ||
        HandleType == SQL_HANDLE_DBC ||
        HandleType == SQL_HANDLE_ENV ))
    DBUG_RETURN(SQL_ERROR);

  if (RecNumber > 1)
    DBUG_RETURN(SQL_NO_DATA_FOUND);

  /* If record number is 0 and if it is not a
     diag header field request, return error
  */
  if (RecNumber == 0 && DiagIdentifier > 0)
    DBUG_RETURN(SQL_ERROR);

  switch(DiagIdentifier ) {

    /* DIAG HEADER FIELDS SECTION */
  case SQL_DIAG_DYNAMIC_FUNCTION:
    if (HandleType != SQL_HANDLE_STMT)
      DBUG_RETURN(SQL_ERROR);

    error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
      StringLengthPtr, "");
    break;

  case SQL_DIAG_DYNAMIC_FUNCTION_CODE:
    *(SQLINTEGER *) DiagInfoPtr = 0;
    break;

  case SQL_DIAG_ROW_NUMBER:
    *(SQLINTEGER *) DiagInfoPtr = SQL_ROW_NUMBER_UNKNOWN;
    break;

  case SQL_DIAG_NUMBER:
    *(SQLINTEGER *) DiagInfoPtr = 1;
    break;

  case SQL_DIAG_RETURNCODE:
    if (HandleType == SQL_HANDLE_STMT)
      *(SQLRETURN *) DiagInfoPtr = ((STMT FAR *)Handle)->error.retcode;

    else if (HandleType == SQL_HANDLE_DBC)
      *(SQLRETURN *) DiagInfoPtr = ((DBC FAR *)Handle)->error.retcode;

    else
      *(SQLRETURN *) DiagInfoPtr = ((ENV FAR *)Handle)->error.retcode;
    break;

  case SQL_DIAG_CURSOR_ROW_COUNT:/* at present, return total rows in rs */
    if (HandleType != SQL_HANDLE_STMT)
      DBUG_RETURN(SQL_ERROR);

    if (!((STMT FAR *)Handle)->result)
      *(SQLINTEGER *) DiagInfoPtr=0;

    else
      *(SQLINTEGER *) DiagInfoPtr = (SQLINTEGER)
  mysql_num_rows(((STMT FAR *) Handle)->result);
    break;

  case SQL_DIAG_ROW_COUNT:
    if (HandleType != SQL_HANDLE_STMT)
      DBUG_RETURN(SQL_ERROR);

    *(SQLINTEGER *) DiagInfoPtr = (SQLINTEGER)
      ((STMT FAR *) Handle)->affected_rows;
    break;

    /* DIAG RECORD FIELDS SECTION */

  case SQL_DIAG_CLASS_ORIGIN:
  case SQL_DIAG_SUBCLASS_ORIGIN:
    error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
      StringLengthPtr, "ISO 9075");
    break;

  case SQL_DIAG_COLUMN_NUMBER:

    *(SQLINTEGER *) DiagInfoPtr = SQL_COLUMN_NUMBER_UNKNOWN;
    break;

  case SQL_DIAG_CONNECTION_NAME:
    /*
      When the connection fails, ODBC DM calls this function, so don't
      return dbc->dsn as the connection name instead return empty string
    */

    if (HandleType == SQL_HANDLE_STMT)
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
        StringLengthPtr,((STMT FAR *)Handle)->dbc->dsn ?
        ((STMT FAR *)Handle)->dbc->dsn:"");

    else if (HandleType == SQL_HANDLE_DBC)
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
        StringLengthPtr,((DBC FAR *)Handle)->dsn ?
        ((DBC FAR *)Handle)->dsn:"");

    else
    {
      *(SQLCHAR *) DiagInfoPtr = 0;
      *StringLengthPtr = 0;
    }
    break;

  case SQL_DIAG_MESSAGE_TEXT:
    if (HandleType == SQL_HANDLE_STMT)
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
        StringLengthPtr,((STMT FAR *)Handle)->error.message);

    else if (HandleType == SQL_HANDLE_DBC)
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
                          StringLengthPtr,((DBC FAR *)Handle)->error.message);
    else
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
        StringLengthPtr,((ENV FAR *)Handle)->error.message);
    break;

  case SQL_DIAG_NATIVE:
    if (HandleType == SQL_HANDLE_STMT)
      *(SQLINTEGER *) DiagInfoPtr = ((STMT FAR *)Handle)->error.native_error;

    else if (HandleType == SQL_HANDLE_DBC)
      *(SQLINTEGER *) DiagInfoPtr = ((DBC FAR *)Handle)->error.native_error;

    else
      *(SQLINTEGER *) DiagInfoPtr = ((ENV FAR *)Handle)->error.native_error;
    break;

  case SQL_DIAG_SERVER_NAME:
    if (HandleType == SQL_HANDLE_STMT)
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
        StringLengthPtr,
        ((STMT FAR *)Handle)->dbc->server ?
        ((STMT FAR *)Handle)->dbc->server : "");

    else if (HandleType == SQL_HANDLE_DBC)
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
        StringLengthPtr,((DBC FAR *)Handle)->server ?
        ((DBC FAR *)Handle)->server : "");
    else
    {
      *(SQLCHAR *) DiagInfoPtr = 0;
      *StringLengthPtr = 0;
    }
    break;

  case SQL_DIAG_SQLSTATE:
    if (HandleType == SQL_HANDLE_STMT)
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
        StringLengthPtr,
        ((STMT FAR *)Handle)->error.sqlstate);

    else if (HandleType == SQL_HANDLE_DBC)
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
        StringLengthPtr,
        ((DBC FAR *)Handle)->error.sqlstate);

    else
      error=copy_str_data(HandleType, Handle, DiagInfoPtr, BufferLength,
        StringLengthPtr,
        ((ENV FAR *)Handle)->error.sqlstate);
    break;

  default:
    DBUG_RETURN(SQL_ERROR);
  }
  DBUG_RETURN(error);
}

/*
  @type    : ODBC 3.0 API
  @purpose : returns the current diagnostic record information
*/

SQLRETURN SQL_API
SQLGetDiagRec(SQLSMALLINT HandleType,
        SQLHANDLE   Handle,
        SQLSMALLINT RecNumber,
        SQLCHAR     *Sqlstate,
        SQLINTEGER  *NativeErrorPtr,
        SQLCHAR     *MessageText,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *TextLengthPtr)
{
  return my_SQLGetDiagRec(HandleType,Handle,RecNumber,Sqlstate,
        NativeErrorPtr,MessageText,
        BufferLength,TextLengthPtr);
}

/*
  @type    : ODBC 1.0 API - depricated
  @purpose : returns error or status information
*/

SQLRETURN SQL_API
SQLError(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt,
         SQLCHAR FAR    *szSqlState,
         SQLINTEGER FAR *pfNativeError,
         SQLCHAR FAR    *szErrorMsg,
         SQLSMALLINT    cbErrorMsgMax,
         SQLSMALLINT FAR *pcbErrorMsg)
{
  SQLRETURN error=SQL_INVALID_HANDLE;
  DBUG_ENTER("SQLError");

  if (hstmt){
    error = my_SQLGetDiagRec(SQL_HANDLE_STMT,hstmt,1,szSqlState,
                             pfNativeError, szErrorMsg,
                       	     cbErrorMsgMax,pcbErrorMsg);
    if(error == SQL_SUCCESS)
      ((STMT FAR*)hstmt)->error.message[0]='\0';
  }
  else if (hdbc){
    error = my_SQLGetDiagRec(SQL_HANDLE_DBC,hdbc,1,szSqlState,
                             pfNativeError, szErrorMsg,
                             cbErrorMsgMax,pcbErrorMsg);
    if(error == SQL_SUCCESS)
      ((DBC FAR*)hdbc)->error.message[0]='\0';
  }
  else if (henv){
    error = my_SQLGetDiagRec(SQL_HANDLE_ENV,henv,1,szSqlState,
                             pfNativeError, szErrorMsg,
                             cbErrorMsgMax,pcbErrorMsg);
    if(error == SQL_SUCCESS)
      ((ENV FAR*)henv)->error.message[0]='\0';  
  }
  DBUG_RETURN(error);
}
