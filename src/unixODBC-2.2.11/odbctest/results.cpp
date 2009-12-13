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
 * $Id: results.cpp,v 1.7 2005/02/01 10:24:24 lurcher Exp $
 *
 * $Log: results.cpp,v $
 * Revision 1.7  2005/02/01 10:24:24  lurcher
 * Cope if SHLIBEXT is not set
 *
 * Revision 1.6  2004/11/22 17:02:49  lurcher
 * Fix unicode/ansi conversion in the SQLGet functions
 *
 * Revision 1.5  2004/10/07 10:10:07  lurcher
 *
 * Fix some warnings
 *
 * Revision 1.4  2001/12/20 17:26:26  lurcher
 *
 * More warnings removed
 *
 * Revision 1.3  2001/12/20 12:30:44  lurcher
 *
 * Fix 64 problem in odbctest and 7.1 Postgres driver
 *
 * Revision 1.2  2001/12/13 13:00:34  lurcher
 *
 * Remove most if not all warnings on 64 bit platforms
 * Add support for new MS 3.52 64 bit changes
 * Add override to disable the stopping of tracing
 * Add MAX_ROWS support in postgres driver
 *
 * Revision 1.1.1.1  2001/10/17 16:40:31  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.6  2001/07/20 09:42:58  nick
 *
 * Replace char[] with QString to avoid buffer overrun
 *
 * Revision 1.5  2001/05/31 16:05:55  nick
 *
 * Fix problems with postgres closing local sockets
 * Make odbctest build with QT 3 (it doesn't work due to what I think are bugs
 * in QT 3)
 * Fix a couple of problems in the cursor lib
 *
 * Revision 1.4  2001/05/04 15:32:54  nick
 *
 * Some makefile tweeks
 *
 * Revision 1.3  2000/12/17 10:51:33  nick
 *
 * Add include for sqlucode.h
 *
 * Revision 1.2  2000/11/02 11:24:02  nick
 *
 * using the wrong array for SQLColAttributes
 *
 * Revision 1.1.1.1  2000/09/04 16:42:53  nick
 * Imported Sources
 *
 * Revision 1.9  2000/06/27 09:19:44  ngorham
 *
 * SQLExtendedFetch was using the wrong orentation value
 *
 * Revision 1.8  2000/06/13 12:30:27  ngorham
 *
 * Enough there for the first release I think
 *
 * Revision 1.7  2000/06/12 18:46:20  ngorham
 *
 * More changes
 *
 * Revision 1.6  2000/06/09 17:04:21  ngorham
 *
 * More, and More
 *
 * Revision 1.5  2000/06/08 17:14:08  ngorham
 *
 * And more
 *
 * Revision 1.4  2000/06/07 17:00:16  ngorham
 *
 * Another update
 *
 * Revision 1.3  2000/06/07 15:10:22  ngorham
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

#include <stdlib.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>
#include <sqltypes.h>
#include "results.h"
#include "odbctest.h"

static attr_value data_types[] = 
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
	{ "SQL_C_INTERVAL_MINUTE_TO_SECOND", SQL_C_INTERVAL_MINUTE_TO_SECOND, "3.0" },
	{ "SQL_C_INTERVAL_MONTH", SQL_C_INTERVAL_MONTH, "3.0" },
	{ "SQL_C_INTERVAL_MINUTE", SQL_C_INTERVAL_MINUTE, "3.0" },
	{ "SQL_C_INTERVAL_SECOND", SQL_C_INTERVAL_SECOND, "3.0" },
	{ "SQL_C_INTERVAL_YEAR", SQL_C_INTERVAL_YEAR, "3.0" },
	{ "SQL_C_INTERVAL_YEAR_TO_MONTH", SQL_C_INTERVAL_YEAR_TO_MONTH, "3.0" },
	{ "SQL_C_LONG", SQL_C_LONG, "2.0" },
	{ "SQL_C_NUMERIC", SQL_C_NUMERIC, "3.0" },
	{ "SQL_C_SBIGINT", SQL_C_SBIGINT, "3.0" },
	{ "SQL_C_SLONG", SQL_C_SLONG, "2.0" },
	{ "SQL_C_SSHORT", SQL_C_SSHORT, "2.0" },
	{ "SQL_C_STINYINT", SQL_C_STINYINT, "2.0" },
	{ "SQL_C_TIME", SQL_C_TIME, "1.0" },
	{ "SQL_C_TIMESTAMP", SQL_C_TIMESTAMP, "1.0" },
	{ "SQL_C_TINYINT", SQL_C_TINYINT, "1.0" },
	{ "SQL_C_TYPE_DATE", SQL_C_TYPE_DATE, "3.0" },
	{ "SQL_C_TYPE_TIME", SQL_C_TYPE_TIME, "3.0" },
	{ "SQL_C_TYPE_TIMESTAMP", SQL_C_TYPE_TIMESTAMP, "3.0" },
	{ "SQL_C_UBIGINT", SQL_C_UBIGINT, "3.0" },
	{ "SQL_C_ULONG", SQL_C_ULONG, "2.0" },
	{ "SQL_C_USHORT", SQL_C_USHORT, "2.0" },
	{ "SQL_C_UTINYINT", SQL_C_UTINYINT, "2.0" },
	{ "SQL_C_VARBOOKMARK", SQL_C_VARBOOKMARK, "3.0" },
	{ "SQL_C_WCHAR", SQL_C_WCHAR, "3.0" },
	{ "SQL_ARD_TYPE", SQL_ARD_TYPE, "3.0" },
	{ "SQL_C_DEFAULT", SQL_C_DEFAULT, "3.0" },
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

static attr_options field_ident_struct[] = 
{
	{ "SQL_DESC_AUTO_UNIQUE_VALUE", SQL_DESC_AUTO_UNIQUE_VALUE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_DESC_BASE_COLUMN_NAME", SQL_DESC_BASE_COLUMN_NAME, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DESC_BASE_TABLE_NAME", SQL_DESC_BASE_TABLE_NAME, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DESC_CASE_SENSITIVE", SQL_DESC_CASE_SENSITIVE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_DESC_CATALOG_NAME", SQL_DESC_CATALOG_NAME, 
		{
			{ NULL }
		}, "2.0", SQL_CHAR
	},
	{ "SQL_DESC_CONCISE_TYPE", SQL_DESC_CONCISE_TYPE, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DESC_COUNT", SQL_DESC_COUNT, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_DESC_DISPLAY_SIZE", SQL_DESC_DISPLAY_SIZE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_DESC_FIXED_PREC_SCALE", SQL_DESC_FIXED_PREC_SCALE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_DESC_LABEL", SQL_DESC_LABEL, 
		{
			{ NULL }
		}, "2.0", SQL_CHAR
	},
	{ "SQL_DESC_LENGTH", SQL_DESC_LENGTH, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DESC_LITERAL_PREFIX", SQL_DESC_LITERAL_PREFIX, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DESC_LITERAL_SUFFIX", SQL_DESC_LITERAL_SUFFIX, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DESC_LOCAL_TYPE_NAME", SQL_DESC_LOCAL_TYPE_NAME, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DESC_NAME", SQL_DESC_NAME, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DESC_NULLABLE", SQL_DESC_NULLABLE, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DESC_NUM_PREC_RADIX", SQL_DESC_NUM_PREC_RADIX, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DESC_OCTET_LENGTH", SQL_DESC_OCTET_LENGTH, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DESC_PRECISION", SQL_DESC_PRECISION, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DESC_SCALE", SQL_DESC_SCALE, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DESC_SCHEMA_NAME", SQL_DESC_SCHEMA_NAME, 
		{
			{ NULL }
		}, "2.0", SQL_CHAR
	},
	{ "SQL_DESC_SEARCHABLE", SQL_DESC_SEARCHABLE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_DESC_TABLE_NAME", SQL_DESC_TABLE_NAME, 
		{
			{ NULL }
		}, "2.0", SQL_CHAR
	},
	{ "SQL_DESC_TYPE", SQL_DESC_TYPE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_DESC_TYPE_NAME", SQL_DESC_TYPE_NAME, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "SQL_DESC_UNNAMED", SQL_DESC_UNNAMED, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DESC_UNSIGNED", SQL_DESC_UNSIGNED, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_DESC_UPDATABLE", SQL_DESC_UPDATABLE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ NULL 
	}
};

static attr_options ca_field_ident_struct[] = 
{
	{ "SQL_COLUMN_AUTO_INCREMENT", SQL_COLUMN_AUTO_INCREMENT, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_CASE_SENSITIVE", SQL_COLUMN_CASE_SENSITIVE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_COUNT", SQL_COLUMN_COUNT, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_DISPLAY_SIZE", SQL_COLUMN_DISPLAY_SIZE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_LABEL", SQL_COLUMN_LABEL, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "SQL_COLUMN_LENGTH", SQL_COLUMN_LENGTH, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_MONEY", SQL_COLUMN_MONEY, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_NAME", SQL_COLUMN_NAME, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "SQL_COLUMN_NULLABLE", SQL_COLUMN_NULLABLE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_OWNER_NAME", SQL_COLUMN_OWNER_NAME, 
		{
			{ NULL }
		}, "2.0", SQL_CHAR
	},
	{ "SQL_COLUMN_PRECISION", SQL_COLUMN_PRECISION, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_QUALIFIER_NAME", SQL_COLUMN_QUALIFIER_NAME, 
		{
			{ NULL }
		}, "2.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_SCALE", SQL_COLUMN_SCALE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_SEARCHABLE", SQL_COLUMN_SEARCHABLE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_TABLE_NAME", SQL_COLUMN_TABLE_NAME, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "SQL_COLUMN_TYPE", SQL_COLUMN_TYPE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_TYPE_NAME", SQL_COLUMN_TYPE_NAME, 
		{
			{ NULL }
		}, "1.0", SQL_CHAR
	},
	{ "SQL_COLUMN_UNSIGNED", SQL_COLUMN_UNSIGNED, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ "SQL_COLUMN_UPDATABLE", SQL_COLUMN_UPDATABLE, 
		{
			{ NULL }
		}, "1.0", SQL_INTEGER
	},
	{ NULL 
	}
};

static attr_value fetch_scroll_orentation[] = 
{
	{ "SQL_FETCH_NEXT", SQL_FETCH_NEXT, "1.0" },
	{ "SQL_FETCH_PRIOR", SQL_FETCH_PRIOR, "1.0" },
	{ "SQL_FETCH_RELATIVE", SQL_FETCH_RELATIVE, "1.0" },
	{ "SQL_FETCH_ABSOLUTE", SQL_FETCH_ABSOLUTE, "1.0" },
	{ "SQL_FETCH_FIRST", SQL_FETCH_FIRST, "1.0" },
	{ "SQL_FETCH_LAST", SQL_FETCH_LAST, "1.0" },
	{ "SQL_FETCH_BOOKMARK", SQL_FETCH_BOOKMARK, "1.0" },
	{ NULL }
};

static attr_value set_scroll_cur_operation[] = 
{
	{ "SQL_CONCUR_READ_ONLY", SQL_CONCUR_READ_ONLY, "1.0" },
	{ "SQL_CONCUR_LOCK", SQL_CONCUR_LOCK, "1.0" },
	{ "SQL_CONCUR_ROWVER", SQL_CONCUR_ROWVER, "1.0" },
	{ "SQL_CONCUR_VALUES", SQL_CONCUR_VALUES, "1.0" },
	{ NULL }
};

static attr_value set_scroll_ks_values[] = 
{
	{ "SQL_SCROLL_FORWARD_ONLY", SQL_SCROLL_FORWARD_ONLY, "1.0" },
	{ "SQL_SCROLL_KEYSET_DRIVEN", SQL_SCROLL_KEYSET_DRIVEN, "1.0" },
	{ "SQL_SCROLL_DYNAMIC", SQL_SCROLL_DYNAMIC, "1.0" },
	{ "SQL_SCROLL_STATIC", SQL_SCROLL_STATIC, "1.0" },
	{ NULL }
};

static attr_value bulk_operation[] = 
{
	{ "SQL_ADD", SQL_ADD, "3.0" },
	{ "SQL_UPDATE_BY_BOOKMARK", SQL_UPDATE_BY_BOOKMARK, "3.0" },
	{ "SQL_DELETE_BY_BOOKMARK", SQL_DELETE_BY_BOOKMARK, "3.0" },
	{ "SQL_FETCH_BY_BOOKMARK", SQL_FETCH_BY_BOOKMARK, "3.0" },
	{ NULL }
};

static attr_value set_pos_operation[] = 
{
	{ "SQL_POSITION", SQL_POSITION },
	{ "SQL_REFRESH", SQL_REFRESH },
	{ "SQL_UPDATE", SQL_UPDATE },
	{ "SQL_DELETE", SQL_DELETE },
	{ NULL }
};

static attr_value set_pos_lock_type[] = 
{
	{ "SQL_LOCK_NO_CHANGE", SQL_LOCK_NO_CHANGE },
	{ "SQL_LOCK_EXCLUSIVE", SQL_LOCK_EXCLUSIVE },
	{ "SQL_LOCK_UNLOCK", SQL_LOCK_UNLOCK },
	{ NULL }
};

void dNumResultCols::out_handle_ptr_clkd()
{
	if ( valid -> isOn() )
	    valid -> setText( "ColumnCountPtr: SQL_NULL_HANDLE" );
	else
	    valid -> setText( "ColumnCountPtr: VALID" );
}

void dNumResultCols::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT num_cols;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLNumResultCols():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( valid -> isOn() )
		txt.sprintf( "    ColumnCountPtr: <null pointer>." );
	else
		txt.sprintf( "    ColumnCountPtr: %p", &num_cols );
	odbctest -> out_win -> insertLineLimited( txt );

	num_cols = -9999;

	SQLRETURN ret = SQLNumResultCols( in_handle,  valid -> isOn() ? NULL : &num_cols );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );
	if ( num_cols == -9999 )
	{
		txt.sprintf( "    *ColumnCountPtr: <unchanged>" );
	}
	else
	{
		txt.sprintf( "    *ColumnCountPtr: %d", num_cols );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dNumResultCols::dNumResultCols( OdbcTest *parent, QString name )
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

	valid = new QCheckBox( "ColumnCountPtr: VALID", this );
	valid -> setGeometry( 10, 80, 300, 15 );

	connect( valid, SIGNAL( clicked()), this, SLOT( out_handle_ptr_clkd()));

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dNumResultCols::~dNumResultCols()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete valid;
}

void dFetch::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT num_cols;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLFetch():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLFetch( in_handle );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dFetch::dFetch( OdbcTest *parent, QString name )
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

dFetch::~dFetch()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
}

void dGetData::target_clkd()
{
	if ( target_valid -> isOn() )
	    target_valid -> setText( "TargetValuePtr: SQL_NULL_POINTER" );
	else
	    target_valid -> setText( "TargetValuePtr: VALID" );
}

void dGetData::strlen_clkd()
{
	if ( strlen_valid -> isOn() )
	    strlen_valid -> setText( "StrLen_or_IndValuePtr: SQL_NULL_POINTER" );
	else
	    strlen_valid -> setText( "StrLen_or_IndValuePtr: VALID" );
}

void dGetData::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	int index;
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT column_number;
	SQLINTEGER b_len, data_type;
    SQLLEN *strlen_ptr, strlen_or_ind;
	char *buf = NULL;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetData():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	column_number = atoi( column_num -> text().ascii());
	txt.sprintf( "    Column Number: %d", column_number );
	odbctest -> out_win -> insertLineLimited( txt );

	index = type -> currentItem(); 

	data_type = data_types[ index ].value;
	txt.sprintf( "    Target Type: %s=%d (%s)", data_types[ index ].text,
		data_types[ index ].value, data_types[ index ].version );
	odbctest -> out_win -> insertLineLimited( txt );

	b_len = atoi( buffer_len -> text().ascii());
	if ( b_len < 1 )
	{
		b_len = 0;
	}

	if ( target_valid -> isOn())
	{
		buf = NULL;
	}
	else if ( b_len < 300 )
	{
		buf = new char[ 300 ];	
	}
	else
	{
		buf = new char[ b_len ];	
	}

	if ( buf )
	{
		txt.sprintf( "    Target Value Ptr: %p", buf );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Target Value Ptr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	txt.sprintf( "    Buffer Length: %d", b_len );
	odbctest -> out_win -> insertLineLimited( txt );

	strlen_or_ind = -999999;

	if ( strlen_valid -> isOn())
	{
		strlen_ptr = NULL;
	}
	else
	{
		strlen_ptr = &strlen_or_ind;
	}

	if ( strlen_ptr )
	{
		txt.sprintf( "    Strlen_or_Ind Ptr: %p", strlen_ptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Strlen_or_Ind Ptr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	SQLRETURN ret = SQLGetData( in_handle, column_number, 
			data_type, buf, b_len, strlen_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );

	if ( SQL_SUCCEEDED( ret ) && strlen_ptr )
	{	
		if ( strlen_or_ind == -999999 )
		{
			txt.sprintf( "    *Strlen_or_Ind Ptr: <unchanged>" );
		}
		else
		{
			txt.sprintf( "    *Strlen_or_Ind Ptr: %d", strlen_or_ind );
		}
		odbctest -> out_win -> insertLineLimited( txt );
	}

	/*
	 * display the result
	 */

	if ( SQL_SUCCEEDED( ret ) && (( strlen_ptr && strlen_or_ind >= 0 ) || !strlen_ptr ))
	{
		switch ( data_type )
		{
			case SQL_C_CHAR:
				if ( strlen( buf ) > 64 )
				{
					buf[ 64 ] = '\0';
					txt.sprintf( "    *Target Value Ptr: \"%s...\"", buf );
				}
				else
				{
					txt.sprintf( "    *Target Value Ptr: \"%s\"", buf );
				}
				break;
	
			case SQL_C_BIT:
			{
				unsigned char bit;

				memcpy( &bit, buf, sizeof( bit ));
				txt.sprintf( "    *Target Value Ptr: %d", bit );
				break;
			}
	
			case SQL_C_DATE:
			case SQL_C_TYPE_DATE:
			{
				SQL_DATE_STRUCT dt;

				memcpy( &dt, buf, sizeof( dt ));
				txt.sprintf( "    *Target Value Ptr: %04d-%02d-%02d", 
							dt.year, dt.month, dt.day );
				break;
			}
	
			case SQL_C_DOUBLE:
			{
				double val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %f", val );
				break;
			}
	
			case SQL_C_FLOAT:
			{
				float val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %f", val );
				break;
			}
	
			case SQL_C_INTERVAL_DAY:
			case SQL_C_INTERVAL_DAY_TO_HOUR:
			case SQL_C_INTERVAL_DAY_TO_MINUTE:
			case SQL_C_INTERVAL_DAY_TO_SECOND:
			case SQL_C_INTERVAL_HOUR:
			case SQL_C_INTERVAL_HOUR_TO_MINUTE:
			case SQL_C_INTERVAL_HOUR_TO_SECOND:
			case SQL_C_INTERVAL_MINUTE_TO_SECOND:
			case SQL_C_INTERVAL_MINUTE:
			case SQL_C_INTERVAL_SECOND:
			{
				SQL_INTERVAL_STRUCT val;
				char str[ 128 ];

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr:\n"  );
				sprintf( str, "        interval.interval_type: %d (%s)\n", 
						val.interval_type, odbctest -> int_type_as_string( val.interval_type ));
				txt += str;
				sprintf( str, "        interval.sign: %d\n", val.interval_sign);
				txt += str;
				sprintf( str, "        interval.intval.day_second.day: %d\n", 
										val.intval.day_second.day);
				txt += str;
				sprintf( str, "        interval.intval.day_second.hour: %d\n", 
										val.intval.day_second.hour);
				txt += str;
				sprintf( str, "        interval.intval.day_second.minute: %d\n", 
										val.intval.day_second.minute);
				txt += str;
				sprintf( str, "        interval.intval.day_second.second: %d\n", 
										val.intval.day_second.second);
				txt += str;
				sprintf( str, "        interval.intval.day_second.fraction: %09d\n", 
										val.intval.day_second.fraction);
				txt += str;
				break;
			}

			case SQL_C_INTERVAL_YEAR:
			case SQL_C_INTERVAL_YEAR_TO_MONTH:
			case SQL_C_INTERVAL_MONTH:
			{
				SQL_INTERVAL_STRUCT val;
				char str[ 128 ];

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr:\n"  );
				sprintf( str, "        interval.interval_type: %d (%s)\n", 
						val.interval_type, odbctest -> int_type_as_string( val.interval_type ));
				txt += str;
				sprintf( str, "        interval.sign: %d\n", val.interval_sign);
				txt += str;
				sprintf( str, "        interval.intval.year_month.year: %d\n", 
										val.intval.year_month.year);
				txt += str;
				sprintf( str, "        interval.intval.year_month.month: %d\n", 
										val.intval.year_month.month);
				txt += str;
				break;
			}

			case SQL_C_LONG:
			{
				long val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %d (0x%x)", val, val );
				break;
			}

			case SQL_C_NUMERIC:
			{
				break;
			}

			case SQL_C_SBIGINT:
			{
				break;
			}

			case SQL_C_SLONG:
			{
				long val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %d (0x%x)", val, val );
				break;
			}

			case SQL_C_SSHORT:
			{
				short val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %d (0x%x)", val, val );
				break;
			}

			case SQL_C_STINYINT:
			{
				char val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %d (0x%x)", val, val );
				break;
			}

			case SQL_C_TIME:
			case SQL_C_TYPE_TIME:
			{
				SQL_TIME_STRUCT dt;

				memcpy( &dt, buf, sizeof( dt ));
				txt.sprintf( "    *Target Value Ptr: %02d:%02d:%02d", 
							dt.hour, dt.minute, dt.second );
				break;
			}

			case SQL_C_TIMESTAMP:
			case SQL_C_TYPE_TIMESTAMP:
			{
				SQL_TIMESTAMP_STRUCT dt;

				memcpy( &dt, buf, sizeof( dt ));
				txt.sprintf( "    *Target Value Ptr: %04d-%02d%02d %02d:%02d:%02d.%0d", 
							dt.year, dt.month, dt.day,
							dt.hour, dt.minute, dt.second, dt.fraction );
				break;
			}

			case SQL_C_TINYINT:
			{
				char val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %d (0x%x)", val, val );
				break;
			}

			case SQL_C_UBIGINT:
			{
				break;
			}

			case SQL_C_ULONG:
			{
				unsigned long val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %lu (0x%lx)", val, val );
				break;
			}

			case SQL_C_USHORT:
			{
				unsigned short val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %u (0x%x)", val, val );
				break;
			}

			case SQL_C_UTINYINT:
			{
				unsigned char val;

				memcpy( &val, buf, sizeof( val ));
				txt.sprintf( "    *Target Value Ptr: %u (0x%x)", val, val );
				break;
			}
	
			case SQL_C_BINARY:
			case SQL_C_GUID:
			case SQL_C_WCHAR:
			case SQL_ARD_TYPE:
			case SQL_C_DEFAULT:
			default:
				txt.sprintf( "    *Target Value Ptr: 0x" );
				if ( strlen_ptr )
				{
					if ( strlen_or_ind > 32 )
					{
						for ( int i = 0; i < 32; i ++ )
						{
							char tmp[ 3 ];
							sprintf( tmp, "%X", buf[ i ] );
							txt += tmp;
						}
					}
					else
					{
						for ( int i = 0; i < strlen_or_ind; i ++ )
						{
							char tmp[ 3 ];
							sprintf( tmp, "%X", buf[ i ] );
							txt += tmp;
						}
					}
				}
				else
				{
					if ( b_len > 32 )
					{
						for ( int i = 0; i < 32; i ++ )
						{
							char tmp[ 3 ];
							sprintf( tmp, "%X", buf[ i ] );
							txt += tmp;
						}
					}
					else
					{
						for ( int i = 0; i < b_len; i ++ )
						{
							char tmp[ 3 ];
							sprintf( tmp, "%X", buf[ i ] );
							txt += tmp;
						}
					}
				}
				break;
		}
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( buf )
		delete buf;

	odbctest -> out_win -> insertLineLimited( "" );
}

dGetData::dGetData( OdbcTest *parent, QString name )
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

	column_num = new QLineEdit( this, "Column Num" );
    column_num -> setGeometry( 130, 80, 70, 20 );
	column_num -> setMaxLength( 6 );
	column_num -> setText( "1" );

	l_column_num = new QLabel( "Column Number:", this );
    l_column_num -> setGeometry( 10, 80, 120, 20 );

	type = new QComboBox( FALSE, this, "Data Type" );
	type -> setGeometry( 130, 110, 340, 20 );

	parent->fill_list_box( data_types, type );

	l_type = new QLabel( "Target Type:", this );
    l_type -> setGeometry( 10, 110, 120, 20 );

	target_valid = new QCheckBox( "TargetValuePtr: VALID", this );
	target_valid -> setGeometry( 10, 140, 300, 15 );

	strlen_valid = new QCheckBox( "StrLen_or_IndValuePtr: VALID", this );
	strlen_valid -> setGeometry( 10, 170, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 400, 140, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "Buffer Len:", this );
    l_buffer_len -> setGeometry( 320, 140, 60, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( target_valid, SIGNAL( clicked()), this, SLOT( target_clkd()));
	connect( strlen_valid, SIGNAL( clicked()), this, SLOT( strlen_clkd()));
}

dGetData::~dGetData()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handle;
	delete column_num;
	delete l_column_num;
	delete type;
	delete l_type;
	delete target_valid;
	delete strlen_valid;
	delete buffer_len;
	delete l_buffer_len;
}

void dDescribeCol::name_clkd()
{
	if ( name_valid -> isOn() )
	    name_valid -> setText( "Column Name Ptr: SQL_NULL_PTR" );
	else
	    name_valid -> setText( "Column Name Ptr: VALID" );
}

void dDescribeCol::type_clkd()
{
	if ( type_valid -> isOn() )
	    type_valid -> setText( "Name Length Ptr: SQL_NULL_PTR" );
	else
	    type_valid -> setText( "Name Length Ptr: VALID" );
}

void dDescribeCol::size_clkd()
{
	if ( size_valid -> isOn() )
	    size_valid -> setText( "Column Size Ptr: SQL_NULL_PTR" );
	else
	    size_valid -> setText( "Column Size Ptr: VALID" );
}

void dDescribeCol::digit_clkd()
{
	if ( digit_valid -> isOn() )
	    digit_valid -> setText( "Decimal Digits Ptr: SQL_NULL_PTR" );
	else
	    digit_valid -> setText( "Decimal Digits Ptr: VALID" );
}

void dDescribeCol::null_clkd()
{
	if ( null_valid -> isOn() )
	    null_valid -> setText( "Nullable Ptr: SQL_NULL_PTR" );
	else
	    null_valid -> setText( "Nullable Ptr: VALID" );
}

const char *dDescribeCol::data_type_to_str( int type )
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

void dDescribeCol::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT column_number;
	SQLSMALLINT buffer_length;
	SQLCHAR *column_name;
	SQLSMALLINT *name_length_ptr, name_length;
	SQLSMALLINT *data_type_ptr, data_type;
	SQLULEN *column_size_ptr, column_size;
	SQLSMALLINT *decimal_digit_ptr, decimal_digit;
	SQLSMALLINT *nullable_ptr, nullable;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLDescribeCol():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	column_number = atoi( column_num -> text().ascii());
	txt.sprintf( "    Column Number: %d", column_number );
	odbctest -> out_win -> insertLineLimited( txt );

	buffer_length = atoi( column_len -> text().ascii());

	if ( name_valid -> isOn())
	{
		column_name = NULL;
	}
	else if ( buffer_length < 300 )
	{
		column_name = new SQLCHAR[ 300 ];	
	}
	else
	{
		column_name = new SQLCHAR[ buffer_length ];	
	}

	if ( column_name )
	{
		txt.sprintf( "    Column Name Ptr: %p", column_name );
		odbctest -> out_win -> insertLineLimited( txt );
		txt.sprintf( "    Column Name Length: %d", buffer_length );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    Column Name Ptr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( name_valid -> isOn() )
	{
		name_length_ptr = NULL;
		txt.sprintf( "    Name Length Ptr: <null pointer>" );
	}
	else
	{
		name_length_ptr = &name_length;
		txt.sprintf( "    Name Length Ptr: %p", name_length_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	name_length = -9999;

	if ( type_valid -> isOn() )
	{
		data_type_ptr = NULL;
		txt.sprintf( "    Data Type Ptr: <null pointer>" );
	}
	else
	{
		data_type_ptr = &data_type;
		txt.sprintf( "    Data Type Ptr: %p", data_type_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	data_type = -9999;

	if ( name_valid -> isOn() )
	{
		column_size_ptr = NULL;
		txt.sprintf( "    Column Size Ptr: <null pointer>" );
	}
	else
	{
		column_size_ptr = &column_size;
		txt.sprintf( "    Column Size Ptr: %p", column_size_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	column_size = 9999;

	if ( digit_valid -> isOn() )
	{
		decimal_digit_ptr = NULL;
		txt.sprintf( "    Decimal Digits Ptr: <null pointer>" );
	}
	else
	{
		decimal_digit_ptr = &decimal_digit;
		txt.sprintf( "    Decimal Digits Ptr: %p", decimal_digit_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	decimal_digit = -9999;

	if ( null_valid -> isOn() )
	{
		nullable_ptr = NULL;
		txt.sprintf( "    Nullable Ptr: <null pointer>" );
	}
	else
	{
		nullable_ptr = &nullable;
		txt.sprintf( "    Nullable Ptr: %p", nullable_ptr );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	nullable = -9999;

	SQLRETURN ret = SQLDescribeCol( in_handle, column_number, 
			column_name,
			buffer_length,
			name_length_ptr,
			data_type_ptr,
			column_size_ptr,
			decimal_digit_ptr,
			nullable_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );

	if ( SQL_SUCCEEDED( ret ))
	{
		if ( column_name )
		{
			txt.sprintf( "    Column Name: %s", column_name );
			odbctest -> out_win -> insertLineLimited( txt );
		}

		if ( name_length_ptr )
		{	
			if ( name_length == -9999 )
			{
				txt.sprintf( "    *Name Length Ptr: <unchanged>" );
			}
			else
			{
				txt.sprintf( "    *Name Length Ptr: %d", name_length );
			}
			odbctest -> out_win -> insertLineLimited( txt );
		}

		if ( data_type_ptr )
		{	
			if ( data_type == -9999 )
			{
				txt.sprintf( "    *Data Type Ptr: <unchanged>" );
			}
			else
			{
				const char *str = data_type_to_str( data_type );

				if ( str )
					txt.sprintf( "    *Data Type Ptr: %s (%d)", str, data_type );
				else
					txt.sprintf( "    *Data Type Ptr: %d", data_type );
			}
			odbctest -> out_win -> insertLineLimited( txt );
		}

		if ( column_size_ptr )
		{	
			if ( column_size == 9999 )
			{
				txt.sprintf( "    *Column Size Ptr: <unchanged>" );
			}
			else
			{
				txt.sprintf( "    *Column Size Ptr: %d", column_size );
			}
			odbctest -> out_win -> insertLineLimited( txt );
		}

		if ( decimal_digit_ptr )
		{	
			if ( decimal_digit == -9999 )
			{
				txt.sprintf( "    *Decimal Digits Ptr: <unchanged>" );
			}
			else
			{
				txt.sprintf( "    *Decimal Digits Ptr: %d", decimal_digit );
			}
			odbctest -> out_win -> insertLineLimited( txt );
		}

		if ( nullable_ptr )
		{	
			if ( nullable == -9999 )
			{
				txt.sprintf( "    *Nullable Ptr: <unchanged>" );
			}
			else
			{
				txt.sprintf( "    *Nullable Ptr: %d", nullable );
			}
			odbctest -> out_win -> insertLineLimited( txt );
		}
	}
	odbctest -> out_win -> insertLineLimited( "" );
}

dDescribeCol::dDescribeCol( OdbcTest *parent, QString name )
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

	column_num = new QLineEdit( this, "Column Num" );
    column_num -> setGeometry( 130, 80, 70, 20 );
	column_num -> setMaxLength( 6 );
	column_num -> setText( "1" );

	l_column_num = new QLabel( "Column Number:", this );
    l_column_num -> setGeometry( 10, 80, 120, 20 );

	column_len = new QLineEdit( this, "Buffer Len" );
    column_len -> setGeometry( 400, 110, 70, 20 );
	column_len -> setMaxLength( 6 );
	column_len -> setText( "300" );

	l_column_len = new QLabel( "Column Buffer Len:", this );
    l_column_len -> setGeometry( 280, 110, 110, 20 );

	name_valid = new QCheckBox( "Column Name Ptr: VALID", this );
	name_valid -> setGeometry( 10, 110, 250, 15 );

	type_valid = new QCheckBox( "Name Length Ptr: VALID", this );
	type_valid -> setGeometry( 10, 140, 250, 15 );

	size_valid = new QCheckBox( "Column Size Ptr: VALID", this );
	size_valid -> setGeometry( 10, 170, 250, 15 );

	digit_valid = new QCheckBox( "Decimal Digits Ptr: VALID", this );
	digit_valid -> setGeometry( 10, 200, 250, 15 );

	null_valid = new QCheckBox( "Nullable Ptr: VALID", this );
	null_valid -> setGeometry( 10, 230, 250, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( name_valid, SIGNAL( clicked()), this, SLOT( name_clkd()));
	connect( type_valid, SIGNAL( clicked()), this, SLOT( type_clkd()));
	connect( size_valid, SIGNAL( clicked()), this, SLOT( size_clkd()));
	connect( digit_valid, SIGNAL( clicked()), this, SLOT( digit_clkd()));
	connect( null_valid, SIGNAL( clicked()), this, SLOT( null_clkd()));
}

dDescribeCol::~dDescribeCol()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handle;
	delete column_num;
	delete l_column_num;
	delete column_len;
	delete l_column_len;
	delete name_valid;
	delete type_valid;
	delete size_valid;
	delete digit_valid;
	delete null_valid;
}

void dRowCount::Valid()
{
	if ( valid -> isOn() )
	    valid -> setText( "RowCountPtr: SQL_NULL_PTR" );
	else
	    valid -> setText( "RowCountPtr: VALID" );
}

void dRowCount::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLLEN num_rows;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLRowCount():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_PTR" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( valid -> isOn() )
		txt.sprintf( "    RowCountPtr: <null pointer." );
	else
		txt.sprintf( "    RowCountPtr: %p", &num_rows );
	odbctest -> out_win -> insertLineLimited( txt );

	num_rows = -9999;

	SQLRETURN ret = SQLRowCount( in_handle,  valid -> isOn() ? NULL : &num_rows );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );
	if ( num_rows == -9999 )
	{
		txt.sprintf( "    *RowCountPtr: <unchanged>" );
	}
	else
	{
		txt.sprintf( "    *RowCountPtr: %d", num_rows );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dRowCount::dRowCount( OdbcTest *parent, QString name )
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

	valid = new QCheckBox( "RowCountPtr: VALID", this );
	valid -> setGeometry( 10, 80, 300, 15 );

	connect( valid, SIGNAL( clicked()), this, SLOT( Valid()));

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dRowCount::~dRowCount()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete valid;
}

void dMoreResults::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT num_cols;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLMoreResults():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLMoreResults( in_handle );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dMoreResults::dMoreResults( OdbcTest *parent, QString name )
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

dMoreResults::~dMoreResults()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
}

void dSetPos::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLUSMALLINT row_num, op_val, lock_val;
	int index;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLSetPos():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	row_num = atoi( row_number -> text().ascii());
	txt.sprintf( "    Row Number: %d", row_num );
	odbctest -> out_win -> insertLineLimited( txt );

	index = operation -> currentItem(); 

	op_val = set_pos_operation[ index ].value;
	txt.sprintf( "    Operation: %s=%d", set_pos_operation[ index ].text,
		set_pos_operation[ index ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	index = lock_type -> currentItem(); 

	lock_val = set_pos_lock_type[ index ].value;
	txt.sprintf( "    Lock Type: %s=%d", set_pos_lock_type[ index ].text,
		set_pos_lock_type[ index ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLSetPos( in_handle, row_num, op_val, lock_val );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dSetPos::dSetPos( OdbcTest *parent, QString name )
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

	row_number = new QLineEdit( this, "Row Number" );
    row_number -> setGeometry( 130, 80, 70, 20 );
	row_number -> setMaxLength( 6 );
	row_number -> setText( "1" );

	l_row_number = new QLabel( "Row Number:", this );
    l_row_number -> setGeometry( 10, 80, 120, 20 );

	operation = new QComboBox( FALSE, this, "Operation" );
	operation -> setGeometry( 130, 110, 200, 20 );

	parent->fill_list_box( set_pos_operation, operation );

	l_operation = new QLabel( "Operation:", this );
    l_operation -> setGeometry( 10, 110, 120, 20 );

	lock_type = new QComboBox( FALSE, this, "Operation" );
	lock_type -> setGeometry( 130, 140, 200, 20 );

	parent->fill_list_box( set_pos_lock_type, lock_type );

	l_lock_type = new QLabel( "Lock Type:", this );
    l_lock_type -> setGeometry( 10, 140, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dSetPos::~dSetPos()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete operation;
	delete lock_type;
	delete l_handle;
	delete l_operation;
	delete l_lock_type;
	delete row_number;
	delete l_row_number;
}

void dBulkOperations::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLUSMALLINT op_val;
	int index;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLBulkOperations():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	index = operation -> currentItem(); 

	op_val = bulk_operation[ index ].value;
	txt.sprintf( "    Operation: %s=%d", bulk_operation[ index ].text,
		bulk_operation[ index ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	index = operation -> currentItem(); 

	op_val = bulk_operation[ index ].value;
	txt.sprintf( "    Operation: %s=%d",  bulk_operation[ index ].text,
		 bulk_operation[ index ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLBulkOperations( in_handle, op_val );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dBulkOperations::dBulkOperations( OdbcTest *parent, QString name )
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

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	operation = new QComboBox( FALSE, this, "Operation" );
	operation -> setGeometry( 130, 80, 250, 20 );

	parent->fill_list_box( bulk_operation, operation );

	l_operation = new QLabel( "Operation:", this );
    l_operation -> setGeometry( 10, 80, 100, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dBulkOperations::~dBulkOperations()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete operation;
	delete l_handle;
	delete l_operation;
}

void dColAttribute::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	int index;
    SQLUSMALLINT column_number;
    SQLUSMALLINT field_identifier;
    SQLPOINTER buf;
    SQLSMALLINT b_len, *b_len_ptr, b_len_value;
    SQLINTEGER *numeric_ptr, numeric_value;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLColAttribute():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	column_number = atoi( col_num -> text().ascii());
	txt.sprintf( "    Column Number: %d", column_number );
	odbctest -> out_win -> insertLineLimited( txt );

	field_identifier = field_ident_struct[ index = field_ident -> currentItem() ].attr;
	txt.sprintf( "    Field Identifier: %s=%d", 
		field_ident_struct[ index ].text,
		field_ident_struct[ index ].attr );

	b_len = atoi( buffer_len -> text().ascii());
	if ( b_len < 1 )
	{
		b_len = 0;
	}

	if ( char_ptr_valid -> isOn())
	{
		buf = NULL;
	}
	else if ( b_len < 300 )
	{
		buf = malloc( 300 );	
	}
	else
	{
		buf = malloc( b_len );	
	}

	if ( buf )
	{
		txt.sprintf( "    CharacterAttributePtr: %p", buf );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    CharacterAttributePtr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	txt.sprintf( "    Buffer Length: %d", b_len );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strlen_valid -> isOn())
	{
		b_len_ptr = NULL;
	}
	else
	{
		b_len_ptr = &b_len_value;	
	}

	if ( b_len_ptr )
	{
		txt.sprintf( "    StringLengthPtr: %p", b_len_ptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    StringLengthPtr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

    b_len_value = -9999;

	if ( numeric_valid -> isOn())
	{
		numeric_ptr = NULL;
	}
	else
	{
		numeric_ptr = &numeric_value;	
	}

	if ( numeric_ptr )
	{
		txt.sprintf( "    NumericAttributePtr: %p", numeric_ptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    NumericAttributePtr: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

    numeric_value = -99999;

	SQLRETURN ret = SQLColAttribute( in_handle, 
            column_number,
            field_identifier,
            buf,
            b_len,
            b_len_ptr,
            numeric_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
		if ( field_ident_struct[ index ].data_type == SQL_CHAR )
        {
            if ( b_len_ptr )
            {
                if ( b_len_value != -9999 )
                {
	                txt.sprintf( "    *StringLengthPtr: %d", b_len_value );
                }
                else
                {
	                txt.sprintf( "    *StringLengthPtr: <unchanged>" );
                }
	            odbctest -> out_win -> insertLineLimited( txt );
            }

            if ( buf )
            {
	            txt.sprintf( "    *CharacterAttributePtr: %s", buf );
	            odbctest -> out_win -> insertLineLimited( txt );
            }
        }
        else
        {
            if ( numeric_ptr )
            {
	            txt.sprintf( "    *NumericAttributePtr: %d", numeric_value );
	            odbctest -> out_win -> insertLineLimited( txt );
            }
        }
    }

	odbctest -> out_win -> insertLineLimited( "" );

    if ( buf )
        free( buf );
}

void dColAttribute::char_ptr_clkd()
{
	if ( char_ptr_valid -> isOn() )
	    char_ptr_valid -> setText( "CharacterAttributePtr: SQL_NULL_HANDLE" );
	else
	    char_ptr_valid -> setText( "CharacterAttributePtr: VALID" );
}

void dColAttribute::strlen_clkd()
{
	if ( strlen_valid -> isOn() )
	    strlen_valid -> setText( "StringLengthPtr: SQL_NULL_HANDLE" );
	else
	    strlen_valid -> setText( "StringLengthPtr: VALID" );
}

void dColAttribute::numeric_clkd()
{
	if ( numeric_valid -> isOn() )
	    numeric_valid -> setText( "NumericAttributePtr: SQL_NULL_HANDLE" );
	else
	    numeric_valid -> setText( "NumericAttributePtr: VALID" );
}

dColAttribute::dColAttribute( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 250,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 330,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 410,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handles = new QLabel( "Statement Handle:", this );
	l_handles -> setGeometry( 10, 50, 120, 20 );

	col_num = new QLineEdit( this, "Col Num" );
    col_num -> setGeometry( 130, 80, 70, 20 );
	col_num -> setMaxLength( 6 );
	col_num -> setText( "1" );

	l_col_num = new QLabel( "ColumnNumber:", this );
    l_col_num -> setGeometry( 10, 80, 100, 20 );

	field_ident = new QComboBox( FALSE, this, "field_ident" );
	field_ident -> setGeometry( 130, 110, 350, 20 );
	parent->fill_list_box( field_ident_struct, field_ident );

	l_field_ident = new QLabel( "FieldIdentifier::", this );
	l_field_ident -> setGeometry( 10, 110, 120, 20 );

	char_ptr_valid = new QCheckBox( "CharacterAttributePtr: VALID", this );
	char_ptr_valid -> setGeometry( 10, 140, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 400, 140, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "BufferLength:", this );
    l_buffer_len -> setGeometry( 300, 140, 80, 20 );

	strlen_valid = new QCheckBox( "StringLengthPtr: VALID", this );
	strlen_valid -> setGeometry( 10, 170, 300, 15 );

	numeric_valid = new QCheckBox( "NumericAttributePtr: VALID", this );
	numeric_valid -> setGeometry( 10, 200, 300, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( char_ptr_valid, SIGNAL( clicked()), this, SLOT( char_ptr_clkd()));
	connect( strlen_valid, SIGNAL( clicked()), this, SLOT( strlen_clkd()));
	connect( numeric_valid, SIGNAL( clicked()), this, SLOT( numeric_clkd()));
}

dColAttribute::~dColAttribute()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
    delete col_num;
    delete l_col_num;
    delete field_ident;
    delete l_field_ident;
    delete char_ptr_valid;
    delete strlen_valid;
    delete numeric_valid;
    delete buffer_len;
    delete l_buffer_len;
}

void dColAttributes::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	int index;
    SQLUSMALLINT column_number;
    SQLUSMALLINT field_identifier;
    SQLPOINTER buf;
    SQLSMALLINT b_len, *b_len_ptr, b_len_value;
    SQLLEN *numeric_ptr, numeric_value;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLColAttributes():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	column_number = atoi( col_num -> text().ascii());
	txt.sprintf( "    icol: %d", column_number );
	odbctest -> out_win -> insertLineLimited( txt );

	field_identifier = ca_field_ident_struct[ index = field_ident -> currentItem() ].attr;
	txt.sprintf( "    fDescType: %s=%d", 
		ca_field_ident_struct[ index ].text,
		ca_field_ident_struct[ index ].attr );

	b_len = atoi( buffer_len -> text().ascii());
	if ( b_len < 1 )
	{
		b_len = 0;
	}

	if ( char_ptr_valid -> isOn())
	{
		buf = NULL;
	}
	else if ( b_len < 300 )
	{
		buf = malloc( 300 );	
	}
	else
	{
		buf = malloc( b_len );	
	}

	if ( buf )
	{
		txt.sprintf( "    rgbDesc: %p", buf );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    rgbDesc: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	txt.sprintf( "    cbDescMax: %d", b_len );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strlen_valid -> isOn())
	{
		b_len_ptr = NULL;
	}
	else
	{
		b_len_ptr = &b_len_value;	
	}

	if ( b_len_ptr )
	{
		txt.sprintf( "    pcbDesc: %p", b_len_ptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    pcbDesc: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

    b_len_value = -9999;

	if ( numeric_valid -> isOn())
	{
		numeric_ptr = NULL;
	}
	else
	{
		numeric_ptr = &numeric_value;	
	}

	if ( numeric_ptr )
	{
		txt.sprintf( "    pfDesc: %p", numeric_ptr );
		odbctest -> out_win -> insertLineLimited( txt );
	}
	else
	{
		txt.sprintf( "    pfDesc: SQL_NULL_POINTER" );
		odbctest -> out_win -> insertLineLimited( txt );
	}

    numeric_value = -99999;

	SQLRETURN ret = SQLColAttributes( in_handle, 
            column_number,
            field_identifier,
            buf,
            b_len,
            b_len_ptr,
            numeric_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
		if ( ca_field_ident_struct[ index ].data_type == SQL_CHAR )
        {
            if ( b_len_ptr )
            {
                if ( b_len_value != -9999 )
                {
	                txt.sprintf( "    *pcbDesc: %d", b_len_value );
                }
                else
                {
	                txt.sprintf( "    *pcbDesc: <unchanged>" );
                }
	            odbctest -> out_win -> insertLineLimited( txt );
            }

            if ( buf )
            {
	            txt.sprintf( "    *rgbDesc: %s", buf );
	            odbctest -> out_win -> insertLineLimited( txt );
            }
        }
        else
        {
            if ( numeric_ptr )
            {
	            txt.sprintf( "    *pfDesc: %d", numeric_value );
	            odbctest -> out_win -> insertLineLimited( txt );
            }
        }
    }

	odbctest -> out_win -> insertLineLimited( "" );

    if ( buf )
        free( buf );
}

void dColAttributes::char_ptr_clkd()
{
	if ( char_ptr_valid -> isOn() )
	    char_ptr_valid -> setText( "rgbDesc: SQL_NULL_HANDLE" );
	else
	    char_ptr_valid -> setText( "rgbDesc: VALID" );
}

void dColAttributes::strlen_clkd()
{
	if ( strlen_valid -> isOn() )
	    strlen_valid -> setText( "pcbDesc: SQL_NULL_HANDLE" );
	else
	    strlen_valid -> setText( "pcbDesc: VALID" );
}

void dColAttributes::numeric_clkd()
{
	if ( numeric_valid -> isOn() )
	    numeric_valid -> setText( "pfDesc: SQL_NULL_HANDLE" );
	else
	    numeric_valid -> setText( "pfDesc: VALID" );
}

dColAttributes::dColAttributes( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 250,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 330,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 410,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handles = new QLabel( "Statement Handle:", this );
	l_handles -> setGeometry( 10, 50, 120, 20 );

	col_num = new QLineEdit( this, "icol" );
    col_num -> setGeometry( 130, 80, 70, 20 );
	col_num -> setMaxLength( 6 );
	col_num -> setText( "1" );

	l_col_num = new QLabel( "icol:", this );
    l_col_num -> setGeometry( 10, 80, 100, 20 );

	field_ident = new QComboBox( FALSE, this, "field_ident" );
	field_ident -> setGeometry( 130, 110, 350, 20 );
	parent->fill_list_box( ca_field_ident_struct, field_ident );

	l_field_ident = new QLabel( "fDescType:", this );
	l_field_ident -> setGeometry( 10, 110, 120, 20 );

	char_ptr_valid = new QCheckBox( "rgbDesc: VALID", this );
	char_ptr_valid -> setGeometry( 10, 140, 300, 15 );

	buffer_len = new QLineEdit( this, "cbDescMax" );
    buffer_len -> setGeometry( 400, 140, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "cbDescMax:", this );
    l_buffer_len -> setGeometry( 300, 140, 80, 20 );

	strlen_valid = new QCheckBox( "pcbDesc: VALID", this );
	strlen_valid -> setGeometry( 10, 170, 300, 15 );

	numeric_valid = new QCheckBox( "pfDesc: VALID", this );
	numeric_valid -> setGeometry( 10, 200, 300, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( char_ptr_valid, SIGNAL( clicked()), this, SLOT( char_ptr_clkd()));
	connect( strlen_valid, SIGNAL( clicked()), this, SLOT( strlen_clkd()));
	connect( numeric_valid, SIGNAL( clicked()), this, SLOT( numeric_clkd()));
}

dColAttributes::~dColAttributes()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
    delete col_num;
    delete l_col_num;
    delete field_ident;
    delete l_field_ident;
    delete char_ptr_valid;
    delete strlen_valid;
    delete numeric_valid;
    delete buffer_len;
    delete l_buffer_len;
}

void dFetchScroll::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT fetch_orentation;
    SQLINTEGER fetch_offset;
	int index;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLFetchScroll():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	index = orentation -> currentItem(); 

	fetch_orentation = fetch_scroll_orentation[ index ].value;
	txt.sprintf( "    FetchOrentation: %s=%d", fetch_scroll_orentation[ index ].text,
		fetch_scroll_orentation[ index ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	fetch_offset = atoi( offset -> text().ascii());
	txt.sprintf( "    FetchOffset: %d", fetch_offset );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLFetchScroll( in_handle, fetch_orentation, fetch_offset );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dFetchScroll::dFetchScroll( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 200,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 280,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 360,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	orentation = new QComboBox( FALSE, this, "orentation" );
	orentation -> setGeometry( 130, 80, 300, 20 );

	parent->fill_list_box( fetch_scroll_orentation, orentation );

	l_orentation = new QLabel( "FetchOffset:", this );
    l_orentation -> setGeometry( 10, 80, 120, 20 );

	offset = new QLineEdit( this, "Offset" );
    offset -> setGeometry( 130, 110, 70, 20 );
	offset -> setMaxLength( 6 );
	offset -> setText( "1" );

	l_offset = new QLabel( "FetchOffset:", this );
    l_offset -> setGeometry( 10, 110, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dFetchScroll::~dFetchScroll()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete orentation;
	delete offset;
	delete l_handle;
	delete l_orentation;
	delete l_offset;
}

void dExtendedFetch::count_ptr_clkd()
{
	if ( count_ptr_valid -> isOn() )
	    count_ptr_valid -> setText( "RowCountPtr: SQL_NULL_HANDLE" );
	else
	    count_ptr_valid -> setText( "RowCountPtr: VALID" );
}

void dExtendedFetch::status_ptr_clkd()
{
	if ( status_ptr_valid -> isOn() )
	    status_ptr_valid -> setText( "RowStatusArray: SQL_NULL_HANDLE" );
	else
	    status_ptr_valid -> setText( "RowStatusArray: VALID" );
}

void dExtendedFetch::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLSMALLINT fetch_orentation;
    SQLINTEGER fetch_offset;
    SQLUINTEGER row_count, *row_count_ptr;
    SQLUSMALLINT *row_status_array;
	int index;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLExtendedFetch():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	index = orentation -> currentItem(); 

	fetch_orentation = fetch_scroll_orentation[ index ].value;
	txt.sprintf( "    FetchOrentation: %s=%d", fetch_scroll_orentation[ index ].text,
		fetch_scroll_orentation[ index ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	fetch_offset = atoi( offset -> text().ascii());
	txt.sprintf( "    FetchOffset: %d", fetch_offset );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( count_ptr_valid -> isOn())
	{
		row_count_ptr = NULL;
	}
    else
    {
        row_count_ptr = &row_count;
    }

	if ( status_ptr_valid -> isOn())
    {
        row_status_array = NULL;
    }
    else
    {
        if ( hand -> row_array_size > 1 )
        {
            row_status_array = new SQLUSMALLINT[ hand -> row_array_size ];
        }
        else
        {
            row_status_array = new SQLUSMALLINT[ 1 ];
        }
    }

	SQLRETURN ret = SQLExtendedFetch( in_handle, fetch_orentation, fetch_offset, row_count_ptr, row_status_array );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
	    odbctest -> out_win -> insertLineLimited( "  Out:" );
        if ( row_count_ptr )
        {
            txt.sprintf( "    *RowCountPtr: %d", row_count);
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( row_status_array )
        {
            if ( hand -> row_array_size > 1 )
            {
                /*
                 * we need to display the rowset here...
                 */
            }
            else
            {
                txt.sprintf( "    *RowStatusArray: %d", row_status_array[ 0 ]);
                odbctest -> out_win -> insertLineLimited( txt );
            }
        }
    }

	odbctest -> out_win -> insertLineLimited( "" );

    if ( row_status_array )
    {
        delete row_status_array;
    }
}

dExtendedFetch::dExtendedFetch( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 200,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 280,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 360,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	orentation = new QComboBox( FALSE, this, "orentation" );
	orentation -> setGeometry( 130, 80, 300, 20 );

	parent->fill_list_box( fetch_scroll_orentation, orentation );

	l_orentation = new QLabel( "FetchOrentation:", this );
    l_orentation -> setGeometry( 10, 80, 120, 20 );

	offset = new QLineEdit( this, "FetchOffset" );
    offset -> setGeometry( 130, 110, 70, 20 );
	offset -> setMaxLength( 6 );
	offset -> setText( "1" );

	l_offset = new QLabel( "FetchOffset:", this );
    l_offset -> setGeometry( 10, 110, 120, 20 );

	count_ptr_valid = new QCheckBox( "RowCountPtr: VALID", this );
	count_ptr_valid -> setGeometry( 10, 140, 300, 15 );

	status_ptr_valid = new QCheckBox( "RowStatusArray: VALID", this );
	status_ptr_valid -> setGeometry( 10, 170, 300, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );

	connect( count_ptr_valid, SIGNAL( clicked()), this, SLOT( count_ptr_clkd()));
	connect( status_ptr_valid, SIGNAL( clicked()), this, SLOT( status_ptr_clkd()));
}

dExtendedFetch::~dExtendedFetch()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete orentation;
	delete offset;
	delete l_handle;
	delete l_orentation;
	delete l_offset;
    delete count_ptr_valid;
    delete status_ptr_valid;
}

void dSetScrollOptions::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLUSMALLINT fConcurrency, crowRowset;
    SQLINTEGER crowKeyset;
	int index;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLSetScrollOptions():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	index = concurrency -> currentItem(); 

	fConcurrency = set_scroll_cur_operation[ index ].value;
	txt.sprintf( "    fConcurrency: %s=%d", set_scroll_cur_operation[ index ].text,
		set_scroll_cur_operation[ index ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	index = keyset  -> currentItem(); 

	crowKeyset = set_scroll_ks_values[ index ].value;
	txt.sprintf( "    crowKeyset: %s=%d", set_scroll_ks_values[ index ].text,
		set_scroll_ks_values[ index ].value );
	odbctest -> out_win -> insertLineLimited( txt );

	crowRowset = atoi( rowset -> text().ascii());
	txt.sprintf( "    crowRowset: %d", crowRowset );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLSetScrollOptions( in_handle, fConcurrency, crowKeyset, crowRowset );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
        if ( crowRowset < 1 )
            crowRowset = 1;
        hand -> row_array_size = crowRowset;
    }

	odbctest -> out_win -> insertLineLimited( "" );
}

dSetScrollOptions::dSetScrollOptions( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 200,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 280,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 360,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Statement Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Statement Handle:", this );
	l_handle -> setGeometry( 10, 50, 120, 20 );

	concurrency = new QComboBox( FALSE, this, "concurrency" );
	concurrency -> setGeometry( 130, 80, 300, 20 );

	parent->fill_list_box( set_scroll_cur_operation, concurrency );

	l_concurrency = new QLabel( "fConcurrency:", this );
    l_concurrency -> setGeometry( 10, 80, 120, 20 );

	keyset = new QComboBox( FALSE, this, "Operation" );
	keyset -> setGeometry( 130, 110, 300, 20 );

	parent->fill_list_box( set_scroll_ks_values, keyset );

	l_keyset = new QLabel( "crowKeyset:", this );
    l_keyset -> setGeometry( 10, 110, 120, 20 );

	rowset = new QLineEdit( this, "crowRowset" );
    rowset -> setGeometry( 130, 140, 70, 20 );
	rowset -> setMaxLength( 6 );
	rowset -> setText( "1" );

	l_rowset = new QLabel( "crowRowset:", this );
    l_rowset -> setGeometry( 10, 140, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dSetScrollOptions::~dSetScrollOptions()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete concurrency;
	delete keyset;
	delete l_handle;
	delete l_concurrency;
	delete l_keyset;
	delete rowset;
	delete l_rowset;
}

void OdbcTest::sqlbindcol()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::sqlbulkoperations()
{
	dBulkOperations *dlg = new dBulkOperations( this, "SQLBulkOperations" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlcolattribute()
{
	dColAttribute *dlg = new dColAttribute( this, "SQLColAttribute" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlcolattributes()
{
	dColAttributes *dlg = new dColAttributes( this, "SQLColAttributes" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqldescribecol()
{
	dDescribeCol *dlg = new dDescribeCol( this, "SQLDescribeCol" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlextendedfetch()
{
	dExtendedFetch *dlg = new dExtendedFetch( this, "SQLExtendedFetch" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlfetch()
{
	dFetch *dlg = new dFetch( this, "SQLFetch" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlfetchscroll()
{
	dFetchScroll *dlg = new dFetchScroll( this, "SQLFetchScroll" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetdata()
{
	dGetData *dlg = new dGetData( this, "SQLGetData" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlmoreresults()
{
	dMoreResults *dlg = new dMoreResults( this, "SQLMoreResults" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlnumresultscols()
{
	dNumResultCols *dlg = new dNumResultCols( this, "SQLNumResultCols" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlrowcount()
{
	dRowCount *dlg = new dRowCount( this, "SQLRowCount" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlsetpos()
{
	dSetPos *dlg = new dSetPos( this, "SQLSetPos" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlsetscrolloptions()
{
	dSetScrollOptions *dlg = new dSetScrollOptions( this, "SQLSetScrollOptions" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::bindcolall()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::describecolall()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::fetchall()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::getdatastmt( SQLHANDLE hstmt )
{
	SQLRETURN ret;
	SQLSMALLINT column_count;
	struct column_info
	{
		SQLCHAR column_name[ 128 ];
		SQLSMALLINT data_type;
		SQLULEN column_size;
		SQLCHAR column_buffer[ 64 ];
		SQLLEN str_len;
	};
	int i;
	int line_count;

	if ( !hstmt )
	{
		return;
	}

	/*
	 * Get number of columns
	 */ 

	ret = SQLNumResultCols( hstmt, &column_count );
	if ( !SQL_SUCCEEDED( ret ))
	{
		dumpError( SQL_HANDLE_STMT, hstmt );
		out_win->insertLineLimited( "SQLNumResultCols() failed\n" );
		return; 
	}

	if ( column_count < 1 )
	{
		out_win->insertLineLimited( "No result columns\n" );
		return; 
	}

	/* 
	 * get column names and types
	 */

	struct column_info *col_info = new struct column_info[ column_count ];

	for ( i = 0; i < column_count; i ++ )
	{
		ret = SQLDescribeCol( hstmt, i + 1,
			col_info[ i ].column_name, sizeof( col_info[ i ].column_name ),
			NULL, &col_info[ i ].data_type, &col_info[ i ].column_size,
			NULL, NULL );

		if ( !SQL_SUCCEEDED( ret ))
		{
			dumpError( SQL_HANDLE_STMT, hstmt );
			out_win->insertLineLimited( "SQLDescribeCol() failed\n" );
			delete col_info;
			return; 
		}
	}

	/*
	 * output buffer
	 */

	char *line = new char[ column_count * ( sizeof( col_info[ i ].column_name ) + 1 )];

	line[ 0 ] = '\0';
	for ( i = 0; i < column_count; i ++ )
	{
		if ( i > 0 )
			strcat( line, ":" );
		strcat( line, (char *)col_info[ i ].column_name );
	}

	out_win -> insertLineLimited( line );

	/*
	 * get the data
	 */

	line_count = 0;

	while( SQL_SUCCEEDED( ret = SQLFetch( hstmt )))
	{
		line_count ++;	
		line[ 0 ] = '\0';

		for ( i = 0; i < column_count; i ++ )
		{
			ret = SQLGetData( hstmt, i + 1, 
				SQL_C_CHAR, col_info[ i ].column_buffer,
				sizeof( col_info[ i ].column_buffer ),
				&col_info[ i ].str_len );

			if ( !SQL_SUCCEEDED( ret ))
			{
				dumpError( SQL_HANDLE_STMT, hstmt );
				out_win->insertLineLimited( "SQLGetData() failed\n" );
				delete line;
				delete col_info;
				return; 
			}

			if ( i > 0 )
				strcat( line, ":" );

			if ( col_info[ i ].str_len < 0 )
			{
				strcat( line, "NULL" );
			}
			else
			{
				strcat( line, (char*)col_info[ i ].column_buffer );
			}
		}
		out_win->insertLineLimited( line );
	}

	if ( ret != SQL_NO_DATA )
	{
		dumpError( SQL_HANDLE_STMT, hstmt );
		out_win->insertLineLimited( "SQLFetch() failed\n" );
		delete line;
		delete col_info;
		return; 
	}

	/*
	 * Eat any trunc errors
	 */

	do
	{
		SQLINTEGER native;
		SQLCHAR sqlstate[ 6 ];
		SQLCHAR msg[ SQL_MAX_MESSAGE_LENGTH ];

		ret = SQLError( SQL_NULL_HENV, SQL_NULL_HDBC, hstmt,
				sqlstate, &native, msg, sizeof( msg ), NULL );
	}
	while( SQL_SUCCEEDED( ret ));

	/*
	 * close off the cursor
	 */ 

	ret = SQLCloseCursor( hstmt );

	if ( !SQL_SUCCEEDED( ret ))
	{
		dumpError( SQL_HANDLE_STMT, hstmt );
		out_win->insertLineLimited( "SQLCloseCursor() failed\n" );
		delete line;
		delete col_info;
		return; 
	}

	sprintf( line, "%d Line(s) Fetched\n", line_count );
	out_win->insertLineLimited( line );

	delete line;
	delete col_info;
}

void OdbcTest::getdataall()
{
	/*
	 * look for a statement handle
	 */

	Handle *hand;
	SQLHANDLE hstmt;
	int handle_count = 0;
	char txt[ 128 ];

    for ( hand=listHandle.first();
		hand != 0; 
		hand=listHandle.next() )
	{
		hand->toStr( txt );
		if ( hand->getType() == SQL_HANDLE_STMT )
		{
			handle_count ++;
			hstmt = hand->getHandle();
		}
	}

	if ( handle_count == 0 )
	{
    	QMessageBox::about( this, "Get Data All",
			"No statement handles avalable" );
	}
	else if ( handle_count == 1 )
	{
		getdatastmt( hstmt );
	}
	else
	{
		getdatastmt();
	}
}

void OdbcTest::showboundcols()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::displayrowset()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}
