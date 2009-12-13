/*********************************************************************
 *
 * Written by Nick Gorham
 * (nick@lurcher.org).
 *
 * copyright (c) 1999 Nick Gorham
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 **********************************************************************
 *
 * $Id: catalog.cpp,v 1.1.1.1 2001/10/17 16:40:30 lurcher Exp $
 *
 * $Log: catalog.cpp,v $
 * Revision 1.1.1.1  2001/10/17 16:40:30  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.4  2001/07/20 09:42:58  nick
 *
 * Replace char[] with QString to avoid buffer overrun
 *
 * Revision 1.3  2001/06/04 15:24:49  nick
 *
 * Add port to MAC OSX and QT3 changes
 *
 * Revision 1.2  2001/05/31 16:05:55  nick
 *
 * Fix problems with postgres closing local sockets
 * Make odbctest build with QT 3 (it doesn't work due to what I think are bugs
 * in QT 3)
 * Fix a couple of problems in the cursor lib
 *
 * Revision 1.1.1.1  2000/09/04 16:42:53  nick
 * Imported Sources
 *
 * Revision 1.5  2000/09/04 15:43:59  ngorham
 *
 * Fix incorrect call in TablePrivs
 *
 * Revision 1.4  2000/06/13 12:30:20  ngorham
 *
 * Enough there for the first release I think
 *
 * Revision 1.3  2000/06/05 16:53:15  ngorham
 *
 * Next lot of updates
 *
 * Revision 1.2  2001/05/31 10:26:26  ngorham
 *
 * Fix a few minor typo's
 *
 * Revision 1.1  2000/05/04 17:04:47  ngorham
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
#include <sql.h>
#include <sqlext.h>
#include "catalog.h"
#include "odbctest.h"

static attr_options get_type_options[] = 
{
	{ "SQL_ALL_TYPES", SQL_ALL_TYPES, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_CHAR", SQL_CHAR, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_VARCHAR", SQL_VARCHAR, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_LONGVARCHAR", SQL_LONGVARCHAR, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_DECIMAL", SQL_DECIMAL, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_NUMERIC", SQL_NUMERIC, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_SMALLINT", SQL_SMALLINT, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_INTEGER", SQL_INTEGER, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_REAL", SQL_REAL, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_FLOAT", SQL_FLOAT, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_DOUBLE", SQL_DOUBLE, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_BIT", SQL_BIT, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_TINYINT", SQL_TINYINT, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_BIGINT", SQL_BIGINT, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_BINARY", SQL_BINARY, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_VARBINARY", SQL_VARBINARY, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_LONGVARBINARY", SQL_LONGVARBINARY, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_TYPE_DATE", SQL_TYPE_DATE, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_DATE", SQL_DATE, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_TYPE_TIME", SQL_TYPE_TIME, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_TIME", SQL_TIME, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_TYPE_TIMESTAMP", SQL_TYPE_TIMESTAMP, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_TIMESTAMP", SQL_TIMESTAMP, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_INTERVAL_MONTH", SQL_INTERVAL_MONTH, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_YEAR", SQL_INTERVAL_YEAR, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_YEAR_TO_MONTH", SQL_INTERVAL_YEAR_TO_MONTH, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_DAY", SQL_INTERVAL_DAY, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_HOUR", SQL_INTERVAL_HOUR, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_MINUTE", SQL_INTERVAL_MINUTE, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_SECOND", SQL_INTERVAL_SECOND, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_DAY_TO_HOUR", SQL_INTERVAL_DAY_TO_HOUR, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_DAY_TO_MINUTE", SQL_INTERVAL_DAY_TO_MINUTE, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_DAY_TO_SECOND", SQL_INTERVAL_DAY_TO_SECOND, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_HOUR_TO_MINUTE", SQL_INTERVAL_HOUR_TO_MINUTE, 
		{
			{ NULL }
		}, "3.0"
	},
	{ "SQL_INTERVAL_HOUR_TO_SECOND", SQL_INTERVAL_HOUR_TO_SECOND, 
		{
			{ NULL }
		}, "3.0"
	},
	{ NULL 
	}
};

static attr_options unique_options[ 3 ] = 
{
	{ "SQL_INDEX_UNIQUE", SQL_INDEX_UNIQUE, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_INDEX_ALL", SQL_INDEX_ALL, 
		{
			{ NULL }
		}, "1.0"
	},
	{ NULL 
	}
};

static attr_options reserved_options[ 3 ] = 
{
	{ "SQL_QUICK", SQL_QUICK, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_ENSURE", SQL_ENSURE, 
		{
			{ NULL }
		}, "1.0"
	},
	{ NULL 
	}
};

static attr_options ident_options[ 3 ] = 
{
	{ "SQL_BEST_ROWID", SQL_BEST_ROWID, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_ROWVER", SQL_ROWVER, 
		{
			{ NULL }
		}, "1.0"
	},
	{ NULL 
	}
};

static attr_options scope_options[ 4 ] = 
{
	{ "SQL_SCOPE_CURROW", SQL_SCOPE_CURROW, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_SCOPE_TRANSACTION", SQL_SCOPE_TRANSACTION, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_SCOPE_SESSION", SQL_SCOPE_SESSION, 
		{
			{ NULL }
		}, "1.0"
	},
	{ NULL 
	}
};

static attr_options null_options[ 3 ] = 
{
	{ "SQL_NO_NULLS", SQL_NO_NULLS, 
		{
			{ NULL }
		}, "1.0"
	},
	{ "SQL_NULLABLE", SQL_NULLABLE, 
		{
			{ NULL }
		}, "1.0"
	},
	{ NULL 
	}
};

void dTables::Ok()
{
	const char *cat, *sch, *tab, *typ;
	const char *lcat, *lsch, *ltab, *ltyp;
	QString qcat, qsch, qtab, qtyp;
	QString qlcat, qlsch, qltab, qltyp;
	int cat_len, sch_len, tab_len, typ_len;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog ->currentText();
	qsch = schema ->currentText();
	qtab = table ->currentText();
	qtyp = type ->currentText();
	qlcat = catalog_len ->currentText();
	qlsch = schema_len ->currentText();
	qltab = table_len ->currentText();
	qltyp = type_len ->currentText();

	cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();
	typ = qtyp.ascii();
	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();
	ltyp = qltyp.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLTables():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    Catalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    Catalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Catalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    Catalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    Catalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    Schema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    Schema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Schema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    Schema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    Schema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    Table: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    Table: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Table: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    Table Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    Table Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( typ, "<null ptr>" ) == 0 )
	{
		typ = NULL;
		odbctest -> out_win -> insertLineLimited( "    Table Type: <null ptr>" );
	}
	else if ( strcmp( typ, "<empty string>" ) == 0 )
	{
		typ = "";
		odbctest -> out_win -> insertLineLimited( "    Table Type: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Table Type: %s", typ );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltyp, "SQL_NTS", 7 ) == 0 )
	{
		typ_len = SQL_NTS;
		txt.sprintf( "    Table Type Len: SQL_NTS=-3" );
	}
	else
	{
		typ_len = atoi( ltyp );
		txt.sprintf( "    Table Type Len: %d", typ_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	SQLRETURN ret = SQLTables( in_handle, 
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len,
			(SQLCHAR*)typ, typ_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dTables::dTables( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 50, 150, 20 );
	l_catalog = new QLabel( "Catalog:", this );
	l_catalog -> setGeometry( 10, 50, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 80, 150, 20 );
	l_schema = new QLabel( "Schema:", this );
	l_schema -> setGeometry( 10, 80, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "table" );
	table -> setGeometry( 100, 110, 150, 20 );
	l_table = new QLabel( "Table:", this );
	l_table -> setGeometry( 10, 110, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	type = new QComboBox( TRUE, this, "table" );
	type -> setGeometry( 100, 140, 150, 20 );
	l_type = new QLabel( "Table Type:", this );
	l_type -> setGeometry( 10, 140, 90, 20 );
	type -> insertItem( "<null ptr>", 0 );
	type -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 50, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 50, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 80, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 80, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "table len" );
	table_len -> setGeometry( 370, 110, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 110, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

	type_len = new QComboBox( TRUE, this, "type len" );
	type_len -> setGeometry( 370, 140, 150, 20 );
	l_type_len = new QLabel( "String Length:", this );
	l_type_len -> setGeometry( 280, 140, 90, 20 );
	type_len -> insertItem( "SQL_NTS=-3", 0 );
	type_len -> insertItem( "0", 1 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dTables::~dTables()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete type;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete l_type;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete type_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
	delete l_type_len;
}

void dColumns::Ok()
{
	const char *cat, *sch, *tab, *col;
	const char *lcat, *lsch, *ltab, *lcol;
	QString qcat, qsch, qtab, qcol;
	QString qlcat, qlsch, qltab, qlcol;
	int cat_len, sch_len, tab_len, col_len;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog ->currentText();
	qsch = schema ->currentText();
	qtab = table ->currentText();
	qcol = column ->currentText();
	qlcat = catalog_len ->currentText();
	qlsch = schema_len ->currentText();
	qltab = table_len ->currentText();
	qlcol = column_len ->currentText();

    cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();
	col = qcol.ascii();
	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();
	lcol = qlcol.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLColumns():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    Catalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    Catalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Catalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    Catalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    Catalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    Schema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    Schema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Schema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    Schema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    Schema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    Table: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    Table: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Table: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    Table Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    Table Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( col, "<null ptr>" ) == 0 )
	{
		col = NULL;
		odbctest -> out_win -> insertLineLimited( "    Column: <null ptr>" );
	}
	else if ( strcmp( col, "<empty string>" ) == 0 )
	{
		col = "";
		odbctest -> out_win -> insertLineLimited( "    Column: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Column: %s", col );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcol, "SQL_NTS", 7 ) == 0 )
	{
		col_len = SQL_NTS;
		txt.sprintf( "    Column Len: SQL_NTS=-3" );
	}
	else
	{
		col_len = atoi( lcol );
		txt.sprintf( "    Column Len: %d", col_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	SQLRETURN ret = SQLColumns( in_handle, 
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len,
			(SQLCHAR*)col, col_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

void dColumnPrivileges::Ok()
{
	const char *cat, *sch, *tab, *col;
	const char *lcat, *lsch, *ltab, *lcol;
	QString qcat, qsch, qtab, qcol;
	QString qlcat, qlsch, qltab, qlcol;
	int cat_len, sch_len, tab_len, col_len;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog ->currentText();
	qsch = schema ->currentText();
	qtab = table ->currentText();
	qcol = column ->currentText();
	qlcat = catalog_len ->currentText();
	qlsch = schema_len ->currentText();
	qltab = table_len ->currentText();
	qlcol = column_len ->currentText();

	cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();
	col = qcol.ascii();
	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();
	lcol = qlcol.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLColumnPrivileges():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    Catalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    Catalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Catalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    Catalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    Catalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    Schema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    Schema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Schema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    Schema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    Schema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    Table: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    Table: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Table: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    Table Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    Table Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( col, "<null ptr>" ) == 0 )
	{
		col = NULL;
		odbctest -> out_win -> insertLineLimited( "    Column: <null ptr>" );
	}
	else if ( strcmp( col, "<empty string>" ) == 0 )
	{
		col = "";
		odbctest -> out_win -> insertLineLimited( "    Column: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Column: %s", col );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcol, "SQL_NTS", 7 ) == 0 )
	{
		col_len = SQL_NTS;
		txt.sprintf( "    Column Len: SQL_NTS=-3" );
	}
	else
	{
		col_len = atoi( lcol );
		txt.sprintf( "    Column Len: %d", col_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	SQLRETURN ret = SQLColumnPrivileges( in_handle, 
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len,
			(SQLCHAR*)col, col_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dColumnPrivileges::dColumnPrivileges( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 50, 150, 20 );
	l_catalog = new QLabel( "Catalog:", this );
	l_catalog -> setGeometry( 10, 50, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 80, 150, 20 );
	l_schema = new QLabel( "Schema:", this );
	l_schema -> setGeometry( 10, 80, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "table" );
	table -> setGeometry( 100, 110, 150, 20 );
	l_table = new QLabel( "Table:", this );
	l_table -> setGeometry( 10, 110, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	column = new QComboBox( TRUE, this, "column" );
	column -> setGeometry( 100, 140, 150, 20 );
	l_column = new QLabel( "Column:", this );
	l_column -> setGeometry( 10, 140, 90, 20 );
	column -> insertItem( "<null ptr>", 0 );
	column -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 50, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 50, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 80, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 80, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "table len" );
	table_len -> setGeometry( 370, 110, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 110, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

	column_len = new QComboBox( TRUE, this, "column len" );
	column_len -> setGeometry( 370, 140, 150, 20 );
	l_column_len = new QLabel( "String Length:", this );
	l_column_len -> setGeometry( 280, 140, 90, 20 );
	column_len -> insertItem( "SQL_NTS=-3", 0 );
	column_len -> insertItem( "0", 1 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dColumnPrivileges::~dColumnPrivileges()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete column;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete l_column;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete column_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
	delete l_column_len;
}

dColumns::dColumns( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 50, 150, 20 );
	l_catalog = new QLabel( "Catalog:", this );
	l_catalog -> setGeometry( 10, 50, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 80, 150, 20 );
	l_schema = new QLabel( "Schema:", this );
	l_schema -> setGeometry( 10, 80, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "table" );
	table -> setGeometry( 100, 110, 150, 20 );
	l_table = new QLabel( "Table:", this );
	l_table -> setGeometry( 10, 110, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	column = new QComboBox( TRUE, this, "column" );
	column -> setGeometry( 100, 140, 150, 20 );
	l_column = new QLabel( "Column:", this );
	l_column -> setGeometry( 10, 140, 90, 20 );
	column -> insertItem( "<null ptr>", 0 );
	column -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 50, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 50, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 80, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 80, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "table len" );
	table_len -> setGeometry( 370, 110, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 110, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

	column_len = new QComboBox( TRUE, this, "column len" );
	column_len -> setGeometry( 370, 140, 150, 20 );
	l_column_len = new QLabel( "String Length:", this );
	l_column_len -> setGeometry( 280, 140, 90, 20 );
	column_len -> insertItem( "SQL_NTS=-3", 0 );
	column_len -> insertItem( "0", 1 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dColumns::~dColumns()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete column;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete l_column;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete column_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
	delete l_column_len;
}

void dForeignKeys::Ok()
{
	const char *cat, *sch, *tab;
	const char *fk_cat, *fk_sch, *fk_tab;
	const char *lcat, *lsch, *ltab;
	const char *fk_lcat, *fk_lsch, *fk_ltab;
	QString qcat, qsch, qtab;
	QString qfk_cat, qfk_sch, qfk_tab;
	QString qlcat, qlsch, qltab;
	QString qfk_lcat, qfk_lsch, qfk_ltab;
	int cat_len, sch_len, tab_len;
	int fk_cat_len, fk_sch_len, fk_tab_len;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog ->currentText();
	qsch = schema ->currentText();
	qtab = table ->currentText();
	qlcat = catalog_len ->currentText();
	qlsch = schema_len ->currentText();
	qltab = table_len ->currentText();
	qfk_cat = fk_catalog ->currentText();
	qfk_sch = fk_schema ->currentText();
	qfk_tab = fk_table ->currentText();
	qfk_lcat = fk_catalog_len ->currentText();
	qfk_lsch = fk_schema_len ->currentText();
	qfk_ltab = fk_table_len ->currentText();

	cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();
	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();
	fk_cat = qfk_cat.ascii();
	fk_sch = qfk_sch.ascii();
	fk_tab = qfk_tab.ascii();
	fk_lcat = qfk_lcat.ascii();
	fk_lsch = qfk_lsch.ascii();
	fk_ltab = qfk_ltab.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLForeignKeys():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    PKCatalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    PKCatalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    PKCatalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    PKCatalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    PKCatalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    PKSchema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    PKSchema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    PKSchema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    PKSchema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    PKSchema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    PKTable: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    PKTable: <empty string>" );
	}
	else
	{
		txt.sprintf( "    PKTable: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    PKTable Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    PKTable Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( fk_cat, "<null ptr>" ) == 0 )
	{
		fk_cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    FKCatalog: <null ptr>" );
	}
	else if ( strcmp( fk_cat, "<empty string>" ) == 0 )
	{
		fk_cat = "";
		odbctest -> out_win -> insertLineLimited( "    FKCatalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    FKCatalog: %s", fk_cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( fk_lcat, "SQL_NTS", 7 ) == 0 )
	{
		fk_cat_len = SQL_NTS;
		txt.sprintf( "    FKCatalog Len: SQL_NTS=-3" );
	}
	else
	{
		fk_cat_len = atoi( fk_lcat );
		txt.sprintf( "    FKCatalog Len: %d", fk_cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( fk_sch, "<null ptr>" ) == 0 )
	{
		fk_sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    FKSchema: <null ptr>" );
	}
	else if ( strcmp( fk_sch, "<empty string>" ) == 0 )
	{
		fk_sch = "";
		odbctest -> out_win -> insertLineLimited( "    FKSchema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    FKSchema: %s", fk_sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( fk_lsch, "SQL_NTS", 7 ) == 0 )
	{
		fk_sch_len = SQL_NTS;
		txt.sprintf( "    FKSchema Len: SQL_NTS=-3" );
	}
	else
	{
		fk_sch_len = atoi( lsch );
		txt.sprintf( "    FKSchema Len: %d", fk_sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( fk_tab, "<null ptr>" ) == 0 )
	{
		fk_tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    FKTable: <null ptr>" );
	}
	else if ( strcmp( fk_tab, "<empty string>" ) == 0 )
	{
		fk_tab = "";
		odbctest -> out_win -> insertLineLimited( "    FKTable: <empty string>" );
	}
	else
	{
		txt.sprintf( "    FKTable: %s", fk_tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( fk_ltab, "SQL_NTS", 7 ) == 0 )
	{
		fk_tab_len = SQL_NTS;
		txt.sprintf( "    FKTable Len: SQL_NTS=-3" );
	}
	else
	{
		fk_tab_len = atoi( ltab );
		txt.sprintf( "    FKTable Len: %d", fk_tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLForeignKeys( in_handle, 
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len,
			(SQLCHAR*)fk_cat, fk_cat_len,
			(SQLCHAR*)fk_sch, fk_sch_len, 
			(SQLCHAR*)fk_tab, fk_tab_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dForeignKeys::dForeignKeys( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 50, 150, 20 );
	l_catalog = new QLabel( "PKCatalog:", this );
	l_catalog -> setGeometry( 10, 50, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 80, 150, 20 );
	l_schema = new QLabel( "PKSchema:", this );
	l_schema -> setGeometry( 10, 80, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "table" );
	table -> setGeometry( 100, 110, 150, 20 );
	l_table = new QLabel( "PKTable:", this );
	l_table -> setGeometry( 10, 110, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	fk_catalog = new QComboBox( TRUE, this, "catalog" );
	fk_catalog -> setGeometry( 100, 140, 150, 20 );
	fk_l_catalog = new QLabel( "FKCatalog:", this );
	fk_l_catalog -> setGeometry( 10, 140, 90, 20 );
	fk_catalog -> insertItem( "<null ptr>", 0 );
	fk_catalog -> insertItem( "<empty string>", 1 );

	fk_schema = new QComboBox( TRUE, this, "schema" );
	fk_schema -> setGeometry( 100, 170, 150, 20 );
	fk_l_schema = new QLabel( "FKSchema:", this );
	fk_l_schema -> setGeometry( 10, 170, 90, 20 );
	fk_schema -> insertItem( "<null ptr>", 0 );
	fk_schema -> insertItem( "<empty string>", 1 );

	fk_table = new QComboBox( TRUE, this, "table" );
	fk_table -> setGeometry( 100, 200, 150, 20 );
	fk_l_table = new QLabel( "FKTable:", this );
	fk_l_table -> setGeometry( 10, 200, 90, 20 );
	fk_table -> insertItem( "<null ptr>", 0 );
	fk_table -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 140, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 140, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 80, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 80, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "table len" );
	table_len -> setGeometry( 370, 110, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 110, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

	fk_catalog_len = new QComboBox( TRUE, this, "catalog len" );
	fk_catalog_len -> setGeometry( 370, 140, 150, 20 );
	fk_l_catalog_len = new QLabel( "String Length:", this );
	fk_l_catalog_len -> setGeometry( 280, 140, 90, 20 );
	fk_catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	fk_catalog_len -> insertItem( "0", 1 );

	fk_schema_len = new QComboBox( TRUE, this, "schema len" );
	fk_schema_len -> setGeometry( 370, 170, 150, 20 );
	fk_l_schema_len = new QLabel( "String Length:", this );
	fk_l_schema_len -> setGeometry( 280, 170, 90, 20 );
	fk_schema_len -> insertItem( "SQL_NTS=-3", 0 );
	fk_schema_len -> insertItem( "0", 1 );

	fk_table_len = new QComboBox( TRUE, this, "table len" );
	fk_table_len -> setGeometry( 370, 200, 150, 20 );
	fk_l_table_len = new QLabel( "String Length:", this );
	fk_l_table_len -> setGeometry( 280, 200, 90, 20 );
	fk_table_len -> insertItem( "SQL_NTS=-3", 0 );
	fk_table_len -> insertItem( "0", 1 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dForeignKeys::~dForeignKeys()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
	delete fk_catalog;
	delete fk_schema;
	delete fk_table;
	delete fk_l_catalog;
	delete fk_l_schema;
	delete fk_l_table;
	delete fk_catalog_len;
	delete fk_schema_len;
	delete fk_table_len;
	delete fk_l_catalog_len;
	delete fk_l_schema_len;
	delete fk_l_table_len;
}

void dPrimaryKeys::Ok()
{
	const char *cat, *sch, *tab;
	const char *lcat, *lsch, *ltab;
	QString qcat, qsch, qtab;
	QString qlcat, qlsch, qltab;
	int cat_len, sch_len, tab_len;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog ->currentText();
	qsch = schema ->currentText();
	qtab = table ->currentText();
	qlcat = catalog_len ->currentText();
	qlsch = schema_len ->currentText();
	qltab = table_len ->currentText();

	cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();
	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLPrimaryKeys():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    Catalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    Catalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Catalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    Catalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    Catalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    Schema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    Schema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Schema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    Schema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    Schema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    Table: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    Table: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Table: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    Table Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    Table Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	SQLRETURN ret = SQLPrimaryKeys( in_handle, 
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dPrimaryKeys::dPrimaryKeys( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 50, 150, 20 );
	l_catalog = new QLabel( "Catalog:", this );
	l_catalog -> setGeometry( 10, 50, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 80, 150, 20 );
	l_schema = new QLabel( "Schema:", this );
	l_schema -> setGeometry( 10, 80, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "table" );
	table -> setGeometry( 100, 110, 150, 20 );
	l_table = new QLabel( "Table:", this );
	l_table -> setGeometry( 10, 110, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 50, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 50, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 80, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 80, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "table len" );
	table_len -> setGeometry( 370, 110, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 110, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dPrimaryKeys::~dPrimaryKeys()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
}

void dProcedures::Ok()
{
	const char *cat, *sch, *tab;
	const char *lcat, *lsch, *ltab;
	QString qcat, qsch, qtab;
	QString qlcat, qlsch, qltab;
	int cat_len, sch_len, tab_len;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog ->currentText();
	qsch = schema ->currentText();
	qtab = table ->currentText();
	qlcat = catalog_len ->currentText();
	qlsch = schema_len ->currentText();
	qltab = table_len ->currentText();

	cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();
	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLProcedures():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    Catalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    Catalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Catalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    Catalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    Catalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    Schema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    Schema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Schema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    Schema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    Schema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    Procedure: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    Procedure: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Procedure: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    Procedure Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    Procedure Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	SQLRETURN ret = SQLProcedures( in_handle, 
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dProcedures::dProcedures( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 50, 150, 20 );
	l_catalog = new QLabel( "Catalog:", this );
	l_catalog -> setGeometry( 10, 50, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 80, 150, 20 );
	l_schema = new QLabel( "Schema:", this );
	l_schema -> setGeometry( 10, 80, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "proc" );
	table -> setGeometry( 100, 110, 150, 20 );
	l_table = new QLabel( "Procedure:", this );
	l_table -> setGeometry( 10, 110, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 50, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 50, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 80, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 80, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "proc len" );
	table_len -> setGeometry( 370, 110, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 110, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dProcedures::~dProcedures()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
}

void dProcedureColumns::Ok()
{
	const char *cat, *sch, *tab, *col;
	const char *lcat, *lsch, *ltab, *lcol;
	QString qcat, qsch, qtab, qcol;
	QString qlcat, qlsch, qltab, qlcol;
	int cat_len, sch_len, tab_len, col_len;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog ->currentText();
	qsch = schema ->currentText();
	qtab = table ->currentText();
	qcol = column ->currentText();
	qlcat = catalog_len ->currentText();
	qlsch = schema_len ->currentText();
	qltab = table_len ->currentText();
	qlcol = column_len ->currentText();

	cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();
	col = qcol.ascii();
	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();
	lcol = qlcol.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLProcedureColumns():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    Catalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    Catalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Catalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    Catalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    Catalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    Schema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    Schema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Schema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    Schema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    Schema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    Procedure: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    Procedure: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Procedure: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    Table Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    Table Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( col, "<null ptr>" ) == 0 )
	{
		col = NULL;
		odbctest -> out_win -> insertLineLimited( "    Column: <null ptr>" );
	}
	else if ( strcmp( col, "<empty string>" ) == 0 )
	{
		col = "";
		odbctest -> out_win -> insertLineLimited( "    Column: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Column: %s", col );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcol, "SQL_NTS", 7 ) == 0 )
	{
		col_len = SQL_NTS;
		txt.sprintf( "    Column Len: SQL_NTS=-3" );
	}
	else
	{
		col_len = atoi( lcol );
		txt.sprintf( "    Column Len: %d", col_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	SQLRETURN ret = SQLProcedureColumns( in_handle, 
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len,
			(SQLCHAR*)col, col_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dProcedureColumns::dProcedureColumns( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 50, 150, 20 );
	l_catalog = new QLabel( "Catalog:", this );
	l_catalog -> setGeometry( 10, 50, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 80, 150, 20 );
	l_schema = new QLabel( "Schema:", this );
	l_schema -> setGeometry( 10, 80, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "procedure" );
	table -> setGeometry( 100, 110, 150, 20 );
	l_table = new QLabel( "Procedure:", this );
	l_table -> setGeometry( 10, 110, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	column = new QComboBox( TRUE, this, "column" );
	column -> setGeometry( 100, 140, 150, 20 );
	l_column = new QLabel( "Column:", this );
	l_column -> setGeometry( 10, 140, 90, 20 );
	column -> insertItem( "<null ptr>", 0 );
	column -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 50, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 50, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 80, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 80, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "proc len" );
	table_len -> setGeometry( 370, 110, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 110, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

	column_len = new QComboBox( TRUE, this, "column len" );
	column_len -> setGeometry( 370, 140, 150, 20 );
	l_column_len = new QLabel( "String Length:", this );
	l_column_len -> setGeometry( 280, 140, 90, 20 );
	column_len -> insertItem( "SQL_NTS=-3", 0 );
	column_len -> insertItem( "0", 1 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dProcedureColumns::~dProcedureColumns()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete column;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete l_column;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete column_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
	delete l_column_len;
}

void dTablePrivileges::Ok()
{
	const char *cat, *sch, *tab;
	const char *lcat, *lsch, *ltab;
	QString qcat, qsch, qtab;
	QString qlcat, qlsch, qltab;
	int cat_len, sch_len, tab_len;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog ->currentText();
	qsch = schema ->currentText();
	qtab = table ->currentText();
	qlcat = catalog_len ->currentText();
	qlsch = schema_len ->currentText();
	qltab = table_len ->currentText();

	cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();
	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLTablePrivileges():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    Catalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    Catalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Catalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    Catalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    Catalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    Schema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    Schema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Schema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    Schema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    Schema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    Table: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    Table: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Table: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    Table Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    Table Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	SQLRETURN ret = SQLTablePrivileges( in_handle, 
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dTablePrivileges::dTablePrivileges( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 50, 150, 20 );
	l_catalog = new QLabel( "Catalog:", this );
	l_catalog -> setGeometry( 10, 50, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 80, 150, 20 );
	l_schema = new QLabel( "Schema:", this );
	l_schema -> setGeometry( 10, 80, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "table" );
	table -> setGeometry( 100, 110, 150, 20 );
	l_table = new QLabel( "Table:", this );
	l_table -> setGeometry( 10, 110, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 50, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 50, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 80, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 80, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "table len" );
	table_len -> setGeometry( 370, 110, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 110, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dTablePrivileges::~dTablePrivileges()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
}

void dGetTypeInfo::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	int index;
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
	SQLINTEGER info_type;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetTypeInfo():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	index = type -> currentItem(); 

	info_type = get_type_options[ index ].attr;
	txt.sprintf( "    Data Type: %s=%d (%s)", get_type_options[ index ].text,
		get_type_options[ index ].attr, get_type_options[ index ].version );
	odbctest -> out_win -> insertLineLimited( txt );

	SQLRETURN ret = SQLGetTypeInfo( in_handle, info_type );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dGetTypeInfo::dGetTypeInfo( OdbcTest *parent, QString name )
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

	type = new QComboBox( FALSE, this, "Info Type" );
	type -> setGeometry( 130, 80, 340, 20 );

	parent->fill_list_box( get_type_options, type );

	l_type = new QLabel( "Data Type:", this );
    l_type -> setGeometry( 10, 80, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dGetTypeInfo::~dGetTypeInfo()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handle;
	delete type;
	delete l_type;
}

void dStatistics::Ok()
{
	const char *cat, *sch, *tab;
	const char *lcat, *lsch, *ltab;
	QString qcat, qsch, qtab;
	QString qlcat, qlsch, qltab;
	int cat_len, sch_len, tab_len;
	int res, res_val, uniq, uniq_val;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog ->currentText();
	qsch = schema ->currentText();
	qtab = table ->currentText();
	qlcat = catalog_len ->currentText();
	qlsch = schema_len ->currentText();
	qltab = table_len -> currentText();

	cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();
	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLStatistics():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    Catalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    Catalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Catalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    Catalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    Catalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    Schema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    Schema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Schema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    Schema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    Schema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    Table: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    Table: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Table: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    Table Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    Table Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	uniq = unique -> currentItem(); 

	uniq_val = unique_options[ uniq ].attr;
	txt.sprintf( "    Unique: %s=%d (%s)", unique_options[ uniq ].text,
		unique_options[ uniq ].attr, unique_options[ uniq ].version );
	odbctest -> out_win -> insertLineLimited( txt );
	
	res = reserved -> currentItem(); 

	res_val = reserved_options[ res ].attr;
	txt.sprintf( "    Reserved: %s=%d (%s)", reserved_options[ res ].text,
		reserved_options[ res ].attr, reserved_options[ res ].version );
	odbctest -> out_win -> insertLineLimited( txt );
	
	SQLRETURN ret = SQLStatistics( in_handle, 
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len, 
			uniq_val,
			res_val );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dStatistics::dStatistics( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 50, 150, 20 );
	l_catalog = new QLabel( "Catalog:", this );
	l_catalog -> setGeometry( 10, 50, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 80, 150, 20 );
	l_schema = new QLabel( "Schema:", this );
	l_schema -> setGeometry( 10, 80, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "table" );
	table -> setGeometry( 100, 110, 150, 20 );
	l_table = new QLabel( "Table:", this );
	l_table -> setGeometry( 10, 110, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 50, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 50, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 80, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 80, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "table len" );
	table_len -> setGeometry( 370, 110, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 110, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

	unique = new QComboBox( FALSE, this, "Info Type" );
	unique -> setGeometry( 100, 140, 200, 20 );
	parent->fill_list_box( unique_options, unique );
	l_unique = new QLabel( "Unique:", this );
	l_unique -> setGeometry( 10, 140, 90, 20 );

	reserved = new QComboBox( FALSE, this, "Info Type" );
	reserved -> setGeometry( 100, 170, 200, 20 );
	parent->fill_list_box( reserved_options, reserved );
	l_reserved = new QLabel( "Reserved:", this );
	l_reserved -> setGeometry( 10, 170, 90, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dStatistics::~dStatistics()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
	delete unique;
	delete reserved;
	delete l_unique;
	delete l_reserved;
}

void dSpecialColumns::Ok()
{
	const char *cat, *sch, *tab;
	const char *lcat, *lsch, *ltab;
	QString qcat, qsch, qtab;
	QString qlcat, qlsch, qltab;
	int cat_len, sch_len, tab_len;
	int nul, nul_val, scp, scp_val;
	int id, id_val;
	Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_STMT, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;

	if ( hand )
		in_handle = hand -> getHandle();

	qcat = catalog->currentText();
	qsch = schema->currentText();
	qtab = table->currentText();

	qlcat = catalog_len->currentText();
	qlsch = schema_len->currentText();
	qltab = table_len->currentText();

	cat = qcat.ascii();
	sch = qsch.ascii();
	tab = qtab.ascii();

	lcat = qlcat.ascii();
	lsch = qlsch.ascii();
	ltab = qltab.ascii();

	odbctest -> out_win -> insertLineLimited( "SQLSpecialColumns():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	if ( in_handle )
		txt.sprintf( "    Statement Handle: %p", in_handle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	id = ident -> currentItem(); 

	id_val = ident_options[ id ].attr;
	txt.sprintf( "    Identifier Type: %s=%d (%s)", ident_options[ id ].text,
		ident_options[ id ].attr, ident_options[ id ].version );
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( cat, "<null ptr>" ) == 0 )
	{
		cat = NULL;
		odbctest -> out_win -> insertLineLimited( "    Catalog: <null ptr>" );
	}
	else if ( strcmp( cat, "<empty string>" ) == 0 )
	{
		cat = "";
		odbctest -> out_win -> insertLineLimited( "    Catalog: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Catalog: %s", cat );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lcat, "SQL_NTS", 7 ) == 0 )
	{
		cat_len = SQL_NTS;
		txt.sprintf( "    Catalog Len: SQL_NTS=-3" );
	}
	else
	{
		cat_len = atoi( lcat );
		txt.sprintf( "    Catalog Len: %d", cat_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( sch, "<null ptr>" ) == 0 )
	{
		sch = NULL;
		odbctest -> out_win -> insertLineLimited( "    Schema: <null ptr>" );
	}
	else if ( strcmp( sch, "<empty string>" ) == 0 )
	{
		sch = "";
		odbctest -> out_win -> insertLineLimited( "    Schema: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Schema: %s", sch );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( lsch, "SQL_NTS", 7 ) == 0 )
	{
		sch_len = SQL_NTS;
		txt.sprintf( "    Schema Len: SQL_NTS=-3" );
	}
	else
	{
		sch_len = atoi( lsch );
		txt.sprintf( "    Schema Len: %d", sch_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );
	
	if ( strcmp( tab, "<null ptr>" ) == 0 )
	{
		tab = NULL;
		odbctest -> out_win -> insertLineLimited( "    Table: <null ptr>" );
	}
	else if ( strcmp( tab, "<empty string>" ) == 0 )
	{
		tab = "";
		odbctest -> out_win -> insertLineLimited( "    Table: <empty string>" );
	}
	else
	{
		txt.sprintf( "    Table: %s", tab );
		odbctest -> out_win -> insertLineLimited( txt );
	}

	if ( strncmp( ltab, "SQL_NTS", 7 ) == 0 )
	{
		tab_len = SQL_NTS;
		txt.sprintf( "    Table Len: SQL_NTS=-3" );
	}
	else
	{
		tab_len = atoi( ltab );
		txt.sprintf( "    Table Len: %d", tab_len );
	}
	odbctest -> out_win -> insertLineLimited( txt );

	scp = scope -> currentItem(); 

	scp_val = unique_options[ scp ].attr;
	txt.sprintf( "    Scope: %s=%d (%s)", scope_options[ scp ].text,
		scope_options[ scp ].attr, scope_options[ scp ].version );
	odbctest -> out_win -> insertLineLimited( txt );
	
	nul = nullable -> currentItem(); 

	nul_val = null_options[ nul ].attr;
	txt.sprintf( "    Nullable: %s=%d (%s)", null_options[ nul ].text,
		null_options[ nul ].attr, null_options[ nul ].version );
	odbctest -> out_win -> insertLineLimited( txt );
	
	SQLRETURN ret = SQLSpecialColumns( in_handle, 
			id_val,
			(SQLCHAR*)cat, cat_len,
			(SQLCHAR*)sch, sch_len, 
			(SQLCHAR*)tab, tab_len, 
			scp_val,
			nul_val );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );
}

dSpecialColumns::dSpecialColumns( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 290,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 370,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 450,10, 70,25 );

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 100, 15, 150, 20 );

	odbctest->fill_handle_list( SQL_HANDLE_STMT, handles );

	l_handle = new QLabel( "Handle:", this );
	l_handle -> setGeometry( 10, 15, 80, 20 );

	ident = new QComboBox( FALSE, this, "Info Type" );
	ident -> setGeometry( 100, 50, 250, 20 );
	parent->fill_list_box( ident_options, ident );
	l_ident = new QLabel( "Identifier Type:", this );
	l_ident -> setGeometry( 10, 50, 90, 20 );

	catalog = new QComboBox( TRUE, this, "catalog" );
	catalog -> setGeometry( 100, 80, 150, 20 );
	l_catalog = new QLabel( "Catalog:", this );
	l_catalog -> setGeometry( 10, 80, 90, 20 );
	catalog -> insertItem( "<null ptr>", 0 );
	catalog -> insertItem( "<empty string>", 1 );

	schema = new QComboBox( TRUE, this, "schema" );
	schema -> setGeometry( 100, 110, 150, 20 );
	l_schema = new QLabel( "Schema:", this );
	l_schema -> setGeometry( 10, 110, 90, 20 );
	schema -> insertItem( "<null ptr>", 0 );
	schema -> insertItem( "<empty string>", 1 );

	table = new QComboBox( TRUE, this, "table" );
	table -> setGeometry( 100, 140, 150, 20 );
	l_table = new QLabel( "Table:", this );
	l_table -> setGeometry( 10, 140, 90, 20 );
	table -> insertItem( "<null ptr>", 0 );
	table -> insertItem( "<empty string>", 1 );

	catalog_len = new QComboBox( TRUE, this, "catalog len" );
	catalog_len -> setGeometry( 370, 80, 150, 20 );
	l_catalog_len = new QLabel( "String Length:", this );
	l_catalog_len -> setGeometry( 280, 80, 90, 20 );
	catalog_len -> insertItem( "SQL_NTS=-3", 0 );
	catalog_len -> insertItem( "0", 1 );

	schema_len = new QComboBox( TRUE, this, "schema len" );
	schema_len -> setGeometry( 370, 110, 150, 20 );
	l_schema_len = new QLabel( "String Length:", this );
	l_schema_len -> setGeometry( 280, 110, 90, 20 );
	schema_len -> insertItem( "SQL_NTS=-3", 0 );
	schema_len -> insertItem( "0", 1 );

	table_len = new QComboBox( TRUE, this, "table len" );
	table_len -> setGeometry( 370, 140, 150, 20 );
	l_table_len = new QLabel( "String Length:", this );
	l_table_len -> setGeometry( 280, 140, 90, 20 );
	table_len -> insertItem( "SQL_NTS=-3", 0 );
	table_len -> insertItem( "0", 1 );

	scope = new QComboBox( FALSE, this, "Info Type" );
	scope -> setGeometry( 100, 170, 250, 20 );
	parent->fill_list_box( scope_options, scope );
	l_scope = new QLabel( "Scope:", this );
	l_scope -> setGeometry( 10, 170, 90, 20 );

	nullable = new QComboBox( FALSE, this, "Info Type" );
	nullable -> setGeometry( 100, 200, 250, 20 );
	parent->fill_list_box( null_options, nullable );
	l_nullable = new QLabel( "Reserved:", this );
	l_nullable -> setGeometry( 10, 200, 90, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dSpecialColumns::~dSpecialColumns()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete catalog;
	delete schema;
	delete table;
	delete l_handle;
	delete l_catalog;
	delete l_schema;
	delete l_table;
	delete catalog_len;
	delete schema_len;
	delete table_len;
	delete l_catalog_len;
	delete l_schema_len;
	delete l_table_len;
	delete scope;
	delete nullable;
	delete l_scope;
	delete l_nullable;
	delete ident;
	delete l_ident;
}

void OdbcTest::sqlcolumns()
{
	dColumns *dlg = new dColumns( this, "SQLColumns" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlcolumnprivileges()
{
	dColumnPrivileges *dlg = new dColumnPrivileges( this, "SQLColumnPrivileges" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgettypeinfo()
{
	dGetTypeInfo *dlg = new dGetTypeInfo( this, "SQLGetTypeInfo" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlforeignkeys()
{
	dForeignKeys *dlg = new dForeignKeys( this, "SQLForeignKeys" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlprimarykeys()
{
	dPrimaryKeys *dlg = new dPrimaryKeys( this, "SQLPrimaryKeys" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlprocedures()
{
	dProcedures *dlg = new dProcedures( this, "SQLProcedures" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlprocedurecolumns()
{
	dProcedureColumns *dlg = new dProcedureColumns( this, "SQLProcedureColumns" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlspecialcolumns()
{
	dSpecialColumns *dlg = new dSpecialColumns( this, "SQLSpecialColumns" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlstatistics()
{
	dStatistics *dlg = new dStatistics( this, "SQLStatistics" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqltables()
{
	dTables *dlg = new dTables( this, "SQLTables" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqltableprivileges()
{
	dTablePrivileges *dlg = new dTablePrivileges( this, "SQLTablePrivileges" );

	dlg -> exec();

	delete dlg;
}
