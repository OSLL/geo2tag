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
 * MYUTIL.H                                                                *
 *                                                                         *
 * @description: Prototype definations needed by the driver                *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Sep-22                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/
#ifndef __MYUTIL_H__
#define __MYUTIL_H__

/*
  utility function prototypes that share among files
*/

SQLRETURN my_SQLExecute(STMT FAR* stmt);
SQLRETURN my_SQLPrepare(SQLHSTMT hstmt,SQLCHAR FAR *szSqlStr,
			SQLINTEGER cbSqlStr);
SQLRETURN SQL_API my_SQLFreeStmt(SQLHSTMT hstmt,SQLUSMALLINT fOption);
SQLRETURN SQL_API my_SQLAllocStmt(SQLHDBC hdbc,SQLHSTMT FAR *phstmt);
SQLRETURN do_query(STMT FAR *stmt,char *query);
char *insert_params(STMT FAR *stmt);
SQLRETURN odbc_stmt(DBC FAR *dbc, const char *query);
void mysql_link_fields(STMT *stmt,MYSQL_FIELD *fields,uint field_count);
void fix_result_types(STMT *stmt);
char *fix_str(char *to,char *from,int length);
char *dupp_str(char *from,int length);
bool empty_str(char *from,int length);
SQLRETURN my_pos_delete(STMT FAR *stmt,STMT FAR *stmtParam,
			SQLUSMALLINT irow,DYNAMIC_STRING *dynStr);
SQLRETURN my_pos_update(STMT FAR *stmt,STMT FAR *stmtParam,
			SQLUSMALLINT irow,DYNAMIC_STRING *dynStr);
my_bool check_if_positioned_cursor_exists(STMT FAR *stmt,
					  STMT FAR **stmtNew);
char *insert_param(MYSQL *mysql, char *to,PARAM_BIND *param);
char *add_to_buffer(NET *net,char *to,char *from,ulong length);
SQLRETURN copy_lresult(DBC FAR *dbc,SQLCHAR FAR *rgbValue,
		       SQLINTEGER cbValueMax, SQLINTEGER FAR *pcbValue,
		       char *src,long src_length, long max_length,
		       long fill_length,long *offset,my_bool binary_data);
SQLRETURN copy_binary_result(DBC FAR *dbc,SQLCHAR FAR *rgbValue,
			     SQLINTEGER cbValueMax, SQLINTEGER FAR *pcbValue,
			     char *src, ulong src_length, ulong max_length,
			     ulong *offset);
SQLRETURN set_dbc_error(DBC FAR *dbc,char *state,char *message,uint errcode);
SQLRETURN set_stmt_error(STMT *stmt,char *state,char *message,uint errcode);
void translate_error(char *save_state,char *default_state,uint mysql_err);
int unireg_to_sql_datatype(STMT FAR *stmt, MYSQL_FIELD *field, char *buff,
			   ulong *transfer_length,ulong *precision,
			   ulong *display_size);
SQLRETURN SQL_API my_SQLBindParameter(SQLHSTMT hstmt,SQLUSMALLINT ipar,
				      SQLSMALLINT fParamType,
				      SQLSMALLINT fCType, SQLSMALLINT fSqlType,
				      SQLUINTEGER cbColDef,
				      SQLSMALLINT ibScale,
				      SQLPOINTER  rgbValue,
				      SQLINTEGER cbValueMax,
				      SQLINTEGER FAR *pcbValue);
SQLRETURN SQL_API my_SQLExtendedFetch(SQLHSTMT hstmt, SQLUSMALLINT fFetchType,
				      SQLINTEGER irow, SQLUINTEGER FAR *pcrow,
				      SQLUSMALLINT FAR *rgfRowStatus);
SQLRETURN copy_stmt_error(STMT FAR *src, STMT FAR *dst);
int unireg_to_c_datatype(MYSQL_FIELD *field);
int default_c_type(int sql_data_type);
ulong bind_length(int sql_data_type,ulong length);
my_bool str_to_date(DATE_STRUCT *rgbValue, const char *str,uint length);
ulong str_to_time(const char *str,uint length);
void init_getfunctions(void);
void myodbc_init(void);
int check_if_server_is_alive(DBC FAR *dbc);
my_bool dynstr_append_quoted_name(DYNAMIC_STRING *str, const char *name);
SQLRETURN set_handle_error(SQLSMALLINT HandleType, SQLHANDLE handle,
			   myodbc_errid errid, SQLCHAR *errtext,
			   SQLINTEGER mysql_err);
SQLRETURN copy_str_data(SQLSMALLINT HandleType, SQLHANDLE Handle,
			SQLCHAR FAR *rgbValue, SQLSMALLINT cbValueMax,
			SQLSMALLINT FAR *pcbValue,char FAR *src);
SQLRETURN SQL_API my_SQLAllocEnv(SQLHENV FAR * phenv);
SQLRETURN SQL_API my_SQLAllocConnect(SQLHENV henv, SQLHDBC FAR *phdbc);
SQLRETURN SQL_API my_SQLFreeConnect(SQLHDBC hdbc);
SQLRETURN SQL_API my_SQLFreeEnv(SQLHENV henv);
SQLRETURN SQL_API my_SQLDisconnect(SQLHDBC hdbc);
SQLRETURN SQL_API my_SQLDriverConnect(SQLHDBC hdbc,SQLHWND hwnd,
				      SQLCHAR FAR *szConnStrIn,
				      SQLSMALLINT cbConnStrIn,
				      SQLCHAR FAR *szConnStrOut,
				      SQLSMALLINT cbConnStrOutMax,
				      SQLSMALLINT FAR *pcbConnStrOut,
				      SQLUSMALLINT fDriverCompletion);
char *extend_buffer(NET *net,char *to,ulong length);
void myodbc_end();
SQLRETURN set_dynamic_result(STMT FAR *stmt);
void set_current_cursor_data(STMT FAR *stmt,SQLUINTEGER irow);
#endif /* __MYUTIL_H__ */
