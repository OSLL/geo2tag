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
 * RESULTS.C                                                               *
 *                                                                         *
 * @description: For handling resultset and resultset information          *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

/*************************************************************************** 
 * The following ODBC APIs are implemented in this file:                   *
 *                                                                         *
 *   SQLRowCount          (ISO 92)                                         *
 *   SQLNumResultCols     (ISO 92)                                         *
 *   SQLDescribeCol       (ISO 92)                                         *
 *   SQLColAttribute      (ISO 92)                                         *
 *   SQLColAttributes     (ODBC, Deprecated)                               *
 *   SQLBindCol           (ISO 92)                                         *
 *   SQLFetch             (ISO 92)                                         *
 *   SQLFetchScroll       (ISO 92)                                         *
 *   SQLGetData           (ISO 92)                                         *
 *   SQLExtendedFetch     (ODBC, Deprecated)                               *
 *   SQLMoreResults       (ODBC)                                           *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"
#include <m_ctype.h>
#include <locale.h>

#define digit(A) ((int) (A - '0'))

SQLRETURN SQL_API sql_get_data(STMT *stm,SQLSMALLINT fCType,MYSQL_FIELD *field,
     SQLPOINTER rgbValue,
     SQLINTEGER  cbValueMax, SQLINTEGER FAR *pcbValue,
     char *value,uint length);

/*
  @type    : myodbc3 internal
  @purpose : execute the query if it is only prepared. This is needed
       because the ODBC standard allows calling some functions
       before SQLExecute(). (Stupid!)
*/

static SQLRETURN check_result(STMT FAR *stmt)
{
  SQLRETURN error;

  switch (stmt->state) {
  case ST_UNKNOWN:
    error=set_stmt_error(stmt,"24000","Invalid cursor state",0);
    break;
  case ST_PREPARED:
    if ((error=my_SQLExecute(stmt)) == SQL_SUCCESS)
      stmt->state=ST_PRE_EXECUTED;  /* mark for execute */
    break;
  case ST_PRE_EXECUTED:
  case ST_EXECUTED:
    error=SQL_SUCCESS;
   }
   return(error);
}

/*
  @type    : myodbc3 internal
  @purpose : does the any open param binding
*/

SQLRETURN do_dummy_parambind(SQLHSTMT hstmt)
{
  STMT FAR *stmt = (STMT FAR *)hstmt;
  uint       nparam;

  for (nparam = 0; nparam < stmt->param_count; nparam++)
  {
    PARAM_BIND *param= dynamic_element(&stmt->params,nparam,PARAM_BIND*);
    if (param->real_param_done != TRUE && param->used != 1)
    {
      /*
        do the dummy bind temporarily to get the result set
        and once everything is done, remove it
      */
      param->used = 1;
      param->SqlType = SQL_VARCHAR;
      param->CType = SQL_C_CHAR;
      param->buffer = "NULL";

      if (set_dynamic(&stmt->params,(gptr)param,nparam))
      {
        return set_stmt_error(stmt,"S1001","Not enough memory",4001);
      }
     }
  }
  stmt->dummy_state = ST_DUMMY_EXECUTED;
  return(SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 API
  @purpose : returns the number of columns in a result set
*/

SQLRETURN SQL_API SQLNumResultCols(SQLHSTMT  hstmt, SQLSMALLINT FAR *pccol)
{
  SQLRETURN error;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  DBUG_ENTER("SQLNumResultCols");

  if (stmt->param_count > 0 && stmt->dummy_state == ST_DUMMY_UNKNOWN &&
      (stmt->state != ST_PRE_EXECUTED || stmt->state != ST_EXECUTED))
  {
    if (do_dummy_parambind(hstmt) != SQL_SUCCESS)
      DBUG_RETURN(SQL_ERROR);
  }
  if ((error=check_result(stmt)) != SQL_SUCCESS)
    DBUG_RETURN(error);

  if (!stmt->result)
    *pccol=0;       /* Not a select */
  else
    *pccol=stmt->result->field_count;

  DBUG_PRINT("exit",("columns: %d",*pccol));
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 API
  @purpose : returns the result column name, type, column size, decimal
       digits, and nullabilityfor one column in the result set
*/

SQLRETURN SQL_API
SQLDescribeCol(SQLHSTMT hstmt, SQLUSMALLINT icol,
         SQLCHAR FAR *szColName,
         SQLSMALLINT cbColNameMax,
         SQLSMALLINT FAR *pcbColName,
         SQLSMALLINT FAR *pfSqlType,
         SQLUINTEGER FAR *pcbColDef,
         SQLSMALLINT FAR *pibScale,
         SQLSMALLINT FAR *pfNullable)
{
  SQLRETURN error;
  MYSQL_FIELD *field;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  ulong transfer_length,precision,display_size;
  int type;
  DBUG_ENTER("SQLDescribeCol");

  if ((error=check_result(stmt)) != SQL_SUCCESS)
    DBUG_RETURN(error);
  if (! stmt->result)
    DBUG_RETURN(set_stmt_error(stmt,"07005","No result set",0));

  mysql_field_seek(stmt->result,icol-1);
  if (!(field=mysql_fetch_field(stmt->result)))
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1002,
         "Invalid column number",0));
  type=unireg_to_sql_datatype(stmt,field,0,&transfer_length,&precision,
            &display_size);
  if (pfSqlType)
    *pfSqlType=type;
  if (pcbColDef)
    *pcbColDef=precision;
  if (pibScale)
    *pibScale=field->decimals;
  if (pfNullable)
    *pfNullable= (((field->flags & (NOT_NULL_FLAG | AUTO_INCREMENT_FLAG)) ==
       NOT_NULL_FLAG) ?
      SQL_NO_NULLS :
      SQL_NULLABLE);
  DBUG_PRINT("info",("col: %d type: %d  precision: %ld  decimals: %d",
         icol,type,precision,field->decimals));
  if (stmt->dbc->flag & FLAG_FULL_COLUMN_NAMES && field->table)
  {
    char *tmp=my_malloc(strlen(field->name)+strlen(field->table)+2,
      MYF(MY_WME));
    SQLRETURN error;
    if (!tmp)
    {
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001));
    }
    strxmov(tmp,field->table,".",field->name,NullS);
    error=copy_str_data(SQL_HANDLE_STMT, stmt, szColName,
      cbColNameMax, pcbColName, tmp);
    my_free((gptr) tmp,MYF(0));
    DBUG_RETURN(error);
  }
  DBUG_RETURN(copy_str_data(SQL_HANDLE_STMT, stmt, szColName,
          cbColNameMax, pcbColName, field->name));
}

/*
  @type    : myodbc3 internal
  @purpose : rerunrs column atribute values
*/

SQLRETURN SQL_API
get_col_attr(SQLHSTMT     StatementHandle,
       SQLUSMALLINT ColumnNumber,
       SQLUSMALLINT FieldIdentifier,
       SQLPOINTER   CharacterAttributePtr,
       SQLSMALLINT  BufferLength,
       SQLSMALLINT  *StringLengthPtr,
       SQLPOINTER   NumericAttributePtr)
{
  MYSQL_FIELD *field;
  STMT FAR *stmt=(STMT FAR*) StatementHandle;
  SQLSMALLINT str_length;
  SQLPOINTER strparam=0;
  SQLPOINTER nparam=0;
  ulong transfer_length,precision,display_size;
  SQLRETURN error;
  DBUG_ENTER("get_col_atrr");
  DBUG_PRINT("enter",("column: %d  type: %d",ColumnNumber, FieldIdentifier));

  if (check_result(stmt) != SQL_SUCCESS)
    DBUG_RETURN(SQL_ERROR);

  if (!stmt->result)
    DBUG_RETURN(set_stmt_error(stmt,"07005","No result set",0));

  if (ColumnNumber > stmt->result->field_count)
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,StatementHandle, MYERR_07009,
         NULL,0));

  if (!StringLengthPtr)
    StringLengthPtr= &str_length;

  if (!CharacterAttributePtr)
    CharacterAttributePtr= nparam;

  if (!NumericAttributePtr)
    NumericAttributePtr= strparam;

  if ((error=check_result(stmt)) != SQL_SUCCESS)
    DBUG_RETURN(error);

  if (FieldIdentifier == SQL_DESC_COUNT ||
      FieldIdentifier == SQL_COLUMN_COUNT )
  {
    *(SQLINTEGER *)NumericAttributePtr= stmt->result->field_count;
    *StringLengthPtr=sizeof(SQLINTEGER);
    DBUG_RETURN(SQL_SUCCESS);
  }
  if (FieldIdentifier == SQL_DESC_TYPE && ColumnNumber == 0)
  {
    *(SQLINTEGER *) NumericAttributePtr=SQL_INTEGER;
    *StringLengthPtr=sizeof(SQLINTEGER);
    DBUG_RETURN(SQL_SUCCESS);
  }
  mysql_field_seek(stmt->result,ColumnNumber-1);
  if (!(field=mysql_fetch_field(stmt->result)))
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1002,
         "Invalid column number",0));

  switch (FieldIdentifier) {

  case SQL_DESC_AUTO_UNIQUE_VALUE:
    *(SQLINTEGER *)NumericAttributePtr = (field->flags & AUTO_INCREMENT_FLAG ?
            SQL_TRUE : SQL_FALSE);
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

    /* We need support from server, when aliasing is there */
  case SQL_DESC_BASE_COLUMN_NAME:
  case SQL_DESC_LABEL:
  case SQL_DESC_NAME:
  case SQL_COLUMN_NAME:
    DBUG_RETURN(copy_str_data(SQL_HANDLE_STMT, stmt, CharacterAttributePtr,
            BufferLength,StringLengthPtr, field->name));

  case SQL_DESC_BASE_TABLE_NAME:
  case SQL_DESC_TABLE_NAME:
    DBUG_RETURN(copy_str_data(SQL_HANDLE_STMT, stmt,CharacterAttributePtr,
            BufferLength,StringLengthPtr,
            field->table ? field->table : ""));

  case SQL_DESC_CASE_SENSITIVE:
    *(SQLINTEGER *)NumericAttributePtr = (field->flags & BINARY_FLAG ?
            SQL_FALSE : SQL_TRUE);
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_DESC_CATALOG_NAME:
    DBUG_RETURN(copy_str_data(SQL_HANDLE_STMT, stmt,CharacterAttributePtr,
            BufferLength, StringLengthPtr,
            stmt->dbc->database));

  case SQL_DESC_DISPLAY_SIZE:
    (void) unireg_to_sql_datatype(stmt,field,0,&transfer_length,&precision,
          &display_size);
    *(SQLINTEGER *)NumericAttributePtr = display_size;
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_DESC_FIXED_PREC_SCALE: /* need to verify later */
  {
    if( field->type == FIELD_TYPE_DECIMAL )
      *(SQLINTEGER *)NumericAttributePtr = SQL_TRUE;
    else
      *(SQLINTEGER *)NumericAttributePtr = SQL_FALSE;
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;
  }

  case SQL_COLUMN_LENGTH:
  case SQL_DESC_LENGTH:
  case SQL_DESC_OCTET_LENGTH: /* need to validate again for octet length..*/
    (void) unireg_to_sql_datatype(stmt,field,0,&transfer_length,&precision,
          &display_size);
    *(SQLINTEGER *)NumericAttributePtr = transfer_length;
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_DESC_LITERAL_PREFIX:
  case SQL_DESC_LITERAL_SUFFIX:
    switch (field->type)  {
    case FIELD_TYPE_LONG_BLOB:
    case FIELD_TYPE_TINY_BLOB:
    case FIELD_TYPE_MEDIUM_BLOB: 
    case FIELD_TYPE_BLOB:    
      DBUG_RETURN(copy_str_data(SQL_HANDLE_STMT, stmt,CharacterAttributePtr,
        BufferLength,StringLengthPtr,"0x"));

    case FIELD_TYPE_DATE:
    case FIELD_TYPE_DATETIME:
    case FIELD_TYPE_NEWDATE:
    case FIELD_TYPE_VAR_STRING:
    case FIELD_TYPE_STRING:
    case FIELD_TYPE_TIMESTAMP:
    case FIELD_TYPE_TIME:
    case FIELD_TYPE_YEAR:
      DBUG_RETURN(copy_str_data(SQL_HANDLE_STMT, stmt,CharacterAttributePtr,
        BufferLength,StringLengthPtr,"'"));

    default:
      DBUG_RETURN(copy_str_data(SQL_HANDLE_STMT, stmt,CharacterAttributePtr,
        BufferLength,StringLengthPtr,""));
    }
    break;

  case SQL_DESC_NULLABLE:
  case SQL_COLUMN_NULLABLE:
    *(SQLINTEGER *)NumericAttributePtr = (((field->flags &
              (NOT_NULL_FLAG |
               AUTO_INCREMENT_FLAG)) ==
             NOT_NULL_FLAG) ?
            SQL_NO_NULLS :
            SQL_NULLABLE);
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_DESC_NUM_PREC_RADIX:
    switch(field->type)  {
    case FIELD_TYPE_SHORT:
    case FIELD_TYPE_LONG:
    case FIELD_TYPE_LONGLONG:
    case FIELD_TYPE_INT24:
    case FIELD_TYPE_TINY:
    case FIELD_TYPE_DECIMAL:
      *(SQLINTEGER *)NumericAttributePtr = 10;
      break;

    case FIELD_TYPE_FLOAT:
    case FIELD_TYPE_DOUBLE:
      *(SQLINTEGER *) NumericAttributePtr = 2;
      break;

    default:
      *(SQLINTEGER *)NumericAttributePtr = 0;
      break;
    }
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_COLUMN_PRECISION:
  case SQL_DESC_PRECISION:
    (void )unireg_to_sql_datatype(stmt,field,0,&transfer_length,&precision,
          &display_size);
    *(SQLINTEGER *)NumericAttributePtr = precision;
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_COLUMN_SCALE:
  case SQL_DESC_SCALE:
    *(SQLINTEGER *)NumericAttributePtr = field->decimals;
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_DESC_SCHEMA_NAME:
    DBUG_RETURN(copy_str_data(SQL_HANDLE_STMT, stmt,CharacterAttributePtr,
            BufferLength,StringLengthPtr, ""));

  case SQL_DESC_SEARCHABLE:
    *(SQLINTEGER *)NumericAttributePtr = SQL_SEARCHABLE;
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_DESC_TYPE:
  case SQL_DESC_CONCISE_TYPE:
    *(SQLINTEGER *)NumericAttributePtr =
      unireg_to_sql_datatype(stmt, field, 0, &transfer_length, &precision,
           &display_size);
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_DESC_TYPE_NAME:
  {
    char buff[40];
    (void)unireg_to_sql_datatype(stmt,field,(char *)buff,&transfer_length,
         &precision, &display_size);
    DBUG_RETURN(copy_str_data(SQL_HANDLE_STMT, stmt,CharacterAttributePtr,
            BufferLength,StringLengthPtr, buff));
  }

  case SQL_DESC_UNNAMED:
    *(SQLINTEGER *)NumericAttributePtr=SQL_NAMED;
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_DESC_UNSIGNED:
    *(SQLINTEGER *) NumericAttributePtr= (field->flags & UNSIGNED_FLAG ?
            SQL_TRUE : SQL_FALSE);
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  case SQL_DESC_UPDATABLE:
    *(SQLINTEGER *)NumericAttributePtr= (field->table && field->table[0] ?
           SQL_ATTR_WRITE :
           SQL_ATTR_READONLY);
    *StringLengthPtr=sizeof(SQLINTEGER);
    break;

  default:
    DBUG_PRINT("warning",("Type: %d is not supported by MyODBC3",
        FieldIdentifier));
  }
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : ODBC 3.0 API
  @purpose : rerunrs column atribute values
*/

SQLRETURN SQL_API
SQLColAttribute(SQLHSTMT  StatementHandle,
    SQLUSMALLINT ColumnNumber,
    SQLUSMALLINT FieldIdentifier,
    SQLPOINTER  CharacterAttributePtr,
    SQLSMALLINT BufferLength,
    SQLSMALLINT *StringLengthPtr,
    SQLPOINTER  NumericAttributePtr)
{
  return get_col_attr(StatementHandle, ColumnNumber,
          FieldIdentifier, CharacterAttributePtr,
          BufferLength, StringLengthPtr,
          NumericAttributePtr);
}

/*
  @type    : ODBC 1.0 API
  @purpose : rerunrs column atribute values
*/

SQLRETURN SQL_API
SQLColAttributes(SQLHSTMT hstmt,SQLUSMALLINT icol,
     SQLUSMALLINT fDescType,
     SQLPOINTER rgbDesc, SQLSMALLINT cbDescMax,
     SQLSMALLINT FAR *pcbDesc,
     SQLINTEGER FAR *pfDesc)
{
  SQLRETURN result=SQL_SUCCESS;
  DBUG_ENTER("SQLColAttributes");

  result = get_col_attr(hstmt, icol, fDescType, rgbDesc,
      cbDescMax, pcbDesc, pfDesc);
  DBUG_RETURN(result);
}

/*
  @type    : ODBC 1.0 API
  @purpose : binds application data buffers to columns in the result set
*/

SQLRETURN SQL_API
SQLBindCol(SQLHSTMT hstmt, SQLUSMALLINT icol,
     SQLSMALLINT fCType, SQLPOINTER rgbValue,
     SQLINTEGER  cbValueMax, SQLINTEGER FAR *pcbValue)
{
  BIND *bind;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  SQLRETURN error;
  DBUG_ENTER("SQLBindCol");
  DBUG_PRINT("enter",
       ("icol: %d  Type: %d  ValueMax: %ld  Valueptr: %lx  Value: %ld",
        icol,fCType,(long) cbValueMax, pcbValue,
        (long) (pcbValue ? *pcbValue : 0L)));

  icol--;
  /*
    The next case if because of VB 5.0 that binds columns before preparing
    a statement
  */

  if (stmt->state == ST_UNKNOWN)
  {
    DBUG_PRINT("info",("Binding columns without a statement; Hope you know what you are doing"));
    if (icol >= stmt->bound_columns)
    {
      if (!(stmt->bind=(BIND*) my_realloc((char*) stmt->bind,
            (icol+1)*sizeof(BIND),
            MYF(MY_ALLOW_ZERO_PTR |
                MY_FREE_ON_ERROR))))
      {
  stmt->bound_columns=0;
  DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,
             4001));
      }
      bzero((gptr) (stmt->bind+stmt->bound_columns),
      (icol+1-stmt->bound_columns)*sizeof(BIND));
      stmt->bound_columns=icol+1;
    }
  }
  else
  {
    /* Bind parameter to current set  ( The normal case ) */
    /* select stmt with parameters */
    if (stmt->param_count > 0 && stmt->dummy_state == ST_DUMMY_UNKNOWN &&
  (stmt->state != ST_PRE_EXECUTED || stmt->state != ST_EXECUTED))
    {
      if(do_dummy_parambind(hstmt) != SQL_SUCCESS)
  DBUG_RETURN(SQL_ERROR);
    }

    if ((error = check_result(stmt)) != SQL_SUCCESS)
    {
      DBUG_RETURN(error);
    }

    if (!stmt->result || (uint) icol >= stmt->result->field_count)
    {
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1002,
           "Invalid column number",0));
    }
    if (!stmt->bind)
    {
      if (!(stmt->bind=(BIND*) my_malloc(sizeof(BIND)*
           stmt->result->field_count,
           MYF(MY_ZEROFILL))))
  DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,
             4001));
      stmt->bound_columns=stmt->result->field_count;
    }
    mysql_field_seek(stmt->result,icol);
    stmt->bind[icol].field=mysql_fetch_field(stmt->result);
  }
  bind=stmt->bind+icol;
  bind->fCType=fCType;
  if (fCType == SQL_C_DEFAULT && stmt->odbc_types)
    bind->fCType=stmt->odbc_types[icol];
  bind->rgbValue=rgbValue;
  bind->cbValueMax=bind_length(bind->fCType,cbValueMax);
  bind->pcbValue=pcbValue;
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : myodbc3 internal
  @purpose : returns the latest resultset(dynamic)
*/

SQLRETURN set_dynamic_result(STMT FAR *stmt)
{
  if (odbc_stmt(stmt->dbc, stmt->query) != SQL_SUCCESS)
    return(SQL_ERROR);

  pthread_mutex_lock(&stmt->dbc->lock);
  x_free((gptr) stmt->odbc_types);
  mysql_free_result(stmt->result);
  stmt->result=0;
  stmt->odbc_types=0;
  stmt->cursor_row = 0;
  stmt->result=mysql_store_result(&stmt->dbc->mysql);
  if (!stmt->result)
  {
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
         mysql_error(&stmt->dbc->mysql),
         mysql_errno(&stmt->dbc->mysql));
    pthread_mutex_unlock(&stmt->dbc->lock);
    return(SQL_ERROR);
  }
  fix_result_types(stmt);
  set_current_cursor_data(stmt,0);
  pthread_mutex_unlock(&stmt->dbc->lock);
  return(SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 API
  @purpose : fetches the next rowset of data from the result set and
       returns data for all bound columns
*/

SQLRETURN SQL_API SQLFetch(SQLHSTMT hstmt)
{
  MYSQL_ROW values;
  SQLRETURN res,tmp_res;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  DBUG_ENTER("SQLFetch");

  if (!stmt->result)
    DBUG_RETURN(set_stmt_error(stmt,"24000","Fetch without a SELECT",0));

  if (stmt->result_array)
  {
    if (stmt->current_row >= stmt->result->row_count)
      DBUG_RETURN(SQL_NO_DATA_FOUND);
    values= (stmt->result_array+(stmt->current_row++)*
       stmt->result->field_count);
  }
  else
  {
    /* check for the previous first row fetch */
    if(stmt->current_row == 0 && stmt->rows_found_in_set > 0)
      mysql_data_seek(stmt->result, 1L);
    if (!(values=mysql_fetch_row(stmt->result)))
      DBUG_RETURN(SQL_NO_DATA_FOUND);
    stmt->current_row++;    /* For SQLGetStmtOption */
    if (stmt->fix_fields)
      values=(*stmt->fix_fields)(stmt,values);
    else
      stmt->result_lengths=mysql_fetch_lengths(stmt->result);
  }
  stmt->current_values=values;    /* For SQLGetData */
  stmt->last_getdata_col= (uint) ~0;

  res=SQL_SUCCESS;
  if (stmt->bind)
  {
    ulong *lengths=stmt->result_lengths;
    BIND *bind,*end;

    if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
      setlocale(LC_NUMERIC,"English");  /* force use of '.' as decimal point */
    for (bind=stmt->bind,end=bind + stmt->result->field_count ;
   bind < end ;
   bind++,values++)
    {
      if (bind->rgbValue || bind->pcbValue)
      {
  stmt->getdata_offset= (ulong) ~0L;
  if ((tmp_res=sql_get_data(stmt,bind->fCType,bind->field,bind->rgbValue,
          bind->cbValueMax, bind->pcbValue,
          *values, lengths ? *lengths : *values ?
          strlen(*values) : 0) )
      != SQL_SUCCESS)
  {
    if (tmp_res == SQL_SUCCESS_WITH_INFO)
    {
      DBUG_PRINT("info",("Problem with column: %d, value: '%s'",
             (int) (bind - stmt->bind)+1,
             *values ? *values : "NULL"));
      if (res == SQL_SUCCESS)
        res= tmp_res;
    }
    else
      res=SQL_ERROR;
  }
      }
      if (lengths)
  lengths++;
    }
    if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
      setlocale(LC_NUMERIC,default_locale);
  }
  stmt->getdata_offset= (ulong) ~0L;
  DBUG_RETURN(res);
}

/*
  @type    : ODBC 1.0 API
  @purpose : retrieves data for a single column in the result set. It can
       be called multiple times to retrieve variable-length data
       in parts
*/

SQLRETURN SQL_API
SQLGetData(SQLHSTMT hstmt,SQLUSMALLINT icol,
     SQLSMALLINT fCType,SQLPOINTER rgbValue,
     SQLINTEGER cbValueMax, SQLINTEGER FAR *pcbValue)
{
  STMT FAR *stmt=(STMT FAR*) hstmt;
  SQLRETURN result;
  DBUG_ENTER("SQLGetData");
  
  DBUG_PRINT("enter",("icol:%d, ctype:%d, rgb:0x%x, len:%d, pcb:0x%x)",
                     icol,fCType,rgbValue,cbValueMax,pcbValue));

  if (!stmt->result || !stmt->current_values)
  {
    set_stmt_error(stmt,"24000","SQLGetData without a preceding SELECT",0);
    DBUG_RETURN(SQL_ERROR);
  }
  icol--;     /* Easier code if start from 0 */
  if (icol != stmt->last_getdata_col)
  {   /* New column. Reset old offset */
    stmt->last_getdata_col=icol;
    stmt->getdata_offset= (ulong) ~0L;
  }

#ifdef LOG_ALL
  DBUG_PRINT("QQ",("icol: %d  fCType: %d  default: %d  value: %.10s",
       icol+1,fCType,
       stmt->odbc_types[icol],
       (stmt->current_values[icol] ? stmt->current_values[icol] :
        "NULL")));
#endif
  if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
    setlocale(LC_NUMERIC,"English");
  result=sql_get_data(stmt,
          (SQLSMALLINT) (fCType == SQL_C_DEFAULT ?
             stmt->odbc_types[icol] :
             fCType),
          stmt->result->fields+icol,
          rgbValue,cbValueMax,pcbValue,
          stmt->current_values[icol],
          (stmt->result_lengths ? stmt->result_lengths[icol] :
           (stmt->current_values[icol] ?
      strlen(stmt->current_values[icol]) : 0 )));
  if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
    setlocale(LC_NUMERIC,default_locale);

  DBUG_PRINT("exit ",("return:%d, rgb:0x%x, pcb:0x%x)",
                     result,rgbValue,pcbValue));
  DBUG_RETURN(result);
}

/*
  @type    : myodbc3 internal
  @purpose : returns the column data
*/

SQLRETURN SQL_API
sql_get_data(STMT    *stmt,
       SQLSMALLINT    fCType,
       MYSQL_FIELD    *field,
       SQLPOINTER     rgbValue,
       SQLINTEGER     cbValueMax,
       SQLINTEGER FAR *pcbValue,
       char       *value,
       uint       length)
{
  DBC *dbc=stmt->dbc;
  long tmp;
  if (!pcbValue)
    pcbValue= &tmp; /* Easier code */

  if (!value)
  {
    *pcbValue=SQL_NULL_DATA;
  }
  else switch (fCType) {
  case SQL_C_CHAR:
    /* Handle BLOB -> CHAR conversion */
    if ((field->flags & (BLOB_FLAG+BINARY_FLAG)) == (BLOB_FLAG+BINARY_FLAG))
      return copy_binary_result(dbc,(char*) rgbValue,cbValueMax,pcbValue,
        value, length, stmt->stmt_options.max_length,
        &stmt->getdata_offset);
    /* fall through */
  case SQL_C_BINARY:
  {
    char buff[21];
    if (field->type == FIELD_TYPE_TIMESTAMP && length != 19)
    {
      /* MySQL doesn't have '-' in timestamp */
      /* Convert timestamp to ANSI format */

      char *pos;
      uint i;
      if (length == 6 || length == 12)    /* YYMMDD or YYMMDDHHMMSS timestamp */
      {
  if (value[0] <= '6')
  {
    buff[0]='2';
    buff[1]='0';
  }
  else
  {
    buff[0]='1';
    buff[1]='9';
  }
      }
      else
      {
  buff[0]=value[0];
  buff[1]=value[1];
  value+=2;
  length-=2;
      }
      buff[2]= *value++;
      buff[3]= *value++;
      buff[4]='-';
      if (value[0] == '0' && value[1] == '0')
      {
  *pcbValue=SQL_NULL_DATA;  /* ODBC can't handle 0000-00-00 dates */
  break;
      }
      pos=buff+5;
      length&= 30;  /* Ensure that length is ok */
      for (i=1, length-=2 ; (int) length > 0 ; length-=2,i++)
      {
  *pos++= *value++;
  *pos++= *value++;
  *pos++= i < 2 ? '-' : (i == 2) ? ' ' : ':';
      }
      for ( ; pos != buff+20 ; i++)
      {
  *pos++= '0';
  *pos++= '0';
  *pos++= i < 2 ? '-' : (i == 2) ? ' ' : ':';
      }
      value=buff;
      length=19;
    }
    return copy_lresult(dbc,(char*) rgbValue,cbValueMax,pcbValue,value,
      length,stmt->stmt_options.max_length,
      field->type == FIELD_TYPE_STRING ? field->length : 0L,
      &stmt->getdata_offset,
      (my_bool) (fCType == SQL_C_BINARY));
  }
  case SQL_C_BIT:
    if (rgbValue)
      *((char*) rgbValue)= (atoi(value) == 0 ? 0 : 1);
    *pcbValue=1;
    break;
  case SQL_C_TINYINT:
  case SQL_C_STINYINT:
    if (rgbValue)
      *((char*) rgbValue)= ((char) atoi(value));
    *pcbValue=1;
    break;
  case SQL_C_UTINYINT:
    if (rgbValue)
      *((uchar*) rgbValue)= ((uchar) (uint) atoi(value));
    *pcbValue=1;
    break;
  case SQL_C_SHORT:
  case SQL_C_SSHORT:
    if (rgbValue)
      *((short*) rgbValue)= (short) atoi(value);
    *pcbValue=sizeof(short);
    break;
  case SQL_C_USHORT:
    if (rgbValue)
      *((ushort*) rgbValue)= (ushort) (uint) atol(value);
    *pcbValue=sizeof(short);
    break;
  case SQL_C_LONG:
  case SQL_C_SLONG:
    if (rgbValue)
    {
      /*--------------------------------------------*/
      /* Check if it could be a date...... :)     */
      /*--------------------------------------------*/
      if (length >= 10 && value[4] == '-' && value[7] == '-' &&
    (!value[10] || value[10] == ' '))
      {
  *((long*) rgbValue)= ((long) atol(value)*10000L+
            (long) atol(value+5)*100L+
            (long) atol(value+8));
      }
      else
  *((long*) rgbValue)= (long) atol(value);
    }
    *pcbValue=sizeof(long);
    break;
  case SQL_C_ULONG:
    if (rgbValue)
    {
      char *end_ptr;
      *((ulong*) rgbValue)= strtoul(value,&end_ptr,10);
    }
    *pcbValue=sizeof(long);
    break;
  case SQL_C_FLOAT:
    if (rgbValue)
      *((float*) rgbValue)= (float) atof(value);
    *pcbValue=sizeof(float);
    break;
  case SQL_C_DOUBLE:
    if (rgbValue)
      *((double*) rgbValue)= (double) atof(value);
    *pcbValue=sizeof(double);
    break;
  case SQL_C_DATE:
  case SQL_C_TYPE_DATE:
  {
    DATE_STRUCT tmp_date;
    if (!rgbValue)
      rgbValue=(char*) &tmp_date;
    if (!str_to_date((DATE_STRUCT *) rgbValue,value,length))
      *pcbValue=sizeof(DATE_STRUCT);
    else
    {
      *pcbValue=SQL_NULL_DATA;  /* ODBC can't handle 0000-00-00 dates */
    }
    break;
  }
  case SQL_C_TIME:
  case SQL_C_TYPE_TIME:
  {
    if (rgbValue)
    {
      ulong time=str_to_time(value,length);
      TIME_STRUCT *time_info=(TIME_STRUCT *) rgbValue;
      time_info->hour=  (SQLUSMALLINT) (time/10000);
      time_info->minute=(SQLUSMALLINT) (time/100%100);
      time_info->second=(SQLUSMALLINT) (time%100);
    }
    *pcbValue=sizeof(TIME_STRUCT);
    break;
  }
  case SQL_C_TIMESTAMP:
  case SQL_C_TYPE_TIMESTAMP:
  {
    uint year,length;
    char buff[15],*to;
    TIMESTAMP_STRUCT tmp_timestamp;
    TIMESTAMP_STRUCT *time_stamp;
    if (!rgbValue)
      rgbValue= (char*) &tmp_timestamp;
    time_stamp=(TIMESTAMP_STRUCT *) rgbValue;
    for (to=buff ; *value && to < buff+sizeof(buff)-1 ; value++)
    {
      if (isdigit(*value))
  *to++ = *value;
    }
    length= (uint) (to-buff);
    if (length == 6 || length == 12)  /* YYMMDD or YYMMDDHHMMSS */
    {
      bmove_upp(to+2,to,length);
      if (buff[0] <= '6')
      {
  buff[0]='2';
  buff[1]='0';
      }
      else
      {
  buff[0]='1';
  buff[1]='9';
      }
      length+=2;
    }
    if (length < 14)
      strfill(to,14 - length,'0');
    else
      *to=0;
    year=(digit(buff[0])*1000+digit(buff[1])*100+digit(buff[2])*10+
    digit(buff[3]));
    if (buff[4] == '0' && buff[5] == '0')
      *pcbValue=SQL_NULL_DATA;  /* ODBC can't handle 0000-00-00 dates */
    else
    {
      time_stamp->year=   year;
      time_stamp->month=  digit(buff[4])*10+digit(buff[5]);
      time_stamp->day=    digit(buff[6])*10+digit(buff[7]);
      time_stamp->hour=   digit(buff[8])*10+digit(buff[9]);
      time_stamp->minute= digit(buff[10])*10+digit(buff[11]);
      time_stamp->second= digit(buff[12])*10+digit(buff[13]);
      time_stamp->fraction=0;
      *pcbValue=sizeof(*time_stamp);
    }
    break;
  }
  case SQL_C_SBIGINT:
  {
    if (rgbValue)
      *((longlong*) rgbValue)= (longlong) strtoll(value,NULL,10);
    *pcbValue=sizeof(longlong);
    break;
  }
  case SQL_C_UBIGINT:
  {
    if (rgbValue)
      *((ulonglong*) rgbValue)= (ulonglong)strtoull(value,NULL,10);
    *pcbValue=sizeof(ulonglong);
    break;
  }
  }
  if (stmt->getdata_offset != (ulong) ~0L)  /* Second call to getdata */
  {
    return SQL_NO_DATA_FOUND;
  }
  stmt->getdata_offset=0L;  /* All data is retrevied */
  return SQL_SUCCESS;
}

/*
  @type    : ODBC 1.0 API
  @purpose : determines whether more results are available on a statement
       containing SELECT, UPDATE, INSERT, or DELETE statements and,
       if so, initializes processing for those results
*/

SQLRETURN SQL_API SQLMoreResults(SQLHSTMT  hstmt)
{
  DBUG_ENTER("SQLMoreResults");
  DBUG_RETURN(SQL_NO_DATA_FOUND);
}

/*
  @type    : ODBC 1.0 API
  @purpose : returns the number of rows affected by an UPDATE, INSERT,
       or DELETE statement;an SQL_ADD, SQL_UPDATE_BY_BOOKMARK,
       or SQL_DELETE_BY_BOOKMARK operation in SQLBulkOperations;
       or an SQL_UPDATE or SQL_DELETE operation in SQLSetPos
*/

SQLRETURN SQL_API SQLRowCount(SQLHSTMT hstmt, SQLINTEGER FAR *pcrow)
{
  STMT FAR *stmt=(STMT FAR*) hstmt;
  DBUG_ENTER("SQLRowCount");
  if (stmt->result)
  {
    *pcrow=(SQLINTEGER) mysql_affected_rows(&stmt->dbc->mysql);
    DBUG_PRINT("exit",("Rows in set: %ld",*pcrow));
  }
  else
  {
    *pcrow=(SQLINTEGER) stmt->affected_rows;
    DBUG_PRINT("exit",("Affected rows: %ld",*pcrow));
  }
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : myodbc3 internal
  @purpose : fetches the specified rowset of data from the result set and
       returns data for all bound columns. Rowsets can be specified
       at an absolute or relative position
*/
SQLRETURN SQL_API
my_SQLExtendedFetch(SQLHSTMT hstmt, SQLUSMALLINT fFetchType,
        SQLINTEGER irow, SQLUINTEGER FAR *pcrow,
        SQLUSMALLINT FAR *rgfRowStatus)
{
  ulong rows_to_fetch;
  long cur_row, max_row;
  uint i;
  SQLRETURN res,tmp_res;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  MYSQL_ROW values;
  MYSQL_ROW_OFFSET save_position;
  SQLUINTEGER dummy_pcrow;
  DBUG_ENTER("SQLExtendedFetch");

  if (!stmt->result)
    DBUG_RETURN(set_stmt_error(stmt,"24000","Fetch without a SELECT",0));
  DBUG_PRINT("enter",
       ("fetchtype: %d  row: %ld  current top-row: %ld  rows_found: %ld",
        fFetchType,irow,stmt->current_row,stmt->rows_found_in_set));

  if (stmt->stmt_options.cursor_type == SQL_CURSOR_FORWARD_ONLY &&
      fFetchType != SQL_FETCH_NEXT &&
      !(stmt->dbc->flag & FLAG_SAFE))
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1106,
         "Wrong fetchtype with FORWARD ONLY cursor",
         0));

  if (stmt->stmt_options.cursor_type == SQL_CURSOR_DYNAMIC &&
      (set_dynamic_result(stmt) != SQL_SUCCESS))
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
         "Driver Failed to set the internal dynamic result",0));

  if (!pcrow)
    pcrow= &dummy_pcrow;

  max_row=(long) mysql_num_rows(stmt->result);
  stmt->last_getdata_col=(uint)  ~0;
  stmt->current_values=0;   /* For SQLGetData */

  switch (fFetchType) {

  case SQL_FETCH_NEXT:
    cur_row = (stmt->current_row < 0 ? 0 :
         stmt->current_row+stmt->rows_found_in_set);
    break;
  case SQL_FETCH_PRIOR:
    cur_row = (stmt->current_row <= 0 ? -1 :
         stmt->current_row-stmt->stmt_options.rows_in_set);
    break;
  case SQL_FETCH_FIRST:
    cur_row=0L;
    break;
  case SQL_FETCH_LAST:
    cur_row=max_row-stmt->stmt_options.rows_in_set;
    break;
  case SQL_FETCH_ABSOLUTE:
    if (irow < 0)
    {
      /* Fetch from end of result set */
      if (max_row+irow < 0 && -irow <= (long) stmt->stmt_options.rows_in_set)
      {
  /*
    | FetchOffset | > LastResultRow AND
    | FetchOffset | <= RowsetSize
  */
  cur_row = 0;        /* Return from beginning */
      }
      else
  cur_row = max_row+irow;     /* Ok if max_row <= -irow */
    }
    else
      cur_row= (long) irow-1;
    break;

  case SQL_FETCH_RELATIVE:
    cur_row = stmt->current_row + irow;
    if (stmt->current_row > 0 && cur_row < 0 &&
  (long) -irow <= (long)stmt->stmt_options.rows_in_set)
      cur_row = 0;
    break;

  default:
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1106,
         "Fetch type out of range",0));
  }

  if ( cur_row < 0)
  {
    stmt->current_row= -1;      /* Before first row */
    stmt->rows_found_in_set=0;
    mysql_data_seek(stmt->result,0L);
    DBUG_RETURN(SQL_NO_DATA_FOUND);
  }
  if (cur_row > max_row)
    cur_row=max_row;

  if (!stmt->result_array)
  {
    /*
      If Dynamic, it looses the stmt->end_of_set, so
       seek to desired row, might have new data or
       might be deleted
    */
    if (stmt->stmt_options.cursor_type != SQL_CURSOR_DYNAMIC &&
  cur_row && cur_row == (long)(stmt->current_row +
             stmt->rows_found_in_set))
      mysql_row_seek(stmt->result,stmt->end_of_set);
    else
      mysql_data_seek(stmt->result,cur_row);
  }
  stmt->current_row=cur_row;

  rows_to_fetch=min(max_row-cur_row, (long)stmt->stmt_options.rows_in_set);
  if (!rows_to_fetch)
  {
    *pcrow=0;
    stmt->rows_found_in_set=0;
    DBUG_RETURN(SQL_NO_DATA_FOUND);
  }

  if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
    setlocale(LC_NUMERIC,"English");
  res=SQL_SUCCESS;
  for (i=0 ; i < rows_to_fetch ; i++)
  {
    if (stmt->result_array)
    {
      values=stmt->result_array+cur_row*stmt->result->field_count;
      if (i == 0)
  stmt->current_values=values;
    }
    else
    {
      if (i == 0)
  save_position=mysql_row_tell(stmt->result);
      if (!(values=mysql_fetch_row(stmt->result)))
  break;
      if (stmt->fix_fields)
  values=(*stmt->fix_fields)(stmt,values);
      else
  stmt->result_lengths=mysql_fetch_lengths(stmt->result);
      stmt->current_values=values;
    }
    if (rgfRowStatus)
    {
      rgfRowStatus[i]=SQL_ROW_SUCCESS;
      stmt->stmt_options.rowStatusPtr = rgfRowStatus;
    }
    if (stmt->bind)
    {     /* Should always be true */
      ulong *lengths=stmt->result_lengths;
      BIND *bind,*end;
      for (bind=stmt->bind,end=bind + stmt->result->field_count ;
     bind < end ;
     bind++,values++)
      {
  if (bind->rgbValue || bind->pcbValue)
  {
    ulong offset,pcb_offset;
    if (stmt->stmt_options.bind_type == SQL_BIND_BY_COLUMN)
    {
      offset=bind->cbValueMax*i;
      pcb_offset=sizeof(SQLINTEGER)*i;
    }
    else
      pcb_offset=offset=stmt->stmt_options.bind_type*i;
    stmt->getdata_offset= (ulong) ~0L;
    if ((tmp_res=sql_get_data(stmt,bind->fCType,
            bind->field,
            (bind->rgbValue ?
             (char*) bind->rgbValue + offset : 0),
            bind->cbValueMax,
            (bind->pcbValue ?
             (SQLINTEGER*) ((char*) bind->pcbValue +
                pcb_offset) : 0),
            *values,
            (lengths ? *lengths : *values ?
             strlen(*values) : 0)))
        != SQL_SUCCESS)
    {
      if (tmp_res == SQL_SUCCESS_WITH_INFO)
      {
        if (res == SQL_SUCCESS)
    res= tmp_res;
      }
      else
        res=SQL_ERROR;
    }
  }
  if (lengths)
    lengths++;
      }
    }
    cur_row++;
  }
  stmt->rows_found_in_set=i;
  *pcrow=i;

  if (rgfRowStatus)
    for ( ; i < stmt->stmt_options.rows_in_set ; i++)
      rgfRowStatus[i]=SQL_ROW_NOROW;

  if (!stmt->result_array)
  {     /* read data from first row */
    stmt->end_of_set=mysql_row_seek(stmt->result,save_position);
    if (i > 1)
    {
      stmt->current_values=mysql_fetch_row(stmt->result);
      if (stmt->fix_fields)
  stmt->current_values=(*stmt->fix_fields)(stmt,stmt->current_values);
      else
  stmt->result_lengths=mysql_fetch_lengths(stmt->result);
    }
  }
  if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
    setlocale(LC_NUMERIC,default_locale);
  DBUG_RETURN(res);
}

/*
  @type    : ODBC 1.0 API
  @purpose : fetches the specified rowset of data from the result set and
       returns data for all bound columns. Rowsets can be specified
       at an absolute or relative position
*/

SQLRETURN SQL_API
SQLExtendedFetch(SQLHSTMT hstmt,SQLUSMALLINT fFetchType,
     SQLINTEGER irow,
     SQLUINTEGER FAR *pcrow,
     SQLUSMALLINT FAR *rgfRowStatus)
{
  return my_SQLExtendedFetch(hstmt, fFetchType, irow,
           pcrow, rgfRowStatus);
}

/*
  @type    : ODBC 3.0 API
  @purpose : fetches the specified rowset of data from the result set and
       returns data for all bound columns. Rowsets can be specified
       at an absolute or relative position
*/

SQLRETURN SQL_API
SQLFetchScroll(SQLHSTMT    StatementHandle,
         SQLSMALLINT FetchOrientation,
         SQLINTEGER  FetchOffset)
{
  SQLRETURN result=SQL_SUCCESS;
  STMT_OPTIONS *options = &((STMT FAR *)StatementHandle)->stmt_options;
  DBUG_ENTER("SQLFetchScroll");

  if (!options->cursor_scollable && FetchOrientation != SQL_FETCH_NEXT)
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,StatementHandle,MYERR_S1106,
         NULL,0));

  result = my_SQLExtendedFetch(StatementHandle, FetchOrientation,
             FetchOffset,
             options->rowsFetchedPtr,
             options->rowStatusPtr);
  DBUG_RETURN(result);
}
