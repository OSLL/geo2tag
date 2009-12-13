/*********************************************************************
 *
 * Written, as part of unixODBC by Nick Gorham
 * (nick@easysoft.com).
 *
 * copyright (c) 2004 Nick Gorham
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 **********************************************************************/

#include <stdio.h>

static void usage( void )
{
        fprintf( stderr, "Usage: odbc-config\n\t\t[--prefix]\n\t\t[--exec-prefix]\n\t\t[--include-prefix]\n\t\t[--lib-prefix]\n\t\t[--bin-prefix]\n\t\t[--version]\n\t\t[--libs]\n\t\t[--static-libs]\n\t\t[--libtool-libs]\n\t\t[--cflags]\n\t\t[--odbcversion]\n\t\t[--longodbcversion]\n\t\t[--odbcini]\n\t\t[--odbcinstini]\n" );

}

static void cflags( void )
{
#ifdef HAVE_UNISTD_H
    printf( "-DHAVE_UNISTD_H " );
#endif

#ifdef HAVE_PWD_H
    printf( "-DHAVE_PWD_H " );
#endif

#ifdef HAVE_SYS_TYPES_H
    printf( "-DHAVE_SYS_TYPES_H " );
#endif

#ifdef ODBC_STD
    printf( "-DODBC_STD " );
#endif

#ifdef UNICODE
    printf( "-DUNICODE " );
#endif

#ifdef GUID_DEFINED
    printf( "-DGUID_DEFINED " );
#endif

#ifdef SQL_WCHART_CONVERT
    printf( "-DSQL_WCHART_CONVERT " );
#endif

#ifdef HAVE_LONG_LONG
    printf( "-DHAVE_LONG_LONG " );
#endif

#ifdef ODBCINT64
    printf( "-DODBCINT64 " );
#endif

#ifdef UODBCINT64
    printf( "UODBCINT64 " );
#endif

#ifdef DISABLE_INI_CACHING
    printf( "-DDISABLE_INI_CACHING " );
#endif

#ifdef SIZEOF_LONG
    printf( "-DSIZEOF_LONG=%d ", SIZEOF_LONG );
#endif

#ifdef ALLREADY_HAVE_WINDOWS_TYPE
    printf( "-DALLREADY_HAVE_WINDOWS_TYPE " );
#endif

#ifdef DONT_TD_VOID
    printf( "-DDONT_TD_VOID " );
#endif

#ifdef DO_YOU_KNOW_WHAT_YOUR_ARE_DOING
    printf( "-DDO_YOU_KNOW_WHAT_YOUR_ARE_DOING " );
#endif

    printf( "\n" );
}

int main( int argc, char **argv )
{
    int i;

    if ( argc < 2 )
    {
        usage();
        exit( -1 );

    }

    for ( i = 1; i < argc; i ++ )
    {
        if ( strcmp( argv[ i ], "--prefix" ) == 0 )
        {
            printf( "%s\n", PREFIX );
        }
        else if ( strcmp( argv[ i ], "--exec-prefix" ) == 0 )
        {
            printf( "%s\n", EXEC_PREFIX );
        }
        else if ( strcmp( argv[ i ], "--bin-prefix" ) == 0 )
        {
            printf( "%s\n", BIN_PREFIX );
        }
        else if ( strcmp( argv[ i ], "--include-prefix" ) == 0 )
        {
            printf( "%s\n", INCLUDE_PREFIX );
        }
        else if ( strcmp( argv[ i ], "--lib-prefix" ) == 0 )
        {
            printf( "%s\n", LIB_PREFIX );
        }
        else if ( strcmp( argv[ i ], "--version" ) == 0 )
        {
            printf( "%s\n", VERSION );
        }
        else if ( strcmp( argv[ i ], "--libs" ) == 0 )
        {
            printf( "-L%s -lodbc\n", LIB_PREFIX );
        }
        else if ( strcmp( argv[ i ], "--static-libs" ) == 0 )
        {
            printf( "%s/libodbc.a\n", LIB_PREFIX );
        }
        else if ( strcmp( argv[ i ], "--libtool-libs" ) == 0 )
        {
            printf( "%s/libodbc.la\n", LIB_PREFIX );
        }
        else if ( strcmp( argv[ i ], "--cflags" ) == 0 )
        {
            cflags();
        }
        else if ( strcmp( argv[ i ], "--odbcversion" ) == 0 )
        {
            printf( "3\n" );
        }
        else if ( strcmp( argv[ i ], "--longodbcversion" ) == 0 )
        {
            printf( "3.52\n" );
        }
        else if ( strcmp( argv[ i ], "--odbcini" ) == 0 )
        {
            printf( "%s/odbc.ini\n", SYSTEM_FILE_PATH );
        }
        else if ( strcmp( argv[ i ], "--odbcinstini" ) == 0 )
        {
            printf( "%s/odbcinst.ini\n", SYSTEM_FILE_PATH );
        }
        else
        {
            usage();
            exit( -1 );
        }
    }
}
