/*********************************************************************
 *
 * This code mimics the AutoTest functions provided by the Microsoft 
 * ODBC test app, in the GTRTST32.DLL library
 *
 * Created by Nick Gorham
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
 * $Id: gtrtst.c,v 1.1.1.1 2001/10/17 16:40:15 lurcher Exp $
 *
 * $Log: gtrtst.c,v $
 * Revision 1.1.1.1  2001/10/17 16:40:15  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.3  2001/04/17 12:57:11  nick
 *
 * Extra for AutoTest
 *
 * Revision 1.2  2001/04/14 10:42:03  nick
 *
 * Extra work on the autotest feature of odbctest
 *
 * Revision 1.1  2001/04/12 17:43:36  nick
 *
 * Change logging and added autotest to odbctest
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <autotest.h>

HINSTANCE hLoadedInst;

typedef int (*CBFN)(lpSERVERINFO,BOOL,BOOL,UINT,LPTSTR,const char*);

BOOL EXTFUNCDECL FAR szLogPrintf( lpSERVERINFO lps, 
                        BOOL fForce, 
                        LPTSTR fmt, 
                        ... )
{
    va_list ap;
    char buffer[ 1024 ];

    /*
     * assemble the buffer
     */

    va_start(ap, fmt);
    vsprintf (buffer, fmt, ap);
    va_end(ap);

    /*
     * the lps->hwnd is not a handle, but a pointer to a callback
     * function in odbctest
     */

    if ( lps->hwnd )
    {
        return ((CBFN)lps->hwnd)(lps,fForce,0,0,NULL,buffer);
    }
    else
    {
        return 0;
    }
}

int EXTFUNCDECL FAR szMessageBox( HWND hwnd, 
                        UINT style, 
                        LPTSTR szTitle, 
                        LPTSTR szFmt, 
                        ... )
{
    char buffer[ 1024 ];
    va_list ap;

    /*
     * assemble the buffer
     */

    va_start(ap, szFmt);
    vsprintf ( buffer, szFmt, ap);
    va_end(ap);

    if ( hwnd )
    {
        /*
         * the hwnd is not a handle, but a pointer to a callback
         * function in odbctest
         */

        return ((CBFN)hwnd)(NULL,0,1,style,szTitle,buffer);
    }
    else
    {
        return 0;
    }
}

/*
 * not sure what this is for yet...
 */

LPTSTR EXTFUN GetRCString( HINSTANCE hInst, 
                        LPTSTR buf, 
                        int cbbuf, 
                        UINT ids )
{
    printf( "GetRCString\n" );
    return NULL;
}
