/*********************************************************************
 *
 * Written by Nick Gorham
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
 * $Id: _logging.c,v 1.1.1.1 2001/10/17 16:40:30 lurcher Exp $
 *
 * $Log: _logging.c,v $
 * Revision 1.1.1.1  2001/10/17 16:40:30  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.1.1.1  2000/09/04 16:42:53  nick
 * Imported Sources
 *
 * Revision 1.1  1999/07/15 06:23:39  ngorham
 *
 * Added functions to remove the need for _init and _fini
 *
 *
 *********************************************************************/

#include <odbcinstext.h>
#include <log.h>

/*
 * I don't like these statics but not sure what else we can do...
 */

static HLOG hODBCINSTLog = NULL;
static int log_tried = 0;

int inst_logPushMsg( char *pszModule, char *pszFunctionName, int nLine, int nSeverity, int nCode, char *pszMessage )
{
    if ( !log_tried )
    {
        log_tried = 1;
        if ( logOpen( &hODBCINSTLog, "odbcinst", NULL, 10 ) != LOG_SUCCESS )
        {
            hODBCINSTLog = NULL;
        }
        else
        {
            logOn( hODBCINSTLog, 1 );
        }
    }
    if ( hODBCINSTLog )
    {
        return logPushMsg( hODBCINSTLog,
                pszModule, 
                pszFunctionName, 
                nLine, 
                nSeverity, 
                nCode, 
                pszMessage );
    }
    else
    {
        return LOG_ERROR;
    }
}

int inst_logPopMsg( char *pszMsgHdr, int *pnCode, char *pszMsg )
{
    if ( hODBCINSTLog )
    {
        return logPopMsg( hODBCINSTLog,
                pszMsgHdr,
                pnCode,
                pszMsg );
    }
    else
    {
        return LOG_ERROR;
    }
}
