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
 * CURSOR.C                                                                *
 *                                                                         *
 * @description: Client side cursor functionality handling                 *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

/*************************************************************************** 
 * The following ODBC APIs are implemented in this file:                   *
 *                                                                         *
 *   SQLSetCursorName    (ISO 92)                                          *
 *   SQLGetCursorName    (ISO 92)                                          *
 *   SQLCloseCursor      (ISO 92)                                          *
 *   SQLSetPos           (ODBC)                                            *
 *   SQLBulkOperations   (ODBC)                                            *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"
#include <locale.h>

/*
  @type    : myodbc3 internal
  @purpose : returns the table used by this query and
       ensures that all columns are from the same table
*/

static const char *find_used_table(STMT *stmt)
{
  MYSQL_FIELD  *field, *end;
  char *table_name;
  MYSQL_RES *result = stmt->result;
  DBUG_ENTER("find_used_table");

  if (stmt->table_name && stmt->table_name[0])
    DBUG_RETURN(stmt->table_name);    /* Return cached name */

  table_name=0;
  for (field=result->fields, end=field+ result->field_count;
       field < end ; field++)
  {
    if (field->table)
    {
      if (!table_name)
        table_name=field->table;
      if (strcmp(field->table, table_name))
      {
        set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
      "Can't modify a row from a statement that uses more than one table",0);
        DBUG_RETURN(NULL);
      }
    }
  }
  /*
    We have to copy the strings as we may have to re-issue the query
    while using cursors.
  */
  stmt->table_name=dupp_str(table_name,SQL_NTS);
  DBUG_RETURN(stmt->table_name);
}


/*
  @type    : myodbc internal
  @purpose : returns the previous token in the query by eating white spaces
       if we found the start of the string, return it
*/

static const char *mystr_get_prev_token(const char **query, const char *start)
{
  const char *pos= *query;
  do
  {
    if (pos == start)
      return (*query=start);      /* Return start of string */
  }
  while (!isspace(*(--pos))) ;
  *query=pos;         /* Remember pos to space */
  return (pos+1);       /* Return found token */
}


/*
  @type    : myodbc3 internal
  @purpose : checks whether the SQL statement contains WHERE CURRENT OF CURSOR
*/

my_bool check_if_positioned_cursor_exists(STMT FAR *stmt,STMT FAR **stmtNew)
{
  if (stmt->query && stmt->query_end)
  {
    char *szQueryEnd = stmt->query_end;
    char *szQuery = stmt->query;
    const char *szCursor = mystr_get_prev_token((const char **)&szQueryEnd,
                                                stmt->query);

    if (!my_casecmp(mystr_get_prev_token((const char **)&szQueryEnd,
                                         stmt->query),"OF",2) &&
        !my_casecmp(mystr_get_prev_token((const char **)&szQueryEnd,
                                         stmt->query),"CURRENT",7) &&
        !my_casecmp(mystr_get_prev_token((const char **)&szQueryEnd,
                                         stmt->query),"WHERE",5))
    {
      LIST *list_element,*next_element;
      DBC FAR *dbc=(DBC FAR*) stmt->dbc;

      for (list_element=dbc->statements ; list_element ;
           list_element=next_element)
      {
        next_element=list_element->next;
        *stmtNew = (HSTMT)list_element->data;

        /*
          Might have the cursor in the stmt without any resultset, so
          avoid crashes, by keeping check on (*stmtNew)->result)
        */
        if ((*stmtNew)->cursor.name &&
            !my_strcasecmp((*stmtNew)->cursor.name,szCursor) &&
            (*stmtNew)->result)
        {
          *szQueryEnd= '\0';
          return(TRUE);
        }
      }
      if (!list_element)
      {
        char buff[100];
        strxmov(buff,"Cursor '",szCursor,"' does not exist",NullS);
        set_stmt_error(stmt,"3400",buff,ER_INVALID_CURSOR_NAME);
      }
      return(TRUE);
    }
  }
  return(FALSE);
}

/*
  @type    : myodbc3 internal
  @purpose : checks whether the Primary Key column exists in the table
       if it exists, returns the PK column name
*/

static SQLRETURN check_if_pk_exists(STMT FAR *stmt)
{
  char buff[NAME_LEN+18];
  MYSQL_ROW row;
  MYSQL_RES *presult;

  if (stmt->cursor.pk_validated)
    return (stmt->cursor.pk_count);

  /*
    Check for the existence of keys in the table
    We quote the table name to allow weird table names.
    TODO: Write a table-name-quote function and use this instead.
  */
  strxmov(buff,"show keys from `",stmt->result->fields->table,"`",NullS);
  pthread_mutex_lock(&stmt->dbc->lock);
  if (mysql_query(&stmt->dbc->mysql,buff) ||
      !(presult=mysql_store_result(&stmt->dbc->mysql)))
  {
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
         mysql_error(&stmt->dbc->mysql),
         mysql_errno(&stmt->dbc->mysql));
    pthread_mutex_unlock(&stmt->dbc->lock);
    return(0);
  }
  pthread_mutex_unlock(&stmt->dbc->lock);

  /*
    TODO: Fix this loop to only return columns that are part of the
    primary key.
  */
  while ((row = mysql_fetch_row(presult)) &&          
         (stmt->cursor.pk_count < MY_MAX_PK_PARTS))
  {
    /* 
       Collect all keys, it may be 
         - PRIMARY or
         - UNIQUE NOT NULL
       TODO: In the future, fix this separatly and use the priority..
    */
    strmov(stmt->cursor.pkcol[stmt->cursor.pk_count++].name,row[4]);
  }
  mysql_free_result(presult);
  stmt->cursor.pk_validated = 1;
  return(stmt->cursor.pk_count);
}


/*
  @type    : myodbc3 internal
  @purpose : positions the data cursor to appropriate row
*/

void set_current_cursor_data(STMT FAR *stmt,SQLUINTEGER irow)
{
  SQLINTEGER nrow, row_pos;
  MYSQL_RES  *result=stmt->result;
  MYSQL_ROWS *dcursor = result->data->data;

  /*
    If irow exists, then position the current row to point
    to the rowsetsize+irow, this is needed for positioned
    calls
  */
  row_pos = irow ? (SQLINTEGER) (stmt->current_row+irow-1) : stmt->current_row;
  if (stmt->cursor_row != row_pos )
  {
    for (nrow=0; nrow < row_pos; nrow++)
      dcursor = dcursor->next;

    stmt->cursor_row = row_pos;
    result->data_cursor = dcursor;
  }
}


/*
  @type    : myodbc3 internal
  @purpose : sets the dynamic cursor, when the cursor is not set
       explicitly by the application
*/

static void set_dynamic_cursor_name(STMT FAR *stmt)
{
  stmt->cursor.name = (char*) my_malloc(MYSQL_MAX_CURSOR_LEN,MYF(MY_ZEROFILL));
  sprintf((char*) stmt->cursor.name,"SQL_CUR%d",stmt->dbc->cursor_count++);
}


/*
  @type    : myodbc3 internal
  @purpose : updates the stmt status information
*/

static SQLRETURN update_status(STMT FAR *stmt,SQLUSMALLINT irow,
             SQLUSMALLINT status)
{
  if (stmt->affected_rows == 0)
    return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_01S03,NULL,0);

  else if (stmt->affected_rows > 1)
    return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_01S04,NULL,0);

  else if (stmt->stmt_options.rowStatusPtr)
  {
    SQLUSMALLINT *ptr=stmt->stmt_options.rowStatusPtr+stmt->current_row;
    SQLUSMALLINT *end=ptr+stmt->affected_rows;
    for ( ; ptr != end ; ptr++)
      *ptr=status;
  }
  return(SQL_SUCCESS);
}


/*
  @type    : myodbc3 internal
  @purpose : copy row buffers to statement
*/

SQLRETURN copy_rowdata(STMT FAR *stmt, PARAM_BIND  param,
           NET **net, SQLCHAR **to)
{
  SQLCHAR *orig_to = *to;
  MYSQL mysql = stmt->dbc->mysql;   
  SQLUINTEGER length = *(param.actual_len)+1;

  if (!(*to=extend_buffer(*net,*to,length)))
    return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001);
  
  if (!(*to = insert_param(&mysql, *to, &param)))
    return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001);

  /* We have to remove zero bytes or we have problems! */
  while ((*to > orig_to) && (*((*to) - 1) == (SQLCHAR)0)) (*to)--;

  /* insert "," */
  param.SqlType = SQL_INTEGER;
  param.CType = SQL_C_CHAR;
  param.buffer = (SQLCHAR *) ",";
  *param.actual_len = 1;
  
  if (!(*to = insert_param(&mysql,*to, &param)))
    return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001);

  return(SQL_SUCCESS);
}


/*
  @type    : myodbc3 internal
  @purpose : executes a statement query
*/

static SQLRETURN exec_stmt_query(STMT FAR *stmt,SQLCHAR *query,
         SQLUINTEGER len)
{
  DBC FAR *dbc=stmt->dbc;
  SQLRETURN error = SQL_SUCCESS;

  pthread_mutex_lock(&dbc->lock);
  if (check_if_server_is_alive(dbc) ||
      mysql_real_query(&dbc->mysql,query,len))
  {
    error = set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
           mysql_error(&dbc->mysql),
           mysql_errno(&dbc->mysql));
  }
  pthread_mutex_unlock(&dbc->lock);
  return(error);
}


/*
  @type    : myodbc3 internal
  @purpose : copy row buffers to statement
*/

static SQLRETURN copy_field_data(STMT FAR *stmt, PARAM_BIND  param,
         NET **net, SQLCHAR **to)
{
  MYSQL mysql = stmt->dbc->mysql;   
  SQLUINTEGER length = *(param.actual_len)+5;

  if (!(*to=extend_buffer(*net,*to,length)))
    return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001);

  if (!(*to = insert_param(&mysql, *to, &param)))
    return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001);

  /* Insert " AND ", where clause with multiple search */
  param.SqlType = SQL_INTEGER;
  param.CType = SQL_C_CHAR;
  param.buffer = (SQLCHAR *)" AND ";
  *param.actual_len = 5;

  if (!(*to = insert_param(&mysql, *to, &param)))
    return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001);
  
  return(SQL_SUCCESS);
}


/*
  @type    : myodbc3 internal
  @purpose : copies field data to statement
*/

SQLRETURN insert_field(STMT FAR *stmt, MYSQL_RES *result,
           DYNAMIC_STRING *dynQuery, SQLUSMALLINT nSrcCol)
{
  PARAM_BIND  param;
  ulong       transfer_length,precision,display_size;
  MYSQL_FIELD *field = mysql_fetch_field_direct(result,nSrcCol);
  MYSQL_ROW   row_data = result->data_cursor->data + nSrcCol;
  NET         *net=&stmt->dbc->mysql.net;
  SQLCHAR     *to=net->buff;
  SQLINTEGER  length;

  /* Copy row buffer data to statement */
  param.used= 1;
  param.SqlType = unireg_to_sql_datatype(stmt,field,0,
           &transfer_length,
           &precision,
           &display_size);
  param.CType = SQL_C_CHAR;

  if (row_data && *row_data)
  {
    param.buffer = (SQLCHAR *) *row_data;
    length = strlen(*row_data);
    
    param.actual_len = &length; 

    if (copy_field_data(stmt,param,&net,&to) != SQL_SUCCESS)
      return(SQL_ERROR);

    length = (uint) ((char *)to - (char*) net->buff);
    dynstr_append_mem(dynQuery, net->buff, length);
  }
  else 
  {
    dynQuery->length--;
    dynstr_append_mem(dynQuery, " IS NULL AND ",13);
  }  
  return(SQL_SUCCESS);
}


/*
  @type    : myodbc3 internal
  @purpose : checks for the existance of pk columns in the resultset,
       if it is, copy that data to query, else get the data by
       building a temporary resultset
*/

SQLRETURN insert_pk_fields(STMT FAR *stmt, DYNAMIC_STRING *dynQuery)
{
  MYSQL_RES   *result = stmt->result;
  MYSQL_FIELD *field;
  SQLUSMALLINT ncol;
  SQLUSMALLINT index;
  MYCURSOR     *cursor = &stmt->cursor;
  SQLUINTEGER  pk_count=0;

  /*
    Look for primary key columns in the current result set,
    if it exists, take that data else query new resultset
  */
  for (ncol = 0; ncol < result->field_count; ncol++)
  {
    field=result->fields+ncol;
    for (index=0; index < cursor->pk_count; index++)
    {
      if (!my_strcasecmp(cursor->pkcol[index].name,field->name))
      {
        /* PK data exists...*/
        dynstr_append(dynQuery,field->name);
        dynstr_append_mem(dynQuery,"=",1);
        if (insert_field(stmt,result,dynQuery,ncol) != SQL_SUCCESS)
          return(SQL_ERROR);
        cursor->pkcol[index].bind_done = true;
        pk_count++;
        break;
      }
    }
  }
  if (pk_count != cursor->pk_count)
  {
    /* Primary key column doesn't exists in the opened rs, so
       get the data by executing a query
    */
    DYNAMIC_STRING query;
    my_bool pk_not_in_set = false;
    MYSQL_RES *presult;
    SQLUSMALLINT field_count=0;

    if (init_dynamic_string(&query, "SELECT ", 1024,1024))
      return set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,4001);

    for (index=0; index < cursor->pk_count; index++)
    {
      if (!cursor->pkcol[index].bind_done)
      {
        dynstr_append(&query,stmt->cursor.pkcol[index].name);
        dynstr_append_mem(&query,",",1);
      }
    }
    query.length-=1;
    dynstr_append_mem(&query," FROM ",6);

    if (!find_used_table(stmt))
    {
      dynstr_free(&query);
      return(SQL_ERROR);
    }

    dynstr_append(&query,stmt->table_name);

    pthread_mutex_lock(&stmt->dbc->lock);
    if (mysql_query(&stmt->dbc->mysql,query.str) ||
        !(presult=mysql_store_result(&stmt->dbc->mysql)))
    {
      set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,mysql_error(&stmt->dbc->mysql),
           mysql_errno(&stmt->dbc->mysql));
      pthread_mutex_unlock(&stmt->dbc->lock);
      dynstr_free(&query);
      return(SQL_ERROR);
    }
    pthread_mutex_unlock(&stmt->dbc->lock);

    for (index=0;index<stmt->current_row;index++)
      presult->data_cursor = presult->data_cursor->next;

    for (index=0; index < cursor->pk_count; index++)
    {
      if (!cursor->pkcol[index].bind_done)
      {
        dynstr_append(dynQuery,cursor->pkcol[index].name);
        dynstr_append_mem(dynQuery,"=",1);

        /* might have multiple pk fields in the missing list ..
           so avoid the wrong query by having internal field_count..
        */
        if (insert_field(stmt,presult,dynQuery,field_count++) != SQL_SUCCESS)
        {
          mysql_free_result(presult);
          dynstr_free(&query);
          return(SQL_ERROR);
        }
      }
    }
    mysql_free_result(presult);
    dynstr_free(&query);
  }
  return(SQL_SUCCESS);
}


/*
  @type    : myodbc3 internal
  @purpose : copies all resultset column data to where clause
*/

SQLRETURN insert_fields(STMT FAR *stmt, DYNAMIC_STRING *dynQuery)
{
  MYSQL_RES    *result = stmt->result;
  MYSQL_FIELD  *field;
  SQLUSMALLINT ncol;  
  SQLCHAR      select[NAME_LEN+15];
  MYSQL_RES    *presult;

  /* As there is no key columns in the current result, 
     try to include all columns from the table in the 
     search clause 
  */
  if(!(find_used_table(stmt)))
    return(SQL_ERROR);
    
  /* Get a temp result of all columns from the table .. */
  strxmov(select,"SELECT * FROM ",stmt->table_name,"",NullS);

  pthread_mutex_lock(&stmt->dbc->lock);
  if((mysql_query(&stmt->dbc->mysql,select) ||
     !(presult=mysql_store_result(&stmt->dbc->mysql))))
  {
    set_handle_error(3,stmt,MYERR_S1000,mysql_error(&stmt->dbc->mysql),
       mysql_errno(&stmt->dbc->mysql));
    pthread_mutex_unlock(&stmt->dbc->lock);  
    return(SQL_ERROR);
  }
  pthread_mutex_unlock(&stmt->dbc->lock);       

  /* If current result set field count is not the total 
     count from the actual table, then use the temp result 
     to have a search condition from all table fields ..

     this can be buggy, if multiple times the same 
     column is used in the select ..rare case ..
  */
  if(presult->field_count != result->field_count)
  {    
    for(ncol=0; ncol<(SQLUSMALLINT)stmt->current_row; ncol++)
      presult->data_cursor = presult->data_cursor->next;  

    result = presult;
  }  
  /* Copy all row buffers to query search clause */
  for (ncol = 0; ncol < result->field_count; ncol++)
  {
    field=result->fields+ncol;
    dynstr_append(dynQuery,field->name);
    dynstr_append_mem(dynQuery,"=",1);

    if(insert_field(stmt,result,dynQuery,ncol) != SQL_SUCCESS) 
    {
      mysql_free_result(presult);
      return(SQL_ERROR);
    }
  }
  mysql_free_result(presult);
  return(SQL_SUCCESS);
}

/*
 @type    : myodbc3 internal
 @purpose : build the where clause
*/

SQLRETURN build_where_clause(STMT FAR *stmt, DYNAMIC_STRING *dynQuery,
           SQLUSMALLINT irow)
{
  set_current_cursor_data(stmt,irow);
  dynstr_append_mem(dynQuery," WHERE ",7);

  if (check_if_pk_exists(stmt))
  {
    /* Primary keys exists, build the search pattern using
       keys
    */
    if (insert_pk_fields(stmt, dynQuery) != SQL_SUCCESS)
      return(SQL_ERROR);
  }
  else
  {
    /* No primary key exists, build the search pattern using
       all current open columns
    */
    if (insert_fields(stmt, dynQuery) != SQL_SUCCESS)
      return(SQL_ERROR);
  }
  dynQuery->length -= 5; 

  /* If irow=0, include search clause for all rows,
     else for all current rowset size
  */
  if (irow) 
  {
    dynstr_append_mem(dynQuery," LIMIT 1",8);
  }
  else
  {
    char buff[32];
    sprintf(buff," LIMIT %d",stmt->stmt_options.rows_in_set);
    dynstr_append(dynQuery,buff);
  }
  return(SQL_SUCCESS);
}

/*
 @type    : myodbc3 internal
 @purpose : if input param buffers exist, copy them to new
      statement
*/

static void copy_input_param(STMT FAR *stmt,STMT FAR *stmtNew,
           SQLUINTEGER pcount)
{
  while (pcount--)
  {
    PARAM_BIND *param=dynamic_element(&stmt->params,pcount,PARAM_BIND*);
    PARAM_BIND *paramNew=dynamic_element(&stmtNew->params,pcount,PARAM_BIND*);
    param->pos_in_query = paramNew->pos_in_query;
    set_dynamic(&stmtNew->params,(gptr) param,pcount);
  }
}

/*
  @type    : myodbc3 internal
  @purpose : set clause building..
*/

SQLRETURN build_set_clause(STMT FAR *stmt, SQLUINTEGER irow,
         DYNAMIC_STRING *dynQuery)
{
  PARAM_BIND  param;
  SQLUINTEGER transfer_length,precision,display_size;
  SQLUINTEGER length, ncol, ignore_count=0;
  MYSQL_FIELD *field;
  MYSQL_RES   *result=stmt->result;
  BIND        *bind;
  NET         *net=&stmt->dbc->mysql.net;
  SQLINTEGER  *pcbValue;

  dynstr_append_mem(dynQuery," SET ",5);

  /*
    To make sure, it points to correct row in the
    current rowset..
  */
  irow = irow ? irow-1: 0;
  for (ncol = 0; ncol < stmt->result->field_count; ncol++)
  {
    SQLCHAR *to=net->buff;
    field = mysql_fetch_field_direct(result,ncol);
    bind = stmt->bind+ncol;

    if (bind && !bind->field ){
      set_stmt_error(stmt,"21S02",
         "Degree of derived table does not match column list",0);
      return(SQL_ERROR);
    }
    pcbValue = bind->pcbValue + irow;
    if(pcbValue)
    {
      /*
        If the pcbValue is SQL_COLUMN_IGNORE, then ignore the
        column in the SET clause
      */
      if (*pcbValue == SQL_COLUMN_IGNORE)
      {
        ignore_count++;
        continue;
      }
      /*
        Take care of SQL_NTS in pcbValue
      */
      else if(*pcbValue == SQL_NTS)
	length = SQL_NTS;

      else length = *pcbValue;
    } 
    else length = SQL_NTS;

    /* TODO : handle ..SQL_DATA_AT_EXEC here....*/

    dynstr_append_quoted_name(dynQuery,field->name);
    dynstr_append_mem(dynQuery,"=",1);

    param.used= 1;
    param.SqlType = unireg_to_sql_datatype(stmt,field,0,
             &transfer_length,&precision,
             &display_size);
    param.CType = bind->fCType;
    param.buffer = (gptr)bind->rgbValue+irow*bind->cbValueMax;
    param.ValueMax = bind->cbValueMax;
    length= (length == SQL_NTS) ? strlen(param.buffer):length;
    param.actual_len= &length;  
    
    if (copy_rowdata(stmt,param,&net,&to) != SQL_SUCCESS)
      return(SQL_ERROR);

    length = (uint) ((char *)to - (char*) net->buff);
    dynstr_append_mem(dynQuery, net->buff, length);
  }
  if (ignore_count == result->field_count)
    return(ER_ALL_COLUMNS_IGNORED);

  dynQuery->str[--dynQuery->length]='\0';      
  return(SQL_SUCCESS);
}

/*
  @type    : myodbc3 internal
  @purpose : deletes the positioned cursor row
*/

SQLRETURN my_pos_delete(STMT FAR *stmt, STMT FAR *stmtParam,
      SQLUSMALLINT irow, DYNAMIC_STRING *dynQuery)
{
  SQLRETURN    sqlRet;

  /* Delete only the positioned row, by building where clause */
  if (build_where_clause(stmt, dynQuery, irow) != SQL_SUCCESS)
    return(SQL_ERROR);

  /* DELETE the row(s) */
  DBUG_PRINT("SQL_DELETE:",("%s",dynQuery->str));
  sqlRet = exec_stmt_query(stmt, dynQuery->str, dynQuery->length);
  if (sqlRet == SQL_SUCCESS || sqlRet == SQL_SUCCESS_WITH_INFO)
  {
    stmtParam->affected_rows=mysql_affected_rows(&stmt->dbc->mysql);
    sqlRet = update_status(stmtParam,irow,SQL_ROW_DELETED);
  }
  return(sqlRet);
}

/*
  @type    : myodbc3 internal
  @purpose : updates the positioned cursor row
*/

SQLRETURN my_pos_update(STMT FAR *stmt, STMT FAR *stmtParam,
      SQLUSMALLINT irow, DYNAMIC_STRING *dynQuery)
{
  SQLRETURN sqlRet;
  SQLHSTMT  hstmtTemp;
  STMT FAR  *stmtTemp;

  if (build_where_clause(stmt, dynQuery, irow) != SQL_SUCCESS)
    return(SQL_ERROR);

  /*
    Prepare and check if parameters exists in set clause..
    this happens with WHERE CURRENT OF statements ..
  */

  if (my_SQLAllocStmt(stmtParam->dbc, &hstmtTemp) != SQL_SUCCESS)
    return(SQL_ERROR);

  stmtTemp = (STMT FAR*)hstmtTemp;

  if (my_SQLPrepare(stmtTemp,dynQuery->str,dynQuery->length) != SQL_SUCCESS)
  {
    my_SQLFreeStmt(stmtTemp,SQL_DROP);
    return(SQL_ERROR);
  }
  if (stmtTemp->param_count)      /* SET clause has parameters */
    copy_input_param(stmtParam,stmtTemp,stmtTemp->param_count);

  sqlRet = my_SQLExecute(stmtTemp);
  if (sqlRet == SQL_SUCCESS || sqlRet == SQL_SUCCESS_WITH_INFO)
  {
    stmtParam->affected_rows=mysql_affected_rows(&stmtTemp->dbc->mysql);
    sqlRet = update_status(stmtParam,irow,SQL_ROW_UPDATED);
  }
  my_SQLFreeStmt(stmtTemp,SQL_DROP);
  return(sqlRet);
}


/*
  @type    : myodbc3 internal
  @purpose : deletes or updated the positioned cursor row using
       SQLSetPos, might have multi rowset size ..
*/

SQLRETURN setpos_delete_update(STMT FAR *stmt, SQLUSMALLINT irow,
             DYNAMIC_STRING *dynQuery,
             SQLUSMALLINT fOption)
{
  SQLUINTEGER  rowset_pos,rowset_end;
  my_ulonglong affected_rows=0;
  SQLRETURN    sqlRet = SQL_SUCCESS;
  SQLUSMALLINT option_type = SQL_ROW_UPDATED;
  my_bool      updatable = true;
  ulong        query_length;
  const char   *table_name;

  if (!(table_name = find_used_table(stmt)))
    return SQL_ERROR;

  if (fOption == SQL_DELETE)
  {
    updatable = false;
    option_type = SQL_ROW_DELETED;

    dynstr_set(dynQuery, "DELETE FROM ");
  }
  else
  {
    dynstr_set(dynQuery, "UPDATE ");
  }

  dynstr_append_quoted_name(dynQuery,table_name);
  query_length=dynQuery->length;

  if (!irow)
  {
    /*
      If irow == 0, then delete or update all rows in the current rowset
    */
    rowset_pos = 1;
    rowset_end = stmt->rows_found_in_set;
  }
  else
  {
    rowset_pos= rowset_end= irow;
  }

  do /* DELETE or UPDATE, irow from current row set */
  {
    dynQuery->length=query_length;
    if (updatable)
    {
      sqlRet = build_set_clause(stmt,rowset_pos,dynQuery);
      if (sqlRet == ER_ALL_COLUMNS_IGNORED)
      {
        /* 
          All columns ignored in the update list, continue
          to the next statement ..
        */
        sqlRet = SQL_SUCCESS;
        continue;
      }
      else if (sqlRet == SQL_ERROR)
      {
        break;
      }
    }
    if ((sqlRet = build_where_clause(stmt,dynQuery,
                  (SQLUSMALLINT) rowset_pos)) != SQL_SUCCESS)
    {
      break;
    }
    DBUG_PRINT("SQLPOS_QUERY:",("%s",dynQuery->str));
    sqlRet = exec_stmt_query(stmt, dynQuery->str, dynQuery->length);
    if (sqlRet == SQL_SUCCESS || sqlRet == SQL_SUCCESS_WITH_INFO)
    {
      affected_rows += mysql_affected_rows(&stmt->dbc->mysql);
    }

  } while (++rowset_pos <= rowset_end);

  if (sqlRet == SQL_SUCCESS)
  {
    stmt->affected_rows=stmt->dbc->mysql.affected_rows=affected_rows;

    if (irow && affected_rows > 1)
      sqlRet = set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_01S04,NULL,0);

    /* If all rows successful, then only update status..else 
       don't update...just for the sake of performance..
    */
    else if (stmt->stmt_options.rowStatusPtr) 
    {
      while(affected_rows--)
        stmt->stmt_options.rowStatusPtr[affected_rows]=option_type;
    }
  }
  return(sqlRet);
}


/*
  @type    : myodbc3 internal
  @purpose : batch insert
*/

SQLRETURN batch_insert(STMT FAR *stmt, SQLUSMALLINT irow,
           DYNAMIC_STRING *ext_query)
{
  MYSQL_RES    *result = stmt->result;
  SQLUINTEGER  insert_count = 1;
  SQLUINTEGER  count=0, length;
  NET          *net;
  SQLUSMALLINT ncol;
  SQLCHAR      *to;
  ulong        query_length;
  my_bool      break_insert = false;
  MYSQL        mysql = stmt->dbc->mysql ;
  PARAM_BIND   param;

  if (!irow && stmt->stmt_options.rows_in_set > 1) /* batch wise */
  {
    insert_count = stmt->stmt_options.rows_in_set;
    query_length = ext_query->length;
  }

  do
  {
    if (break_insert)
    {
      /* 
         If query exceeded its length, then set the query 
         from begining..
      */        
      ext_query->length = query_length;
    }
    while (count < insert_count)
    {
      net = &mysql.net;
      to = net->buff;
      
      dynstr_append_mem(ext_query,"(", 1);

      for (ncol = 0; ncol < result->field_count; ncol++)
      {
        SQLUINTEGER transfer_length,precision,display_size;
        MYSQL_FIELD *field = mysql_fetch_field_direct(result,ncol);
        BIND        *bind = stmt->bind+ncol;

        param.SqlType= unireg_to_sql_datatype(stmt,field,0,
                       &transfer_length,&precision,
                       &display_size);
        param.CType  = bind->fCType;
        param.buffer = (gptr)bind->rgbValue+count*bind->cbValueMax;

        if (param.buffer)
        { 
          if(bind->pcbValue)
          {
            if(*bind->pcbValue == SQL_NTS)
              length = strlen(param.buffer);
            else length = *bind->pcbValue;
          }
          else length = bind->cbValueMax;
        }
        else 
        {
          length = SQL_NULL_DATA;
        }
        param.actual_len = &length;

        if (copy_rowdata(stmt,param,&net,&to) != SQL_SUCCESS)
        {
          return(SQL_ERROR);
        }

      } /* END OF for (ncol = 0; ncol < result->field_count; ncol++) */

      length = (uint) ((char *)to - (char*) net->buff);
      dynstr_append_mem(ext_query, net->buff, length-1);
      dynstr_append_mem(ext_query, "),", 2);
      count++;
      if (ext_query->length+length >= net_buffer_length)
      {
        break_insert = true;
        break;
      }

    }  /* END OF while(count < insert_count) */

    ext_query->str[--ext_query->length] = '\0';
    DBUG_PRINT("batch_insert:",("%s",ext_query->str));
    if (exec_stmt_query(stmt, ext_query->str, ext_query->length) != SQL_SUCCESS)
    {
      return(SQL_ERROR);
    }

  } while (break_insert && count < insert_count);

  stmt->affected_rows=stmt->dbc->mysql.affected_rows=insert_count;
  if ( stmt->stmt_options.rowStatusPtr)
  {
    for (count = insert_count; count--;)
      stmt->stmt_options.rowStatusPtr[count] = SQL_ROW_ADDED;
  }
  return(SQL_SUCCESS);
}


/*
  @type    : myodbc internal
  @purpose : sets the cursor position in a rowset and allows an application
       to refresh data in the rowset or to update or delete data in
       the result set
*/

SQLRETURN SQL_API my_SQLSetPos(SQLHSTMT hstmt,
             SQLUSMALLINT irow,
             SQLUSMALLINT fOption,
             SQLUSMALLINT fLock)
{
  STMT FAR  *stmt=(STMT FAR*) hstmt;
  SQLRETURN sqlRet=SQL_SUCCESS;
  MYSQL_RES *result = stmt->result;

  DBUG_ENTER("SQLSetPos");
  DBUG_PRINT("enter",("irow, refresh: %d   Lock: %d",irow,fOption,fLock));

  if ( !result )
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1010,NULL,0));

  /* If irow > maximum rows in the resultset */
  if ( fOption != SQL_ADD && irow > mysql_num_rows(result))
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1107,NULL,0));

  /* Not a valid lock type ..*/
  if ( fLock != SQL_LOCK_NO_CHANGE )
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1C00,NULL,0));

  switch(fOption) {

  case SQL_POSITION:
  {
    if (irow == 0)
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1109,NULL,0));

    if (irow > stmt->rows_found_in_set)
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1107,NULL,0));

    /* If Dynamic cursor, fetch the latest resultset */
    if (stmt->stmt_options.cursor_type == SQL_CURSOR_DYNAMIC &&
       (set_dynamic_result(stmt) != SQL_SUCCESS))
    {
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
           "Driver Failed to set the internal dynamic result",0));
    }

    irow--;
    sqlRet = SQL_SUCCESS;
    stmt->cursor_row = stmt->current_row+irow;
    mysql_data_seek(result,stmt->cursor_row);
    break;
  }

  case SQL_DELETE:
  case SQL_UPDATE:
  {
    DYNAMIC_STRING dynQuery;

    if (irow > stmt->rows_found_in_set)
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1107,NULL,0));

    if (stmt->stmt_options.cursor_type == SQL_CURSOR_DYNAMIC &&
       (set_dynamic_result(stmt) != SQL_SUCCESS))
    {
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
           "Driver Failed to set the internal dynamic result",0));
    }
    if (init_dynamic_string(&dynQuery, NULL, 1024, 1024))
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,
                                   MYERR_S1001,NULL,4001));

    sqlRet = setpos_delete_update(stmt,irow,&dynQuery,fOption);
    dynstr_free(&dynQuery);
    break;
  }

  case SQL_ADD:
  {
    MYSQL_FIELD *field, *end;
    const char  *table_name;
    DYNAMIC_STRING dynQuery;

    if (stmt->stmt_options.cursor_type == SQL_CURSOR_DYNAMIC &&
       (set_dynamic_result(stmt) != SQL_SUCCESS))
    {
      DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
           "Driver Failed to set the internal dynamic result",0));
    }
    if (!(table_name = find_used_table(stmt)))
      DBUG_RETURN(SQL_ERROR);

    if (init_dynamic_string(&dynQuery, "INSERT INTO ", 1024,1024))
      DBUG_RETURN(set_stmt_error(stmt,"S1001","Not enough memory",4001));

    dynstr_append_quoted_name(&dynQuery,table_name);
    dynstr_append_mem(&dynQuery,"(",1);

    for (field=result->fields, end=field+ result->field_count;
         field < end ; field++)
    {
      dynstr_append_quoted_name(&dynQuery,field->name);
      dynstr_append_mem(&dynQuery,",",1);
    }
    dynQuery.length--;        /* Remove last ',' */
    dynstr_append_mem(&dynQuery,") VALUES ",9);
    sqlRet = batch_insert(stmt, irow, &dynQuery);
    dynstr_free(&dynQuery);
    break;
  }

  case SQL_REFRESH:
  {
    /*
      Bug ...SQL_REFRESH is not suppose to fetch any
      new rows, instead it needs to refresh the positioned
      buffers
    */
    sqlRet = my_SQLExtendedFetch(hstmt, SQL_FETCH_ABSOLUTE, irow,
                                 stmt->stmt_options.rowsFetchedPtr,
                                 stmt->stmt_options.rowStatusPtr);
    break;
  }

  default:
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1009,NULL,0));
  }
  DBUG_RETURN(sqlRet);
}


/*
  @type    : ODBC 1.0 API
  @purpose : associates a cursor name with an active statement.
       If an application does not call SQLSetCursorName, the driver
       generates cursor names as needed for SQL statement processing
*/

SQLRETURN SQL_API SQLSetCursorName(SQLHSTMT    hstmt,
           SQLCHAR     *szCursor,
           SQLSMALLINT cbCursor)
{
  STMT FAR *stmt=(STMT FAR*) hstmt;
  DBUG_ENTER("SQLSetCursorName");

  if (!szCursor)
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1009,NULL,0));

  if (cbCursor == SQL_NTS)
    cbCursor = (SQLSMALLINT) strlen(szCursor);

  if (cbCursor < 0 )
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1090,NULL,0));

  if ((cbCursor == 0) || 
      (cbCursor > MYSQL_MAX_CURSOR_LEN) ||
      (my_casecmp(szCursor, "SQLCUR", 6) == 0)  ||
      (my_casecmp(szCursor, "SQL_CUR", 7) == 0))
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_34000,NULL,0));  

  x_free((gptr) stmt->cursor.name );  
  stmt->cursor.name=dupp_str(szCursor,cbCursor);
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : ODBC 1.0 API
  @purpose : returns the cursor name associated with a specified statement
*/

SQLRETURN SQL_API SQLGetCursorName(SQLHSTMT hstmt,
           SQLCHAR FAR *szCursor,
           SQLSMALLINT cbCursorMax,
           SQLSMALLINT FAR *pcbCursor)
{
  STMT FAR    *stmt=(STMT FAR*) hstmt;
  SQLINTEGER  nLength;
  SQLSMALLINT nDummyLength;
  DBUG_ENTER("SQLGetCursorName");

  if (cbCursorMax < 0)
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1090,NULL,0));

  if (!pcbCursor)
    pcbCursor = &nDummyLength;

  if (cbCursorMax)
    cbCursorMax -= sizeof(char);

  if (!stmt->cursor.name )
    set_dynamic_cursor_name(stmt);

  *pcbCursor = strlen(stmt->cursor.name);
  if (szCursor && cbCursorMax > 0)
    strmake(szCursor, stmt->cursor.name, cbCursorMax);

  nLength = min(*pcbCursor , cbCursorMax );

  if (nLength != *pcbCursor)
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_01004,NULL,0));

  DBUG_RETURN(SQL_SUCCESS);
}


/*
  @type    : ODBC 1.0 API
  @purpose : sets the cursor position in a rowset and allows an application
       to refresh data in the rowset or to update or delete data in
       the result set
*/

SQLRETURN SQL_API SQLSetPos( SQLHSTMT hstmt,
           SQLUSMALLINT irow,
           SQLUSMALLINT fOption,
           SQLUSMALLINT fLock)
{
  return my_SQLSetPos(hstmt,irow,fOption,fLock);
}

/*
  @type    : ODBC 1.0 API
  @purpose : performs bulk insertions and bulk bookmark operations,
       including update, delete, and fetch by bookmark
*/

SQLRETURN SQL_API SQLBulkOperations(SQLHSTMT  Handle,
            SQLSMALLINT Operation)
{
  DBUG_ENTER("SQLBulkOperations");

  if (Operation == SQL_ADD)
    DBUG_RETURN(my_SQLSetPos(Handle, 0, SQL_ADD, SQL_LOCK_NO_CHANGE));

  DBUG_RETURN(set_handle_error(SQL_HANDLE_DBC,Handle,MYERR_S1C00,NULL,0));
}


/*
  @type    : ODBC 3.0 API
  @purpose : closes a cursor that has been opened on a statement
       and discards any pending results
*/

SQLRETURN SQL_API SQLCloseCursor(SQLHSTMT Handle)
{
  DBUG_ENTER("SQLCloseCursor");
  DBUG_RETURN(my_SQLFreeStmt(Handle, SQL_CLOSE));
}
