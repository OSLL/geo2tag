monty@hundin.mysql.fi|cursor.c|20010620221113|28661|bd2aeb8473194747 venu@myvenu.com|cursor.c|20020215224409|08235
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
 * CATALOG.C                                                               *
 *                                                                         *
 * @description: Handling ODBC catalog APIs                                *
 *                                                                         *
 * @author     : MySQL AB (monty@mysql.com, venu@mysql.com)                *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

/*************************************************************************** 
 * The following ODBC APIs are implemented in this file:                   *
 *                                                                         *
 *   SQLColumnPrivileges (ODBC)                                            *
 *   SQLColumns          (X/Open)                                          *
 *   SQLForeignKeys      (ODBC)                                            *
 *   SQLPrimaryKeys      (ODBC)                                            *
 *   SQLProcedureColumns (ODBC)                                            *
 *   SQLProcedures       (ODBC)                                            *
 *   SQLSpecialColumns   (X/Open)                                          *
 *   SQLStatistics       (ISO 92)                                          *
 *   SQLTablePrivileges  (ODBC)                                            *
 *   SQLTables           (ODBC)                                            *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"

static MYSQL_ROW fix_fields_copy(STMT FAR *hstmt,MYSQL_ROW row);
static int check_parameters(SQLHSTMT hstmt,SQLCHAR FAR *szTableQualifier,
			    SQLSMALLINT cbTableQualifier,
			    SQLCHAR FAR *szTableOwner,
			    SQLSMALLINT cbTableOwner,SQLCHAR FAR *szTableName,
			    SQLSMALLINT *cbTableName, char *table_name,
			    bool no_wildcards);

uint SQLTABLES_order[]={2};
char *SQLTABLES_values[]={"","",NULL,"TABLE","MySQL table"};
char *SQLTABLES_qualifier_values[]={"",NULL,NULL,NULL,NULL};
char *SQLTABLES_owner_values[]={NULL,"",NULL,NULL,NULL};
char *SQLTABLES_type_values[]={NULL,NULL,NULL,"TABLE",NULL};

MYSQL_FIELD SQLTABLES_fields[] = {
  {"TABLE_CAT","Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_SCHEM","Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_NAME","Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,0},
  {"TABLE_TYPE","Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,5,0},
  {"REMARKS","Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,11}};

const uint SQLTABLES_FIELDS=array_elements(SQLTABLES_values);

/*
  @type    : ODBC 1.0 API
  @purpose : returns the list of table, catalog, or schema names, and
	     table types, stored in a specific data source. The driver
	     returns the information as a result set
*/

SQLRETURN SQL_API SQLTables(SQLHSTMT hstmt,
			    SQLCHAR FAR *szTableQualifier,
			    SQLSMALLINT cbTableQualifier,
			    SQLCHAR FAR *szTableOwner,SQLSMALLINT cbTableOwner,
			    SQLCHAR FAR *szTableName,SQLSMALLINT cbTableName,
			    SQLCHAR FAR *szTableType,SQLSMALLINT cbTableType)
{
  char Qualifier_buff[NAME_LEN+1],Owner_buff[NAME_LEN+1],Name_buff[NAME_LEN+1],
       *type_buff,*TableQualifier,*TableOwner,*TableName,*TableType;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  DBUG_ENTER("SQLTables");
  DBUG_PRINT("enter",("Qualifier: '%s'  Owner: '%s'  Table: '%s'  Type: '%s'",
		      szTableQualifier ? (char*) szTableQualifier : "null",
		      szTableOwner ? (char*) szTableOwner : "null",
		      szTableName ? (char*) szTableName : "null",
		      szTableType ? (char*) szTableType : "null"));

  my_SQLFreeStmt(hstmt,MYSQL_RESET);
  if (!(type_buff=my_malloc(szTableType && cbTableType != SQL_NTS ?
			    cbTableType+1 : 1,MYF(0))))
  {
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1001,NULL,0));
  }

  TableQualifier=fix_str((char FAR *) Qualifier_buff,szTableQualifier,
			 cbTableQualifier);
  TableOwner=	 fix_str((char FAR*) Owner_buff,szTableOwner,cbTableOwner);
  TableName=	 fix_str((char FAR*) Name_buff,szTableName,cbTableName);
  TableType=	 fix_str((char FAR*) type_buff,szTableType,cbTableType);

  if (!strcmp(TableQualifier,"%") && !TableOwner[0] && !TableName[0])
  {
    /* Return set of allowed qualifiers */
    DBUG_PRINT("info",("Return set of table qualifiers / Catalog names"));
    stmt->result=(MYSQL_RES*) my_malloc(sizeof(MYSQL_RES),MYF(MY_ZEROFILL));
    stmt->result_array=(MYSQL_ROW) my_memdup((gptr) SQLTABLES_qualifier_values,
					    sizeof(SQLTABLES_qualifier_values),
					    MYF(0));
    stmt->result->row_count=1;
    mysql_link_fields(stmt,SQLTABLES_fields, SQLTABLES_FIELDS);
    my_free(type_buff,MYF(0));
    DBUG_RETURN(SQL_SUCCESS);
  }

  if (!TableQualifier[0] && !strcmp(TableOwner,"%") && !TableName[0])
  {
    /* Return set of allowed Table owners */
    DBUG_PRINT("info",("Return set of table owners / Schema names"));
    stmt->result=(MYSQL_RES*) my_malloc(sizeof(MYSQL_RES),MYF(MY_ZEROFILL));
    stmt->result_array=(MYSQL_ROW) my_memdup((gptr) SQLTABLES_owner_values,
					      sizeof(SQLTABLES_owner_values),
					      MYF(0));
    stmt->result->row_count=1;
    mysql_link_fields(stmt,SQLTABLES_fields, SQLTABLES_FIELDS);
    my_free(type_buff,MYF(0));
    DBUG_RETURN(SQL_SUCCESS);
  }

  if (!TableQualifier[0] && !TableOwner[0] && !TableName[0] &&
      !strcmp(TableType,"%"))
  {
    /* Return set of TableType qualifiers */
    DBUG_PRINT("info",("Return set of table types"));
    stmt->result=(MYSQL_RES*) my_malloc(sizeof(MYSQL_RES),MYF(MY_ZEROFILL));
    stmt->result_array=(MYSQL_ROW) my_memdup((gptr) SQLTABLES_type_values,
					     sizeof(SQLTABLES_type_values),
					     MYF(0));
    stmt->result->row_count=1;
    mysql_link_fields(stmt,SQLTABLES_fields, SQLTABLES_FIELDS);
    my_free(type_buff,MYF(0));
    DBUG_RETURN(SQL_SUCCESS);
  }

  /* Return empty set if unknown TableType or if Qualifier or Owner is used */

  if ((TableType[0] && !strinstr(TableType,"TABLE")) ||
      TableQualifier[0] && strcmp(TableQualifier,"%") &&
      strcmp(TableQualifier,stmt->dbc->database) ||
      TableOwner[0] && strcmp(TableOwner,"%") &&
      strcmp(TableOwner,stmt->dbc->database))
  {
    DBUG_PRINT("info",("Can't match anything; Returning empty set"));
    stmt->result=(MYSQL_RES*) my_malloc(sizeof(MYSQL_RES),MYF(MY_ZEROFILL));
    stmt->result->row_count=0;
    stmt->result_array=(MYSQL_ROW) my_memdup((gptr) SQLTABLES_values,
					    sizeof(SQLTABLES_values), MYF(0));
    mysql_link_fields(stmt,SQLTABLES_fields, SQLTABLES_FIELDS);
    my_free(type_buff,MYF(0));
    DBUG_RETURN(SQL_SUCCESS);
  }

  /* This was a normal request for tables.  Return matching tables */

  pthread_mutex_lock(&stmt->dbc->lock);
  stmt->result=mysql_list_tables(&stmt->dbc->mysql,TableName);
  pthread_mutex_unlock(&stmt->dbc->lock);
  if (!stmt->result)
  {
    my_free(type_buff,MYF(0));
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
				 "Could not read table names",0));
  }
  stmt->order=	     SQLTABLES_order;
  stmt->order_count= array_elements(SQLTABLES_order);
  stmt->fix_fields=  fix_fields_copy;
  stmt->array=(MYSQL_ROW) my_memdup((gptr) SQLTABLES_values,
				    sizeof(SQLTABLES_values),MYF(0));

  if (stmt->dbc->flag & FLAG_NO_CATALOG)
  {
    stmt->array[0]="user";
    stmt->array[1]="user";
  }
  mysql_link_fields(stmt,SQLTABLES_fields,5);
  my_free(type_buff,MYF(0));
  DBUG_RETURN(SQL_SUCCESS);
}


static MYSQL_ROW fix_fields_copy(STMT FAR *stmt,MYSQL_ROW row)
{
  uint i;
  for (i=0 ; i < stmt->order_count; i++)
    stmt->array[stmt->order[i]]=row[i];
  return stmt->array;
}


/*  Have DBMS set up result set of Columns.*/

char SC_type[10],SC_typename[20],SC_precision[10],SC_length[10],SC_scale[10],
  SC_nullable[10], SC_coldef[10], SC_sqltype[10],SC_octlen[10],
  SC_pos[10],SC_isnull[10];

char *SQLCOLUMNS_values[]= {
  "","",NULL,NULL,SC_type,SC_typename,
  SC_precision,
  SC_length,SC_scale,"10",SC_nullable,"MySQL column",
  SC_coldef,SC_sqltype,NULL,SC_octlen,NULL,SC_isnull
};

MYSQL_FIELD SQLCOLUMNS_fields[] = {
  {"TABLE_CAT","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_SCHEM","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_NAME","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"COLUMN_NAME","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"DATA_TYPE","MySQL_Catalog",NULL,FIELD_TYPE_SHORT,5,5,NOT_NULL_FLAG},
  {"TYPE_NAME","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,20,20,NOT_NULL_FLAG},
  {"COLUMN_SIZE","MySQL_Catalog",NULL,FIELD_TYPE_LONG,11,11},
  {"BUFFER_LENGTH","MySQL_Catalog",NULL,FIELD_TYPE_LONG,11,11},
  {"DECIMAL_DIGITS","MySQL_Catalog",NULL,FIELD_TYPE_SHORT,2,2},
  {"NUM_PREC_RADIX","MySQL_Catalog",NULL,FIELD_TYPE_SHORT,2,2},
  {"NULLABLE","MySQL_Catalog",NULL,FIELD_TYPE_SHORT,5,5,NOT_NULL_FLAG},
  {"REMARKS","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN},  
  {"COLUMN_DEF","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN},
  {"SQL_DATA_TYPE","MySQL_Catalog",NULL,FIELD_TYPE_SHORT,5,5,NOT_NULL_FLAG},
  {"SQL_DATTIME_SUB","MySQL_Catalog",NULL,FIELD_TYPE_SHORT,2,2},
  {"CHAR_OCTET_LENGTH","MySQL_Catalog",NULL,FIELD_TYPE_LONG,11,11},
  {"ORDINAL_POSITION","MySQL_Catalog",NULL,FIELD_TYPE_LONG,11,11,NOT_NULL_FLAG},
  {"IS_NULLABLE","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,3,3}};

const uint SQLCOLUMNS_FIELDS=array_elements(SQLCOLUMNS_values);

/*
  @type    : ODBC 1.0 API
  @purpose : returns the list of column names in specified tables.
	     The driver returns this information as a result set on the
	     specified StatementHandle
*/

SQLRETURN SQL_API SQLColumns(SQLHSTMT hstmt,
			     SQLCHAR FAR *szTableQualifier,
			     SQLSMALLINT cbTableQualifier,
			     SQLCHAR FAR *szTableOwner,
			     SQLSMALLINT cbTableOwner,
			     SQLCHAR FAR *szTableName, SQLSMALLINT cbTableName,
			     SQLCHAR FAR *szColumnName,
			     SQLSMALLINT cbColumnName)
{
  char buff[80],table_name[NAME_LEN+1],column_name[NAME_LEN+1];
  ulong transfer_length,precision,display_size;
  MYSQL_FIELD *curField;
  char **row;
  MEM_ROOT *alloc;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  DBUG_ENTER("SQLColumns");

  if (check_parameters(hstmt,szTableQualifier,cbTableQualifier,szTableOwner,
		       cbTableOwner,szTableName,&cbTableName,table_name,1))
    return SQL_ERROR;
  pthread_mutex_lock(&stmt->dbc->lock);
  stmt->result=mysql_list_fields(&stmt->dbc->mysql,table_name,
				 fix_str(column_name,szColumnName,
				 cbColumnName));
  if (!stmt->result)
  {
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
		     mysql_error(&stmt->dbc->mysql),
		     mysql_errno(&stmt->dbc->mysql));
    pthread_mutex_unlock(&stmt->dbc->lock);
    DBUG_RETURN(SQL_ERROR);
  }
  pthread_mutex_unlock(&stmt->dbc->lock);
  stmt->result_array= (char**) my_malloc(sizeof(char*)*SQLCOLUMNS_FIELDS*
					 stmt->result->field_count,
					 MYF(MY_FAE | MY_ZEROFILL));

  /* Convert mysql fields to data that odbc wants */
  alloc=&stmt->result->field_alloc;

  for (row= stmt->result_array ;
       (curField = mysql_fetch_field(stmt->result)) ; )
  {
    int type;
    row[0]="";		      /* No qualifers */
    row[1]="";		      /* No owner */
    row[2]= curField->table;
    row[3]= curField->name;
    curField->max_length=curField->length;
    type=unireg_to_sql_datatype(stmt,curField,buff,&transfer_length,&precision,&display_size);
    row[5]=strdup_root(alloc,buff);
    sprintf(buff,"%d",type);
    row[13]=row[4]=strdup_root(alloc,buff);
    sprintf(buff,"%ld",precision);
    row[6]=strdup_root(alloc,buff);
    sprintf(buff,"%ld",transfer_length);
    row[7]=strdup_root(alloc,buff);
    if (IS_NUM(curField->type))
    {	  
      sprintf(buff,"%d",curField->decimals);
      row[8]=strdup_root(alloc,buff);	/* Scale */
      row[9]="10";	  
    }
    else
    {
      row[8]=row[9]=NullS;
      row[15]=strdup_root(alloc,buff);
    }
    if((curField->flags & (NOT_NULL_FLAG | AUTO_INCREMENT_FLAG)) == NOT_NULL_FLAG)
    {	
      sprintf(buff,"%d",SQL_NO_NULLS);
      row[10]=strdup_root(alloc,buff);
      row[17]=strdup_root(alloc,"NO");
    }
    else 
    {		
      sprintf(buff,"%d",SQL_NULLABLE);
      row[10]=strdup_root(alloc,buff);
      row[17]=strdup_root(alloc,"YES");
    }
    row[11]="";	
    row+=SQLCOLUMNS_FIELDS;
  }
  stmt->result->row_count=stmt->result->field_count;
  mysql_link_fields(stmt,SQLCOLUMNS_fields,SQLCOLUMNS_FIELDS);
  DBUG_RETURN(SQL_SUCCESS);
}


/* Have DBMS set up result set of Statistics. */

char SS_type[10];
uint SQLSTAT_order[]={2,3,5,7,8,9,10};
char *SQLSTAT_values[]={NULL,NULL,"","",NULL,"",SS_type,"","","","",NULL,NULL};

MYSQL_FIELD SQLSTAT_fields[] = {
  {"TABLE_CAT","MySQL_Stat",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_SCHEM","MySQL_Stat",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_NAME","MySQL_Stat",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"NON_UNIQUE","MySQL_Stat",NULL,FIELD_TYPE_SHORT,1,1,NOT_NULL_FLAG},
  {"INDEX_QUALIFIER","MySQL_Stat",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"INDEX_NAME","MySQL_Stat",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN},
  {"TYPE","MySQL_Stat",NULL,FIELD_TYPE_SHORT,1,1,NOT_NULL_FLAG},
  {"ORDINAL_POSITION","MySQL_Stat",NULL,FIELD_TYPE_SHORT,1,2,NOT_NULL_FLAG},
  {"COLUMN_NAME","MySQL_Stat",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"ASC_OR_DESC","MySQL_Stat",NULL,FIELD_TYPE_VAR_STRING,1,1},
  {"CARDINALITY","MySQL_Stat",NULL,FIELD_TYPE_LONG,11,11},
  {"PAGES","MySQL_Stat",NULL,FIELD_TYPE_LONG,9,9},
  {"FILTER_CONDITION","MySQL_Stat",NULL,FIELD_TYPE_VAR_STRING,10,10},
};

const uint SQLSTAT_FIELDS=array_elements(SQLSTAT_fields);

/*
  @type    : ODBC 1.0 API
  @purpose : retrieves a list of statistics about a single table and the
	     indexes associated with the table. The driver returns the
	     information as a result set.
*/

SQLRETURN SQL_API SQLStatistics(SQLHSTMT hstmt,
				SQLCHAR FAR *szTableQualifier,
				SQLSMALLINT cbTableQualifier,
				SQLCHAR FAR *szTableOwner,
				SQLSMALLINT cbTableOwner,
				SQLCHAR FAR *szTableName,
				SQLSMALLINT cbTableName,
				SQLUSMALLINT fUnique,SQLUSMALLINT fAccuracy)
{
  char buff[100],table_name[NAME_LEN+1];
  STMT FAR *stmt=(STMT FAR*) hstmt;
  DBUG_ENTER("SQLStatistics");

  if (check_parameters(hstmt,szTableQualifier,cbTableQualifier,szTableOwner,
		      cbTableOwner,szTableName,&cbTableName,table_name,1))
    DBUG_RETURN(SQL_ERROR);
  myodbc_remove_escape(&stmt->dbc->mysql,table_name);
  strxmov(buff,"show keys from ",table_name,NullS);

  pthread_mutex_lock(&stmt->dbc->lock);
  if (check_if_server_is_alive(stmt->dbc) ||
      mysql_query(&stmt->dbc->mysql,buff) ||
      !(stmt->result=mysql_store_result(&stmt->dbc->mysql)))
  {
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
		     mysql_error(&stmt->dbc->mysql),
		     mysql_errno(&stmt->dbc->mysql));
    pthread_mutex_unlock(&stmt->dbc->lock);
    DBUG_RETURN(SQL_ERROR);
  }
  pthread_mutex_unlock(&stmt->dbc->lock);
  int2str(SQL_INDEX_OTHER,SS_type,10);
  stmt->order=	     SQLSTAT_order;
  stmt->order_count= array_elements(SQLSTAT_order);
  stmt->fix_fields=  fix_fields_copy;
  stmt->array=(MYSQL_ROW) my_memdup((gptr) SQLSTAT_values,
				    sizeof(SQLSTAT_values),MYF(0));
  if (fUnique == SQL_INDEX_UNIQUE)
  {	    /* This is too low level... */
    MYSQL_ROWS **prev,*pos;
    prev= &stmt->result->data->data;
    for (pos= *prev ; pos ; pos=pos->next)
    {
      if (pos->data[1][0] == '0') /* Unlink nonunique index */
      {
	(*prev)=pos;
	prev= &pos->next;
      }
      else
	stmt->result->row_count--;
    }
    (*prev)=0;
     mysql_data_seek(stmt->result,0);	/* Restore pointer */
  }
  mysql_link_fields(stmt,SQLSTAT_fields,SQLSTAT_FIELDS);
  DBUG_RETURN(SQL_SUCCESS);
}

/*
  @type    : internal
  @purpose : appends the wildcards
*/

static void my_append_wild(char *to, char *end, 
                           const char *wild)
{
  end-=5;         /* Some extra */
  to=strmov(to," like '");
  while (*wild && to < end)
  {
    if (*wild == '\\' || *wild == '\'')
      *to++='\\';
    *to++= *wild++;
  }
  *to++='%';        /* Nicer this way */
  to[0]='\'';
  to[1]=0;
}

/*
  @type    : internal
  @purpose : checks for the grantability 
*/

static my_bool is_grantable(char *grant_list)
{
  char *grant=dupp_str(grant_list,SQL_NTS);;
  if (grant_list && grant_list[0])
  {
    char seps[]   = ",";
    char *token;
    token = strtok( grant, seps );
    while( token != NULL )
    {
      if(!strcmp(token,"Grant")) 
      {
        x_free(grant);
        return(1);
      }
      token = strtok( NULL, seps );
    }
  }
  x_free(grant);
  return(0);
}


/*
  @type    : internal
  @purpose : returns a table privileges result 
*/
MYSQL_RES *mysql_list_table_priv(MYSQL *mysql, 
				 const char *qualifier, 
				 const char *table)
{
  char buff[255+2*NAME_LEN+1];
  DBUG_ENTER("mysql_list_table_priv");
 
  my_append_wild(strmov(buff,
    "SELECT Db,User,Table_name,Grantor,Table_priv\
    FROM mysql.tables_priv WHERE Table_name"),
    buff+sizeof(buff),table);
  strxmov(buff,buff," AND Db",NullS);
  my_append_wild(strmov(buff,buff),buff+sizeof(buff),qualifier);

  if (mysql_query(mysql,buff))
    DBUG_RETURN(0);
  DBUG_RETURN (mysql_store_result(mysql));
}


/*  Have DBMS set up result set of TABLE privileges.*/


char *SQLTABLES_priv_values[]= {
  NULL,"",NULL,NULL,NULL,NULL,NULL
};

MYSQL_FIELD SQLTABLES_priv_fields[] = {
  {"TABLE_CAT","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_SCHEM","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_NAME","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"GRANTOR","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"GRANTEE","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"PRIVILEGE","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"IS_GRANTABLE","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
};

const uint SQLTABLES_PRIV_FIELDS=array_elements(SQLTABLES_priv_values);

/*
  @type    : ODBC 1.0 API
  @purpose : returns a list of tables and the privileges associated with
             each table. The driver returns the information as a result
             set on the specified statement.
*/

SQLRETURN SQL_API SQLTablePrivileges(SQLHSTMT hstmt,
				     SQLCHAR FAR *szTableQualifier,
				     SQLSMALLINT cbTableQualifier,
				     SQLCHAR FAR *szTableOwner,
				     SQLSMALLINT cbTableOwner,
				     SQLCHAR FAR *szTableName,
				     SQLSMALLINT cbTableName)
{
  char **data;
  char Qualifier_buff[NAME_LEN+1],Name_buff[NAME_LEN+1],
       *TableQualifier,*TableName;
  char **row;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  DBUG_ENTER("SQLTablePrivileges");
  DBUG_PRINT("enter",("Qualifier: '%s'  Owner: '%s'  Table: '%s'",
          szTableQualifier ? (char*) szTableQualifier : "null",
          szTableOwner ? (char*) szTableOwner : "null",
          szTableName ? (char*) szTableName : "null"));

  TableQualifier=fix_str((char FAR *) Qualifier_buff,szTableQualifier,
         cbTableQualifier);
  TableName=fix_str((char FAR*) Name_buff,szTableName,cbTableName);

  pthread_mutex_lock(&stmt->dbc->lock);
  stmt->result=mysql_list_table_priv(&stmt->dbc->mysql,TableQualifier,
         TableName);
  if (!stmt->result)
  {
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
         mysql_error(&stmt->dbc->mysql),
         mysql_errno(&stmt->dbc->mysql));
    pthread_mutex_unlock(&stmt->dbc->lock);
    DBUG_RETURN(SQL_ERROR);
  }
  pthread_mutex_unlock(&stmt->dbc->lock);
  stmt->result_array= (char**) my_malloc(sizeof(char*)*SQLTABLES_PRIV_FIELDS*
         (ulong) stmt->result->row_count, MYF(MY_FAE | MY_ZEROFILL));
  data=stmt->result_array;
  while ((row = mysql_fetch_row(stmt->result)))
  { 
    data[0]=row[0];         
    data[1]="";         
    data[2]=row[2];
    data[3]=row[3];   
    data[4]=row[1];
    data[5]=row[4];
    data[6]=is_grantable(row[4]) ? "YES":"NO";
    data+=SQLTABLES_PRIV_FIELDS;
  }
  mysql_link_fields(stmt,SQLTABLES_priv_fields,SQLTABLES_PRIV_FIELDS);
  DBUG_RETURN(SQL_SUCCESS);
}


/*
  @type    : internal
  @purpose : returns a column privileges result 
*/
MYSQL_RES *mysql_list_column_priv(MYSQL *mysql, 
				  const char *qualifier,
				  const char *table, 
				  const char *column)
{
  char buff[255+3*NAME_LEN+1];
  DBUG_ENTER("mysql_list_column_priv");

  my_append_wild(strmov(buff,
    "SELECT c.Db, c.User,c.Table_name,c.Column_name,\
    t.Grantor,c.Column_priv,t.Table_priv FROM mysql.columns_priv as c,\
    mysql.tables_priv as t WHERE c.Table_name"),
    buff+sizeof(buff),table);
  strxmov(buff,buff," AND c.Db",NullS);
  my_append_wild(strmov(buff,buff),buff+sizeof(buff),qualifier);
  strxmov(buff,buff," AND c.Column_name",NullS);
  my_append_wild(strmov(buff,buff),buff+sizeof(buff),column);
  strxmov(buff,buff," AND c.Table_name=t.Table_name",NullS);
  
  if (mysql_query(mysql,buff))
    DBUG_RETURN(0);

  DBUG_RETURN (mysql_store_result(mysql));
}

/*  Have DBMS set up result set of TABLE privileges.*/


char *SQLCOLUMNS_priv_values[]= {
  NULL,"",NULL,NULL,NULL,NULL,NULL,NULL
};

MYSQL_FIELD SQLCOLUMNS_priv_fields[] = {
  {"TABLE_CAT","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_SCHEM","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_NAME","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"COLUMN_NAME","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"GRANTOR","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"GRANTEE","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"PRIVILEGE","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,NAME_LEN,NOT_NULL_FLAG},
  {"IS_GRANTABLE","MySQL_Catalog",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
};

const uint SQLCOLUMNS_PRIV_FIELDS=array_elements(SQLCOLUMNS_priv_values);

/*
  @type    : ODBC 1.0 API
  @purpose : returns a list of columns and associated privileges for the
             specified table. The driver returns the information as a result
             set on the specified StatementHandle.
*/

SQLRETURN SQL_API SQLColumnPrivileges(SQLHSTMT hstmt,
				      SQLCHAR FAR *szTableQualifier,
				      SQLSMALLINT cbTableQualifier,
				      SQLCHAR FAR *szTableOwner,
				      SQLSMALLINT cbTableOwner,
				      SQLCHAR FAR *szTableName,
				      SQLSMALLINT cbTableName,
				      SQLCHAR FAR *szColumnName,
				      SQLSMALLINT cbColumnName)
{ 
  STMT FAR *stmt=(STMT FAR*) hstmt;
  char Qualifier_buff[NAME_LEN+1],Table_buff[NAME_LEN+1],
       Column_buff[NAME_LEN+1],
       *TableQualifier,*TableName, *ColumnName;
  char **row, **data;  
  DBUG_ENTER("SQLColumnPrivileges");
  DBUG_PRINT("enter",("Qualifier: '%s'  Owner: '%s'  Table: '%s'  column: '%s'",
          szTableQualifier ? (char*) szTableQualifier : "null",
          szTableOwner ? (char*) szTableOwner : "null",
          szTableName ? (char*) szTableName : "null",
          szColumnName ? (char*) szColumnName : "null"));

  TableQualifier=fix_str((char FAR *) Qualifier_buff,szTableQualifier,
         cbTableQualifier);
  TableName=   fix_str((char FAR*) Table_buff,szTableName,cbTableName);
  ColumnName=  fix_str((char FAR*) Column_buff,szColumnName,cbColumnName);

  pthread_mutex_lock(&stmt->dbc->lock);
  stmt->result=mysql_list_column_priv(&stmt->dbc->mysql,TableQualifier,
         TableName,ColumnName);
  if (!stmt->result)
  {
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
         mysql_error(&stmt->dbc->mysql),
         mysql_errno(&stmt->dbc->mysql));
    pthread_mutex_unlock(&stmt->dbc->lock);
    DBUG_RETURN(SQL_ERROR);
  }
  pthread_mutex_unlock(&stmt->dbc->lock);
  stmt->result_array= (char**) my_malloc(sizeof(char*)*SQLCOLUMNS_PRIV_FIELDS*
         (ulong) stmt->result->row_count, MYF(MY_FAE | MY_ZEROFILL));
  data=stmt->result_array;
  while ((row = mysql_fetch_row(stmt->result)))
  { 
    data[0]=row[0];         
    data[1]="";         
    data[2]=row[2];
    data[3]=row[3];
    data[4]=row[4];
    data[5]=row[1];
    data[6]=row[5];
    data[7]=is_grantable(row[6]) ? "YES":"NO";
    data+=SQLCOLUMNS_PRIV_FIELDS;
  }  
  mysql_link_fields(stmt,SQLCOLUMNS_priv_fields,SQLCOLUMNS_PRIV_FIELDS);
  DBUG_RETURN(SQL_SUCCESS); 
}

MYSQL_FIELD SQLSPECIALCOLUMNS_fields[] = {
  {"SCOPE","MySQL_SpecialColumns",NULL,FIELD_TYPE_SHORT,5,5,
   NOT_NULL_FLAG},
  {"COLUMN_NAME","MySQL_SpecialColumns",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,
   NAME_LEN,NOT_NULL_FLAG},
  {"DATA_TYPE","MySQL_SpecialColumns",NULL,FIELD_TYPE_SHORT,5,5,
   NOT_NULL_FLAG},
  {"TYPE_NAME","MySQL_SpecialColumns",NULL,FIELD_TYPE_VAR_STRING,20,20,
   NOT_NULL_FLAG},
  {"COLUMN_SIZE","MySQL_SpecialColumns",NULL,FIELD_TYPE_LONG,7,7},
  {"BUFFER_LENGTH","MySQL_SpecialColumns",NULL,FIELD_TYPE_LONG,7,7},
  {"DECIMAL_DIGITS","MySQL_SpecialColumns",NULL,FIELD_TYPE_SHORT,3,3},
  {"PSEUDO_COLUMN","MySQL_SpecialColumns",NULL,FIELD_TYPE_SHORT,3,3}
};

const uint SQLSPECIALCOLUMNS_FIELDS=array_elements(SQLSPECIALCOLUMNS_fields);


/*
  @type    : ODBC 1.0 API
  @purpose : retrieves the following information about columns within a
	     specified table:
	     - The optimal set of columns that uniquely identifies a row
	       in the table.
	     - Columns that are automatically updated when any value in the
	       row is updated by a transaction
*/

SQLRETURN SQL_API SQLSpecialColumns(SQLHSTMT hstmt,SQLUSMALLINT fColType,
				    SQLCHAR FAR *szTableQualifier,
				    SQLSMALLINT cbTableQualifier,
				    SQLCHAR FAR *szTableOwner,
				    SQLSMALLINT cbTableOwner,
				    SQLCHAR FAR *szTableName,
				    SQLSMALLINT cbTableName,
				    SQLUSMALLINT fScope,
				    SQLUSMALLINT fNullable)
{
  char buff[80],table_name[NAME_LEN+1];
  uint field_count;
  ulong transfer_length,precision,display_size;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  char **row;
  MEM_ROOT *alloc;
  bool primary_key;
  MYSQL_FIELD *field;
  DBUG_ENTER("SQLSpecialColumns");

  if (check_parameters(hstmt,szTableQualifier,cbTableQualifier,szTableOwner,
		       cbTableOwner,szTableName,&cbTableName,table_name,1))
     DBUG_RETURN(SQL_ERROR);

  pthread_mutex_lock(&stmt->dbc->lock);
  stmt->result=mysql_list_fields(&stmt->dbc->mysql,table_name,0);
  if (!stmt->result)
  {
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
		     mysql_error(&stmt->dbc->mysql),
		     mysql_errno(&stmt->dbc->mysql));
    pthread_mutex_unlock(&stmt->dbc->lock);
    return(SQL_ERROR);
  }
  pthread_mutex_unlock(&stmt->dbc->lock);

  if (fColType == SQL_ROWVER)
  {	      /* Find possible timestamp */
    stmt->result_array=
      (char**) my_malloc(sizeof(char*)*SQLSPECIALCOLUMNS_FIELDS*
      stmt->result->field_count, MYF(MY_FAE | MY_ZEROFILL));
    /* Convert mysql fields to data that odbc wants */
    alloc=&stmt->result->field_alloc;
    field_count=0;
    mysql_field_seek(stmt->result,0);
    for (row= stmt->result_array ;
	 (field = mysql_fetch_field(stmt->result)) ;)
    {
      int type;
      if ((field->type != FIELD_TYPE_TIMESTAMP))
	continue;
      field_count++;
      sprintf(buff,"%d",SQL_SCOPE_SESSION);
      row[0]=strdup_root(alloc,buff);
      row[1]= field->name;
      type=unireg_to_sql_datatype(stmt,field,buff,&transfer_length,
				  &precision,&display_size);
      row[3]=strdup_root(alloc,buff);
      sprintf(buff,"%d",type);
      row[2]=strdup_root(alloc,buff);
      sprintf(buff,"%d",precision);
      row[4]=strdup_root(alloc,buff);
      sprintf(buff,"%d",transfer_length);
      row[5]=strdup_root(alloc,buff);
      sprintf(buff,"%d",field->decimals);
      row[6]=strdup_root(alloc,buff);
      sprintf(buff,"%d",SQL_PC_NOT_PSEUDO);
      row[7]=strdup_root(alloc,buff);
      row+=SQLSPECIALCOLUMNS_FIELDS;
    }
    stmt->result->row_count=field_count;
    mysql_link_fields(stmt,SQLSPECIALCOLUMNS_fields,
		      SQLSPECIALCOLUMNS_FIELDS);
    DBUG_RETURN(SQL_SUCCESS);
  }

  if (fColType != SQL_BEST_ROWID)
  {
    DBUG_RETURN(set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
				 "Unsupported argument to SQLSpecialColumns",
				 4000));
  }

  /*
   * The optimal set of columns for identifing a row is either
   * the primary key, or if there is no primary key, then
   * all the fields.
   */

  /* Check if there is a primary (unique) key */
  primary_key=0;
  while ((field = mysql_fetch_field(stmt->result)))
  {
    if (field->flags & PRI_KEY_FLAG)
    {
      primary_key=1;
      break;
    }
  }
  stmt->result_array=
    (char**) my_malloc(sizeof(char*)*SQLSPECIALCOLUMNS_FIELDS*
		       stmt->result->field_count, MYF(MY_FAE | MY_ZEROFILL));

  /* Convert MySQL fields to data that odbc wants */
  alloc=&stmt->result->field_alloc;
  field_count=0;
  mysql_field_seek(stmt->result,0);
  for (row= stmt->result_array ;
       (field = mysql_fetch_field(stmt->result)) ; )
  {
    int type;
    if (primary_key && !(field->flags & PRI_KEY_FLAG))
      continue;
#ifndef SQLSPECIALCOLUMNS_RETURN_ALL_COLUMNS
    /* The ODBC 'standard' doesn't want us to return all columns if there is
       no primary or unique key */
    if (!primary_key)
      continue;
#endif
    field_count++;
    sprintf(buff,"%d",SQL_SCOPE_SESSION);
    row[0]=strdup_root(alloc,buff);
    row[1]= field->name;
    type=unireg_to_sql_datatype(stmt,field,buff,&transfer_length,
			       &precision,&display_size);
    row[3]=strdup_root(alloc,buff);
    sprintf(buff,"%d",type);
    row[2]=strdup_root(alloc,buff);
    sprintf(buff,"%d",precision);
    row[4]=strdup_root(alloc,buff);
    sprintf(buff,"%d",transfer_length);
    row[5]=strdup_root(alloc,buff);
    sprintf(buff,"%d",field->decimals);
    row[6]=strdup_root(alloc,buff);
    sprintf(buff,"%d",SQL_PC_NOT_PSEUDO);
    row[7]=strdup_root(alloc,buff);
    row+=SQLSPECIALCOLUMNS_FIELDS;
  }
  stmt->result->row_count=field_count;
  mysql_link_fields(stmt,SQLSPECIALCOLUMNS_fields,
		    SQLSPECIALCOLUMNS_FIELDS);
  DBUG_RETURN(SQL_SUCCESS);
}


/* Have DBMS set up result set of PrimaryKeys. */

MYSQL_FIELD SQLPRIM_KEYS_fields[] = {
  {"TABLE_CAT","MySQL_Primary_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_SCHEM","MySQL_Primary_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"TABLE_NAME","MySQL_Primary_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,
   NAME_LEN, NOT_NULL_FLAG},
  {"COLUMN_NAME","MySQL_Primary_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,
   NAME_LEN, NOT_NULL_FLAG},
  {"KEY_SEQ","MySQL_Primary_keys",NULL,FIELD_TYPE_SHORT,2,2,NOT_NULL_FLAG},
  {"PK_NAME","MySQL_Primary_keys",NULL,FIELD_TYPE_VAR_STRING,128,0},
};

const uint SQLPRIM_KEYS_FIELDS=array_elements(SQLPRIM_KEYS_fields);


/*
  @type    : ODBC 1.0 API
  @purpose : returns the column names that make up the primary key for a table.
	     The driver returns the information as a result set. This function
	     does not support returning primary keys from multiple tables in
	     a single call
*/

SQLRETURN SQL_API SQLPrimaryKeys(SQLHSTMT hstmt,
				 SQLCHAR FAR *szTableQualifier,
				 SQLSMALLINT cbTableQualifier,
				 SQLCHAR FAR *szTableOwner,
				 SQLSMALLINT cbTableOwner,
				 SQLCHAR FAR *szTableName,
				 SQLSMALLINT cbTableName)
{
  char buff[100],table_name[NAME_LEN+1],**data;
  STMT FAR *stmt=(STMT FAR*) hstmt;
  uint row_count;
  MYSQL_ROW row;
  DBUG_ENTER("SQLPrimaryKeys");

  if (check_parameters(hstmt,szTableQualifier,cbTableQualifier,szTableOwner,
		      cbTableOwner,szTableName,&cbTableName,table_name,1))
    DBUG_RETURN(SQL_ERROR);
  myodbc_remove_escape(&stmt->dbc->mysql,table_name);
  strxmov(buff,"show keys from ",table_name,NullS);
  pthread_mutex_lock(&stmt->dbc->lock);
  if (check_if_server_is_alive(stmt->dbc) ||
      mysql_query(&stmt->dbc->mysql,buff) ||
      !(stmt->result=mysql_store_result(&stmt->dbc->mysql)))
  {
    set_handle_error(SQL_HANDLE_STMT,stmt,MYERR_S1000,
		     mysql_error(&stmt->dbc->mysql),
		     mysql_errno(&stmt->dbc->mysql));
    pthread_mutex_unlock(&stmt->dbc->lock);
    DBUG_RETURN(SQL_ERROR);
  }
  pthread_mutex_unlock(&stmt->dbc->lock);
  stmt->result_array= (char**) my_malloc(sizeof(char*)*SQLPRIM_KEYS_FIELDS*
				       (ulong) stmt->result->row_count,
				       MYF(MY_FAE | MY_ZEROFILL));
  row_count=0;
  data=stmt->result_array;
  while ((row = mysql_fetch_row(stmt->result)))
  {
    if (row[1][0] == '0')     /* If unique index */
    {
      if (row_count && !strcmp(row[3],"1"))
  break;	  /* Allready found unique key */
      row_count++;
      data[0]=data[1]=0;
      data[2]=row[0];
      data[3]=row[4];
      data[4]=row[3];
      data[5]="PRIMARY";
      data+=SQLPRIM_KEYS_FIELDS;
    }
  }
  stmt->result->row_count=row_count;
  mysql_link_fields(stmt,SQLPRIM_KEYS_fields,SQLPRIM_KEYS_FIELDS);
  DBUG_RETURN(SQL_SUCCESS);
}

/*
** Have DBMS set up result set of ForeignKeys.
** mysql dosen't have foreing key, return empty set
*/

MYSQL_FIELD SQLFORE_KEYS_fields[] = {
  {"PKTABLE_CAT","MySQL_Foreign_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"PKTABLE_SCHEM","MySQL_Foreign_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"PKTABLE_NAME","MySQL_Foreign_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,
   NAME_LEN,NOT_NULL_FLAG},
  {"PKCOLUMN_NAME","MySQL_Foreign_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,
   NAME_LEN,NOT_NULL_FLAG},
  {"FKTABLE_CAT","MySQL_Foreign_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,0},
  {"FKTABLE_SCHEM","MySQL_Foreign_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,
   NAME_LEN,NOT_NULL_FLAG},
  {"FKCOLUMN_NAME","MySQL_Foreign_keys",NULL,FIELD_TYPE_VAR_STRING,NAME_LEN,
   NAME_LEN,NOT_NULL_FLAG},
  {"KEY_SEQ","MySQL_Foreign_keys",NULL,FIELD_TYPE_SHORT,2,2,NOT_NULL_FLAG},
  {"UPDATE_RULE","MySQL_Foreign_keys",NULL,FIELD_TYPE_SHORT,2,2},
  {"DELETE_RULE","MySQL_Foreign_keys",NULL,FIELD_TYPE_SHORT,2,2},
  {"FK_NAME","MySQL_Foreign_keys",NULL,FIELD_TYPE_VAR_STRING,128,0},
  {"PK_NAME","MySQL_Foreign_keys",NULL,FIELD_TYPE_VAR_STRING,128,0},
  {"DEFERRABILITY","MySQL_Foreign_keys",NULL,FIELD_TYPE_SHORT,2,2},
};

const uint SQLFORE_KEYS_FIELDS=array_elements(SQLFORE_KEYS_fields);

/*
  @type    : ODBC 1.0 API
  @purpose : returns
	     - A list of foreign keys in the specified table (columns
	       in the specified table that refer to primary keys in
	       other tables).
	     - A list of foreign keys in other tables that refer to the primary
	       key in the specified table
*/

SQLRETURN SQL_API
SQLForeignKeys(SQLHSTMT hstmt,
	       SQLCHAR FAR *szPkTableQualifier,
	       SQLSMALLINT cbPkTableQualifier,
	       SQLCHAR FAR *szPkTableOwner,
	       SQLSMALLINT cbPkTableOwner,
	       SQLCHAR FAR *szPkTableName,SQLSMALLINT cbPkTableName,
	       SQLCHAR FAR *szFkTableQualifier,
	       SQLSMALLINT cbFkTableQualifier,
	       SQLCHAR FAR *szFkTableOwner,
	       SQLSMALLINT cbFkTableOwner,
	       SQLCHAR FAR *szFkTableName,SQLSMALLINT cbFkTableName)
{
  STMT FAR *stmt=(STMT FAR*) hstmt;
  DBUG_ENTER("SQLForeignKeys");

  my_SQLFreeStmt(hstmt,MYSQL_RESET);
  stmt->result=(MYSQL_RES*) my_malloc(sizeof(MYSQL_RES),MYF(MY_ZEROFILL));
  stmt->result->eof=1;
  mysql_link_fields(stmt,SQLFORE_KEYS_fields,SQLFORE_KEYS_FIELDS);
  DBUG_RETURN(SQL_SUCCESS);
}


/*
  @type    : ODBC 1.0 API
  @purpose : returns the list of procedure names stored in a specific data
	     source. Procedure is a generic term used to describe an
	     executable object, or a named entity that can be invoked
	     using input and output parameters
*/

SQLRETURN SQL_API
SQLProcedures(SQLHSTMT hstmt,
	      SQLCHAR FAR *szProcQualifier,SQLSMALLINT cbProcQualifier,
	      SQLCHAR FAR *szProcOwner,SQLSMALLINT cbProcOwner,
	      SQLCHAR FAR *szProcName,SQLSMALLINT cbProcName)
{
  return set_handle_error(SQL_HANDLE_STMT,hstmt,MYERR_S1000,
			 "Driver doesn't support this yet",4000);
}

/*
  @type    : ODBC 1.0 API
  @purpose : returns the list of input and output parameters, as well as
	     the columns that make up the result set for the specified
	     procedures. The driver returns the information as a result
	     set on the specified statement
*/

SQLRETURN SQL_API SQLProcedureColumns(SQLHSTMT hstmt,
				      SQLCHAR FAR *szProcQualifier,
				      SQLSMALLINT cbProcQualifier,
				      SQLCHAR FAR *szProcOwner,
				      SQLSMALLINT cbProcOwner,
				      SQLCHAR FAR *szProcName,
				      SQLSMALLINT cbProcName,
				      SQLCHAR FAR *szColumnName,
				      SQLSMALLINT cbColumnName)
{
  return set_handle_error(SQL_HANDLE_STMT, hstmt,MYERR_S1000,
			  "Driver doesn't support this yet",4000);
}

/*
  @type    : myodbc internal
  @purpose : validates the catalog api input parameters
*/

static int check_parameters(SQLHSTMT hstmt,SQLCHAR FAR *szTableQualifier,
			    SQLSMALLINT cbTableQualifier,
			    SQLCHAR FAR *szTableOwner,
			    SQLSMALLINT cbTableOwner,
			    SQLCHAR FAR *szTableName,
			    SQLSMALLINT *cbTableName,
			    char *table_name,bool no_wildcards)
{
  /* Check table name and copy it to table_name */
  if (!szTableName)
  {
    szTableName="";
    *cbTableName=0;
  }
  if (*cbTableName == SQL_NTS)
    *cbTableName=strlen(szTableName);
  if ((uint) *cbTableName > NAME_LEN)
    return set_handle_error(SQL_HANDLE_STMT,hstmt,MYERR_S1090,
			    "Invalid table name",0);
  strmake(table_name,szTableName,*cbTableName);
  if (no_wildcards && (!table_name[0] || strchr(table_name,'%')))
    return(set_handle_error(SQL_HANDLE_STMT,hstmt,MYERR_S1C00,
			   "Can't use wildcards in table name",0));
  my_SQLFreeStmt(hstmt,MYSQL_RESET);
  return 0;
}

