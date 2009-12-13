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
 * $Id: desc.cpp,v 1.2 2001/12/13 13:00:34 lurcher Exp $
 *
 * $Log: desc.cpp,v $
 * Revision 1.2  2001/12/13 13:00:34  lurcher
 *
 * Remove most if not all warnings on 64 bit platforms
 * Add support for new MS 3.52 64 bit changes
 * Add override to disable the stopping of tracing
 * Add MAX_ROWS support in postgres driver
 *
 * Revision 1.1.1.1  2001/10/17 16:40:30  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.4  2001/07/20 09:42:58  nick
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
 * Revision 1.2  2001/05/04 15:32:54  nick
 *
 * Some makefile tweeks
 *
 * Revision 1.1.1.1  2000/09/04 16:42:53  nick
 * Imported Sources
 *
 * Revision 1.5  2000/06/13 12:30:25  ngorham
 *
 * Enough there for the first release I think
 *
 * Revision 1.4  2000/06/07 08:29:47  ngorham
 *
 * More additions
 *
 * Revision 1.3  2000/06/05 16:53:19  ngorham
 *
 * Next lot of updates
 *
 * Revision 1.2  2001/05/31 10:26:27  ngorham
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
#include <qlineedit.h>
#include <sql.h>
#include <sqlext.h>
#include "odbctest.h"
#include "desc.h"

static attr_options field_ident_struct[] = 
{
	{ "SQL_DESC_ALLOC_TYPE", SQL_DESC_ALLOC_TYPE, 
		{
			{ NULL }
		}, "3.0", SQL_IS_USMALLINT
	},
	{ "SQL_DESC_ARRAY_SIZE", SQL_DESC_ARRAY_SIZE, 
		{
			{ NULL }
		}, "3.0", SQL_IS_UINTEGER
	},
	{ "SQL_DESC_ARRAY_STATUS_PTR", SQL_DESC_ARRAY_STATUS_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_IS_POINTER
	},
	{ "SQL_DESC_BIND_OFFSET_PTR", SQL_DESC_BIND_OFFSET_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_IS_POINTER
	},
	{ "SQL_DESC_BIND_TYPE", SQL_DESC_BIND_TYPE, 
		{
			{ NULL }
		}, "3.0", SQL_IS_UINTEGER
	},
	{ "SQL_DESC_COUNT", SQL_DESC_COUNT, 
		{
			{ NULL }
		}, "3.0", SQL_IS_SMALLINT
	},
	{ "SQL_DESC_ROWS_PROCESSED_PTR", SQL_DESC_ROWS_PROCESSED_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_IS_POINTER
	},
	{ "SQL_DESC_AUTO_UNIQUE_VALUE", SQL_DESC_AUTO_UNIQUE_VALUE, 
		{
			{ NULL }
		}, "3.0", SQL_IS_INTEGER
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
		}, "3.0", SQL_IS_INTEGER
	},
	{ "SQL_DESC_CATALOG_NAME", SQL_DESC_CATALOG_NAME, 
		{
			{ NULL }
		}, "3.0", SQL_CHAR
	},
	{ "SQL_DESC_CONCISE_TYPE", SQL_DESC_CONCISE_TYPE, 
		{
			{ NULL }
		}, "3.0", SQL_IS_SMALLINT
	},
	{ "SQL_DESC_DATA_PTR", SQL_DESC_DATA_PTR, 
		{
			{ NULL }
		}, "3.0", SQL_IS_POINTER
	},
	{ "SQL_DESC_DATETIME_INTERVAL_CODE", SQL_DESC_DATETIME_INTERVAL_CODE, 
		{
			{ NULL }
		}, "3.0", SQL_IS_SMALLINT
	},
	{ NULL 
	}
};

void dCopyDesc::Ok()
{
    Handle *desc1 = odbctest->extract_handle_list( SQL_HANDLE_DESC, handle1 );
	SQLHANDLE in_handle1 = SQL_NULL_HANDLE;
    Handle *desc2 = odbctest->extract_handle_list( SQL_HANDLE_DESC, handle2 );
	SQLHANDLE in_handle2 = SQL_NULL_HANDLE;

	if ( desc1 )
		in_handle1 = desc1 -> getHandle();
	if ( desc2 )
		in_handle2 = desc2 -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLCopyDesc():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle1 )
		txt.sprintf( "    Source Handle: %p", in_handle1 );
	else
		txt.sprintf( "    Source Handle: SQL_NULL_DESC" );

	odbctest -> out_win -> insertLineLimited( txt );

	if ( in_handle2 )
		txt.sprintf( "    Destination Handle: %p", in_handle2 );
	else
		txt.sprintf( "    Destination Handle: SQL_NULL_DESC" );

	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLCopyDesc( in_handle1, in_handle2 );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dCopyDesc::dCopyDesc( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 130,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 210,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 290,10, 70,25 );

	handle1 = new QComboBox( FALSE, this, "Source Handle 1" );
	handle1 -> setGeometry( 160, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_DESC, handle1 );

	l_handle1 = new QLabel( "Source Handle:", this );
	l_handle1 -> setGeometry( 10, 50, 120, 20 );

	handle2 = new QComboBox( FALSE, this, "Destination Handle 2" );
	handle2 -> setGeometry( 160, 80, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_DESC, handle2 );

	l_handle2 = new QLabel( "Destination Handle:", this );
	l_handle2 -> setGeometry( 10, 80, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dCopyDesc::~dCopyDesc()
{
	delete ok;
	delete cancel;
	delete help;
	delete handle1;
	delete l_handle1;
	delete handle2;
	delete l_handle2;
}

void dGetDescField::ptr_clkd()
{
	if ( ptr_valid -> isOn() )
	    ptr_valid -> setText( "ValuePtr: SQL_NULL_POINTER" );
	else
	    ptr_valid -> setText( "ValuePtr: VALID" );
}

void dGetDescField::strlen_clkd()
{
	if ( strlen_valid -> isOn() )
	    strlen_valid -> setText( "StringLengthPtr: SQL_NULL_POINTER" );
	else
	    strlen_valid -> setText( "StringLengthPtr: VALID" );
}

void dGetDescField::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_DESC, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
    SQLSMALLINT b_len, attribute;
    SQLINTEGER *strlen_ptr, strlen_val;
    SQLINTEGER rec_number;
    SQLPOINTER buf;
    SQLPOINTER pval;
    SQLINTEGER ival;
    SQLSMALLINT sval;
    SQLUINTEGER uival;
    SQLUSMALLINT usval;
    int buf_allocated = 0;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetDescField():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Handle: %p", in_handle );
	else
		txt.sprintf( "    Handle: SQL_NULL_HANDLE" );
	odbctest -> out_win -> insertLineLimited( txt );

    rec_number = atoi( rec_num -> text().ascii());
    txt.sprintf( "    RecNumber: %d", rec_number );
	odbctest -> out_win -> insertLineLimited( txt );

	attribute = field_ident_struct[ diag_info -> currentItem() ].attr;
	txt.sprintf( "    FieldIdentifier: %s=%d", 
		field_ident_struct[ diag_info -> currentItem() ].text,
		field_ident_struct[ diag_info -> currentItem() ].attr );
	odbctest -> out_win -> insertLineLimited( txt );

    b_len = atoi( buffer_len -> text().ascii());
	if ( ptr_valid -> isOn() )
    {
        buf = NULL;
		txt.sprintf( "    ValuePtr: <null pointer>" );
    }
	else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_POINTER )
    {
        buf = &pval;
    }
	else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_INTEGER )
    {
        buf = &ival;
    }
	else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_UINTEGER )
    {
        buf = &uival;
    }
	else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_SMALLINT )
    {
        buf = &sval;
    }
	else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_USMALLINT )
    {
        buf = &usval;
    }
    else
    {
        if ( b_len < 0 )
            b_len = 300;

        buf = malloc( b_len );

        buf_allocated = 1;

    }

    if ( buf )
    {
        txt.sprintf( "    ValuePtr: %p", buf );
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

    SQLRETURN ret = SQLGetDescField( in_handle, rec_number,
            attribute, buf, b_len, strlen_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
        odbctest -> out_win -> insertLineLimited( "  Out:" );
        if ( buf )
        {
            if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_CHAR )
            {
                txt.sprintf( "    ValuePtr: %s", buf );
            }
            else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_POINTER )
            {
                txt.sprintf( "    ValuePtr: %p", pval );
            }
            else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_INTEGER )
            {
                txt.sprintf( "    ValuePtr: %d", ival );
            }
            else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_UINTEGER )
            {
                txt.sprintf( "    ValuePtr: %d", ival );
            }
            else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_SMALLINT )
            {
                txt.sprintf( "    ValuePtr: %d", sval );
            }
            else if ( field_ident_struct[ diag_info -> currentItem() ].data_type == SQL_IS_USMALLINT )
            {
                txt.sprintf( "    ValuePtr: %d", sval );
            }
            odbctest -> out_win -> insertLineLimited( txt );
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

    if ( buf && buf_allocated )
        free( buf );
} 

dGetDescField::dGetDescField( OdbcTest *parent, QString name )
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

	handles = new QComboBox( FALSE, this, "Handles" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_DESC, handles );

	l_handles = new QLabel( "Handle:", this );
	l_handles -> setGeometry( 10, 50, 120, 20 );

	rec_num = new QLineEdit( this, "Rec Num" );
    rec_num -> setGeometry( 130, 80, 70, 20 );
	rec_num -> setMaxLength( 6 );
	rec_num -> setText( "0" );

	l_rec_num = new QLabel( "Rec Number:", this );
    l_rec_num -> setGeometry( 10, 80, 100, 20 );

	diag_info = new QComboBox( FALSE, this, "Info" );
	diag_info -> setGeometry( 130, 110, 350, 20 );
	parent->fill_list_box( field_ident_struct, diag_info );

	l_diag_info = new QLabel( "Field Identifier:", this );
	l_diag_info -> setGeometry( 10, 110, 120, 20 );

	ptr_valid = new QCheckBox( "ValuePtr: VALID", this );
	ptr_valid -> setGeometry( 10, 140, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 130, 170, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "Buffer Length:", this );
    l_buffer_len -> setGeometry( 10, 170, 100, 20 );

	strlen_valid = new QCheckBox( "StringLengthPtr: VALID", this );
	strlen_valid -> setGeometry( 10, 200, 300, 15 );

	connect( ptr_valid, SIGNAL( clicked()), this, SLOT( ptr_clkd()));
	connect( strlen_valid, SIGNAL( clicked()), this, SLOT( strlen_clkd()));

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dGetDescField::~dGetDescField()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
    delete ptr_valid;
    delete strlen_valid;
    delete buffer_len;
    delete l_buffer_len;
    delete rec_num;
    delete l_rec_num;
    delete l_diag_info;
    delete diag_info;
}

void dGetDescRec::name_clkd()
{
	if ( name_valid -> isOn() )
	    name_valid -> setText( "Name: SQL_NULL_POINTER" );
	else
	    name_valid -> setText( "Name: VALID" );
}

void dGetDescRec::strlen_clkd()
{
	if ( strlen_valid -> isOn() )
	    strlen_valid -> setText( "StringLengthPtr: SQL_NULL_POINTER" );
	else
	    strlen_valid -> setText( "StringLengthPtr: VALID" );
}

void dGetDescRec::type_clkd()
{
	if ( type_valid -> isOn() )
	    type_valid -> setText( "TypePtr: SQL_NULL_POINTER" );
	else
	    type_valid -> setText( "TypePtr: VALID" );
}

void dGetDescRec::sub_type_clkd()
{
	if ( sub_type_valid -> isOn() )
	    sub_type_valid -> setText( "SubTypePtr: SQL_NULL_POINTER" );
	else
	    sub_type_valid -> setText( "SubTypePtr: VALID" );
}

void dGetDescRec::length_clkd()
{
	if ( length_valid -> isOn() )
	    length_valid -> setText( "LengthPtr: SQL_NULL_POINTER" );
	else
	    length_valid -> setText( "LengthPtr: VALID" );
}

void dGetDescRec::precision_clkd()
{
	if ( precision_valid -> isOn() )
	    precision_valid -> setText( "PrecisionPtr: SQL_NULL_POINTER" );
	else
	    precision_valid -> setText( "PrecisionPtr: VALID" );
}

void dGetDescRec::scale_clkd()
{
	if ( scale_valid -> isOn() )
	    scale_valid -> setText( "ScalePtr: SQL_NULL_POINTER" );
	else
	    scale_valid -> setText( "ScalePtr: VALID" );
}

void dGetDescRec::nullable_clkd()
{
	if ( nullable_valid -> isOn() )
	    nullable_valid -> setText( "NullablePtr: SQL_NULL_POINTER" );
	else
	    nullable_valid -> setText( "NullablePtr: VALID" );
}

void dGetDescRec::Ok()
{
    Handle *hand = odbctest->extract_handle_list( SQL_HANDLE_DESC, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
    SQLSMALLINT b_len;
    SQLSMALLINT *strlen_ptr, strlen_val;
    SQLSMALLINT rec_number;
    SQLSMALLINT *type_ptr, type_val;
    SQLSMALLINT *sub_type_ptr, sub_type_val;
    SQLLEN *length_ptr, length_val;
    SQLSMALLINT *precision_ptr, precision_val;
    SQLSMALLINT *scale_ptr, scale_val;
    SQLSMALLINT *nullable_ptr, nullable_val;
    SQLCHAR *buf;

	if ( hand )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLGetDescRec():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Handle: %p", in_handle );
	else
		txt.sprintf( "    Handle: SQL_NULL_HANDLE" );
	odbctest -> out_win -> insertLineLimited( txt );

    rec_number = atoi( rec_num -> text().ascii());
    txt.sprintf( "    RecNumber: %d", rec_number );
	odbctest -> out_win -> insertLineLimited( txt );

    b_len = atoi( buffer_len -> text().ascii());
	if ( name_valid -> isOn() )
    {
        buf = NULL;
		txt.sprintf( "    Name: <null pointer>" );
    }
    else
    {
        if ( b_len < 0 )
            b_len = 300;

        buf = new SQLCHAR[ b_len ];
        txt.sprintf( "    Name: %p", buf );
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

	if ( type_valid -> isOn() )
    {
		txt.sprintf( "    TypePtr: <null pointer>" );
        type_ptr = NULL;
    }
	else
    {
        type_ptr = &type_val;
        type_val = -9999;
		txt.sprintf( "    TypePtr: %p", type_ptr );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( sub_type_valid -> isOn() )
    {
		txt.sprintf( "    SubTypePtr: <null pointer>" );
        sub_type_ptr = NULL;
    }
	else
    {
        sub_type_ptr = &sub_type_val;
        sub_type_val = -9999;
		txt.sprintf( "    SubTypePtr: %p", sub_type_ptr );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( length_valid -> isOn() )
    {
		txt.sprintf( "    LengthPtr: <null pointer>" );
        length_ptr = NULL;
    }
	else
    {
        length_ptr = &length_val;
        length_val = -9999;
		txt.sprintf( "    LengthPtr: %p", length_ptr );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( precision_valid -> isOn() )
    {
		txt.sprintf( "    PrecisionPtr: <null pointer>" );
        precision_ptr = NULL;
    }
	else
    {
        precision_ptr = &precision_val;
        precision_val = -9999;
		txt.sprintf( "    PrecisionPtr: %p", precision_ptr );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( scale_valid -> isOn() )
    {
		txt.sprintf( "    ScalePtr: <null pointer>" );
        scale_ptr = NULL;
    }
	else
    {
        scale_ptr = &scale_val;
        scale_val = -9999;
		txt.sprintf( "    ScalePtr: %p", scale_ptr );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( nullable_valid -> isOn() )
    {
		txt.sprintf( "    NullablePtr: <null pointer>" );
        nullable_ptr = NULL;
    }
	else
    {
        nullable_ptr = &nullable_val;
        nullable_val = -9999;
		txt.sprintf( "    NullablePtr: %p", nullable_ptr );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLGetDescRec( in_handle, rec_number,
            buf, b_len, strlen_ptr,
            type_ptr, 
            sub_type_ptr,
            length_ptr,
            precision_ptr,
            scale_ptr,
            nullable_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
        odbctest -> out_win -> insertLineLimited( "  Out:" );
        if ( buf )
        {
            txt.sprintf( "    Name: %s", buf );
            odbctest -> out_win -> insertLineLimited( txt );
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

        if ( type_ptr )
        {	
            if ( type_val == -9999 )
            {
                txt.sprintf( "    *TypePtr: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *TypePtr: %d", type_val );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }

        if ( sub_type_ptr )
        {	
            if ( sub_type_val == -9999 )
            {
                txt.sprintf( "    *SubTypePtr: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *SubTypePtr: %d", sub_type_val );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }

        if ( length_ptr )
        {	
            if ( length_val == -9999 )
            {
                txt.sprintf( "    *LengthPtr: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *LengthPtr: %d", length_val );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }

        if ( precision_ptr )
        {	
            if ( precision_val == -9999 )
            {
                txt.sprintf( "    *PrecisionPtr: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *PrecisionPtr: %d", precision_val );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }

        if ( nullable_ptr )
        {	
            if ( nullable_val == -9999 )
            {
                txt.sprintf( "    *NullablePtr: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *NullablePtr: %d", nullable_val );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
    }

    odbctest -> out_win -> insertLineLimited( "" );

    if ( buf )
        delete buf;
}

dGetDescRec::dGetDescRec( OdbcTest *parent, QString name )
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

	handles = new QComboBox( FALSE, this, "Handles" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_DESC, handles );

	l_handles = new QLabel( "Handle:", this );
	l_handles -> setGeometry( 10, 50, 120, 20 );

	rec_num = new QLineEdit( this, "Rec Num" );
    rec_num -> setGeometry( 130, 80, 70, 20 );
	rec_num -> setMaxLength( 6 );
	rec_num -> setText( "0" );

	l_rec_num = new QLabel( "Rec Number:", this );
    l_rec_num -> setGeometry( 10, 80, 100, 20 );

	name_valid = new QCheckBox( "Name: VALID", this );
	name_valid -> setGeometry( 10, 110, 300, 15 );

	buffer_len = new QLineEdit( this, "Buffer Len" );
    buffer_len -> setGeometry( 130, 140, 70, 20 );
	buffer_len -> setMaxLength( 6 );
	buffer_len -> setText( "300" );

	l_buffer_len = new QLabel( "Buffer Length:", this );
    l_buffer_len -> setGeometry( 10, 140, 100, 20 );

	strlen_valid = new QCheckBox( "StringLengthPtr: VALID", this );
	strlen_valid -> setGeometry( 10, 170, 220, 15 );

	type_valid = new QCheckBox( "TypePtr: VALID", this );
	type_valid -> setGeometry( 300, 170, 220, 15 );

	sub_type_valid = new QCheckBox( "SubTypePtr: VALID", this );
	sub_type_valid -> setGeometry( 10, 200, 220, 15 );

	length_valid = new QCheckBox( "LengthPtr: VALID", this );
	length_valid -> setGeometry( 300, 200, 220, 15 );

	precision_valid = new QCheckBox( "PrecisionPtr: VALID", this );
	precision_valid -> setGeometry( 10, 230, 220, 15 );

	scale_valid = new QCheckBox( "ScalePtr: VALID", this );
	scale_valid -> setGeometry( 300, 230, 220, 15 );

	nullable_valid = new QCheckBox( "NullablePtr: VALID", this );
	nullable_valid -> setGeometry( 10, 260, 220, 15 );

	connect( name_valid, SIGNAL( clicked()), this, SLOT( name_clkd()));
	connect( strlen_valid, SIGNAL( clicked()), this, SLOT( strlen_clkd()));
	connect( type_valid, SIGNAL( clicked()), this, SLOT( type_clkd()));
	connect( sub_type_valid, SIGNAL( clicked()), this, SLOT( sub_type_clkd()));
	connect( length_valid, SIGNAL( clicked()), this, SLOT( length_clkd()));
	connect( scale_valid, SIGNAL( clicked()), this, SLOT( scale_clkd()));
	connect( precision_valid, SIGNAL( clicked()), this, SLOT( precision_clkd()));
	connect( nullable_valid, SIGNAL( clicked()), this, SLOT( nullable_clkd()));

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dGetDescRec::~dGetDescRec()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
    delete name_valid;
    delete strlen_valid;
    delete type_valid;
    delete sub_type_valid;
    delete length_valid;
    delete precision_valid;
    delete nullable_valid;
    delete scale_valid;
    delete buffer_len;
    delete l_buffer_len;
    delete rec_num;
    delete l_rec_num;
}

void OdbcTest::sqlcopydesc()
{
	dCopyDesc *dlg = new dCopyDesc( this, "SQLCopyDesc" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetdescfield()
{
	dGetDescField *dlg = new dGetDescField( this, "SQLGetDescField" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlgetdescrec()
{
	dGetDescRec *dlg = new dGetDescRec( this, "SQLGetDescRec" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlsetdescfield()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::sqlsetdescrec()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::getdescriptorsall()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}
