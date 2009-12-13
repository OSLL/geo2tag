/*********************************************************************
 *
 * This is based on code created by Peter Harvey,
 * (pharvey@codebydesign.com).
 *
 * Modified and extended by Nick Gorham
 * (nick@easysoft.com).
 *
 * Any bugs or problems should be considered the fault of Nick and not
 * Peter.
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
 * $Id: SQLSetStmtAttrW.c,v 1.5 2003/10/30 18:20:46 lurcher Exp $
 *
 * $Log: SQLSetStmtAttrW.c,v $
 * Revision 1.5  2003/10/30 18:20:46  lurcher
 *
 * Fix broken thread protection
 * Remove SQLNumResultCols after execute, lease S4/S% to driver
 * Fix string overrun in SQLDriverConnect
 * Add initial support for Interix
 *
 * Revision 1.4  2003/03/05 09:48:45  lurcher
 *
 * Add some 64 bit fixes
 *
 * Revision 1.3  2002/12/05 17:44:31  lurcher
 *
 * Display unknown return values in return logging
 *
 * Revision 1.2  2002/05/28 13:30:34  lurcher
 *
 * Tidy up for AIX
 *
 * Revision 1.1.1.1  2001/10/17 16:40:07  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.4  2001/08/08 17:05:17  nick
 *
 * Add support for attribute setting in the ini files
 *
 * Revision 1.3  2001/07/03 09:30:41  nick
 *
 * Add ability to alter size of displayed message in the log
 *
 * Revision 1.2  2001/04/12 17:43:36  nick
 *
 * Change logging and added autotest to odbctest
 *
 * Revision 1.1  2000/12/31 20:30:54  nick
 *
 * Add UNICODE support
 *
 *
 *
 **********************************************************************/

#include "drivermanager.h"

static char const rcsid[]= "$RCSfile: SQLSetStmtAttrW.c,v $";

SQLRETURN SQLSetStmtAttrW( SQLHSTMT statement_handle,
           SQLINTEGER attribute,
           SQLPOINTER value,
           SQLINTEGER string_length )
{
    DMHSTMT statement = (DMHSTMT) statement_handle;
    SQLRETURN ret;
    SQLCHAR s1[ 100 + LOG_MESSAGE_LEN ];

    /*
     * check statement
     */

    if ( !__validate_stmt( statement ))
    {
        dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: SQL_INVALID_HANDLE" );

        return SQL_INVALID_HANDLE;
    }

    function_entry( statement );

    if ( log_info.log_flag )
    {
        sprintf( statement -> msg, "\n\t\tEntry:\
            \n\t\t\tStatement = %p\
            \n\t\t\tAttribute = %s\
            \n\t\t\tValue = %p\
            \n\t\t\tStrLen = %d",
                statement,
                __stmt_attr_as_string( s1, attribute ),
                value, 
                (int)string_length );

        dm_log_write( __FILE__, 
                __LINE__, 
                LOG_INFO, 
                LOG_INFO, 
                statement -> msg );
    }

    thread_protect( SQL_HANDLE_STMT, statement );

    /*
     * check states
     */

    if ( attribute == SQL_ATTR_CONCURRENCY ||
            attribute == SQL_ATTR_CURSOR_TYPE ||
            attribute == SQL_ATTR_SIMULATE_CURSOR ||
            attribute == SQL_ATTR_USE_BOOKMARKS ||
            attribute == SQL_ATTR_CURSOR_SCROLLABLE ||
            attribute == SQL_ATTR_CURSOR_SENSITIVITY )
    {
        if ( statement -> state == STATE_S2 ||
                statement -> state == STATE_S3 )
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: HY011" );

            __post_internal_error( &statement -> error,
                 ERROR_HY011, NULL,
                 statement -> connection -> environment -> requested_version );

            return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
        }
        else if ( statement -> state == STATE_S4 ||
                statement -> state == STATE_S5 ||
                statement -> state == STATE_S6 ||
                statement -> state == STATE_S7 )
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: 24000" );

            __post_internal_error( &statement -> error,
                 ERROR_24000, NULL,
                 statement -> connection -> environment -> requested_version );

            return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
        }
        else if ( statement -> state == STATE_S8 ||
                statement -> state == STATE_S9 ||
                statement -> state == STATE_S10 ||
                statement -> state == STATE_S11 ||
                statement -> state == STATE_S12 )
        {
            if ( statement -> prepared )
            {
                dm_log_write( __FILE__, 
                        __LINE__, 
                        LOG_INFO, 
                        LOG_INFO, 
                        "Error: HY011" );

                __post_internal_error( &statement -> error,
                     ERROR_HY011, NULL,
                     statement -> connection -> environment -> requested_version );

                return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
            }
            else
            {
                dm_log_write( __FILE__, 
                        __LINE__, 
                        LOG_INFO, 
                        LOG_INFO, 
                        "Error: HY010" );

                __post_internal_error( &statement -> error,
                     ERROR_HY010, NULL,
                     statement -> connection -> environment -> requested_version );

                return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
            }
        }
    }
    else
    {
        if ( statement -> state == STATE_S8 ||
                statement -> state == STATE_S9 ||
                statement -> state == STATE_S10 ||
                statement -> state == STATE_S11 ||
                statement -> state == STATE_S12 )
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: HY010" );

            __post_internal_error( &statement -> error,
                 ERROR_HY010, NULL,
                 statement -> connection -> environment -> requested_version );

            return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
        }
    }

    if ( !CHECK_SQLSETSTMTATTRW( statement -> connection ))
    {
        dm_log_write( __FILE__, 
                __LINE__, 
                LOG_INFO, 
                LOG_INFO, 
                "Error: IM001" );

        __post_internal_error( &statement -> error,
                ERROR_IM001, NULL,
                statement -> connection -> environment -> requested_version );

        return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
    }

    /*
     * map descriptors to our copies
     */

    if ( attribute == SQL_ATTR_APP_ROW_DESC )
    {
        DMHDESC desc = ( DMHDESC ) value;

        if ( !__validate_desc( desc ))
        {
            thread_release( SQL_HANDLE_STMT, statement );

            return SQL_INVALID_HANDLE;
        }

        if ( desc -> implicit &&
                desc != statement -> implicit_ard )
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: HY017" );

            __post_internal_error( &statement -> error,
                    ERROR_HY017, NULL,
                    statement -> connection -> environment -> requested_version );

            return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
        }

        if ( desc -> connection !=
                statement -> connection )
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: HY024" );

            __post_internal_error( &statement -> error,
                    ERROR_HY024, NULL,
                    statement -> connection -> environment -> requested_version );

            return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
        }

        /*
         * set the value to the driver descriptor handle
         */
        value = ( SQLPOINTER ) desc -> driver_desc;
        statement -> ard = desc;
    }

    if ( attribute == SQL_ATTR_APP_PARAM_DESC )
    {
        DMHDESC desc = ( DMHDESC ) value;

        if ( !__validate_desc( desc ))
        {
            sprintf( statement -> msg, 
                    "\n\t\tExit:[%s]",
                    __get_return_status( SQL_INVALID_HANDLE, s1 ));

            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    statement -> msg );

            thread_release( SQL_HANDLE_STMT, statement );

            return SQL_INVALID_HANDLE;
        }

        if ( desc -> implicit &&
                desc != statement -> implicit_apd )
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: HY017" );

            __post_internal_error( &statement -> error,
                    ERROR_HY017, NULL,
                    statement -> connection -> environment -> requested_version );

            return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
        }

        if ( desc -> connection !=
                statement -> connection )
        {
            dm_log_write( __FILE__, 
                    __LINE__, 
                    LOG_INFO, 
                    LOG_INFO, 
                    "Error: HY024" );

            __post_internal_error( &statement -> error,
                    ERROR_HY024, NULL,
                    statement -> connection -> environment -> requested_version );

            return function_return( SQL_HANDLE_STMT, statement, SQL_ERROR );
        }

        /*
         * set the value to the driver descriptor handle
         */
        value = ( SQLPOINTER ) desc -> driver_desc;
        statement -> apd = desc;
    }

    /*
     * save for internal use
     */

    if ( attribute == SQL_ATTR_METADATA_ID )
    {
        statement -> metadata_id = (long int) value;
    }

    /*
     * is it something overridden
     */

    value = __attr_override( statement, SQL_HANDLE_STMT, attribute, value, &string_length );

    /*
     * does the call need mapping from 3 to 2
     */

    if ( attribute == SQL_ATTR_FETCH_BOOKMARK_PTR &&
            statement -> connection -> driver_act_ver == SQL_OV_ODBC2 &&
            CHECK_SQLEXTENDEDFETCH( statement -> connection ) &&
            !CHECK_SQLFETCHSCROLL( statement -> connection ))
    {
        statement -> fetch_bm_ptr = (SQLUINTEGER*) value;
        ret = SQL_SUCCESS;
    }
    else if ( attribute == SQL_ATTR_ROW_STATUS_PTR &&
	      statement -> connection -> driver_act_ver == SQL_OV_ODBC2 )
    {
        statement -> row_st_arr = (SQLUSMALLINT*) value;
        ret = SQL_SUCCESS;
    }
    else if ( attribute == SQL_ATTR_ROWS_FETCHED_PTR &&
	      statement -> connection -> driver_act_ver == SQL_OV_ODBC2 )
    {
        statement -> row_ct_ptr = (SQLUINTEGER*) value;
        ret = SQL_SUCCESS;
    }
    else if ( attribute == SQL_ATTR_ROW_ARRAY_SIZE &&
            statement -> connection -> driver_act_ver == SQL_OV_ODBC2 )
    {
        ret = SQLSETSTMTATTRW( statement -> connection,
                statement -> driver_stmt,
                SQL_ROWSET_SIZE,
                value,
                string_length );
    }
    else 
    {
        ret = SQLSETSTMTATTRW( statement -> connection,
                statement -> driver_stmt,
                attribute,
                value,
                string_length );
    }

    /*
     * take notice of this
     */

    if ( attribute == SQL_ATTR_USE_BOOKMARKS && SQL_SUCCEEDED( ret ))
    {
        statement -> bookmarks_on = (SQLULEN) value;
    }

    if ( log_info.log_flag )
    {
        sprintf( statement -> msg, 
                "\n\t\tExit:[%s]",
                    __get_return_status( ret, s1 ));

        dm_log_write( __FILE__, 
                __LINE__, 
                LOG_INFO, 
                LOG_INFO, 
                statement -> msg );
    }

    return function_return( SQL_HANDLE_STMT, statement, ret ); 
}
