/*********************************************************************
 *
 * unixODBC Cursor Library
 *
 * Created by Nick Gorham
 * (nick@easysoft.com).
 *
 * copyright (c) 1999 Nick Gorham
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 **********************************************************************
 *
 * $Id: SQLError.c,v 1.1.1.1 2001/10/17 16:40:15 lurcher Exp $
 *
 * $Log: SQLError.c,v $
 * Revision 1.1.1.1  2001/10/17 16:40:15  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.2  2001/03/28 14:57:22  nick
 *
 * Fix bugs in corsor lib introduced bu UNCODE and other changes
 *
 * Revision 1.1.1.1  2000/09/04 16:42:52  nick
 * Imported Sources
 *
 * Revision 1.1  1999/09/19 22:22:50  ngorham
 *
 *
 * Added first cursor library work, read only at the moment and only works
 * with selects with no where clause
 *
 *
 **********************************************************************/

#include "cursorlibrary.h"

SQLRETURN CLError( SQLHENV environment_handle,
           SQLHDBC connection_handle,
           SQLHSTMT statement_handle,
           SQLCHAR *sqlstate,
           SQLINTEGER *native_error,
           SQLCHAR *message_text,
           SQLSMALLINT buffer_length,
           SQLSMALLINT *text_length )
{
    if ( statement_handle )
    {
        CLHSTMT cl_statement = (CLHSTMT) statement_handle; 

        if ( CHECK_SQLERROR( cl_statement -> cl_connection ))
        {
            return SQLERROR( cl_statement -> cl_connection,
               SQL_NULL_HENV,
               SQL_NULL_HDBC,
               cl_statement -> driver_stmt,
               sqlstate,
               native_error,
               message_text,
               buffer_length,
               text_length );
        }
        else
        {
            SQLRETURN ret;

            ret = SQLGETDIAGREC( cl_statement -> cl_connection,
               SQL_HANDLE_STMT,
               cl_statement -> driver_stmt,
               cl_statement -> error_count,
               sqlstate,
               native_error,
               message_text,
               buffer_length,
               text_length );

            if ( SQL_SUCCEEDED( ret ))
            {
               cl_statement -> error_count ++;
            }
            else
            {
               cl_statement -> error_count = 0;
            }

            return ret;
        }
    }
    else if ( connection_handle )
    {
        CLHDBC cl_connection = (CLHDBC) connection_handle; 

        if ( CHECK_SQLERROR( cl_connection ))
        {
            return SQLERROR( cl_connection,
               SQL_NULL_HENV,
               cl_connection -> driver_dbc,
               SQL_NULL_HSTMT,
               sqlstate,
               native_error,
               message_text,
               buffer_length,
               text_length );
        }
        else
        {
            SQLRETURN ret;

            ret = SQLGETDIAGREC( cl_connection,
               SQL_HANDLE_DBC,
               cl_connection -> driver_dbc,
               cl_connection -> error_count,
               sqlstate,
               native_error,
               message_text,
               buffer_length,
               text_length );

            if ( SQL_SUCCEEDED( ret ))
            {
               cl_connection -> error_count ++;
            }
            else
            {
               cl_connection -> error_count = 0;
            }

            return ret;
        }
    }
    else if ( environment_handle )
    {
        /*
         * shouldn't get here
         */

        return SQL_NO_DATA;
    }
}
