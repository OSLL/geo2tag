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
 * UTILITY.C                                                               *
 *                                                                         *
 * @description: Utility functions                                         *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

#include "myodbc3.h"
#include "errmsg.h"
#include "m_ctype.h"

/*
  @type    : myodbc internal
  @purpose : executes the specified sql statement
*/

SQLRETURN odbc_stmt(DBC FAR *dbc, const char *query)
{
  SQLRETURN result=SQL_SUCCESS;
  DBUG_ENTER("odbc_stmt");
  DBUG_PRINT("enter",("stmt: %s",query));

  pthread_mutex_lock(&dbc->lock);
  if (check_if_server_is_alive(dbc) ||
      mysql_real_query(&dbc->mysql,query,strlen(query)))
  {
    result = set_handle_error(SQL_HANDLE_DBC,dbc,MYERR_S1000,
            mysql_error(&dbc->mysql),
            mysql_errno(&dbc->mysql));
  }
  pthread_mutex_unlock(&dbc->lock);
  DBUG_RETURN(result);
}

/*
  @type    : myodbc internal
  @purpose : use own fields instead of sql fields
*/

void mysql_link_fields(STMT *stmt,MYSQL_FIELD *fields,uint field_count)
{
  MYSQL_RES *result=stmt->result;
  result->fields=fields;
  result->field_count=field_count;
  result->current_field=0;
  fix_result_types(stmt);
}

/*
  @type    : myodbc internal
  @purpose : fixes the result types
*/

void fix_result_types(STMT *stmt)
{
  uint i;
  MYSQL_RES *result=stmt->result;
  DBUG_ENTER("fix_result_types");

  stmt->state=ST_EXECUTED;  /* Mark set found */
  if ((stmt->odbc_types=(SQLSMALLINT*)
       my_malloc(sizeof(SQLSMALLINT)*result->field_count, MYF(0))))
  {
    for (i=0 ; i < result->field_count ; i++)
    {
      MYSQL_FIELD *field=result->fields+i;
      stmt->odbc_types[i]=(SQLSMALLINT) unireg_to_c_datatype(field);
    }
  }
  /*
    Fix default values for bound columns
    Normally there isn't any bound columns at this stage !
  */
  if (stmt->bind)
  {
    if (stmt->bound_columns < result->field_count)
    {
      if (!(stmt->bind=(BIND*) my_realloc((char*) stmt->bind,
            sizeof(BIND) * result->field_count,
            MYF(MY_FREE_ON_ERROR))))
      {
  /* We should in principle give an error here */
  stmt->bound_columns=0;
  DBUG_VOID_RETURN;
      }
      bzero((gptr) (stmt->bind+stmt->bound_columns),
      (result->field_count -stmt->bound_columns)*sizeof(BIND));
      stmt->bound_columns=result->field_count;
    }
    /* Fix default types and pointers to fields */

    mysql_field_seek(result,0);
    for (i=0; i < result->field_count ; i++)
    {
      if (stmt->bind[i].fCType == SQL_C_DEFAULT)
  stmt->bind[i].fCType=stmt->odbc_types[i];
      stmt->bind[i].field=mysql_fetch_field(result);
    }
  }
  DBUG_VOID_RETURN;
}

/*
  @type    : myodbc internal
  @purpose : change a string + length to a zero terminated string
*/

char *fix_str(char *to,char *from,int length)
{
  if (!from)
    return "";
  if (length == SQL_NTS)
    return from;
  strmake(to,from,length);
  return to;
}

/*
  @type    : myodbc internal
  @purpose : duplicate the string
*/

char *dupp_str(char *from,int length)
{
  char *to;
  if (!from)
    return my_strdup("",MYF(MY_WME));
  if (length == SQL_NTS)
    length=strlen(from);
  if ((to=my_malloc(length+1,MYF(MY_WME))))
  {
    memcpy(to,from,length);
    to[length]=0;
  }
  return to;
}

/*
  @type    : myodbc internal
  @purpose : returns 1 if from is a null pointer or a empty string
*/

bool empty_str(char *from,int length)
{
  if (!from)
    return 1;
  if (length == SQL_NTS)
    return from[0] == 0;
  return !length;
}

/*
  @type    : myodbc internal
  @purpose : copies the string data to rgbValue buffer. If rgbValue
       is NULL, then returns warning with full length, else
       copies the cbValueMax length from 'src' and returns it.
*/

SQLRETURN copy_str_data(SQLSMALLINT HandleType, SQLHANDLE Handle,
      SQLCHAR FAR *rgbValue,
      SQLSMALLINT cbValueMax,
      SQLSMALLINT FAR *pcbValue,char FAR *src)
{
  SQLSMALLINT dummy;

  if (!pcbValue)
    pcbValue = &dummy;

  if (cbValueMax == SQL_NTS)
    cbValueMax = *pcbValue = strlen(src);

  else if (cbValueMax < 0)
    return set_handle_error(HandleType,Handle,MYERR_S1090,NULL,0);

  else
  {
    cbValueMax = cbValueMax - 1;
    *pcbValue = strlen(src);
  }

  if (rgbValue)
    strmake(rgbValue, src, cbValueMax);

  if (min(*pcbValue , cbValueMax ) != *pcbValue)
    return (SQL_SUCCESS_WITH_INFO);

  return (SQL_SUCCESS);
}

/*
  @type    : myodbc internal
  @purpose : returns (possibly truncated) results
       if result is truncated the result length contains
       length of the truncted result
*/

SQLRETURN
copy_lresult(DBC FAR *dbc,SQLCHAR FAR *rgbValue,
       SQLINTEGER cbValueMax,
       SQLINTEGER FAR *pcbValue,char *src,long src_length,
       long max_length,long fill_length,long *offset,
       my_bool binary_data)
{
  char *dst=(char*) rgbValue;
  ulong length;

  if (src && src_length == SQL_NTS)
    src_length= strlen(src);

  if (cbValueMax && !binary_data)   /* If not length check */
    cbValueMax--;       /* Room for end null */
  else if (!cbValueMax)
    dst=0;          /* Don't copy anything! */
  if (max_length)       /* If limit on char lengths */
  {
    set_if_smaller(cbValueMax,(long) max_length);
    set_if_smaller(src_length,max_length);
    set_if_smaller(fill_length,max_length);
  }
  if (fill_length < src_length || !dbc || !(dbc->flag & FLAG_PAD_SPACE))
    fill_length=src_length;
  if (*offset == (ulong) ~0L)
    *offset=0;          /* First call */
  else if (*offset >= fill_length)
    return SQL_NO_DATA_FOUND;
  src+= *offset ; src_length-= *offset; fill_length-= *offset;
  length=min(fill_length, cbValueMax);
  (*offset)+=length;        /* Fix for next call */
  if (pcbValue)
    *pcbValue=fill_length;
  if (dst)          /* Bind allows null pointers */
  {
    ulong copy_length= ((long) src_length >= (long) length ? length :
      ((long) src_length >= 0 ? src_length : 0L));
    memcpy(dst,src,copy_length);
    bfill(dst+copy_length,length-copy_length,' ');
    if (!binary_data || length != (ulong) cbValueMax)
      dst[length]=0;
  }
  if (!dst || cbValueMax >= fill_length)
    return SQL_SUCCESS;
  DBUG_PRINT("info",("Returned %ld characters from offset: %ld",
         length,*offset - length));
  if (dbc)
    set_dbc_error(dbc,"01004","Data truncated",4002);
  return SQL_SUCCESS_WITH_INFO;
}

/*
  @type    : myodbc internal
  @purpose : is used when converting a binary string to a SQL_C_CHAR
*/

extern char NEAR _dig_vec[];

SQLRETURN
copy_binary_result(DBC FAR *dbc,SQLCHAR FAR *rgbValue,SQLINTEGER cbValueMax,
       SQLINTEGER FAR *pcbValue,char *src,ulong src_length,
       ulong max_length,ulong *offset)
{
  char *dst=(char*) rgbValue;
  ulong length;

  if (!cbValueMax)
    dst=0;      /* Don't copy anything! */
  if (max_length) /* If limit on char lengths */
  {
    set_if_smaller(cbValueMax,(long) max_length+1);
    set_if_smaller(src_length,(max_length+1)/2);
  }
  if (*offset == (ulong) ~0L)
    *offset=0;    /* First call */
  else if (*offset >= src_length)
    return SQL_NO_DATA_FOUND;
  src+= *offset ; src_length-= *offset;
  length=min(src_length,(ulong) (cbValueMax-1)/2);
  (*offset)+=length;      /* Fix for next call */
  if (pcbValue)
    *pcbValue=src_length*2;
  if (dst)      /* Bind allows null pointers */
  {
    ulong i;
    for (i=0 ; i < length ; i++)
    {
      *dst++= _dig_vec[(uchar) *src >> 4];
      *dst++= _dig_vec[(uchar) *src++ & 15];
    }
    *dst=0;
  }
  if (!dst || (ulong) cbValueMax > length*2)
    return SQL_SUCCESS;
  DBUG_PRINT("info",("Returned %ld characters from offset: %ld",
         length,*offset - length));
  if (dbc)
    set_dbc_error(dbc,"01004","Data truncated",4002);
  return SQL_SUCCESS_WITH_INFO;
}

/*
  @type    : myodbc internal
  @purpose : get type, transfer length and precision for a unireg column
       note that timestamp is changed to YYYY-MM-DD HH:MM:SS type
*/

int unireg_to_sql_datatype(STMT FAR *stmt, MYSQL_FIELD *field, char *buff,
         ulong *transfer_length, ulong *precision,
         ulong *display_size)
{
  char *pos;
  if (stmt->dbc->flag & (FLAG_FIELD_LENGTH | FLAG_SAFE))
    *transfer_length= *precision= *display_size= max(field->length,
                 field->max_length);
  else
    *transfer_length= *precision= *display_size= field->max_length;

  switch(field->type) {
  case FIELD_TYPE_DECIMAL:
    *display_size= max(field->length,field->max_length) -
      test(!(field->flags & UNSIGNED_FLAG)) -
      test(field->decimals);
    *precision= *display_size;
    if (buff) strmov(buff,"decimal");
    return SQL_DECIMAL;
  case FIELD_TYPE_CHAR:
    if (buff)
    {
      pos=strmov(buff,"tinyint");
      if (field->flags & UNSIGNED_FLAG)
  strmov(pos," unsigned");
    }
    *transfer_length=1;
    return SQL_TINYINT;
  case FIELD_TYPE_SHORT:
    if (buff)
    {
      pos=strmov(buff,"smallint");
      if (field->flags & UNSIGNED_FLAG)
  strmov(pos," unsigned");
    }
    *transfer_length=2;
    return SQL_SMALLINT;
  case FIELD_TYPE_INT24:
    if (buff)
    {
      pos=strmov(buff,"mediumint");
      if (field->flags & UNSIGNED_FLAG)
  strmov(pos," unsigned");
    }
    *transfer_length=4;
    return SQL_INTEGER;
  case FIELD_TYPE_LONG:
    if (buff)
    {
      pos= strmov(buff,"integer");
      if (field->flags & UNSIGNED_FLAG)
  strmov(pos," unsigned");
    }
    *transfer_length=4;
    return SQL_INTEGER;
  case FIELD_TYPE_LONGLONG:
    if (buff)
    {
      pos= strmov(buff,"bigint");
      if (field->flags & UNSIGNED_FLAG)
  strmov(pos," unsigned");
    }
    *transfer_length=20;
    return (stmt->dbc->flag & FLAG_NO_BIGINT) ? SQL_INTEGER : SQL_BIGINT;
  case FIELD_TYPE_FLOAT:
    if (buff)
    {
      pos= strmov(buff,"float");
      if (field->flags & UNSIGNED_FLAG)
  strmov(pos," unsigned");
    }
    *transfer_length=4;
    return SQL_REAL;
  case FIELD_TYPE_DOUBLE:
    if (buff)
    {
      pos= strmov(buff,"double");
      if (field->flags & UNSIGNED_FLAG)
  strmov(pos," unsigned");
    }
    *transfer_length=8;
    return SQL_DOUBLE;
  case FIELD_TYPE_NULL:
    if (buff) strmov(buff,"null");
    return SQL_VARCHAR;
  case FIELD_TYPE_YEAR:
    if (buff)
      pos=strmov(buff,"year");
    *transfer_length=2;
    return SQL_SMALLINT;
  case FIELD_TYPE_TIMESTAMP:
    if (buff) strmov(buff,"timestamp");
    *transfer_length=16;      /* size of timestamp_struct */
    *precision= *display_size=19;
    if (stmt->dbc->env->odbc_ver == SQL_OV_ODBC3)
      return SQL_TYPE_TIMESTAMP;
    return SQL_TIMESTAMP;
  case FIELD_TYPE_DATETIME:
    if (buff) strmov(buff,"datetime");
    *transfer_length=16;      /* size of timestamp_struct */
    *precision= *display_size=19;
    if (stmt->dbc->env->odbc_ver == SQL_OV_ODBC3)
      return SQL_TYPE_TIMESTAMP;
    return SQL_TIMESTAMP;
  case FIELD_TYPE_NEWDATE:
  case FIELD_TYPE_DATE:
    if (buff) strmov(buff,"date");
    *transfer_length=6;       /* size of date struct */
    *precision= *display_size=10;
    if (stmt->dbc->env->odbc_ver == SQL_OV_ODBC3)
      return SQL_TYPE_DATE;
    return SQL_DATE;
  case FIELD_TYPE_TIME:
    if (buff) strmov(buff,"time");
    *transfer_length=6;       /* size of time struct */
    *precision= *display_size=8;
    if (stmt->dbc->env->odbc_ver == SQL_OV_ODBC3)
      return SQL_TYPE_TIME;
    return SQL_TIME;
  case FIELD_TYPE_STRING:
    if (buff) strmov(buff,"char");
    return SQL_CHAR;
  case FIELD_TYPE_VAR_STRING:
    if (buff) strmov(buff,"varchar");
    return SQL_VARCHAR;
  case FIELD_TYPE_TINY_BLOB:
    if (buff)
      strmov(buff,(field->flags & BINARY_FLAG) ? "tinyblob" : "tinytext");
    if (stmt->dbc->flag & (FLAG_FIELD_LENGTH | FLAG_SAFE))
      *transfer_length= *precision= *display_size= 255;
    return (field->flags & BINARY_FLAG) ? SQL_LONGVARBINARY : SQL_LONGVARCHAR;
  case FIELD_TYPE_BLOB:
    if (buff)
      strmov(buff,(field->flags & BINARY_FLAG) ? "blob" : "text");
    if (stmt->dbc->flag & (FLAG_FIELD_LENGTH | FLAG_SAFE))
      *transfer_length= *precision= *display_size= 65535;
    return (field->flags & BINARY_FLAG) ? SQL_LONGVARBINARY : SQL_LONGVARCHAR;
  case FIELD_TYPE_MEDIUM_BLOB:
    if (buff)
      strmov(buff,((field->flags & BINARY_FLAG) ? "mediumblob" : "mediumtext"));
    if (stmt->dbc->flag & (FLAG_FIELD_LENGTH | FLAG_SAFE))
      *transfer_length= *precision= *display_size= (1L << 24)-1L;
    return (field->flags & BINARY_FLAG) ? SQL_LONGVARBINARY : SQL_LONGVARCHAR;
  case FIELD_TYPE_LONG_BLOB:
    if (buff)
      strmov(buff,((field->flags & BINARY_FLAG) ? "longblob": "longtext"));
    if (stmt->dbc->flag & (FLAG_FIELD_LENGTH | FLAG_SAFE))
      *transfer_length= *precision= *display_size= INT_MAX32;
    return (field->flags & BINARY_FLAG) ? SQL_LONGVARBINARY : SQL_LONGVARCHAR;
  case FIELD_TYPE_ENUM:
    if (buff) strmov(buff,"enum");
    return SQL_CHAR;
  case FIELD_TYPE_SET:
    if (buff) strmov(buff,"set");
    return SQL_CHAR;
  }
  return 0; /* Impossible */
}

/*
  @type    : myodbc internal
  @purpose : returns internal type to C type
*/

int unireg_to_c_datatype(MYSQL_FIELD *field)
{
  switch(field->type) {
  case FIELD_TYPE_LONGLONG:     /* Must be returned as char */
  default:
    return SQL_C_CHAR;
  case FIELD_TYPE_CHAR:
    return SQL_C_TINYINT;
  case FIELD_TYPE_YEAR:
  case FIELD_TYPE_SHORT:
    return SQL_C_SHORT;
  case FIELD_TYPE_INT24:
  case FIELD_TYPE_LONG:
    return SQL_C_LONG;
  case FIELD_TYPE_FLOAT:
    return SQL_C_FLOAT;
  case FIELD_TYPE_DOUBLE:
    return SQL_C_DOUBLE;
  case FIELD_TYPE_TIMESTAMP:
  case FIELD_TYPE_DATETIME:
    return SQL_C_TIMESTAMP;
  case FIELD_TYPE_NEWDATE:
  case FIELD_TYPE_DATE:
    return SQL_C_DATE;
  case FIELD_TYPE_TIME:
    return SQL_C_TIME;
  }
}

/*
  @type    : myodbc internal
  @purpose : returns default C type for a given SQL type
*/

int default_c_type(int sql_data_type)
{
  switch (sql_data_type) {
  case SQL_CHAR:
  case SQL_VARCHAR:
  case SQL_LONGVARCHAR:
  case SQL_DECIMAL:
  case SQL_NUMERIC:
  default:
    return SQL_C_CHAR;
  case SQL_BIGINT:
    return SQL_C_SBIGINT;
  case SQL_BIT:
    return SQL_C_BIT;
  case SQL_TINYINT:
    return SQL_C_TINYINT;
  case SQL_SMALLINT:
    return SQL_C_SHORT;
  case SQL_INTEGER:
    return SQL_C_LONG;
  case SQL_REAL:
  case SQL_FLOAT:
    return SQL_C_FLOAT;
  case SQL_DOUBLE:
    return SQL_C_DOUBLE;
  case SQL_BINARY:
  case SQL_VARBINARY:
  case SQL_LONGVARBINARY:
    return SQL_C_BINARY;
  case SQL_DATE:
  case SQL_TYPE_DATE:
    return SQL_C_DATE;
  case SQL_TIME:
  case SQL_TYPE_TIME:
    return SQL_C_TIME;
  case SQL_TIMESTAMP:
  case SQL_TYPE_TIMESTAMP:
    return SQL_C_TIMESTAMP;
  }
}

/*
  @type    : myodbc internal
  @purpose : returns bind length
*/

ulong bind_length(int sql_data_type,ulong length)
{
  switch (sql_data_type) {
  default:          /* For CHAR, VARCHAR, BLOB...*/
    return length;
  case SQL_C_BIT:
  case SQL_C_TINYINT:
  case SQL_C_STINYINT:
  case SQL_C_UTINYINT:
    return 1;
  case SQL_C_SHORT:
  case SQL_C_SSHORT:
  case SQL_C_USHORT:
    return 2;
  case SQL_C_LONG:
  case SQL_C_SLONG:
  case SQL_C_ULONG:
    return sizeof(long);
  case SQL_C_FLOAT:
    return sizeof(float);
  case SQL_C_DOUBLE:
    return sizeof(double);
  case SQL_C_DATE:
  case SQL_C_TYPE_DATE:
    return sizeof(DATE_STRUCT);
  case SQL_C_TIME:
  case SQL_C_TYPE_TIME:
    return sizeof(TIME_STRUCT);
  case SQL_C_TIMESTAMP:
  case SQL_C_TYPE_TIMESTAMP:
    return sizeof(TIMESTAMP_STRUCT);
  case SQL_C_SBIGINT:
  case SQL_C_UBIGINT:
    return sizeof(longlong);
  }
}

/*
  @type    : myodbc internal
  @purpose : convert a possible string to a data value
*/

my_bool str_to_date(DATE_STRUCT *rgbValue, const char *str,uint length)
{
  uint field_length,year_length,digits,i,date[3];
  const char *pos;
  const char *end=str+length;
  for (; !isdigit(*str) && str != end ; str++) ;
  /*
    Calculate first number of digits.
    If length= 4, 8 or >= 14 then year is of format YYYY
    (YYYY-MM-DD,  YYYYMMDD)
  */
  for (pos=str; pos != end && isdigit(*pos) ; pos++) ;
  digits= (uint) (pos-str);
  year_length= (digits == 4 || digits == 8 || digits >= 14) ? 4 : 2;
  field_length=year_length-1;

  for (i=0 ; i < 3 && str != end; i++)
  {
    uint tmp_value=(uint) (uchar) (*str++ - '0');
    while (str != end && isdigit(str[0]) && field_length--)
    {
      tmp_value=tmp_value*10 + (uint) (uchar) (*str - '0');
      str++;
    }
    date[i]=tmp_value;
    while (str != end && !isdigit(*str))
      str++;
    field_length=1;   /* Rest fields can only be 2 */
  }
  if (i <= 1 || date[1] == 0)   /* Wrong date */
    return 1;
  while (i < 3)
    date[i++]=1;

  rgbValue->year=date[0];
  rgbValue->month=date[1];
  rgbValue->day=date[2];
  return 0;
}

/*
  @type    : myodbc internal
  @purpose : convert a time string to a (ulong) value.
       At least following formats are recogniced
       HHMMSS HHMM HH HH.MM.SS   {t HH:MM:SS }
  @return  : HHMMSS
*/

ulong str_to_time(const char *str,uint length)
{
  uint i,date[3];
  const char *end=str+length;
  for (; !isdigit(*str) && str != end ; str++) ;

  for (i=0 ; i < 3 && str != end; i++)
  {
    uint tmp_value=(uint) (uchar) (*str++ - '0');
    if (str != end && isdigit(str[0]))
    {
      tmp_value=tmp_value*10 + (uint) (uchar) (*str - '0');
      str++;
    }
    date[i]=tmp_value;
    while (str != end && !isdigit(*str))
      str++;
  }
  while (i < 3)
    date[i++]=0;
  return (ulong) date[0]*10000L + (ulong) (date[1]*100+date[2]);
}

/*
  @type    : myodbc internal
  @purpose : if there was a long time since last question, check that
       the server is up with mysql_ping (to force a reconnect)
*/

int check_if_server_is_alive(DBC FAR *dbc)
{
  time_t seconds= (time_t) time((time_t*) 0);
  int result=0;
  if ((ulong) (seconds - dbc->last_query_time) >= CHECK_IF_ALIVE)
  {
    if (mysql_ping(&dbc->mysql) &&
  mysql_errno(&dbc->mysql) == CR_SERVER_GONE_ERROR)
      result=1;
  }
  dbc->last_query_time=seconds;
  return result;
}

/*
  @type    : myodbc3 internal
  @purpose : appends quoted string to dynamic string
*/

my_bool dynstr_append_quoted_name(DYNAMIC_STRING *str, const char *name)
{
  uint tmp=strlen(name);
  char *pos;
  if (dynstr_realloc(str,tmp+3))
    return 1;
  pos=str->str+str->length;
  *pos='`';
  memcpy(pos+1,name,tmp);
  pos[tmp+1]='`';
  pos[tmp+2]=0;         /* Safety */
  str->length+=tmp+2;
  return 0;
}


#ifndef _UNIX_

/* Define functions that dosen't exist in a dll */

/* _exit is called by safemalloc, mystatic & my_malloc */
#ifndef __WIN__
void exit(int exit)
{
  abort();
}
#endif /* !__WIN__ */

/* perror is called by dbug.c */
void perror(const char *str)
{
}

/* clock is called by dbug.c when profiling */
long clock(void)
{
  return 0L;
}


#ifndef THREAD
long getpid()
{
  return 0;
}
#else

int pthread_dummy(int return_value)
{
  return return_value;
}

#endif /* !THREAD */
#endif /* !_UNIX_ */
