/*********************************************************************
 *
 * Written by Nick Gorham
 * (nick@lurcher.org).
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
 * $Id: stmt.cpp,v 1.4 2002/05/29 10:22:58 lurcher Exp $
 *
 * $Log: stmt.cpp,v $
 * Revision 1.4  2002/05/29 10:22:58  lurcher
 *
 * Changes and tidy's
 *
 * Revision 1.3  2001/12/20 17:26:26  lurcher
 *
 * More warnings removed
 *
 * Revision 1.2  2001/12/20 12:30:44  lurcher
 *
 * Fix 64 problem in odbctest and 7.1 Postgres driver
 *
 * Revision 1.1.1.1  2001/10/17 16:40:31  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.5  2001/10/02 16:32:52  nick
 *
 * Alter unicode TCHAR defs
 *
 * Revision 1.4  2001/07/20 09:42:59  nick
 *
 * Replace char[] with QString to avoid buffer overrun
 *
 * Revision 1.3  2001/05/31 16:05:55  nick
 *
 * Fix problems with postgres closing local sockets
 * Make odbctest build with QT 3 (it doesn't work due to what I think are bugs
 * in QT 3)
 * Fix a couple of problems in the cursor lib
 *
 * Revision 1.2  2000/12/17 10:51:33  nick
 *
 * Add include for sqlucode.h
 *
 * Revision 1.1.1.1  2000/09/04 16:42:53  nick
 * Imported Sources
 *
 * Revision 1.8  2000/06/23 16:23:24  ngorham
 *
 * Couple of small changes
 *
 * Revision 1.7  2000/06/15 09:31:04  ngorham
 *
 * Add some fixes and porting options after 1.8.9 has been released
 *
 * Revision 1.6  2000/06/13 12:30:29  ngorham
 *
 * Enough there for the first release I think
 *
 * Revision 1.5  2000/06/09 17:04:22  ngorham
 *
 * More, and More
 *
 * Revision 1.4  2000/06/07 15:10:23  ngorham
 *
 * More additions
 *
 * Revision 1.3  2000/06/07 08:29:48  ngorham
 *
 * More additions
 *
 * Revision 1.2  2001/05/31 10:26:27  ngorham
 *
 * Fix a few minor typo's
 *
 * Revision 1.1  2000/05/04 17:04:48  ngorham
 *
 * Initial commit
 *
 *
 **********************************************************************/

#include <qapplication.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>
#include <stdlib.h>
#include "stmt.h"
#include "odbctest.h"

static attr_value io_type_option[] = 
{
	{ "SQL_PARAM_INPUT", SQL_PARAM_INPUT, "2.0" },
	{ "SQL_PARAM_INPUT_OUTPUT", SQL_PARAM_INPUT_OUTPUT, "2.0" },
	{ "SQL_PARAM_INPUT", SQL_PARAM_OUTPUT, "2.0" },
	{ NULL }
};

static attr_value value_type_option[] = 
{
	{ "SQL_C_CHAR", SQL_C_CHAR, "1.0" },
	{ "SQL_C_BINARY", SQL_C_BINARY, "1.0" },
	{ "SQL_C_BIT", SQL_C_BIT, "1.0" },
	{ "SQL_C_BOOKMARK", SQL_C_BOOKMARK, "2.0" },
	{ "SQL_C_DATE", SQL_C_DATE, "1.0" },
	{ "SQL_C_DOUBLE", SQL_C_DOUBLE, "1.0" },
	{ "SQL_C_FLOAT", SQL_C_FLOAT, "1.0" },
	{ "SQL_C_GUID", SQL_C_GUID, "1.0" },
	{ "SQL_C_INTERVAL_DAY", SQL_C_INTERVAL_DAY, "3.0" },
	{ "SQL_C_INTERVAL_DAY_TO_HOUR", SQL_C_INTERVAL_DAY_TO_HOUR, "3.0" },
	{ "SQL_C_INTERVAL_DAY_TO_MINUTE", SQL_C_INTERVAL_DAY_TO_MINUTE, "3.0" },
	{ "SQL_C_INTERVAL_DAY_TO_SECOND", SQL_C_INTERVAL_DAY_TO_SECOND, "3.0" },
	{ "SQL_C_INTERVAL_HOUR", SQL_C_INTERVAL_HOUR, "3.0" },
	{ "SQL_C_INTERVAL_HOUR_TO_MINUTE", SQL_C_INTERVAL_HOUR_TO_MINUTE, "3.0" },
	{ "SQL_C_INTERVAL_HOUR_TO_SECOND", SQL_C_INTERVAL_HOUR_TO_SECOND, "3.0" },
	{ "SQL_C_INTERVAL_MINUTE", SQL_C_INTERVAL_MINUTE, "3.0" },
    { "SQL_C_INTERVAL_MINUTE_TO_SECOND", SQL_C_INTERVAL_MINUTE_TO_SECOND, "3.0" },
	{ "SQL_C_INTERVAL_SECOND", SQL_C_INTERVAL_SECOND, "3.0" },
	{ "SQL_C_INTERVAL_YEAR", SQL_C_INTERVAL_YEAR, "3.0" },
	{ "SQL_C_INTERVAL_MONTH", SQL_C_INTERVAL_MONTH, "3.0" },
	{ "SQL_C_INTERVAL_YEAR_TO_MONTH", SQL_C_INTERVAL_YEAR_TO_MONTH, "3.0" },
	{ "SQL_C_LONG", SQL_C_LONG, "2.0" },
	{ "SQL_C_NUMERIC", SQL_C_NUMERIC, "3.0" },
	{ "SQL_C_SBIGINT", SQL_C_SBIGINT, "3.0" },
	{ "SQL_C_SLONG", SQL_C_SLONG, "2.0" },
	{ "SQL_C_SHORT", SQL_C_SHORT, "2.0" },
	{ "SQL_C_SSHORT", SQL_C_SSHORT, "2.0" },
	{ "SQL_C_STINYINT", SQL_C_STINYINT, "2.0" },
	{ "SQL_C_TIME", SQL_C_TIME, "1.0" },
	{ "SQL_C_TIMESTAMP", SQL_C_TIMESTAMP, "1.0" },
	{ "SQL_C_TINYINT", SQL_C_TINYINT, "1.0" },
	{ "SQL_C_TYPE_DATE", SQL_C_TYPE_DATE, "3.0" },
	{ "SQL_C_TYPE_TIME", SQL_C_TYPE_TIME, "3.0" },
	{ "SQL_C_TYPE_TIMESTAMP", SQL_C_TYPE_TIMESTAMP, "3.0" },
	{ "SQL_C_UBIGINT", SQL_C_UBIGINT, "2.0" },
	{ "SQL_C_USHORT", SQL_C_USHORT, "2.0" },
    { "SQL_C_UTINYINT", SQL_C_UTINYINT, "2.0" },
	{ "SQL_C_VARBOOKMARK", SQL_C_VARBOOKMARK, "3.0" },
	{ "SQL_C_WCHAR", SQL_C_WCHAR, "3.0" },
	{ "SQL_ARD_TYPE", SQL_ARD_TYPE, "3.0" },
	{ "SQL_C_DEFAULT", SQL_C_DEFAULT, "1.0" },
	{ NULL }
};

static attr_value param_type_option[] = 
{
	{ "SQL_CHAR", SQL_CHAR, "1.0" },
	{ "SQL_BIGINT", SQL_BIGINT, "1.0" },
	{ "SQL_BINARY", SQL_BINARY, "1.0" },
	{ "SQL_BIT", SQL_BIT, "1.0" },
	{ "SQL_DATE", SQL_DATE, "1.0" },
	{ "SQL_DECIMAL", SQL_DECIMAL, "1.0" },
	{ "SQL_DEFAULT", SQL_DECIMAL, "3.0" },
	{ "SQL_DOUBLE", SQL_DOUBLE, "1.0" },
	{ "SQL_FLOAT", SQL_FLOAT, "1.0" },
	{ "SQL_GUID", SQL_GUID, "1.0" },
	{ "SQL_INTEGER", SQL_INTEGER, "1.0" },
	{ "SQL_INTERVAL_DAY", SQL_INTERVAL_DAY, "3.0" },
	{ "SQL_INTERVAL_DAY_TO_HOUR", SQL_INTERVAL_DAY_TO_HOUR, "3.0" },
	{ "SQL_INTERVAL_DAY_TO_MINUTE", SQL_INTERVAL_DAY_TO_MINUTE, "3.0" },
	{ "SQL_INTERVAL_DAY_TO_SECOND", SQL_INTERVAL_DAY_TO_SECOND, "3.0" },
	{ "SQL_INTERVAL_HOUR", SQL_INTERVAL_HOUR, "3.0" },
	{ "SQL_INTERVAL_HOUR_TO_MINUTE", SQL_INTERVAL_HOUR_TO_MINUTE, "3.0" },
	{ "SQL_INTERVAL_HOUR_TO_SECOND", SQL_INTERVAL_HOUR_TO_SECOND, "3.0" },
	{ "SQL_INTERVAL_MINUTE", SQL_INTERVAL_MINUTE, "3.0" },
    { "SQL_INTERVAL_MINUTE_TO_SECOND", SQL_INTERVAL_MINUTE_TO_SECOND, "3.0" },
	{ "SQL_INTERVAL_SECOND", SQL_INTERVAL_SECOND, "3.0" },
	{ "SQL_INTERVAL_YEAR", SQL_INTERVAL_YEAR, "3.0" },
	{ "SQL_INTERVAL_MONTH", SQL_INTERVAL_MONTH, "3.0" },
	{ "SQL_INTERVAL_YEAR_TO_MONTH", SQL_INTERVAL_YEAR_TO_MONTH, "3.0" },
	{ "SQL_LONGVARCHAR", SQL_LONGVARCHAR, "1.0" },
	{ "SQL_LONGVARBINARY", SQL_LONGVARBINARY, "1.0" },
	{ "SQL_NUMERIC", SQL_NUMERIC, "1.0" },
	{ "SQL_REAL", SQL_REAL, "1.0" },
	{ "SQL_SMALLINT", SQL_SMALLINT, "1.0" },
	{ "SQL_TINYINT", SQL_TINYINT, "1.0" },
	{ "SQL_TIME", SQL_TIME, "1.0" },
	{ "SQL_TIMESTAMP", SQL_TIMESTAMP, "1.0" },
	{ "SQL_TYPE_DATE", SQL_TYPE_DATE, "3.0" },
	{ "SQL_TYPE_TIME", SQL_TYPE_TIME, "3.0" },
	{ "SQL_TYPE_TIMESTAMP", SQL_TYPE_TIMESTAMP, "3.0" },
	{ "SQL_VARBINARY", SQL_VARBINARY, "1.0" },
	{ "SQL_VARCHAR", SQL_VARCHAR, "1.0" },
	{ "SQL_WCHAR", SQL_WCHAR, "3.0" },
	{ "SQL_WLONGVARCHAR", SQL_WLONGVARCHAR, "3.0" },
	{ "SQL_WVARCHAR", SQL_WVARCHAR, "3.0" },
	{ NULL }
};

static attr_value free_stmt_option[] = 
{
	{ "SQL_CLOSE", SQL_CLOSE },
	{ "SQL_DROP", SQL_DROP },
	{ "SQL_UNBIND", SQL_UNBIND },
	{ "SQL_RESET_PARAMS", SQL_RESET_PARAMS },
	{ NULL }
};

static attr_value sql_data_types[] = 
{
	{ "SQL_CHAR", SQL_CHAR, "1.0" },
	{ "SQL_VARCHAR", SQL_VARCHAR, "1.0" },
	{ "SQL_LONGVARCHAR", SQL_LONGVARCHAR, "1.0" },
	{ "SQL_BINARY", SQL_BINARY, "1.0" },
	{ "SQL_VARBINARY", SQL_VARBINARY, "1.0" },
	{ "SQL_LONGVARBINARY", SQL_LONGVARBINARY, "1.0" },
	{ "SQL_TINYINT", SQL_TINYINT, "1.0" },
	{ "SQL_SMALLINT", SQL_SMALLINT, "1.0" },
	{ "SQL_INTEGER", SQL_INTEGER, "1.0" },
	{ "SQL_BIGINT", SQL_BIGINT, "1.0" },
	{ "SQL_FLOAT", SQL_FLOAT, "1.0" },
	{ "SQL_DOUBLE", SQL_DOUBLE, "1.0" },
	{ "SQL_REAL", SQL_REAL, "1.0" },
	{ "SQL_NUMERIC", SQL_NUMERIC, "1.0" },
	{ "SQL_DATETIME", SQL_DATETIME, "1.0" },
	{ "SQL_DATE", SQL_DATE, "1.0" },
	{ "SQL_TIME", SQL_TIME, "1.0" },
	{ "SQL_TIMESTAMP", SQL_TIMESTAMP, "1.0" },
	{ "SQL_INTERVAL_DAY", SQL_INTERVAL_DAY, "3.0" },
	{ "SQL_INTERVAL_DAY_TO_HOUR", SQL_INTERVAL_DAY_TO_HOUR, "3.0" },
	{ "SQL_INTERVAL_DAY_TO_MINUTE", SQL_INTERVAL_DAY_TO_MINUTE, "3.0" },
	{ "SQL_INTERVAL_DAY_TO_SECOND", SQL_INTERVAL_DAY_TO_SECOND, "3.0" },
	{ "SQL_INTERVAL_HOUR", SQL_INTERVAL_HOUR, "3.0" },
	{ "SQL_INTERVAL_HOUR_TO_MINUTE", SQL_INTERVAL_HOUR_TO_MINUTE, "3.0" },
	{ "SQL_INTERVAL_HOUR_TO_SECOND", SQL_INTERVAL_HOUR_TO_SECOND, "3.0" },
	{ "SQL_INTERVAL_MINUTE_TO_SECOND", SQL_INTERVAL_MINUTE_TO_SECOND, "3.0" },
	{ "SQL_INTERVAL_MONTH", SQL_INTERVAL_MONTH, "3.0" },
	{ "SQL_INTERVAL_MINUTE", SQL_INTERVAL_MINUTE, "3.0" },
	{ "SQL_INTERVAL_SECOND", SQL_INTERVAL_SECOND, "3.0" },
	{ "SQL_INTERVAL_YEAR", SQL_INTERVAL_YEAR, "3.0" },
	{ "SQL_INTERVAL_YEAR_TO_MONTH", SQL_INTERVAL_YEAR_TO_MONTH, "3.0" },
	{ "SQL_GUID", SQL_GUID, "3.0" },
	{ NULL }
};

void dPrepare::Ok()
{
	const char *sql, *lname;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLINTEGER n_len;

	if ( hand )
		in_handle = hand -> getHandle();

	sql = str -> currentText();

	odbctest -> out_win -> insertLineLimited( "SQLPrepare():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( sql, "<null ptr>" ) == 0 )
	{
		sql = NULL;
		odbctest -> out_win -> insertLineLimited( "    Text: <null ptr>" );
	}
	else if ( strcmp( sql, "<empty string>" ) == 0 )
	{
		sql = "";
		odbctest -> out_win -> insertLineLimited( "    Text: <empty string>" );
	}
	else if ( strcmp( sql, "<input window>" ) == 0 )
	{
		QString *s = new QString( odbctest->in_win->text());
		if ( s -> isEmpty())
			sql = "";
		else if ( s -> isNull())
			sql = NULL;
		else 
			sql = s->latin1();
		txt.sprintf( "    Text: %s", sql );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Text: %s", sql );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	lname = name_len -> currentText();

	if ( strncmp( lname, "SQL_NTS", 7 ) == 0 )
	{
		n_len = SQL_NTS;
		txt.sprintf( "    Statement Len: SQL_NTS=-3" );
	}
	else
	{
		n_len = atoi( lname );
		txt.sprintf( "    Statement Len: %d", n_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLPrepare( in_handle, (SQLCHAR*) sql, n_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dPrepare::dPrepare( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 190,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 270,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 350,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	str = new QComboBox( TRUE, this, "Input Text" );
	str -> setGeometry( 130, 80, 290, 20 );
	str -> insertItem( "<input window>", 0 );
	str -> insertItem( "<empty string>", 1 );
	str -> insertItem( "<null ptr>", 2 );

	name_len = new QComboBox( TRUE, this, "name len" );
	name_len -> setGeometry( 130, 110, 150, 20 );
	l_name_len = new QLabel( "Statement Length:", this );
	l_name_len -> setGeometry( 10, 110, 110, 20 );
	name_len -> insertItem( "SQL_NTS=-3", 0 );
	name_len -> insertItem( "0", 1 );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	l_str = new QLabel( "SQL Text:", this );
	l_str -> setGeometry( 10, 80, 70, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dPrepare::~dPrepare()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete str;
	delete l_handle;
	delete l_str;
	delete name_len;
	delete l_name_len;
}

void dExecute::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLExecute():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLExecute( in_handle );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dExecute::dExecute( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 90,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 170,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 250,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 190, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dExecute::~dExecute()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handle;
}

void dExecDirect::Ok()
{
	const char *sql, *lname;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLINTEGER n_len;

	if ( hand )
		in_handle = hand -> getHandle();

	sql = str -> currentText();

	odbctest -> out_win -> insertLineLimited( "SQLExecDirect():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( sql, "<null ptr>" ) == 0 )
	{
		sql = NULL;
		odbctest -> out_win -> insertLineLimited( "    Text: <null ptr>" );
	}
	else if ( strcmp( sql, "<empty string>" ) == 0 )
	{
		sql = "";
		odbctest -> out_win -> insertLineLimited( "    Text: <empty string>" );
	}
	else if ( strcmp( sql, "<input window>" ) == 0 )
	{
		QString *s = new QString( odbctest->in_win->text());
		if ( s -> isEmpty())
			sql = "";
		else if ( s -> isNull())
			sql = NULL;
		else 
			sql = s->latin1();
		txt.sprintf( "    Text: %s", sql );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Text: %s", sql );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	lname = name_len -> currentText();

	if ( strncmp( lname, "SQL_NTS", 7 ) == 0 )
	{
		n_len = SQL_NTS;
		txt.sprintf( "    Statement Len: SQL_NTS=-3" );
	}
	else
	{
		n_len = atoi( lname );
		txt.sprintf( "    Statement Len: %d", n_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLExecDirect( in_handle, (SQLCHAR*) sql, n_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dExecDirect::dExecDirect( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 190,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 270,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 350,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	str = new QComboBox( TRUE, this, "Input Text" );
	str -> setGeometry( 130, 80, 290, 20 );
	str -> insertItem( "<input window>", 0 );
	str -> insertItem( "<empty string>", 1 );
	str -> insertItem( "<null ptr>", 2 );

	name_len = new QComboBox( TRUE, this, "name len" );
	name_len -> setGeometry( 130, 110, 150, 20 );
	l_name_len = new QLabel( "Statement Length:", this );
	l_name_len -> setGeometry( 10, 110, 110, 20 );
	name_len -> insertItem( "SQL_NTS=-3", 0 );
	name_len -> insertItem( "0", 1 );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	l_str = new QLabel( "SQL Text:", this );
	l_str -> setGeometry( 10, 80, 70, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dExecDirect::~dExecDirect()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete str;
	delete l_handle;
	delete l_str;
	delete name_len;
	delete l_name_len;
}

void dNumParams::Valid()
{
	if ( valid -> isOn() )
	    valid -> setText( "ParamCountPtr: SQL_NULL_POINTER" );
	else
	    valid -> setText( "ParamCountPtr: VALID" );
}

void dNumParams::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT num_params;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLNumParam:" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( valid -> isOn() )
		txt.sprintf( "    ParamCountPtr: <null pointer>" );
	else
		txt.sprintf( "    ParamCountPtr: %p", &num_params );
	odbctest -> out_win -> insertLineLimited( txt );

	num_params = -9999;

	SQLRETURN ret = SQLNumParams( in_handle,  valid -> isOn() ? NULL : &num_params );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );
	if ( num_params == -9999 )
	{
		txt.sprintf( "    *ParamCountPtr: <unchanged>" );
	}
	else
	{
		txt.sprintf( "    *ParamCountPtr: %d", num_params );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dNumParams::dNumParams( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 100,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 180,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 260,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
    l_handle -> setGeometry( 10, 50, 120, 20 );

	valid = new QCheckBox( "ParamCountPtr: VALID", this );
	valid -> setGeometry( 10, 80, 300, 15 );

	connect( valid, SIGNAL( clicked()), this, SLOT( Valid()));

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dNumParams::~dNumParams()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete valid;
}

void dCancel::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT num_cols;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLCancel():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLCancel( in_handle );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dCancel::dCancel( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 100,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 180,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 260,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
    l_handle -> setGeometry( 10, 50, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dCancel::~dCancel()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
}

void dCloseCursor::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT num_cols;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLCloseCursor():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLCloseCursor( in_handle );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dCloseCursor::dCloseCursor( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 100,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 180,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 260,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
    l_handle -> setGeometry( 10, 50, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dCloseCursor::~dCloseCursor()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
}

void dGetCursorName::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT b_len;
	SQLCHAR *buf;
	SQLSMALLINT name_length, *name_length_ptr;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetCursorName():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	b_len = atoi( buffer_len -> text().ascii());
	if ( b_len < 1 )
	{
		b_len = 0;
	}

	if ( cursor_valid -> isOn())
	{
		buf = NULL;
	}
	else if ( b_len < 300 )
	{
		buf = new SQLCHAR[ 300 ];	
	}
	else
	{
		buf = new SQLCHAR[ b_len ];	
	}

	if ( buf )
	{
		txt.sprintf( "    CursorName: %p", buf );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    CursorName: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	txt.sprintf( "    Buffer Length: %d", b_len );
	odbctest -> out_win -> insertLineLimited( txt );

	name_length = -9999;

	if ( name_valid -> isOn())
	{
		name_length_ptr = NULL;
	}
	else
	{
		name_length_ptr = &name_length;
	}

	if ( name_length_ptr )
	{
		txt.sprintf( "    NameLengthPtr: %p", name_length_ptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    NameLengthPtr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	SQLRETURN ret = SQLGetCursorName( in_handle, buf, b_len, name_length_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( SQL_SUCCEEDED( ret ))
	{
		if ( buf )
		{
			txt.sprintf( "    *CursorName: \"%s\"", buf );
			odbctest -> out_win -> insertLineLimited( txt );
		}
		if ( name_length == -9999 )
		{
			txt.sprintf( "    *NameLengthPtr: <unchanged>" );
		}
		else
		{
			txt.sprintf( "    *NameLengthPtr: %d", name_length );
		}
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( buf )
		delete buf;

	odbctest -> out_win -> insertLineLimited( "" );
}

void dGetCursorName::NameValid()
{
	if ( name_valid -> isOn() )
	    name_valid -> setText( "NameLengthPtr: SQL_NULL_POINTER" );
	else
	    name_valid -> setText( "NameLengthPtr: VALID" );
}

void dGetCursorName::CursorValid()
{
	if ( cursor_valid -> isOn() )
	    cursor_valid -> setText( "CursorName: SQL_NULL_POINTER" );
	else
	    cursor_valid -> setText( "CursorName: VALID" );
}

dGetCursorName::dGetCursorName( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 190,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 270,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 350,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	cursor_valid = new QCheckBox( "CursorName: VALID", this );
	cursor_valid -> setGeometry( 10, 80, 300, 15 );

	name_valid = new QCheckBox( "NameLengthPtr: VALID", this );
	name_valid -> setGeometry( 10, 110, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 350, 80, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "Buffer Len:", this );
    l_buffer_len -> setGeometry( 270, 80, 60, 20 );

	connect( cursor_valid, SIGNAL( clicked()), this, SLOT( CursorValid()));
	connect( name_valid, SIGNAL( clicked()), this, SLOT( NameValid()));

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dGetCursorName::~dGetCursorName()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handle;
	delete l_buffer_len;
	delete buffer_len;
	delete name_valid;
	delete cursor_valid;
}

void dFreeStmt::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLUSMALLINT option_val;
	int index;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLFreeStmt():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	index = option -> currentItem(); 

	option_val = free_stmt_option[ index ].value;
	txt.sprintf( "    Option: %s=%d", free_stmt_option[ index ].text,
		free_stmt_option[ index ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLFreeStmt( in_handle, option_val );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ) && free_stmt_option[ index ].value == SQL_DROP )
    {
		odbctest->listHandle.remove( hand );
    }

	odbctest -> out_win -> insertLineLimited( "" );
}

dFreeStmt::dFreeStmt( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 100,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 180,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 260,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	option = new QComboBox( FALSE, this, "Option" );
	option -> setGeometry( 130, 80, 200, 20 );

	parent->fill_list_box( free_stmt_option, option );

	l_option = new QLabel( "Option:", this );
    l_option -> setGeometry( 10, 80, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dFreeStmt::~dFreeStmt()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete option;
	delete l_handle;
	delete l_option;
}

void dSetCursorName::Ok()
{
	const char *sql, *lname;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT clen;
	SQLINTEGER n_len;

	if ( hand )
		in_handle = hand -> getHandle();

	sql = str -> currentText();

	odbctest -> out_win -> insertLineLimited( "SQLSetCursorName():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( sql, "<null ptr>" ) == 0 )
	{
		sql = NULL;
		odbctest -> out_win -> insertLineLimited( "    CursorName: <null ptr>" );
	}
	else if ( strcmp( sql, "<empty string>" ) == 0 )
	{
		sql = "";
		odbctest -> out_win -> insertLineLimited( "    CursorName: <empty string>" );
	}
	else if ( strcmp( sql, "<input window>" ) == 0 )
	{
		QString *s = new QString( odbctest->in_win->text());
		if ( s -> isEmpty())
			sql = "";
		else if ( s -> isNull())
			sql = NULL;
		else 
			sql = s->latin1();
		txt.sprintf( "    CursorName: %s", sql );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    CursorName: %s", sql );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	lname = name_len -> currentText();

	if ( strncmp( lname, "SQL_NTS", 7 ) == 0 )
	{
		n_len = SQL_NTS;
		txt.sprintf( "    Statement Len: SQL_NTS=-3" );
	}
	else
	{
		n_len = atoi( lname );
		txt.sprintf( "    Statement Len: %d", n_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLSetCursorName( in_handle, (SQLCHAR*) sql, n_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dSetCursorName::dSetCursorName( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 190,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 270,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 350,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	str = new QComboBox( TRUE, this, "CursorName" );
	str -> setGeometry( 130, 80, 290, 20 );
	str -> insertItem( "<input window>", 0 );
	str -> insertItem( "<empty string>", 1 );
	str -> insertItem( "<null ptr>", 2 );

	name_len = new QComboBox( TRUE, this, "name len" );
	name_len -> setGeometry( 130, 110, 150, 20 );
	l_name_len = new QLabel( "CursorName Length:", this );
	l_name_len -> setGeometry( 10, 110, 110, 20 );
	name_len -> insertItem( "SQL_NTS=-3", 0 );
	name_len -> insertItem( "0", 1 );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	l_str = new QLabel( "SQL Text:", this );
	l_str -> setGeometry( 10, 80, 70, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dSetCursorName::~dSetCursorName()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete str;
	delete l_handle;
	delete l_str;
	delete name_len;
	delete l_name_len;
}

const char *dDescribeParam::data_type_to_str( int type )
{
	int index = 0;
	struct attr_value *data_type = sql_data_types;

	while( TRUE )
	{
		if ( !data_type -> text )
			break;

		if ( data_type -> value == type )
			break;

		data_type ++;
	};

	if ( data_type -> text )
		return data_type -> text;
	else
		return NULL;
}

void dDescribeParam::type_clkd()
{
	if ( type_valid -> isOn() )
	    type_valid -> setText( "DataTypePtr: SQL_NULL_PTR" );
	else
	    type_valid -> setText( "DataTypePtr: VALID" );
}

void dDescribeParam::size_clkd()
{
	if ( size_valid -> isOn() )
	    size_valid -> setText( "ParameterSizePtr: SQL_NULL_PTR" );
	else
	    size_valid -> setText( "ParameterSizePtr: VALID" );
}

void dDescribeParam::digit_clkd()
{
	if ( digit_valid -> isOn() )
	    digit_valid -> setText( "DecimalDigitsPtr: SQL_NULL_PTR" );
	else
	    digit_valid -> setText( "DecimalDigitsPtr: VALID" );
}

void dDescribeParam::null_clkd()
{
	if ( null_valid -> isOn() )
	    null_valid -> setText( "NullablePtr: SQL_NULL_PTR" );
	else
	    null_valid -> setText( "NullablePtr: VALID" );
}

void dDescribeParam::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLUSMALLINT parameter_number;
	SQLSMALLINT *data_type_ptr, data_type;
	SQLULEN *param_size_ptr, param_size;
	SQLSMALLINT *decimal_digits_ptr, decimal_digits;
	SQLSMALLINT *nullable_ptr, nullable;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLDescribeParam():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	parameter_number = atoi( param_num -> text().ascii());
	txt.sprintf( "    Parameter Number: %d", parameter_number );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( type_valid -> isOn() )
	{
		data_type_ptr = NULL;
		txt.sprintf( "    DataTypePtr: <null pointer>" );
	}
	else
	{
		data_type_ptr = &data_type;
		txt.sprintf( "    DataTypePtr: %p", data_type_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	data_type = -9999;

	if ( size_valid -> isOn() )
	{
		param_size_ptr = NULL;
		txt.sprintf( "    ParameterSizePtr: <null pointer>" );
	}
	else
	{
		param_size_ptr = &param_size;
		txt.sprintf( "    ParameterSizePtr: %p", param_size_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	param_size = 9999;

	if ( digit_valid -> isOn() )
	{
		decimal_digits_ptr = NULL;
		txt.sprintf( "    DecimalDigitsPtr: <null pointer>" );
	}
	else
	{
		decimal_digits_ptr = &decimal_digits;
		txt.sprintf( "    DecimalDigitsPtr: %p", decimal_digits );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	decimal_digits = -9999;

	if ( null_valid -> isOn() )
	{
		nullable_ptr = NULL;
		txt.sprintf( "    NullablePtr: <null pointer>" );
	}
	else
	{
		nullable_ptr = &nullable;
		txt.sprintf( "    NullablePtr: %p", nullable_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	nullable = -9999;

	SQLRETURN ret = SQLDescribeParam( in_handle, 
            parameter_number, 
			data_type_ptr,
			param_size_ptr,
			decimal_digits_ptr,
			nullable_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );

	if ( SQL_SUCCEEDED( ret ))
	{
		if ( data_type_ptr )
		{	
			if ( data_type == -9999 )
			{
				txt.sprintf( "    *DataTypePtr: <unchanged>" );
			}
			else
			{
				const char *str = data_type_to_str( data_type );

				if ( str )
					txt.sprintf( "    *DataTypePtr: %s (%d)", str, data_type );
				else
					txt.sprintf( "    *DataTypePtr: %d", data_type );
			}
			odbctest -> out_win -> insertLineLimited( txt );
		}

		if ( param_size_ptr )
		{	
			if ( param_size == 9999 )
			{
				txt.sprintf( "    *ParamSizePtr: <unchanged>" );
			}
			else
			{
				txt.sprintf( "    *ParamSizePtr: %d", param_size );
			}
			odbctest -> out_win -> insertLineLimited( txt );
		}

		if ( decimal_digits_ptr )
		{	
			if ( decimal_digits == 9999 )
			{
				txt.sprintf( "    *DecimalDigitsPtr: <unchanged>" );
			}
			else
			{
				txt.sprintf( "    *DecimalDigits: %d", decimal_digits );
			}
			odbctest -> out_win -> insertLineLimited( txt );
		}

		if ( nullable_ptr )
		{	
			if ( nullable == -9999 )
			{
				txt.sprintf( "    *NullablePtr: <unchanged>" );
			}
			else
			{
				txt.sprintf( "    *NullablePtr: %d", nullable );
			}
			odbctest -> out_win -> insertLineLimited( txt );
		}
	}
	odbctest -> out_win -> insertLineLimited( "" );
}

dDescribeParam::dDescribeParam( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 240,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 320,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 400,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
    l_handle -> setGeometry( 10, 50, 120, 20 );

	param_num = new QLineEdit( this, "Param Num" );
    param_num -> setGeometry( 130, 80, 70, 20 );
	param_num -> setMaxLength( 6 );
	param_num -> setText( "1" );

	l_param_num = new QLabel( "ParameterNumber:", this );
    l_param_num -> setGeometry( 10, 80, 120, 20 );

	type_valid = new QCheckBox( "DataTypePtr: VALID", this );
	type_valid -> setGeometry( 10, 110, 250, 15 );

	size_valid = new QCheckBox( "ParameterSizePtr: VALID", this );
	size_valid -> setGeometry( 10, 140, 250, 15 );

	digit_valid = new QCheckBox( "DecimalDigits Ptr: VALID", this );
	digit_valid -> setGeometry( 300, 110, 250, 15 );

	null_valid = new QCheckBox( "NullablePtr: VALID", this );
	null_valid -> setGeometry( 300, 140, 250, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( type_valid, SIGNAL( clicked()), this, SLOT( type_clkd()));
	connect( size_valid, SIGNAL( clicked()), this, SLOT( size_clkd()));
	connect( digit_valid, SIGNAL( clicked()), this, SLOT( digit_clkd()));
	connect( null_valid, SIGNAL( clicked()), this, SLOT( null_clkd()));
}

dDescribeParam::~dDescribeParam()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handle;
	delete param_num;
	delete l_param_num;
	delete type_valid;
	delete size_valid;
	delete digit_valid;
	delete null_valid;
}

void dParamData::value_clkd()
{
	if ( value_valid -> isOn() )
	    value_valid -> setText( "ValuePtr: SQL_NULL_PTR" );
	else
	    value_valid -> setText( "ValuePtr: VALID" );
}

void dParamData::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLPOINTER *value_ptr, value;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLParamData():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( value_valid -> isOn() )
	{
		value_ptr = NULL;
		txt.sprintf( "    ValuePtr: <null pointer>" );
	}
	else
	{
		value_ptr = &value;
		txt.sprintf( "    ValuePtr: %p", value_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLParamData( in_handle, 
			value_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );

	if ( SQL_SUCCEEDED( ret ))
	{
		if ( value_ptr )
		{	
            txt.sprintf( "    *ValuePtr: %d", value );
			odbctest -> out_win -> insertLineLimited( txt );
		}
	}
	odbctest -> out_win -> insertLineLimited( "" );
}

dParamData::dParamData( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 100,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 180,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 260,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
    l_handle -> setGeometry( 10, 50, 120, 20 );

	value_valid = new QCheckBox( "ValuePtr: VALID", this );
	value_valid -> setGeometry( 10, 80, 250, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( value_valid, SIGNAL( clicked()), this, SLOT( value_clkd()));
}

dParamData::~dParamData()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handle;
	delete value_valid;
}

void dParamOption::pirow_clkd()
{
	if ( pirow_valid -> isOn() )
	    pirow_valid -> setText( "pirow: SQL_NULL_PTR" );
	else
	    pirow_valid -> setText( "pirow: VALID" );
}

void dParamOption::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLULEN *pirow_ptr, pirow;
	SQLULEN crow;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLParamOptions():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	crow = atoi( crow_num -> text().ascii());
	txt.sprintf( "    crow: %d", crow );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( pirow_valid -> isOn() )
	{
		pirow_ptr = NULL;
		txt.sprintf( "    pirow: <null pointer>" );
	}
	else
	{
		pirow_ptr = &pirow;
		txt.sprintf( "    pirow: %p", pirow_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLParamOptions( in_handle, 
            crow,
			pirow_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
}

dParamOption::dParamOption( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 100,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 180,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 260,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
    l_handle -> setGeometry( 10, 50, 120, 20 );

	crow_num = new QLineEdit( this, "Crow Num" );
    crow_num -> setGeometry( 130, 80, 70, 20 );
	crow_num -> setMaxLength( 6 );
	crow_num -> setText( "1" );

	l_crow_num = new QLabel( "crow:", this );
    l_crow_num -> setGeometry( 10, 80, 120, 20 );

	pirow_valid = new QCheckBox( "pirow: VALID", this );
	pirow_valid -> setGeometry( 10, 110, 250, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( pirow_valid, SIGNAL( clicked()), this, SLOT( pirow_clkd()));
}

dParamOption::~dParamOption()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handle;
	delete pirow_valid;
    delete crow_num;
    delete l_crow_num;
}

void dPutData::Ok()
{
	const char *sql;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLPOINTER data_ptr_var;
	SQLINTEGER strlen_or_len;
    const char *data_str, *strlen_or_len_str;

	if ( hand )
		in_handle = hand -> getHandle();

	data_str = data_ptr -> currentText();

	odbctest -> out_win -> insertLineLimited( "SQLPutData():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( data_str, "<null ptr>" ) == 0 )
	{
		data_ptr_var = NULL;
		odbctest -> out_win -> insertLineLimited( "    DataPtr: <null ptr>" );
	}
	else if ( strcmp( data_str, "<empty string>" ) == 0 )
	{
		data_ptr_var = (SQLPOINTER) "";
		odbctest -> out_win -> insertLineLimited( "    DataPtr: <empty string>" );
	}
	else if ( strcmp( data_str, "<input window>" ) == 0 )
	{
		QString *s = new QString( odbctest->in_win->text());
		if ( s -> isEmpty())
			data_ptr_var = (SQLPOINTER)"";
		else if ( s -> isNull())
			data_ptr_var = NULL;
		else 
			data_ptr_var = (SQLPOINTER) s->latin1();
		txt.sprintf( "    DataPtr: %s", data_ptr_var );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
        data_ptr_var = ( SQLPOINTER ) data_str;
		txt.sprintf( "    DataPtr: %s", data_ptr_var );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	strlen_or_len_str = str_len -> currentText();

	if ( strncmp( strlen_or_len_str, "SQL_NTS", 7 ) == 0 )
	{
		strlen_or_len = SQL_NTS;
		txt.sprintf( "    StrLen_or_ind: SQL_NTS=-3" );
	}
    else if ( strncmp( strlen_or_len_str, "SQL_NULL_DATA", 13 ) == 0 )
	{
		strlen_or_len = SQL_NULL_DATA;
		txt.sprintf( "    StrLen_or_ind: SQL_NULL_DATA=-1" );
	}
    else if ( strncmp( strlen_or_len_str, "SQL_DEFAULT_PARAM", 17 ) == 0 )
	{
		strlen_or_len = SQL_DEFAULT_PARAM;
		txt.sprintf( "    StrLen_or_ind: SQL_DEFAULT_PARAM=-5" );
	}
	else
	{
		strlen_or_len = atoi( strlen_or_len_str );
		txt.sprintf( "    StrLen_or_ind: %d", strlen_or_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLPutData( in_handle, data_ptr_var, strlen_or_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dPutData::dPutData( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 150,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 230,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 310,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	data_ptr = new QComboBox( TRUE, this, "data_ptr" );
	data_ptr -> setGeometry( 130, 80, 250, 20 );
	data_ptr -> insertItem( "<input window>", 0 );
	data_ptr -> insertItem( "<empty string>", 1 );
	data_ptr -> insertItem( "<null ptr>", 2 );

	str_len = new QComboBox( TRUE, this, "str len" );
	str_len -> setGeometry( 130, 110, 200, 20 );
    
	str_len -> insertItem( "SQL_NTS=-3", 0 );
	str_len -> insertItem( "SQL_NULL_DATA=-1", 1 );
	str_len -> insertItem( "SQL_DEFAULT_PARAM=-5", 2 );
	str_len -> insertItem( "0", 3 );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	l_data_ptr = new QLabel( "DataPtr:", this );
	l_data_ptr -> setGeometry( 10, 80, 70, 20 );

	l_str_len = new QLabel( "StrLen_or_IndPtr:", this );
	l_str_len -> setGeometry( 10, 110, 70, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dPutData::~dPutData()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete data_ptr;
	delete l_handle;
	delete l_data_ptr;
	delete str_len;
	delete l_str_len;
}

void dBindParameter::Ok()
{
}

dBindParameter::dBindParameter( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 450,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 530,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 610,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	param_num = new QLineEdit( this, "Param Num" );
    param_num -> setGeometry( 130, 80, 70, 20 );
	param_num -> setMaxLength( 6 );
	param_num -> setText( "1" );

	l_param_num = new QLabel( "ParameterNumber:", this );
    l_param_num -> setGeometry( 10, 80, 120, 20 );

	io_type = new QComboBox( FALSE, this, "io_type" );
	io_type -> setGeometry( 420, 80, 250, 20 );

	parent->fill_list_box( io_type_option, io_type );

	l_io_type = new QLabel( "InputOutputType:", this );
    l_io_type -> setGeometry( 320, 80, 100, 20 );

	value_type = new QComboBox( FALSE, this, "value_type" );
	value_type -> setGeometry( 130, 110, 360, 20 );

	parent->fill_list_box( value_type_option, value_type );

	l_value_type = new QLabel( "ValueType:", this );
    l_value_type -> setGeometry( 10, 110, 100, 20 );

	column_size = new QLineEdit( this, "column_size" );
    column_size -> setGeometry( 600, 110, 70, 20 );
	column_size -> setMaxLength( 6 );
	column_size -> setText( "0" );

	l_column_size = new QLabel( "ColumnSize:", this );
    l_column_size -> setGeometry( 500, 110, 100, 20 );

	param_type = new QComboBox( FALSE, this, "param_type" );
	param_type -> setGeometry( 130, 140, 360, 20 );

	parent->fill_list_box( param_type_option, param_type );

	l_param_type = new QLabel( "ParameterType:", this );
    l_param_type -> setGeometry( 10, 140, 100, 20 );

	decimal_digits = new QLineEdit( this, "decimal_digits" );
    decimal_digits -> setGeometry( 600, 140, 70, 20 );
	decimal_digits -> setMaxLength( 6 );
	decimal_digits -> setText( "0" );

	l_decimal_digits = new QLabel( "DecimalDigits:", this );
    l_decimal_digits -> setGeometry( 500, 140, 100, 20 );

	param_value = new QComboBox( FALSE, this, "param_value" );
	param_value -> setGeometry( 130, 170, 360, 20 );

	param_value -> insertItem( "<valid pointer>", 0 );
	param_value -> insertItem( "<null pointer>", 1 );

	l_param_value = new QLabel( "ParameterValuePtr:", this );
    l_param_value -> setGeometry( 10, 170, 120, 20 );

	buffer_length = new QLineEdit( this, "buffer_length" );
    buffer_length -> setGeometry( 600, 170, 70, 20 );
	buffer_length -> setMaxLength( 6 );
	buffer_length -> setText( "0" );

	l_buffer_length = new QLabel( "BufferLength:", this );
    l_buffer_length -> setGeometry( 500, 170, 100, 20 );

	strlen_or_ind = new QComboBox( TRUE, this, "strlen_or_ind" );
	strlen_or_ind -> setGeometry( 130, 200, 360, 20 );

	strlen_or_ind -> insertItem( "SQL_NTS=-3", 0 );
	strlen_or_ind -> insertItem( "SQL_NULL_DATA=-1", 1 );
	strlen_or_ind -> insertItem( "SQL_DATA_AT_EXEC=-2", 2 );
    strlen_or_ind -> insertItem( "SQL_DEFAULT_PARAM=-5", 3 );
	strlen_or_ind -> insertItem( "<null pointer>", 4 );
	strlen_or_ind -> insertItem( "0", 5 );

	l_strlen_or_ind = new QLabel( "StrLen_or_IndPtr:", this );
    l_strlen_or_ind -> setGeometry( 10, 200, 100, 20 );

	len_data_at_exec = new QCheckBox( "SQL_LEN_DATA_AT_EXEC", this );
	len_data_at_exec -> setGeometry( 500, 200, 190, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dBindParameter::~dBindParameter()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
    delete l_handle;
    delete io_type;
    delete l_io_type;
    delete param_num;
    delete l_param_num;
    delete value_type;
    delete l_value_type;
    delete column_size;
    delete l_column_size;
    delete param_type;
    delete l_param_type;
    delete decimal_digits;
    delete l_decimal_digits;
    delete param_value;
    delete l_param_value;
    delete buffer_length;
    delete l_buffer_length;
    delete strlen_or_ind;
    delete l_strlen_or_ind;
    delete len_data_at_exec;
}

void dBindParam::Ok()
{
}

dBindParam::dBindParam( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 450,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 530,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 610,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	param_num = new QLineEdit( this, "Param Num" );
    param_num -> setGeometry( 130, 80, 70, 20 );
	param_num -> setMaxLength( 6 );
	param_num -> setText( "1" );

	l_param_num = new QLabel( "ParameterNumber:", this );
    l_param_num -> setGeometry( 10, 80, 120, 20 );

	value_type = new QComboBox( FALSE, this, "value_type" );
	value_type -> setGeometry( 130, 110, 360, 20 );

	parent->fill_list_box( value_type_option, value_type );

	l_value_type = new QLabel( "ValueType:", this );
    l_value_type -> setGeometry( 10, 110, 100, 20 );

	column_size = new QLineEdit( this, "column_size" );
    column_size -> setGeometry( 600, 110, 70, 20 );
	column_size -> setMaxLength( 6 );
	column_size -> setText( "0" );

	l_column_size = new QLabel( "ColumnSize:", this );
    l_column_size -> setGeometry( 500, 110, 100, 20 );

	param_type = new QComboBox( FALSE, this, "param_type" );
	param_type -> setGeometry( 130, 140, 360, 20 );

	parent->fill_list_box( param_type_option, param_type );

	l_param_type = new QLabel( "ParameterType:", this );
    l_param_type -> setGeometry( 10, 140, 100, 20 );

	decimal_digits = new QLineEdit( this, "decimal_digits" );
    decimal_digits -> setGeometry( 600, 140, 70, 20 );
	decimal_digits -> setMaxLength( 6 );
	decimal_digits -> setText( "0" );

	l_decimal_digits = new QLabel( "DecimalDigits:", this );
    l_decimal_digits -> setGeometry( 500, 140, 100, 20 );

	param_value = new QComboBox( FALSE, this, "param_value" );
	param_value -> setGeometry( 130, 170, 360, 20 );

	param_value -> insertItem( "<valid pointer>", 0 );
	param_value -> insertItem( "<null pointer>", 1 );

	l_param_value = new QLabel( "ParameterValuePtr:", this );
    l_param_value -> setGeometry( 10, 170, 120, 20 );

	strlen_or_ind = new QComboBox( TRUE, this, "strlen_or_ind" );
	strlen_or_ind -> setGeometry( 130, 200, 360, 20 );

	strlen_or_ind -> insertItem( "SQL_NTS=-3", 0 );
	strlen_or_ind -> insertItem( "SQL_NULL_DATA=-1", 1 );
	strlen_or_ind -> insertItem( "SQL_DATA_AT_EXEC=-2", 2 );
    strlen_or_ind -> insertItem( "SQL_DEFAULT_PARAM=-5", 3 );
	strlen_or_ind -> insertItem( "<null pointer>", 4 );
	strlen_or_ind -> insertItem( "0", 5 );

	l_strlen_or_ind = new QLabel( "StrLen_or_IndPtr:", this );
    l_strlen_or_ind -> setGeometry( 10, 200, 100, 20 );

	len_data_at_exec = new QCheckBox( "SQL_LEN_DATA_AT_EXEC", this );
	len_data_at_exec -> setGeometry( 500, 200, 190, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dBindParam::~dBindParam()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
    delete l_handle;
    delete param_num;
    delete l_param_num;
    delete value_type;
    delete l_value_type;
    delete column_size;
    delete l_column_size;
    delete param_type;
    delete l_param_type;
    delete decimal_digits;
    delete l_decimal_digits;
    delete param_value;
    delete l_param_value;
    delete strlen_or_ind;
    delete l_strlen_or_ind;
    delete len_data_at_exec;
}

void dAllocStmt::handle_clkd()
{
	if ( handle_valid -> isOn() )
	    handle_valid -> setText( "phstmt: SQL_NULL_POINTER" );
	else
	    handle_valid -> setText( "phstmt: VALID" );
}

void dAllocStmt::Ok()
{
    SQLHSTMT hstmt, *hstmt_ptr;
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_DBC, handles );
	SQLHANDLE in_handle;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLAllocStmt():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Connection Handle: %p", in_handle );
	else
		txt.sprintf( "    Connection Handle: SQL_NULL_HDBC" );

	odbctest -> out_win -> insertLineLimited( txt );

    if ( handle_valid -> isOn())
    {
        hstmt_ptr = NULL;
		txt.sprintf( "    phstmt: SQL_NULL_HANDLE" );
    }
    else
    {
        hstmt_ptr = &hstmt;
		txt.sprintf( "    phstmt: %p", hstmt_ptr );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLAllocStmt( in_handle, hstmt_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ) && hstmt_ptr )
    {
		txt.sprintf( "    *phstmt: %p", hstmt );
	    odbctest -> out_win -> insertLineLimited( txt );

	    odbctest->listHandle.append( new Handle( SQL_HANDLE_STMT, hstmt, odbctest->listHandle ));
    }

	odbctest -> out_win -> insertLineLimited( "" );
}

dAllocStmt::dAllocStmt( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 90,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 170,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 250,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 50, 200, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_DBC, handles );

	l_handles = new QLabel( "Handle:", this );
	l_handles -> setGeometry( 10, 50, 80, 20 );

	handle_valid = new QCheckBox( "phstmt: VALID", this );
	handle_valid -> setGeometry( 10, 80, 300, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( handle_valid, SIGNAL( clicked()), this, SLOT( handle_clkd()));
}

dAllocStmt::~dAllocStmt()
{
	delete ok;
	delete cancel;
	delete help;
	delete handle_valid;
    delete handles;
    delete l_handles;
}

void OdbcTest::sqlbindparameter()
{
	dBindParameter *dlg = new dBindParameter( this, "SQLBindParameter" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlcancel()
{
	dCancel *dlg = new dCancel( this, "SQLCancel" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlclosecursor()
{
	dCloseCursor *dlg = new dCloseCursor( this, "SQLCloseCursor" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqldescribeparam()
{
	dDescribeParam *dlg = new dDescribeParam( this, "SQLDescribeParam" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlexecute()
{
	dExecute *dlg = new dExecute( this, "SQLExecute" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlexecdirect()
{
	dExecDirect *dlg = new dExecDirect( this, "SQLExecDirect" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlfreestmt()
{
	dFreeStmt *dlg = new dFreeStmt( this, "SQLFreeStmt" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetcursorname()
{
	dGetCursorName *dlg = new dGetCursorName( this, "SQLGetCursorName" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlnumparams()
{
	dNumParams *dlg = new dNumParams( this, "SQLNumParams" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlparamdata()
{
	dParamData *dlg = new dParamData( this, "SQLParamData" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlparamoptions()
{
	dParamOption *dlg = new dParamOption( this, "SQLParamOptions" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlprepare()
{
	dPrepare *dlg = new dPrepare( this, "SQLPrepare" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlputdata()
{
	dPutData *dlg = new dPutData( this, "SQLPutData" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlsetcursorname()
{
	dSetCursorName *dlg = new dSetCursorName( this, "SQLSetCursorName" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlallocstmt()
{
	dAllocStmt *dlg = new dAllocStmt( this, "SQLAllocStmt" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlbindparam()
{
	dBindParam *dlg = new dBindParam( this, "SQLBindParam" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::fillparam()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::showparam()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::showcursorsettings()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}
