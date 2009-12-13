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
 * $Id: env.cpp,v 1.3 2004/06/21 10:01:17 lurcher Exp $
 *
 * $Log: env.cpp,v $
 * Revision 1.3  2004/06/21 10:01:17  lurcher
 *
 * Fix a couple of 64 bit issues
 *
 * Revision 1.2  2001/12/20 17:26:26  lurcher
 *
 * More warnings removed
 *
 * Revision 1.1.1.1  2001/10/17 16:40:31  lurcher
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
 * Revision 1.7  2000/06/23 16:23:22  ngorham
 *
 * Couple of small changes
 *
 * Revision 1.6  2000/06/16 14:09:28  ngorham
 *
 * Fix a couple of bugs in GetStmtOptions
 *
 * Revision 1.5  2000/06/13 12:30:25  ngorham
 *
 * Enough there for the first release I think
 *
 * Revision 1.4  2000/06/09 17:04:20  ngorham
 *
 * More, and More
 *
 * Revision 1.3  2000/06/05 16:53:20  ngorham
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

#include <stdio.h>
#include <stdlib.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <sql.h>
#include <sqlext.h>
#include "odbctest.h"
#include "env.h"

static attr_value handle_type_struct[] = 
{
	{ "SQL_HANDLE_ENV", SQL_HANDLE_ENV },
	{ "SQL_HANDLE_DBC", SQL_HANDLE_DBC },
	{ NULL }
};

static attr_value completion_type_struct[] = 
{
	{ "SQL_COMMIT", SQL_COMMIT },
	{ "SQL_ROLLBACK", SQL_ROLLBACK },
	{ NULL }
};

static attr_value data_sources_directions[] = 
{
	{ "SQL_FETCH_NEXT", SQL_FETCH_NEXT },
	{ "SQL_FETCH_FIRST", SQL_FETCH_FIRST },
	{ "SQL_FETCH_FIRST_USER", SQL_FETCH_FIRST_USER },
	{ "SQL_FETCH_FIRST_SYSTEM", SQL_FETCH_FIRST_SYSTEM },
	{ NULL }
};

static attr_value drivers_directions[] = 
{
	{ "SQL_FETCH_NEXT", SQL_FETCH_NEXT },
	{ "SQL_FETCH_FIRST", SQL_FETCH_FIRST },
	{ NULL }
};

void dAllocHandle::Ok()
{
SQLHANDLE out_handle, in_handle = SQL_NULL_HANDLE;
SQLRETURN ret;
SQLINTEGER type;
const char *handle;

	switch( types -> currentItem())
	{
		case 0:
			type = SQL_HANDLE_ENV;
			handle = "SQL_HANDLE_ENV=1";
			break;

		case 1:
			type = SQL_HANDLE_DBC;
			handle = "SQL_HANDLE_DBC=2";
			break;

		case 2:
			type = SQL_HANDLE_STMT;
			handle = "SQL_HANDLE_STMT=3";
			break;

		case 3:
			type = SQL_HANDLE_DESC;
			handle = "SQL_HANDLE_DESC=4";
			break;
	}

	/*
	 * get input handle
	 */

    Handle *hand = odbctest->extract_handle_list( -1, handles );

	if ( hand )
		in_handle = hand -> getHandle();

	/*
	 * set output handle to something
	 */
	out_handle = 0;

	if ( valid -> isOn() )
		ret = SQLAllocHandle( type, in_handle, SQL_NULL_HANDLE );
	else
		ret = SQLAllocHandle( type, in_handle, &out_handle );

	odbctest -> out_win -> insertLineLimited( "SQLAllocHandle():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	txt.sprintf( "    Handle Type: %s", handle );
	odbctest -> out_win -> insertLineLimited( txt );
	if ( in_handle )
	    txt.sprintf( "    InputHandle: %p", in_handle );
	else
	    txt.sprintf( "    InputHandle: SQL_NULL_HANDLE" );
	odbctest -> out_win -> insertLineLimited( txt );
	if ( valid -> isOn() )
		txt.sprintf( "    OutputHandle: SQL_NULL_HANDLE" );
	else
		txt.sprintf( "    OutputHandle: %p", &out_handle );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Out:" );
	if ( out_handle )
		txt.sprintf( "    *OutputHandle: %p", out_handle );
	else
		txt.sprintf( "    *OutputHandle: <unmodified>" );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );

	if ( SQL_SUCCEEDED( ret ))
	{
	    odbctest->listHandle.append( new Handle( type, out_handle, odbctest->listHandle ));
	}
}

void dAllocHandle::out_handle_ptr_clkd()
{
	if ( valid -> isOn() )
	    valid -> setText( "OutputHandlePtr: SQL_NULL_HANDLE" );
	else
	    valid -> setText( "OutputHandlePtr: VALID" );
}

dAllocHandle::dAllocHandle( OdbcTest *parent, QString name )
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

	handles = new QComboBox( FALSE, this, "Input Handle" );
	handles -> setGeometry( 120, 80, 200, 20 );

	odbctest->fill_handle_list( -1, handles );

	types = new QComboBox( FALSE, this, "Handle Type" );
	types -> setGeometry( 120, 50, 200, 20 );

	types -> insertItem( "SQL_HANDLE_ENV=1 (3.0)", 0 );
	types -> insertItem( "SQL_HANDLE_DBC=2 (3.0)", 1 );
	types -> insertItem( "SQL_HANDLE_STMT=3 (3.0)", 2 );
	types -> insertItem( "SQL_HANDLE_DESC=3 (3.0)", 3 );

	valid = new QCheckBox( "OutputHandlePtr: VALID", this );
	valid -> setGeometry( 10, 120, 200, 15 );

	connect( valid, SIGNAL( clicked()), this, SLOT( out_handle_ptr_clkd()));

	l_handle = new QLabel( "InputHandle:", this );
	l_handle -> setGeometry( 10, 80, 70, 20 );

	l_types = new QLabel( "HandleType:", this );
	l_types -> setGeometry( 10, 50, 70, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dAllocHandle::~dAllocHandle()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete l_handle;
	delete l_types;
	delete valid;
}

void dFreeHandle::Ok()
{
SQLHANDLE in_handle = SQL_NULL_HANDLE;
SQLRETURN ret;
SQLINTEGER type;
const char *handle;

	switch( types -> currentItem())
	{
		case 0:
			type = SQL_HANDLE_ENV;
			handle = "SQL_HANDLE_ENV=1";
			break;

		case 1:
			type = SQL_HANDLE_DBC;
			handle = "SQL_HANDLE_DBC=2";
			break;

		case 2:
			type = SQL_HANDLE_STMT;
			handle = "SQL_HANDLE_STMT=3";
			break;

		case 3:
			type = SQL_HANDLE_DESC;
			handle = "SQL_HANDLE_DESC=4";
			break;
	}

	/*
	 * get input handle
	 */

    Handle *hand = odbctest->extract_handle_list( -1, handles );

	if ( hand )
		in_handle = hand -> getHandle();

	ret = SQLFreeHandle( type, in_handle );

	odbctest -> out_win -> insertLineLimited( "SQLFreeHandle():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );
	txt.sprintf( "    Handle Type: %s", handle );
	odbctest -> out_win -> insertLineLimited( txt );
	if ( in_handle )
	    txt.sprintf( "    InputHandle: %p", in_handle );
	else
	    txt.sprintf( "    InputHandle: SQL_NULL_HANDLE" );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );
	odbctest -> out_win -> insertLineLimited( "" );

	if ( SQL_SUCCEEDED( ret ))
	{
		odbctest->listHandle.remove( hand );
	}
}

dFreeHandle::dFreeHandle( OdbcTest *parent, QString name )
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

	handles = new QComboBox( FALSE, this, "Input Handle" );
	handles -> setGeometry( 120, 80, 200, 20 );

	odbctest->fill_handle_list( -1, handles );

	types = new QComboBox( FALSE, this, "Handle Type" );
	types -> setGeometry( 120, 50, 200, 20 );

	types -> insertItem( "SQL_HANDLE_ENV=1 (3.0)", 0 );
	types -> insertItem( "SQL_HANDLE_DBC=2 (3.0)", 1 );
	types -> insertItem( "SQL_HANDLE_STMT=3 (3.0)", 2 );
	types -> insertItem( "SQL_HANDLE_DESC=3 (3.0)", 3 );

	l_handle = new QLabel( "InputHandle:", this );
	l_handle -> setGeometry( 10, 80, 70, 20 );

	l_types = new QLabel( "HandleType:", this );
	l_types -> setGeometry( 10, 50, 70, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dFreeHandle::~dFreeHandle()
{
	delete ok;
	delete cancel;
	delete help;
	delete types;
	delete handles;
	delete l_handle;
	delete l_types;
}

void dDataSources::server_clkd()
{
	if ( server_valid -> isOn() )
	    server_valid -> setText( "ServerName: SQL_NULL_POINTER" );
	else
	    server_valid -> setText( "ServerName: VALID" );
}

void dDataSources::description_clkd()
{
	if ( description_valid -> isOn() )
	    description_valid -> setText( "Description: SQL_NULL_POINTER" );
	else
	    description_valid -> setText( "Description: VALID" );
}

void dDataSources::nlp1_clkd()
{
	if ( nlp1_valid -> isOn() )
	    nlp1_valid -> setText( "NameLengthPtr1: SQL_NULL_POINTER" );
	else
	    nlp1_valid -> setText( "NameLengthPtr1: VALID" );
}

void dDataSources::nlp2_clkd()
{
	if ( nlp2_valid -> isOn() )
	    nlp2_valid -> setText( "NameLengthPtr2: SQL_NULL_POINTER" );
	else
	    nlp2_valid -> setText( "NameLengthPtr2: VALID" );
}

void dDataSources::Ok()
{
    Handle *env = odbctest->extract_handle_list( SQL_HANDLE_ENV, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
    SQLCHAR *server_name, *description;
    SQLSMALLINT bl1, bl2, *nlp1, *nlp2, nl1, nl2;
    SQLUSMALLINT direc;
    
	if ( env )
		in_handle = env -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLDataSources():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Environment Handle: %p", in_handle );
	else
		txt.sprintf( "    Environment Handle: SQL_NULL_HENV" );
	odbctest -> out_win -> insertLineLimited( txt );

	direc = data_sources_directions[ direction -> currentItem() ].value;
	txt.sprintf( "    Direction: %s=%d", 
		data_sources_directions[ direction -> currentItem() ].text,
		data_sources_directions[ direction -> currentItem() ].value );

    bl1 = atoi( server_len -> text().ascii());
	if ( server_valid -> isOn() )
    {
        server_name = NULL;
	    txt.sprintf( "    ServerName: <null pointer>" );
    }
	else
    {
        if ( bl1 < 0 )
            bl1 = 300;

        server_name = new SQLCHAR[ bl1 ];

		txt.sprintf( "    ServerName: %p", bl1 );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( nlp1_valid -> isOn() )
    {
        nlp1 = NULL;
	    txt.sprintf( "    NameLengthPtr 1: <null pointer>" );
    }
    else
    {
        nl1 = -9999;
        nlp1 = &nl1;
	    txt.sprintf( "    NameLengthPtr 1: %p", nlp1 );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    bl2 = atoi( description_len -> text().ascii());
	if ( description_valid -> isOn() )
    {
        description = NULL;
	    txt.sprintf( "    Description: <null pointer>" );
    }
	else
    {
        if ( bl1 < 0 )
            bl1 = 300;

        description = new SQLCHAR[ bl1 ];

		txt.sprintf( "    Description: %p", bl1 );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( nlp2_valid -> isOn() )
    {
        nlp2 = NULL;
	    txt.sprintf( "    NameLengthPtr 2: <null pointer>" );
    }
    else
    {
        nl2 = -9999;
        nlp2 = &nl2;
	    txt.sprintf( "    NameLengthPtr 2: %p", nlp2 );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLDataSources( in_handle, direc, server_name, bl1, nlp1,
            description, bl2, nlp2 );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
        odbctest -> out_win -> insertLineLimited( "  Out:" );
        if ( server_name )
        {
	        txt.sprintf( "    ServerName: %s", server_name );
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( nlp1 )
        {
            if ( nl1 == 9999 )
            {
                txt.sprintf( "    *NameLengthPtr 1: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *NameLengthPtr 1: %d", nl1 );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( description )
        {
	        txt.sprintf( "    Description: %s", description );
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( nlp2 )
        {
            if ( nl2 == 9999 )
            {
                txt.sprintf( "    *NameLengthPtr 2: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *NameLengthPtr 2: %d", nl2 );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
    }

    odbctest -> out_win -> insertLineLimited( "" );

    if ( server_name )
        delete server_name;
    if ( description )
        delete description;
}

dDataSources::dDataSources( OdbcTest *parent, QString name )
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

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_ENV, handles );

	l_handles = new QLabel( "Environment Handle:", this );
	l_handles -> setGeometry( 10, 50, 120, 20 );

	direction = new QComboBox( FALSE, this, "Direction" );
	direction -> setGeometry( 130, 80, 200, 20 );
	parent->fill_list_box( data_sources_directions, direction );

	l_direction = new QLabel( "Direction:", this );
	l_direction -> setGeometry( 10, 80, 120, 20 );

	server_valid = new QCheckBox( "ServerName: VALID", this );
	server_valid -> setGeometry( 10, 110, 300, 15 );

	server_len = new QLineEdit( this, "ServerName" );
    server_len -> setGeometry( 350, 110, 70, 20 );
	server_len -> setMaxLength( 6 );
	server_len -> setText( "300" );

	l_server_len = new QLabel( "BufferLength 1:", this );
    l_server_len -> setGeometry( 240, 110, 100, 20 );

	nlp1_valid = new QCheckBox( "Name Length Ptr 1: VALID", this );
	nlp1_valid -> setGeometry( 10, 140, 300, 15 );

	description_valid = new QCheckBox( "Description: VALID", this );
	description_valid -> setGeometry( 10, 170, 300, 15 );

	description_len = new QLineEdit( this, "Description" );
    description_len -> setGeometry( 350, 170, 70, 20 );
	description_len -> setMaxLength( 6 );
	description_len -> setText( "300" );

	l_description_len = new QLabel( "BufferLength 2:", this );
    l_description_len -> setGeometry( 240, 170, 100, 20 );

	nlp2_valid = new QCheckBox( "Name Length Ptr 2: VALID", this );
	nlp2_valid -> setGeometry( 10, 200, 300, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );

	connect( server_valid, SIGNAL( clicked()), this, SLOT( server_clkd()));
	connect( nlp1_valid, SIGNAL( clicked()), this, SLOT( nlp1_clkd()));
	connect( description_valid, SIGNAL( clicked()), this, SLOT( description_clkd()));
	connect( nlp2_valid, SIGNAL( clicked()), this, SLOT( nlp2_clkd()));
}

dDataSources::~dDataSources()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
    delete direction;
    delete l_direction;
    delete server_valid;
    delete server_len;
    delete l_server_len;
    delete nlp1_valid;
    delete description_valid;
    delete description_len;
    delete l_description_len;
    delete nlp2_valid;
}

void dDrivers::server_clkd()
{
	if ( server_valid -> isOn() )
	    server_valid -> setText( "ServerName: SQL_NULL_POINTER" );
	else
	    server_valid -> setText( "ServerName: VALID" );
}

void dDrivers::description_clkd()
{
	if ( description_valid -> isOn() )
	    description_valid -> setText( "Description: SQL_NULL_POINTER" );
	else
	    description_valid -> setText( "Description: VALID" );
}

void dDrivers::nlp1_clkd()
{
	if ( nlp1_valid -> isOn() )
	    nlp1_valid -> setText( "NameLengthPtr1: SQL_NULL_POINTER" );
	else
	    nlp1_valid -> setText( "NameLengthPtr1: VALID" );
}

void dDrivers::nlp2_clkd()
{
	if ( nlp2_valid -> isOn() )
	    nlp2_valid -> setText( "NameLengthPtr2: SQL_NULL_POINTER" );
	else
	    nlp2_valid -> setText( "NameLengthPtr2: VALID" );
}

void dDrivers::Ok()
{
    Handle *env = odbctest->extract_handle_list( SQL_HANDLE_ENV, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
    SQLCHAR *server_name, *description;
    SQLSMALLINT bl1, bl2, *nlp1, *nlp2, nl1, nl2;
    SQLUSMALLINT direc;
    
	if ( env )
		in_handle = env -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLDrivers():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    Environment Handle: %p", in_handle );
	else
		txt.sprintf( "    Environment Handle: SQL_NULL_HENV" );
	odbctest -> out_win -> insertLineLimited( txt );

	direc = drivers_directions[ direction -> currentItem() ].value;
	txt.sprintf( "    Direction: %s=%d", 
		data_sources_directions[ direction -> currentItem() ].text,
		data_sources_directions[ direction -> currentItem() ].value );

    bl1 = atoi( server_len -> text().ascii());
	if ( server_valid -> isOn() )
    {
        server_name = NULL;
	    txt.sprintf( "    ServerName: <null pointer>" );
    }
	else
    {
        if ( bl1 < 0 )
            bl1 = 300;

        server_name = new SQLCHAR[ bl1 ];

		txt.sprintf( "    ServerName: %p", bl1 );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( nlp1_valid -> isOn() )
    {
        nlp1 = NULL;
	    txt.sprintf( "    NameLengthPtr 1: <null pointer>" );
    }
    else
    {
        nl1 = -9999;
        nlp1 = &nl1;
	    txt.sprintf( "    NameLengthPtr 1: %p", nlp1 );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    bl2 = atoi( description_len -> text().ascii());
	if ( description_valid -> isOn() )
    {
        description = NULL;
	    txt.sprintf( "    Description: <null pointer>" );
    }
	else
    {
        if ( bl1 < 0 )
            bl1 = 300;

        description = new SQLCHAR[ bl1 ];

		txt.sprintf( "    Description: %p", bl1 );
    }
	odbctest -> out_win -> insertLineLimited( txt );

	if ( nlp2_valid -> isOn() )
    {
        nlp2 = NULL;
	    txt.sprintf( "    NameLengthPtr 2: <null pointer>" );
    }
    else
    {
        nl2 = -9999;
        nlp2 = &nl2;
	    txt.sprintf( "    NameLengthPtr 2: %p", nlp2 );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLDrivers( in_handle, direc, server_name, bl1, nlp1,
            description, bl2, nlp2 );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ))
    {
        odbctest -> out_win -> insertLineLimited( "  Out:" );
        if ( server_name )
        {
	        txt.sprintf( "    ServerName: %s", server_name );
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( nlp1 )
        {
            if ( nl1 == 9999 )
            {
                txt.sprintf( "    *NameLengthPtr 1: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *NameLengthPtr 1: %d", nl1 );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( description )
        {
	        txt.sprintf( "    Description: %s", description );
            odbctest -> out_win -> insertLineLimited( txt );
        }
        if ( nlp2 )
        {
            if ( nl2 == 9999 )
            {
                txt.sprintf( "    *NameLengthPtr 2: <unchanged>" );
            }
            else
            {
                txt.sprintf( "    *NameLengthPtr 2: %d", nl2 );
            }
            odbctest -> out_win -> insertLineLimited( txt );
        }
    }

    odbctest -> out_win -> insertLineLimited( "" );

    if ( server_name )
        delete server_name;
    if ( description )
        delete description;
}

dDrivers::dDrivers( OdbcTest *parent, QString name )
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

	handles = new QComboBox( FALSE, this, "Handle" );
	handles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_ENV, handles );

	l_handles = new QLabel( "Environment Handle:", this );
	l_handles -> setGeometry( 10, 50, 120, 20 );

	direction = new QComboBox( FALSE, this, "Direction" );
	direction -> setGeometry( 130, 80, 200, 20 );
	parent->fill_list_box( drivers_directions, direction );

	l_direction = new QLabel( "Direction:", this );
	l_direction -> setGeometry( 10, 80, 120, 20 );

	server_valid = new QCheckBox( "ServerName: VALID", this );
	server_valid -> setGeometry( 10, 110, 300, 15 );

	server_len = new QLineEdit( this, "ServerName" );
    server_len -> setGeometry( 350, 110, 70, 20 );
	server_len -> setMaxLength( 6 );
	server_len -> setText( "300" );

	l_server_len = new QLabel( "BufferLength 1:", this );
    l_server_len -> setGeometry( 240, 110, 100, 20 );

	nlp1_valid = new QCheckBox( "Name Length Ptr 1: VALID", this );
	nlp1_valid -> setGeometry( 10, 140, 300, 15 );

	description_valid = new QCheckBox( "Description: VALID", this );
	description_valid -> setGeometry( 10, 170, 300, 15 );

	description_len = new QLineEdit( this, "Description" );
    description_len -> setGeometry( 350, 170, 70, 20 );
	description_len -> setMaxLength( 6 );
	description_len -> setText( "300" );

	l_description_len = new QLabel( "BufferLength 2:", this );
    l_description_len -> setGeometry( 240, 170, 100, 20 );

	nlp2_valid = new QCheckBox( "Name Length Ptr 2: VALID", this );
	nlp2_valid -> setGeometry( 10, 200, 300, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );

	connect( server_valid, SIGNAL( clicked()), this, SLOT( server_clkd()));
	connect( nlp1_valid, SIGNAL( clicked()), this, SLOT( nlp1_clkd()));
	connect( description_valid, SIGNAL( clicked()), this, SLOT( description_clkd()));
	connect( nlp2_valid, SIGNAL( clicked()), this, SLOT( nlp2_clkd()));
}

dDrivers::~dDrivers()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
    delete direction;
    delete l_direction;
    delete server_valid;
    delete server_len;
    delete l_server_len;
    delete nlp1_valid;
    delete description_valid;
    delete description_len;
    delete l_description_len;
    delete nlp2_valid;
}

void dEndTran::sel_handle( int index )
{
    int handle_t;

	handles -> clear();
	handle_t = handle_type_struct[ handle_type -> currentItem() ].value;

	odbctest->fill_handle_list( handle_t, handles );
}

void dEndTran::Ok()
{
    int htype = handle_type_struct[ handle_type -> currentItem() ].value;
    Handle *hand = odbctest->extract_handle_list( htype, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
    SQLSMALLINT completion;

	if ( hand  )
		in_handle = hand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLEndTran():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

    txt.sprintf( "    Handle Type: %s", handle_type_struct[ handle_type -> currentItem() ].text );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( in_handle )
		txt.sprintf( "    Handle: %p", in_handle );
	else
		txt.sprintf( "    Handle: SQL_NULL_HANDLE" );
	odbctest -> out_win -> insertLineLimited( txt );

    completion = completion_type_struct[ completion_type -> currentItem() ].value;

    SQLRETURN ret = SQLEndTran( htype, in_handle, completion );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dEndTran::dEndTran( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 110,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 190,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 270,10, 70,25 );

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

	completion_type = new QComboBox( FALSE, this, "Handle" );
	completion_type -> setGeometry( 130, 110, 200, 20 );
	parent->fill_list_box( completion_type_struct, completion_type );

	l_completion_type = new QLabel( "Completion Type:", this );
	l_completion_type -> setGeometry( 10, 110, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );

	connect( handle_type, SIGNAL(activated(int)), this, SLOT( sel_handle(int)));
}

dEndTran::~dEndTran()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
	delete handle_type;
	delete l_handle_type;
    delete completion_type;
    delete l_completion_type;
}

void dAllocEnv::handle_clkd()
{
	if ( handle_valid -> isOn() )
	    handle_valid -> setText( "phenv: SQL_NULL_POINTER" );
	else
	    handle_valid -> setText( "phenv: VALID" );
}

void dAllocEnv::Ok()
{
    SQLHENV henv, *henv_ptr;

	odbctest -> out_win -> insertLineLimited( "SQLAllocEnv():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

    if ( handle_valid -> isOn())
    {
        henv_ptr = NULL;
		txt.sprintf( "    phenv: SQL_NULL_HANDLE" );
    }
    else
    {
        henv_ptr = &henv;
		txt.sprintf( "    phenv: %p", henv_ptr );
    }
	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLAllocEnv( henv_ptr );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ) && henv_ptr )
    {
		txt.sprintf( "    *phenv: %p", henv );
	    odbctest -> out_win -> insertLineLimited( txt );

		odbctest->listHandle.append( new Handle( SQL_HANDLE_ENV, henv ) );
    }

	odbctest -> out_win -> insertLineLimited( "" );
}

dAllocEnv::dAllocEnv( OdbcTest *parent, QString name )
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

	handle_valid = new QCheckBox( "phenv: VALID", this );
	handle_valid -> setGeometry( 10, 50, 300, 15 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	connect( handle_valid, SIGNAL( clicked()), this, SLOT( handle_clkd()));
}

dAllocEnv::~dAllocEnv()
{
	delete ok;
	delete cancel;
	delete help;
	delete handle_valid;
}

void dFreeEnv::Ok()
{
    Handle *env = odbctest->extract_handle_list( SQL_HANDLE_ENV, handles );
	SQLHANDLE in_handle = SQL_NULL_HANDLE;
    
	if ( env )
		in_handle = env -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLFreeEnv():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( in_handle )
		txt.sprintf( "    henv: %p", in_handle );
	else
		txt.sprintf( "    henv: SQL_NULL_HENV" );
	odbctest -> out_win -> insertLineLimited( txt );

    SQLRETURN ret = SQLFreeEnv( in_handle );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

    if ( SQL_SUCCEEDED( ret ) && in_handle )
    {
		odbctest->listHandle.remove( env );
    }

	odbctest -> out_win -> insertLineLimited( "" );
}

dFreeEnv::dFreeEnv( OdbcTest *parent, QString name )
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

	handles = new QComboBox( FALSE, this, "henv" );
	handles -> setGeometry( 120, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_ENV, handles );

	l_handles = new QLabel( "henv:", this );
	l_handles -> setGeometry( 10, 50, 100, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dFreeEnv::~dFreeEnv()
{
	delete ok;
	delete cancel;
	delete help;
	delete handles;
	delete l_handles;
}

void dTransact::Ok()
{
    Handle *ehand = odbctest->extract_handle_list( SQL_HANDLE_ENV, ehandles );
    Handle *chand = odbctest->extract_handle_list( SQL_HANDLE_DBC, chandles );
	SQLHANDLE ein_handle = SQL_NULL_HANDLE;
	SQLHANDLE cin_handle = SQL_NULL_HANDLE;
    SQLSMALLINT completion;

	if ( ehand  )
		ein_handle = ehand -> getHandle();
	if ( chand  )
		cin_handle = chand -> getHandle();

	odbctest -> out_win -> insertLineLimited( "SQLTransact():" );
	odbctest -> out_win -> insertLineLimited( "  In:" );

	if ( ein_handle )
		txt.sprintf( "    henv: %p", ein_handle );
	else
		txt.sprintf( "    henv: SQL_NULL_HENV" );
	odbctest -> out_win -> insertLineLimited( txt );

	if ( cin_handle )
		txt.sprintf( "    hdbc: %p", cin_handle );
	else
		txt.sprintf( "    hdbc: SQL_NULL_HDBC" );
	odbctest -> out_win -> insertLineLimited( txt );

    completion = completion_type_struct[ completion_type -> currentItem() ].value;

    SQLRETURN ret = SQLTransact( ein_handle, cin_handle, completion );

	odbctest -> out_win -> insertLineLimited( "  Return:" );
	txt.sprintf( "    %s=%d", odbctest->return_as_text( ret ), ret );
	odbctest -> out_win -> insertLineLimited( txt );

	odbctest -> out_win -> insertLineLimited( "" );
}

dTransact::dTransact( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    ok = new QPushButton( "OK", this );
    ok->setGeometry( 110,10, 70,25 );

    cancel = new QPushButton( "Cancel", this );
    cancel->setGeometry( 190,10, 70,25 );

    help = new QPushButton( "Help", this );
    help->setGeometry( 270,10, 70,25 );

	ehandles = new QComboBox( FALSE, this, "Handles" );
	ehandles -> setGeometry( 130, 50, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_ENV, ehandles );

	l_ehandles = new QLabel( "henv:", this );
	l_ehandles -> setGeometry( 10, 50, 120, 20 );

	chandles = new QComboBox( FALSE, this, "Handles" );
	chandles -> setGeometry( 130, 80, 200, 20 );
	odbctest->fill_handle_list( SQL_HANDLE_DBC, chandles );

	l_chandles = new QLabel( "hdbc:", this );
	l_chandles -> setGeometry( 10, 80, 120, 20 );

	completion_type = new QComboBox( FALSE, this, "Handle" );
	completion_type -> setGeometry( 130, 110, 200, 20 );
	parent->fill_list_box( completion_type_struct, completion_type );

	l_completion_type = new QLabel( "fType Type:", this );
	l_completion_type -> setGeometry( 10, 110, 120, 20 );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
}

dTransact::~dTransact()
{
	delete ok;
	delete cancel;
	delete help;
	delete ehandles;
	delete l_ehandles;
	delete chandles;
	delete l_chandles;
    delete completion_type;
    delete l_completion_type;
}

void OdbcTest::sqlallochandle()
{
	dAllocHandle *dlg = new dAllocHandle( this, "SQLAllocHandle" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqldatasources()
{
	dDataSources *dlg = new dDataSources( this, "SQLDataSources" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqldrivers()
{
	dDrivers *dlg = new dDrivers( this, "SQLDrivers" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlendtran()
{
	dEndTran *dlg = new dEndTran( this, "SQLEndTran" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlfreehandle()
{
	dFreeHandle *dlg = new dFreeHandle( this, "SQLFreeHandle" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlallocenv()
{
	dAllocEnv *dlg = new dAllocEnv( this, "SQLAllocEnv" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqlfreeenv()
{
	dFreeEnv *dlg = new dFreeEnv( this, "SQLFreeEnv" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::sqltransact()
{
	dTransact *dlg = new dTransact( this, "SQLTransact" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::datasourcesall()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::driversall()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}
