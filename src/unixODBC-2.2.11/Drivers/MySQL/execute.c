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
 * EXECUTE.C                                                               *
 *                                                                         *
 * @description: Driver code for executing SQL Commands                    *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

/*************************************************************************** 
 * The following ODBC APIs are implemented in this file:                   *
 *                                                                         *
 *   SQLExecute          (ISO 92)                                          *
 *   SQLExecDirect       (ISO 92)                                          *
 *   SQLParamData        (ISO 92)                                          *
 *   SQLPutData          (ISO 92)                                          *
 *   SQLCancel           (ISO 92)                                          *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"
#include <locale.h>

/*
  @type    : myodbc3 internal
  @purpose : internal function to execute query and return result
       frees query if query != stmt->query
*/

SQLRETURN do_query(STMT FAR *stmt,char *query)
{
  int error=SQL_ERROR;
  DBUG_ENTER("do_query");

  if (!query)
    DBUG_RETURN(error);       /* Probably error from insert_param */
  if (stmt->stmt_options.max_rows && stmt->stmt_options.max_rows !=
      (ulong) ~0L)
  {
    /* Add limit to select statement */
    char *pos,*tmp_buffer;
    for (pos=query; isspace(*pos) ; pos++) ;
    if (!my_casecmp(pos,"select",6))
    {
      uint length=strlen(pos);
      if ((tmp_buffer=my_malloc(length+30,MYF(0))))
      {
  memcpy(tmp_buffer,query,length);
  sprintf(tmp_buffer+length," limit %lu",stmt->stmt_options.max_rows);
  if (query != stmt->query)
    my_free((gptr) query,MYF(0));
  query=tmp_buffer;
      }
    }
  }
  pthread_mutex_lock(&stmt->dbc->lock);
  if (check_if_server_is_alive(stmt->dbc) ||
      mysql_query(&stmt->dbc->mysql,query))
  {
    set_stmt_error(stmt,"S1000",mysql_error(&stmt->dbc->mysql),
       mysql_errno(&stmt->dbc->mysql));
    translate_error(stmt->error.sqlstate,"S1000",
        mysql_errno(&stmt->dbc->mysql));
    goto exit;
  }

#ifdef NOT_USED
  /* We can't use USE_RESULT because SQLRowCount will fail in this case! */
  if (stmt->stmt_options.cursor_type == SQL_CURSOR_FORWARD_ONLY &&
      !(stmt->dbc->flag & FLAG_SAFE))
    stmt->result=mysql_use_result(&stmt->dbc->mysql);
  else
#endif

    stmt->result=mysql_store_result(&stmt->dbc->mysql);
  if (!stmt->result)
  {
    if (!mysql_field_count(&stmt->dbc->mysql))
    {
      error=SQL_SUCCESS;     /* no result set */
      stmt->state=ST_EXECUTED;
      stmt->affected_rows=mysql_affected_rows(&stmt->dbc->mysql);
      DBUG_PRINT("info",("affected rows: %d",stmt->affected_rows));
      goto exit;
    }
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
         mysql_error(&stmt->dbc->mysql),
         mysql_errno(&stmt->dbc->mysql));
    goto exit;
  }
  fix_result_types(stmt);
  error=SQL_SUCCESS;

exit:
  pthread_mutex_unlock(&stmt->dbc->lock);
  if (query != stmt->query)
    my_free((gptr) query,MYF(0));
  DBUG_RETURN(error);
}

/*
  @type    : myodbc3 internal
  @purpose : help function to enlarge buffer if necessary
*/

char *extend_buffer(NET *net,char *to,ulong length)
{
  ulong nead;
  DBUG_ENTER("extend_buffer");
  DBUG_PRINT("enter",("current_length: %ld  length: %ld  buffer_length: %ld",
          (ulong) (to - (char*) net->buff),
          (ulong) length,
          (ulong) net->max_packet));

  if (!to ||
      (nead=(ulong) (to - (char*) net->buff)+length) > net->max_packet-10)
  {
    ulong pkt_length=(nead+8192) & ~(8192-1);
    uchar *buff;

    if (pkt_length > max_allowed_packet)
    {
      DBUG_PRINT("error",("Needed %ld but max_allowed_packet is %ld",
        pkt_length,max_allowed_packet));
      DBUG_RETURN(0);     /* Too large packet */
    }
    if (!(buff=(uchar*) my_realloc((char*) net->buff,pkt_length,
           MYF(MY_WME))))
      DBUG_RETURN(0);
    to=buff+nead-length;
    net->buff=net->write_pos=buff;
    net->buff_end=buff+(net->max_packet=pkt_length);
  }
  DBUG_RETURN(to);
}

/*
  @type    : myodbc3 internal
  @purpose : help function to extend the buffer and copy the data
*/

char *add_to_buffer(NET *net,char *to,char *from,ulong length)
{
  DBUG_ENTER( "add_to_buffer" );
  DBUG_PRINT("enter",("from: '%-.32s'  length: %ld",
    from ? from: "<null>", (long int) length ));
  if (!(to=extend_buffer(net,to,length)))
    DBUG_RETURN( 0 );
  memcpy(to,from,length);
  DBUG_RETURN( to+length );
}

/*
  @type    : myodbc3 internal
  @purpose : help function to extend the buffer
*/

static char * extend_escape_buffer(void *net, char *to, ulong *length)
{
  if ((to=extend_buffer((NET*) net, to, *length)))
  {
    /*
      Buffer has been extended;  We now need to return in length the
      ammount of space available in the buffer.
      'max_packet' is total length of buffer,
      'to' is next available spot within 'buff' to place data,
      'buff' is start of buffer
      so.... return "total space" less the "amount already used".
    */
    *length = (((NET*) net)->max_packet
         - (ulong) (to - (char*) ((NET*) net)->buff));
  }
  return to;
}

/*
  @type    : myodbc3 internal
  @purpose : insert sql params at parameter positions
*/

char *insert_params(STMT FAR *stmt)
{
  char *query=stmt->query,*to;
  uint i,length;
  NET *net;
  DBUG_ENTER("insert_params");

  pthread_mutex_lock(&stmt->dbc->lock);
  net= &stmt->dbc->mysql.net;
  to=net->buff;
  if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
    setlocale(LC_NUMERIC,"English");  /* force use of '.' as decimal point */
  for (i=0; i < stmt->param_count; i++)
  {
    PARAM_BIND *param=dynamic_element(&stmt->params,i,PARAM_BIND*);
    char *pos;
    if (!param->used)
    {
      if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
  setlocale(LC_NUMERIC,default_locale);
      set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_07001,NULL,0);
      pthread_mutex_unlock(&stmt->dbc->lock);
      DBUG_RETURN(0);
    }
    pos=param->pos_in_query;
    length=(uint) (pos-query);
    DBUG_PRINT( "info", ("pos_in_query: %p  query: %p", pos, query) );
    if (!(to=add_to_buffer(net,to,query,length)))
      goto error;
    query=pos+1;  /* Skipp '?' */
    if (!(to=insert_param(&stmt->dbc->mysql,to,param)))
      goto error;
  }
  length=(uint) (stmt->query_end - query);
  if (!(to=add_to_buffer(net,to,query,length+1)))
    goto error;
  if (!(to=(char*) my_memdup((char*) net->buff,
           (uint) (to - (char*) net->buff),MYF(0))))
  {
    if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
      setlocale(LC_NUMERIC,default_locale);
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001);
    pthread_mutex_unlock(&stmt->dbc->lock);
    DBUG_RETURN(0);
  }
  pthread_mutex_unlock(&stmt->dbc->lock);
  if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
    setlocale(LC_NUMERIC,default_locale);
  DBUG_RETURN(to);

error:      /* Too much data */
  pthread_mutex_unlock(&stmt->dbc->lock);
  if (!(stmt->dbc->flag & FLAG_NO_LOCALE))
    setlocale(LC_NUMERIC,default_locale);
  set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,
       NULL,4001);
  DBUG_RETURN(0);
}

/*
  @type    : myodbc3 internal
  @purpose : insert sql param the specified parameter position
*/

char *insert_param(MYSQL *mysql, char *to,PARAM_BIND *param)
{
  uint length;
  char buff[128],*data;
  bool convert=0;
  NET *net= &mysql->net;

  if (!param->actual_len || *(param->actual_len) == SQL_NTS)
  {
    if ((data=param->buffer))
    {
      if (param->ValueMax)
        length=strnlen(data,param->ValueMax);
      else
        length=strlen(data);
    }
    else
    {
      DBUG_PRINT("warning",("data is a null pointer"));
      length=0;     /* This is actually an error */
    }
  }
  else if (*(param->actual_len) == SQL_NULL_DATA)
  {
    return add_to_buffer(net,to,"NULL",4);
  }
  else if (*param->actual_len == SQL_DATA_AT_EXEC ||
     *param->actual_len <= SQL_LEN_DATA_AT_EXEC_OFFSET)
  {
    length= param->value_length;
    if (!(data=param->value))
      return add_to_buffer(net,to,"NULL",4);
  }
  else
  {
    data=param->buffer;
    length= *param->actual_len;
  }
  DBUG_PRINT("info",("param: %lx  ctype: %d  SqlType: %d  data: %lx  length: %d  actual_len: %d  pos_in_query: %p",
         param,param->CType,param->SqlType,data,length,
         param->actual_len ? *param->actual_len : 0,
         param->pos_in_query));

  switch (param->CType) {
  case SQL_C_BINARY:
  case SQL_C_CHAR:
    convert=1;
    break;
  case SQL_C_BIT:
  case SQL_C_TINYINT:
  case SQL_C_STINYINT:
    length=int2str((long) *((signed char*) data),buff,-10) -buff;
    data=buff;
    break;
  case SQL_C_UTINYINT:
    length=int2str((long) *((unsigned char*) data),buff,-10) -buff;
    data=buff;
    break;
  case SQL_C_SHORT:
  case SQL_C_SSHORT:
    length=int2str((long) *((short int*) data),buff,-10) -buff;
    data=buff;
    break;
  case SQL_C_USHORT:
    length=int2str((long) *((unsigned short int*) data),buff,-10) -buff;
    data=buff;
    break;
  case SQL_C_LONG:
  case SQL_C_SLONG:
    length=int2str(*((long int*) data),buff,-10) -buff;
    data=buff;
    break;
  case SQL_C_ULONG:
    length=int2str(*((long int*) data),buff,10) -buff;
    data=buff;
    break;
  case SQL_C_SBIGINT:
    length=longlong2str(*((longlong*) data),buff, -10) - buff;
    data=buff;
    break;
  case SQL_C_UBIGINT:
    length=longlong2str(*((ulonglong*) data),buff, 10) - buff;
    data=buff;
    break;
  case SQL_C_FLOAT:
    sprintf(buff,"%.17e",*((float*) data));
    length=strlen(data=buff);
    break;
  case SQL_C_DOUBLE:
    sprintf(buff,"%.17e",*((double*) data));
    length=strlen(data=buff);
    break;
  case SQL_C_DATE:
  case SQL_C_TYPE_DATE:
  {
    DATE_STRUCT *date=(DATE_STRUCT*) data;
    sprintf(buff,"%04d%02d%02d",date->year,date->month,date->day);
    data=buff;
    length=8;
    break;
  }
  case SQL_C_TIME:
  case SQL_C_TYPE_TIME:
  {
    TIME_STRUCT *time=(TIME_STRUCT*) data;
    sprintf(buff,"%02d%02d%02d",time->hour,time->minute,time->second);
    data=buff;
    length=6;
    break;
  }
  case SQL_C_TIMESTAMP:
  case SQL_C_TYPE_TIMESTAMP:
  {
    TIMESTAMP_STRUCT *time=(TIMESTAMP_STRUCT*) data;
    sprintf(buff,"%04d%02d%02d%02d%02d%02d",time->year,time->month,time->day,
      time->hour,time->minute,time->second);
    data=buff;
    length=14;
    break;
  }
  }
  switch (param->SqlType) {
  case SQL_DATE:
  case SQL_TYPE_DATE:
  case SQL_TYPE_TIMESTAMP:
  case SQL_TIMESTAMP:
    if (data[0] == '{')       /* Of type {d date } */
      return add_to_buffer(net,to,data,length);
    /* else threat as a string */
  case SQL_CHAR:
  case SQL_VARCHAR:
  case SQL_LONGVARCHAR:
  case SQL_BINARY:
  case SQL_VARBINARY:
  case SQL_LONGVARBINARY:
  {
    *to++='\'';
    to=mysql_odbc_escape_string(mysql,
        to, (net->max_packet -
             (ulong) (to - (char*) net->buff)),
        data, length,
        (void*) net, extend_escape_buffer);
    if (to)     /* escape was ok */
    {
      *to++='\'';
    }
    return to;
  }
  case SQL_TIME:
  case SQL_TYPE_TIME:
    if (param->CType == SQL_C_TIMESTAMP ||
  param->CType == SQL_C_TYPE_TIMESTAMP)
    {
      TIMESTAMP_STRUCT *time=(TIMESTAMP_STRUCT*) param->buffer;
      sprintf(buff,"'%02d:%02d:%02d'",time->hour,time->minute,time->second);
      return add_to_buffer(net,to,buff,10);
    }
    else
    {
      ulong time=str_to_time(data,length);
      sprintf(buff,"'%02d:%02d:%02d'",time/10000,time/100%100,time%100);
      return add_to_buffer(net,to,buff,10);
    }
  case SQL_FLOAT:
  case SQL_REAL:
  case SQL_DOUBLE:
    /* If we have string -> float ; Fix locale characters for number */
    if (convert)
    {
      char *to=buff, *from=data;
      char *end=from+length;
      while (*from && from < end)
      {
  if (from[0] == thousands_sep[0] && is_prefix(from,thousands_sep))
    from+=thousands_sep_length;
  else if (from[0] == decimal_point[0] && is_prefix(from,decimal_point))
  {
    from+=decimal_point_length;
    *to++='.';
  }
  else
    *to++= *from++;
      }
      if (to == buff)
  *to++='0';      /* Fix for empty strings */
      data=buff; length=(uint) (to-buff);
    }
    /* Fall through */
  default:
    return add_to_buffer(net,to,data,length);
  }
}

/*
  @type    : myodbc3 internal
  @purpose : positioned cursor update/delete
*/

SQLRETURN do_my_pos_cursor(STMT FAR *stmt,STMT FAR *stmtNew)
{
  SQLCHAR *query = stmt->query;
  DYNAMIC_STRING dynQuery;
  SQLRETURN sqlRet;

  if (stmt->error.native_error == ER_INVALID_CURSOR_NAME)
    return (SQL_ERROR);

  while (isspace(*query)) query++;

  if (init_dynamic_string(&dynQuery, query, 1024, 1024))
    return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001);

  if (!my_casecmp(query,"delete",6))
    sqlRet = my_pos_delete(stmtNew,stmt,1,&dynQuery);

  else if (!my_casecmp(query,"update",6))
    sqlRet = my_pos_update(stmtNew,stmt,1,&dynQuery);

  else
    sqlRet = set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
            "Specified SQL syntax is not positioned update or delete",
            0);

  if (sqlRet == SQL_SUCCESS || sqlRet == SQL_SUCCESS_WITH_INFO)
    stmt->state = ST_EXECUTED;

  dynstr_free(&dynQuery);
  return(sqlRet);
}

/*
  @type    : ODBC 1.0 API
  @purpose : executes a prepared statement, using the current values
       of the parameter marker variables if any parameter markers
       exist in the statement
*/

SQLRETURN SQL_API SQLExecute(SQLHSTMT hstmt)
{
  return my_SQLExecute((STMT FAR*) hstmt);
}

/*
  @type    : myodbc3 internal
  @purpose : executes a prepared statement, using the current values
       of the parameter marker variables if any parameter markers
       exist in the statement
*/

SQLRETURN my_SQLExecute(STMT FAR* stmt)
{
  char *query;
  uint i;
  uint  nIndex;
  PARAM_BIND *param;
  STMT FAR *stmtNew = stmt;
  DBUG_ENTER("SQLExecute");
  DBUG_PRINT("enter",("stmt: %lx",stmt));

  if (!stmt)
    DBUG_RETURN(SQL_ERROR);

  if (!stmt->query)
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1010,
         "No previous SQLPrepare done",0));

  if (check_if_positioned_cursor_exists(stmt,&stmtNew))
    DBUG_RETURN(do_my_pos_cursor(stmt,stmtNew));

  for (nIndex=0 ; nIndex < stmt->param_count ; )
  {
    param = dynamic_element(&stmt->params,nIndex++,PARAM_BIND*);
    if ( param->real_param_done == FALSE && param->used == 1 )
    {
      mysql_free_result(stmt->result);
      break;
    }
  }
  if (stmt->dummy_state == ST_DUMMY_EXECUTED)
    stmt->state = ST_PREPARED;
  if (stmt->state == ST_PRE_EXECUTED)
  {
    stmt->state=ST_EXECUTED;
    DBUG_RETURN(SQL_SUCCESS);
  }
  my_SQLFreeStmt((SQLHSTMT)stmt,MYSQL_RESET_BUFFERS);
  query=stmt->query;

  if (stmt->param_count)
  {
    /*
     * If any parameters are required at execution time, cannot perform the
     * statement. It will be done throught SQLPutData() and SQLParamData().
     */
    for (i=0; i < stmt->param_count; i++)
    {
      PARAM_BIND *param=dynamic_element(&stmt->params,i,PARAM_BIND*);
      if (param->actual_len &&
    (*param->actual_len == (long) SQL_DATA_AT_EXEC ||
     *param->actual_len <= SQL_LEN_DATA_AT_EXEC_OFFSET))
      {
  stmt->current_param=i;      /* Fix by Giovanni */
  param->value=0;
  param->alloced=0;
  DBUG_RETURN(SQL_NEED_DATA);
      }
    }
    query=insert_params(stmt);      /* Checked in do_query */
  }
  DBUG_RETURN(do_query(stmt,query));
}

/*
  @type    : ODBC 1.0 API
  @purpose : executes a preparable statement, using the current values of
       the parameter marker variables if any parameters exist in the
       statement
*/

SQLRETURN SQL_API
SQLExecDirect(SQLHSTMT hstmt,SQLCHAR FAR *szSqlStr, SQLINTEGER cbSqlStr)
{
  int error;
  DBUG_ENTER("SQLExecDirect");

  if ((error=my_SQLPrepare(hstmt,szSqlStr,cbSqlStr)))
    DBUG_RETURN(error);
  DBUG_RETURN(my_SQLExecute((STMT FAR*) hstmt));
}

/*
  @type    : ODBC 1.0 API
  @purpose : returns the SQL string as modified by the driver
*/

SQLRETURN SQL_API
SQLNativeSql(SQLHDBC hdbc,
       SQLCHAR FAR *szSqlStrIn, SQLINTEGER cbSqlStrIn,
       SQLCHAR FAR *szSqlStr, SQLINTEGER cbSqlStrMax,
       SQLINTEGER FAR *pcbSqlStr)
{
  ulong offset=0;
  DBUG_ENTER("SQLNativeSql");
  DBUG_RETURN(copy_lresult((DBC FAR*) hdbc,szSqlStr,cbSqlStrMax,pcbSqlStr,
         szSqlStrIn, cbSqlStrIn,0L,0L,&offset,0));
}

/*
  @type    : ODBC 1.0 API
  @purpose : is used in conjunction with SQLPutData to supply parameter
       data at statement execution time
*/

SQLRETURN SQL_API SQLParamData(SQLHSTMT hstmt, SQLPOINTER FAR *prbgValue)
{
  STMT FAR *stmt=(STMT FAR*) hstmt;
  uint i;
  DBUG_ENTER("SQLParamData");
  for (i=stmt->current_param; i < stmt->param_count; i++)
  {
    PARAM_BIND *param=dynamic_element(&stmt->params,i,PARAM_BIND*);
    if (param->actual_len &&
  (*param->actual_len == (long) SQL_DATA_AT_EXEC ||
   *param->actual_len <= SQL_LEN_DATA_AT_EXEC_OFFSET))
    {
      stmt->current_param=i+1;
      if (prbgValue)
  *prbgValue= param->buffer;
      param->value=0;
      param->alloced=0;
      DBUG_RETURN(SQL_NEED_DATA);
    }
  }
  DBUG_RETURN(do_query(stmt,insert_params(stmt)));
}

/*
  @type    : ODBC 1.0 API
  @purpose : allows an application to send data for a parameter or column to
       the driver at statement execution time. This function can be used
       to send character or binary data values in parts to a column with
       a character, binary, or data source specific data type.
*/

SQLRETURN SQL_API
SQLPutData(SQLHSTMT hstmt, SQLPOINTER rgbValue,
     SQLINTEGER cbValue)
{
  STMT FAR *stmt=(STMT FAR*) hstmt;
  PARAM_BIND *param;
  DBUG_ENTER("SQLPutData");

  if (!stmt)
    DBUG_RETURN(SQL_ERROR);

  if (cbValue == SQL_NTS)
    cbValue=strlen(rgbValue);
  param=dynamic_element(&stmt->params,stmt->current_param-1,PARAM_BIND*);
  if (cbValue == SQL_NULL_DATA)
  {
    if (param->alloced)
      my_free(param->value,MYF(0));
    param->alloced=0;
    param->value=0;
    DBUG_RETURN(SQL_SUCCESS);
  }
  if (param->value)
  {           /* Append to old value */
    if (param->alloced)
    {
      if (!(param->value=my_realloc(param->value,param->value_length+cbValue+1,
                                    MYF(0))))
  DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,
             4001));
    }
    else
    {     /* This should never happen */
      gptr old_pos=param->value;
      if (!(param->value=my_malloc(param->value_length+cbValue+1,MYF(0))))
  DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,
             4001));
      memcpy(param->value,old_pos,param->value_length);
    }
    memcpy(param->value+param->value_length,rgbValue,cbValue);
    param->value_length+=cbValue;
    param->value[param->value_length]=0;
    param->alloced=1;
  }
  else
  {       /* New value */
    if (!(param->value=my_malloc(cbValue+1,MYF(0))))
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,
           4001));
    memcpy(param->value,rgbValue,cbValue);
    param->value_length=cbValue;
    param->value[param->value_length]=0;
    param->alloced=1;
  }
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 API
  @purpose : cancels the processing on a statement
*/

SQLRETURN SQL_API SQLCancel(SQLHSTMT hstmt)
{
  DBUG_ENTER("SQLCancel");
  DBUG_RETURN(my_SQLFreeStmt(hstmt,SQL_CLOSE));
}
