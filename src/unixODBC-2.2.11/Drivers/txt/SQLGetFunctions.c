/*********************************************************************
 *
 * Added By Nick Gorham
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
 * $Id: SQLGetFunctions.c,v 1.1.1.1 2001/10/17 16:40:13 lurcher Exp $
 *
 * $Log: SQLGetFunctions.c,v $
 * Revision 1.1.1.1  2001/10/17 16:40:13  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.1  2001/04/23 17:42:22  nick
 *
 * Add Drivers/txt/SQLGetFunctions.c
 *
 * Revision 1.2  2001/04/12 17:43:36  nick
 *
 *********************************************************************/

#include "driver.h"

static int supported_functions[] = 
{
    SQL_API_SQLALLOCENV,
    SQL_API_SQLALLOCCONNECT,
    SQL_API_SQLALLOCSTMT,
    SQL_API_SQLFREEENV,
    SQL_API_SQLFREECONNECT,
    SQL_API_SQLGETSTMTOPTION,
    SQL_API_SQLSETSTMTOPTION,
    SQL_API_SQLGETCONNECTOPTION,
    SQL_API_SQLSETCONNECTOPTION,

    SQL_API_SQLALLOCHANDLE,
    SQL_API_SQLBINDCOL,
    SQL_API_SQLBINDPARAMETER,       
    SQL_API_SQLCANCEL,
    SQL_API_SQLCLOSECURSOR,
    SQL_API_SQLCOLATTRIBUTE,
    SQL_API_SQLCOLUMNPRIVILEGES,
    SQL_API_SQLCOLUMNS,
    SQL_API_SQLCONNECT,
    SQL_API_SQLDRIVERCONNECT,
    SQL_API_SQLDESCRIBECOL,
    SQL_API_SQLDESCRIBEPARAM,
    SQL_API_SQLDISCONNECT,
    SQL_API_SQLENDTRAN,
    SQL_API_SQLEXECDIRECT,
    SQL_API_SQLEXECUTE,
    SQL_API_SQLEXTENDEDFETCH,
    SQL_API_SQLFETCH,
    SQL_API_SQLFETCHSCROLL,
    /* SQL_API_SQLFOREIGNKEYS, not at the moment */
    SQL_API_SQLFREEHANDLE,
    SQL_API_SQLFREESTMT,
    SQL_API_SQLGETCONNECTATTR,
    SQL_API_SQLGETCURSORNAME,
    SQL_API_SQLGETDATA,
    SQL_API_SQLGETDESCFIELD,
    SQL_API_SQLGETDESCREC,
    SQL_API_SQLGETDIAGFIELD,
    SQL_API_SQLGETDIAGREC,
    SQL_API_SQLGETENVATTR,
    SQL_API_SQLGETFUNCTIONS,
    SQL_API_SQLGETINFO,             
    SQL_API_SQLGETSTMTATTR,
    SQL_API_SQLGETTYPEINFO,
    SQL_API_SQLMORERESULTS,
    SQL_API_SQLNATIVESQL,
    SQL_API_SQLNUMPARAMS,
    SQL_API_SQLNUMRESULTCOLS,
    SQL_API_SQLPROCEDURES,
    SQL_API_SQLPROCEDURECOLUMNS,
    SQL_API_SQLPARAMDATA,
    SQL_API_SQLPUTDATA,
    SQL_API_SQLPREPARE,
    SQL_API_SQLPRIMARYKEYS,
    SQL_API_SQLROWCOUNT,            
    SQL_API_SQLSETDESCFIELD,
    SQL_API_SQLSETDESCREC,
    SQL_API_SQLSETCONNECTATTR,
    SQL_API_SQLSETCURSORNAME,
    SQL_API_SQLSETENVATTR, 
    SQL_API_SQLSETSTMTATTR, 
    SQL_API_SQLSPECIALCOLUMNS,
    SQL_API_SQLSTATISTICS,
    SQL_API_SQLTABLEPRIVILEGES,
    SQL_API_SQLTABLES,
};

SQLRETURN SQL_API SQLGetFunctions(
    SQLHDBC            ConnectionHandle,
    SQLUSMALLINT    FunctionId,
    SQLUSMALLINT    *SupportedPtr )
{
    int i;

    if ( FunctionId == SQL_API_ODBC3_ALL_FUNCTIONS )
    {
        for ( i = 0; i < SQL_API_ODBC3_ALL_FUNCTIONS_SIZE; i ++ )
        {
            SupportedPtr[ i ] = 0x0000;
        }
        for ( i = 0; i < sizeof( supported_functions ) / sizeof( supported_functions[ 0 ] ); i ++ )
        {
        int id = supported_functions[ i ];
    
            SupportedPtr[ id >> 4 ] |= ( 1 << ( id & 0x000F ));
        }
    }
    else if ( FunctionId == SQL_API_ALL_FUNCTIONS )
    {
        for ( i = 0; i < 100; i ++ )
        {
            SupportedPtr[ i ] = SQL_FALSE;
        }
        for ( i = 0; i < sizeof( supported_functions ) / sizeof( supported_functions[ 0 ] ); i ++ )
        {
            if ( supported_functions[ i ] < 100 )
            {
                SupportedPtr[ supported_functions[ i ]] = SQL_TRUE;
            }
        }
    }
    else
    {
        *SupportedPtr = SQL_FALSE;
        for ( i = 0; i < sizeof( supported_functions ) / sizeof( supported_functions[ 0 ] ); i ++ )
        {
            if ( supported_functions[ i ] == FunctionId )
            {
                *SupportedPtr = SQL_TRUE;
                break;
            }
        }
    }

    return SQL_SUCCESS;
}
