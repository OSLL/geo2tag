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
 * ERROR.H                                                                 *
 *                                                                         *
 * @description: Definations for error handling                            *
 *                                                                         *
 * @author     : MySQL AB(monty@mysql.com, venu@mysql.com)                 *
 * @date       : 2001-Aug-15                                               *
 * @product    : myodbc3                                                   *
 *                                                                         *
****************************************************************************/

#ifndef __ERROR_H__
#define __ERROR_H__

/*
  myodbc internal error constants
*/
#define ER_INVALID_CURSOR_NAME	    514
#define ER_ALL_COLUMNS_IGNORED	    537

/*
  myodbc3 error prefix
*/
#define MYODBC3_ERROR_PREFIX "[MySQL][ODBC 3.51 Driver]"

/*
  error handler structure
*/
typedef struct tagERROR {

  SQLCHAR     sqlstate[6];
  SQLCHAR     message[MYSQL_ERRMSG_SIZE];
  SQLINTEGER  native_error;
  SQLRETURN   retcode;

} MYERROR;

/*
  list of MyODBC3 error codes
*/
typedef enum myodbc_errid
{
    MYERR_01000,
    MYERR_01004,
    MYERR_01S02,
    MYERR_01S03,
    MYERR_01S04,
    MYERR_01S06,
    MYERR_07001,
    MYERR_07005,
    MYERR_07009,
    MYERR_08002,
    MYERR_08003,
    MYERR_24000,
    MYERR_25000,
    MYERR_25S01,
    MYERR_34000,
    MYERR_S1000,
    MYERR_S1001,
    MYERR_S1002,
    MYERR_S1003,
    MYERR_S1004,
    MYERR_S1009,
    MYERR_S1010,
    MYERR_S1011,
    MYERR_S1012,
    MYERR_S1013,
    MYERR_S1015,
    MYERR_S1024,
    MYERR_S1090,
    MYERR_S1091,
    MYERR_S1092,
    MYERR_S1093,
    MYERR_S1095,
    MYERR_S1106,
    MYERR_S1107,
    MYERR_S1109,
    MYERR_S1C00

} myodbc_errid;

/*
  error handler-predefined structure
  odbc2 state, odbc3 state, message and return code
*/
typedef struct myodbc3_err_str {

  SQLCHAR     sqlstate2[6]; /* ODBC2 STATE */
  SQLCHAR     sqlstate3[6]; /* ODBC3 STATE */
  SQLCHAR     message[MYSQL_ERRMSG_SIZE];/* ERROR MSG */
  SQLRETURN   retcode;	    /* RETURN CODE */

} MYODBC3_ERR_STR;

#endif /* __ERROR_H__ */
