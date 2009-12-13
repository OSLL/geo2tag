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
 * $Id: diag.cpp,v 1.1.1.1 2001/10/17 16:40:30 lurcher Exp $
 *
 * $Log: diag.cpp,v $
 * Revision 1.1.1.1  2001/10/17 16:40:30  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.3  2001/07/20 09:42:58  nick
 *
 * Replace char[] with QString to avoid buffer overrun
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
 * Revision 1.4  2000/06/13 12:30:25  ngorham
 *
 * Enough there for the first release I think
 *
 * Revision 1.3  2000/06/05 16:53:19  ngorham
 *
 * Next lot of updates
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
#include <qlineedit.h>
#include "diag.h"
#include "odbctest.h"
#include <sql.h>
#include <sqlext.h>

static attr_value handle_type_struct[] = 
{
	{ "SQL_HANDLE_ENV", SQL_HANDLE_ENV },
	{ "SQL_HANDLE_DBC", SQL_HANDLE_DBC },
	{ "SQL_HANDLE_STMT", SQL_HANDLE_STMT },
	{ "SQL_HANDLE_DESC", SQL_HANDLE_DESC },
	{ NULL }
};

static attr_options field_type_struct[] = 
{
	{ "SQL_DIAG_CURSOR_ROW_COUNT", SQL_DIAG_CURSOR_ROW_COUNT, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DIAG_DYNAMIC_FUNCTION", SQL_DIAG_DYNAMIC_FUNCTION, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DIAG_DYNAMIC_FUNCTION_CODE", SQL_DIAG_DYNAMIC_FUNCTION_CODE, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DIAG_NUMBER", SQL_DIAG_NUMBER, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DIAG_RETURNCODE", SQL_DIAG_RETURNCODE, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DIAG_ROW_COUNT", SQL_DIAG_ROW_COUNT, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DIAG_CLASS_ORIGIN", SQL_DIAG_CLASS_ORIGIN, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DIAG_COLUMN_NUMBER", SQL_DIAG_COLUMN_NUMBER, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DIAG_CONNECTION_NAME", SQL_DIAG_CONNECTION_NAME, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DIAG_MESSAGE_TEXT", SQL_DIAG_MESSAGE_TEXT, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DIAG_NATIVE", SQL_DIAG_NATIVE, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DIAG_ROW_NUMBER", SQL_DIAG_ROW_NUMBER, 
		{
			{ NULL }
		}, "3.0", SQL_INTEGER
	},
	{ "SQL_DIAG_SERVER_NAME", SQL_DIAG_SERVER_NAME, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DIAG_SQLSTATE", SQL_DIAG_SQLSTATE, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DIAG_SUBCLASS_ORIGIN", SQL_DIAG_SUBCLASS_ORIGIN, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ NULL 
	}
};

void dError::sqlstate_clkd()
{
	if ( sqlstate_valid -> isOn() )
	    sqlstate_valid -> setText( "szSqlState: SQL_NULL_POINTER" );
	else
	    sqlstate_valid -> setText( "szSqlState: VALID" );
}

void dError::native_clkd()
{
	if ( native_valid -> isOn() )
	    native_valid -> setText( "pfNativeError: SQL_NULL_POINTER" );
	else
	    native_valid -> setText( "pfNativeError: VALID" );
}

void dError::error_clkd()
{
	if ( error_valid -> isOn() )
	    error_valid -> setText( "szErrorMsg: SQL_NULL_POINTER" );
	else
	    error_valid -> setText( "szErrorMsg: VALID" );
}

void dError::perror_clkd()
{
	if ( perror_valid -> isOn() )
	    perror_valid -> setText( "pcbErrorMsg: SQL_NULL_POINTER" );
	else
	    perror_valid -> setText( "pcbErrorMsg: VALID" );
}

void dError::Ok()
{
    Handle *stmt = odbctest->extract_handle_list( SQL_HANDLE_STMT, shandles );
    Handle *dbc = odbctest->extract_handle_list( SQL_HANDLE_DBC, chandles );
    Handle *env = odbctest->extract_handle_list( SQL_HANDLE_ENV, ehandles );
	SQLHANDLE in_shandle = SQL_NULL_HANDLE;
	SQLHANDLE in_dhandle = SQL_NULL_HANDLE;
	SQLHANDLE in_ehandle = SQL_NULL_HANDLE;
    SQLCHAR sqlstate[ 7 ], *psqlstate;
    SQLSMALLINT b_len, text_len, *ptext_len;
    SQLINTEGER native, *pnative;
    SQLCHAR *buf;

	if ( env )
		in_ehandle = env -> getHandle();
	if ( dbc )
		in_dhandle = dbc -> getHandle();
	if ( stmt )
		in_shandle = stmt -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLError():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_ehandle )
		txt.sprintf( "    Environment Handle: %p", in_ehandle );
	else
		txt.sprintf( "    Environment Handle: SQL_NULL_HENV" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( in_dhandle )
		txt.sprintf( "    Connection Handle: %p", in_dhandle );
	else
		txt.sprintf( "    Connection Handle: SQL_NULL_HDBC" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( in_shandle )
		txt.sprintf( "    Statement Handle: %p", in_shandle );
	else
		txt.sprintf( "    Statement Handle: SQL_NULL_HSTMT" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( sqlstate_valid -> isOn() )
    {
		txt.sprintf( "    szSqlState: <null pointer>" );
        psqlstate = NULL;
    }
	else
    {
        psqlstate = sqlstate;
        strcpy(( char * ) sqlstate, "XYXYXZ" );
		txt.sprintf( "    szSqlState: %p", psqlstate );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( native_valid -> isOn() )
    {
		txt.sprintf( "    pfNativeError: <null pointer>" );
        pnative = NULL;
    }
	else
    {
        pnative = &native;
        native = -999999;
		txt.sprintf( "    pfNativeError: %p", pnative );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    b_len = atoi( buffer_len -> text().ascii());
	if ( error_valid -> isOn() )
    {
        buf = NULL;
		txt.sprintf( "    szErrorMsg: <null pointer>" );
    }
	else
    {
        if ( b_len < 0 )
            b_len = 300;

        buf = new SQLCHAR[ b_len ];

		txt.sprintf( "    szErrorMsg: %p", buf );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    txt.sprintf( "    cbErrorMsgMax: %d", b_len );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( perror_valid -> isOn() )
    {
		txt.sprintf( "    pcbErrorMsg: <null pointer>" );
        ptext_len = NULL;
    }
	else
    {
        ptext_len = &text_len;
        text_len = -9999;
		txt.sprintf( "    pcbErrorMsg: %p", ptext_len );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLError( in_ehandle, in_dhandle, in_shandle,
            psqlstate, pnative, buf, b_len, ptext_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
        odbctest -> out_win -> insertLineLimited( "  Out:" );
        if ( psqlstate )
        {	
            if ( strcmp(( char * ) sqlstate, "XYXYXZ" ) == 0 )
            {
                txt.sprintf( "    *szSqlState: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *szSqlState: %6s", sqlstate );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( pnative )
        {	
            if ( native == -999999 )
            {
                txt.sprintf( "    *pfNativeError: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *pfNativeError: %d", native );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( buf )
        {	
            txt.sprintf( "    *szErrorMsg: %s", buf );
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( ptext_len )
        {	
            if ( text_len == -9999 )
            {
                txt.sprintf( "    *pcbErrorMsg: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *pcbErrorMsg: %d", text_len );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
    }

    odbctest -> out_win -> insertLineLimited( "" );

    if ( buf )
        delete buf;
}

dError::dError( OdbcTest *parent, QString name )
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

	ehandles = new QComboBox( FALSE, this, "Environment Handle" );
	ehandles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_ENV, ehandles );

	l_ehandle = new QLabel( "Environment Handle:", this );
	l_ehandle -> setGeometry( 10, 50, 120, 20 );

	chandles = new QComboBox( FALSE, this, "Connection Handle" );
	chandles -> setGeometry( 130, 80, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_DBC, chandles );

	l_chandle = new QLabel( "Connection Handle:", this );
	l_chandle -> setGeometry( 10, 80, 120, 20 );

	shandles = new QComboBox( FALSE, this, "Statement Handle" );
	shandles -> setGeometry( 130, 110, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_STMT, shandles );

	l_shandle = new QLabel( "Statement Handle:", this );
	l_shandle -> setGeometry( 10, 110, 120, 20 );

	sqlstate_valid = new QCheckBox( "szSqlState: VALID", this );
	sqlstate_valid -> setGeometry( 10, 140, 300, 15 );

	native_valid = new QCheckBox( "pfNativeError: VALID", this );
	native_valid -> setGeometry( 10, 170, 300, 15 );

	error_valid = new QCheckBox( "szErrorMsg: VALID", this );
	error_valid -> setGeometry( 10, 200, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 350, 200, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "cbErrorMsgMax:", this );
    l_buffer_len -> setGeometry( 240, 200, 100, 20 );

	perror_valid = new QCheckBox( "pcbErrorMsg: VALID", this );
	perror_valid -> setGeometry( 10, 230, 300, 15 );

	connect( native_valid, SIGNAL( clicked()), this, SLOT( native_clkd()));
	connect( sqlstate_valid, SIGNAL( clicked()), this, SLOT( sqlstate_clkd()));
	connect( error_valid, SIGNAL( clicked()), this, SLOT( error_clkd()));
	connect( perror_valid, SIGNAL( clicked()), this, SLOT( perror_clkd()));

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dError::~dError()
{
	delete ok;
	delete cancel;
	delete help;
	delete ehandles;
	delete chandles;
	delete shandles;
	delete l_ehandle;
	delete l_chandle;
	delete l_shandle;
    delete sqlstate_valid;
    delete native_valid;
    delete error_valid;
    delete perror_valid;
    delete buffer_len;
    delete l_buffer_len;
}

void dGetDiagRec::sel_handle( int index )
{
    int handle_t;

	handles -> clear();
	handle_t = handle_type_struct[ handle_type -> currentItem() ].value;

	odbctest->fill_handle_list( handle_t, handles );
}

void dGetDiagRec::sqlstate_clkd()
{
	if ( sqlstate_valid -> isOn() )
	    sqlstate_valid -> setText( "szSqlState: SQL_NULL_POINTER" );
	else
	    sqlstate_valid -> setText( "szSqlState: VALID" );
}

void dGetDiagRec::native_clkd()
{
	if ( native_valid -> isOn() )
	    native_valid -> setText( "pfNativeError: SQL_NULL_POINTER" );
	else
	    native_valid -> setText( "pfNativeError: VALID" );
}

void dGetDiagRec::error_clkd()
{
	if ( error_valid -> isOn() )
	    error_valid -> setText( "szErrorMsg: SQL_NULL_POINTER" );
	else
	    error_valid -> setText( "szErrorMsg: VALID" );
}

void dGetDiagRec::perror_clkd()
{
	if ( perror_valid -> isOn() )
	    perror_valid -> setText( "pcbErrorMsg: SQL_NULL_POINTER" );
	else
	    perror_valid -> setText( "pcbErrorMsg: VALID" );
}

void dGetDiagRec::Ok()
{
    int htype = handle_type_struct[ handle_type -> currentItem() ].value;
    Handle *hand = odbctest->extract_handle_list( htype, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
    SQLCHAR sqlstate[ 7 ], *psqlstate;
    SQLSMALLINT b_len, text_len, *ptext_len;
    SQLINTEGER native, *pnative, rec_number;
    SQLCHAR *buf;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetDiagRec():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

    txt.sprintf( "    Handle Type: %s", handle_type_struct[ handle_type -> currentItem() ].text );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( in_handle )
		txt.sprintf( "    Handle: %p", in_handle );
	else
		txt.sprintf( "    Handle: SQL_NULL_HANDLE" );
	odbctest -> out_win -> insertLineLimited( txt );

    rec_number = atoi( rec_num -> text().ascii());
    txt.sprintf( "    RecNumber: %d", rec_number );

	if ( sqlstate_valid -> isOn() )
    {
		txt.sprintf( "    szSqlState: <null pointer>" );
        psqlstate = NULL;
    }
	else
    {
        psqlstate = sqlstate;
        strcpy(( char * ) sqlstate, "XYXYXZ" );
		txt.sprintf( "    szSqlState: %p", psqlstate );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( native_valid -> isOn() )
    {
		txt.sprintf( "    pfNativeError: <null pointer>" );
        pnative = NULL;
    }
	else
    {
        pnative = &native;
        native = -999999;
		txt.sprintf( "    pfNativeError: %p", pnative );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    b_len = atoi( buffer_len -> text().ascii());
	if ( error_valid -> isOn() )
    {
        buf = NULL;
		txt.sprintf( "    szErrorMsg: <null pointer>" );
    }
	else
    {
        if ( b_len < 0 )
            b_len = 300;

        buf = new SQLCHAR[ b_len ];

		txt.sprintf( "    szErrorMsg: %p", buf );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    txt.sprintf( "    cbErrorMsgMax: %d", b_len );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( perror_valid -> isOn() )
    {
		txt.sprintf( "    pcbErrorMsg: <null pointer>" );
        ptext_len = NULL;
    }
	else
    {
        ptext_len = &text_len;
        text_len = -9999;
		txt.sprintf( "    pcbErrorMsg: %p", ptext_len );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLGetDiagRec( htype, in_handle, rec_number,
            psqlstate, pnative, buf, b_len, ptext_len );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
        odbctest -> out_win -> insertLineLimited( "  Out:" );
        if ( psqlstate )
        {	
            if ( strcmp(( char * ) sqlstate, "XYXYXZ" ) == 0 )
            {
                txt.sprintf( "    *szSqlState: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *szSqlState: %6s", sqlstate );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( pnative )
        {	
            if ( native == -999999 )
            {
                txt.sprintf( "    *pfNativeError: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *pfNativeError: %d", native );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( buf )
        {	
            txt.sprintf( "    *szErrorMsg: %s", buf );
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( ptext_len )
        {	
            if ( text_len == -9999 )
            {
                txt.sprintf( "    *pcbErrorMsg: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *pcbErrorMsg: %d", text_len );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
    }

    odbctest -> out_win -> insertLineLimited( "" );

    if ( buf )
        delete buf;
}

dGetDiagRec::dGetDiagRec( OdbcTest *parent, QString name )
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

	handle_type = new QComboBox( FALSE, this, "Handle" );
	handle_type -> setGeometry( 130, 50, 200, 20 );
	parent->fill_list_box( handle_type_struct, handle_type );

	l_handle_type = new QLabel( "Handle Type:", this );
	l_handle_type -> setGeometry( 10, 50, 120, 20 );

	handles = new QComboBox( FALSE, this, "Handles" );
	handles -> setGeometry( 130, 80, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_ENV, handles );

	l_handles = new QLabel( "Handle:", this );
	l_handles -> setGeometry( 10, 80, 120, 20 );

	rec_num = new QLineEdit( this, "Rec Num" );
    rec_num -> setGeometry( 130, 110, 70, 20 );
	rec_num -> setMaxLength( 6 );
	rec_num -> setText( "1" );

	l_rec_num = new QLabel( "Rec Number:", this );
    l_rec_num -> setGeometry( 10, 110, 100, 20 );

	sqlstate_valid = new QCheckBox( "szSqlState: VALID", this );
	sqlstate_valid -> setGeometry( 10, 140, 300, 15 );

	native_valid = new QCheckBox( "pfNativeError: VALID", this );
	native_valid -> setGeometry( 10, 170, 300, 15 );

	error_valid = new QCheckBox( "szErrorMsg: VALID", this );
	error_valid -> setGeometry( 10, 200, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 350, 200, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "cbErrorMsgMax:", this );
    l_buffer_len -> setGeometry( 240, 200, 100, 20 );

	perror_valid = new QCheckBox( "pcbErrorMsg: VALID", this );
	perror_valid -> setGeometry( 10, 230, 300, 15 );

	connect( native_valid, SIGNAL( clicked()), this, SLOT( native_clkd()));
	connect( sqlstate_valid, SIGNAL( clicked()), this, SLOT( sqlstate_clkd()));
	connect( error_valid, SIGNAL( clicked()), this, SLOT( error_clkd()));
	connect( perror_valid, SIGNAL( clicked()), this, SLOT( perror_clkd()));

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );

	connect( handle_type, SIGNAL(activated(int)), this, SLOT( sel_handle(int)));
}

dGetDiagRec::~dGetDiagRec()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
	delete handle_type;
	delete l_handle_type;
    delete sqlstate_valid;
    delete native_valid;
    delete error_valid;
    delete perror_valid;
    delete buffer_len;
    delete l_buffer_len;
    delete rec_num;
    delete l_rec_num;
}

void dGetDiagField::sel_handle( int index )
{
    int handle_t;

	handles -> clear();
	handle_t = handle_type_struct[ handle_type -> currentItem() ].value;

	odbctest->fill_handle_list( handle_t, handles );
}

void dGetDiagField::diag_ptr_clkd()
{
	if ( diag_ptr_valid -> isOn() )
	    diag_ptr_valid -> setText( "DiagInfoPtr: SQL_NULL_POINTER" );
	else
	    diag_ptr_valid -> setText( "DiagInfoPtr: VALID" );
}

void dGetDiagField::strlen_clkd()
{
	if ( strlen_valid -> isOn() )
	    strlen_valid -> setText( "StringLengthPtr: SQL_NULL_POINTER" );
	else
	    strlen_valid -> setText( "StringLengthPtr: VALID" );
}

void dGetDiagField::Ok()
{
    int htype = handle_type_struct[ handle_type -> currentItem() ].value;
    Handle *hand = odbctest->extract_handle_list( htype, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
    SQLSMALLINT b_len, *strlen_ptr, strlen_val, attribute;
    SQLINTEGER rec_number;
    SQLCHAR *buf;

	if ( hand  )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetDiagField():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

    txt.sprintf( "    Handle Type: %s", handle_type_struct[ handle_type -> currentItem() ].text );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( in_handle )
		txt.sprintf( "    Handle: %p", in_handle );
	else
		txt.sprintf( "    Handle: SQL_NULL_HANDLE" );
	odbctest -> out_win -> insertLineLimited( txt );

    rec_number = atoi( rec_num -> text().ascii());
    txt.sprintf( "    RecNumber: %d", rec_number );

	attribute = field_type_struct[ diag_info -> currentItem() ].attr;
	txt.sprintf( "    DiagIdentifier: %s=%d", 
		field_type_struct[ diag_info -> currentItem() ].text,
		field_type_struct[ diag_info -> currentItem() ].attr );

    b_len = atoi( buffer_len -> text().ascii());
	if ( diag_ptr_valid -> isOn() )
    {
        buf = NULL;
		txt.sprintf( "    DiagInfo: <null pointer>" );
    }
	else
    {
        if ( b_len < 0 )
            b_len = 300;

        buf = new SQLCHAR[ b_len ];

		txt.sprintf( "    DiagInfo: %p", buf );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( strlen_valid -> isOn() )
    {
		txt.sprintf( "    StringLengthPtr: <null pointer>" );
        strlen_ptr = NULL;
    }
	else
    {
        strlen_ptr = &strlen_val;
        strlen_val = -9999;
		txt.sprintf( "    StringLengthPtr: %p", strlen_ptr );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLGetDiagField( htype, in_handle, rec_number,
            attribute, buf, b_len, strlen_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
        odbctest -> out_win -> insertLineLimited( "  Out:" );
		if ( field_type_struct[ diag_info -> currentItem() ].data_type == SQL_INTEGER )
        {
            if ( buf )
            {	
                int lval;

                memcpy( &lval, buf, sizeof( lval ));
                txt.sprintf( "    *DiagInfo: %d", lval );
                odbctest -> out_win -> insertLineLimited( txt );
            }
        }
        else
        {
            if ( buf )
            {	
                txt.sprintf( "    *DiagInfo: %s", buf );
                odbctest -> out_win -> insertLineLimited( txt );
            }
        }

        if ( strlen_ptr )
        {	
            if ( strlen_val == -9999 )
            {
                txt.sprintf( "    *StringLengthPtr: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *StringLengthPtr: %d", strlen_val );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
    }

    odbctest -> out_win -> insertLineLimited( "" );

    if ( buf )
        delete buf;
}

dGetDiagField::dGetDiagField( OdbcTest *parent, QString name )
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

	handle_type = new QComboBox( FALSE, this, "Handle" );
	handle_type -> setGeometry( 130, 50, 200, 20 );
	parent->fill_list_box( handle_type_struct, handle_type );

	l_handle_type = new QLabel( "Handle Type:", this );
	l_handle_type -> setGeometry( 10, 50, 120, 20 );

	handles = new QComboBox( FALSE, this, "Handles" );
	handles -> setGeometry( 130, 80, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_ENV, handles );

	l_handles = new QLabel( "Handle:", this );
	l_handles -> setGeometry( 10, 80, 120, 20 );

	rec_num = new QLineEdit( this, "Rec Num" );
    rec_num -> setGeometry( 130, 110, 70, 20 );
	rec_num -> setMaxLength( 6 );
	rec_num -> setText( "1" );

	l_rec_num = new QLabel( "Rec Number:", this );
    l_rec_num -> setGeometry( 10, 110, 100, 20 );

	diag_info = new QComboBox( FALSE, this, "Info" );
	diag_info -> setGeometry( 130, 140, 350, 20 );
	parent->fill_list_box( field_type_struct, diag_info );

	l_diag_info = new QLabel( "Diag Identifier::", this );
	l_diag_info -> setGeometry( 10, 140, 120, 20 );

	diag_ptr_valid = new QCheckBox( "DiagInfoPtr: VALID", this );
	diag_ptr_valid -> setGeometry( 10, 170, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 130, 200, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "Buffer Length:", this );
    l_buffer_len -> setGeometry( 10, 200, 100, 20 );

	strlen_valid = new QCheckBox( "pcbErrorMsg: VALID", this );
	strlen_valid -> setGeometry( 10, 230, 300, 15 );

	connect( diag_ptr_valid, SIGNAL( clicked()), this, SLOT( diag_ptr_clkd()));
	connect( strlen_valid, SIGNAL( clicked()), this, SLOT( strlen_clkd()));

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );

	connect( handle_type, SIGNAL(activated(int)), this, SLOT( sel_handle(int)));
}

dGetDiagField::~dGetDiagField()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
	delete handle_type;
	delete l_handle_type;
    delete diag_ptr_valid;
    delete strlen_valid;
    delete buffer_len;
    delete l_buffer_len;
    delete rec_num;
    delete l_rec_num;
}

void OdbcTest::sqlgetdiagrec()
{
	dGetDiagRec *dlg = new dGetDiagRec( this, "SQLGetDiagRec" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetdiagfield()
{
	dGetDiagField *dlg = new dGetDiagField( this, "SQLGetDiagField" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlerror()
{
	dError *dlg = new dError( this, "SQLError" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::errorall()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}
