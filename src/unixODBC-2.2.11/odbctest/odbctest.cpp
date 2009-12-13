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
 * $Id: odbctest.cpp,v 1.3 2002/03/01 14:57:32 lurcher Exp $
 *
 * $Log: odbctest.cpp,v $
 * Revision 1.3  2002/03/01 14:57:32  lurcher
 *
 * alter default size of odbctest
 *
 * Revision 1.2  2001/12/20 17:26:26  lurcher
 *
 * More warnings removed
 *
 * Revision 1.1.1.1  2001/10/17 16:40:31  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.8  2001/06/05 01:04:40  nick
 *
 * More tweeks for QT 2 and 3
 *
 * Revision 1.7  2001/06/04 15:24:49  nick
 *
 * Add port to MAC OSX and QT3 changes
 *
 * Revision 1.6  2001/05/31 16:05:55  nick
 *
 * Fix problems with postgres closing local sockets
 * Make odbctest build with QT 3 (it doesn't work due to what I think are bugs
 * in QT 3)
 * Fix a couple of problems in the cursor lib
 *
 * Revision 1.5  2001/04/17 16:29:39  nick
 *
 * More checks and autotest fixes
 *
 * Revision 1.4  2001/04/17 12:57:11  nick
 *
 * Extra for AutoTest
 *
 * Revision 1.3  2001/04/14 19:00:34  nick
 *
 * Add support for ODBC Test handles, and catch a few problems with
 * empty Gator.ini
 *
 * Revision 1.2  2001/04/12 17:43:36  nick
 *
 * Change logging and added autotest to odbctest
 *
 * Revision 1.1.1.1  2000/09/04 16:42:53  nick
 * Imported Sources
 *
 * Revision 1.11  2000/06/23 16:23:23  ngorham
 *
 * Couple of small changes
 *
 * Revision 1.10  2000/06/21 08:59:43  ngorham
 *
 * More NEWS
 *
 * Revision 1.9  2000/06/15 09:31:04  ngorham
 *
 * Add some fixes and porting options after 1.8.9 has been released
 *
 * Revision 1.8  2000/06/13 12:30:26  ngorham
 *
 * Enough there for the first release I think
 *
 * Revision 1.7  2000/06/09 17:04:20  ngorham
 *
 * More, and More
 *
 * Revision 1.6  2000/06/08 17:14:08  ngorham
 *
 * And more
 *
 * Revision 1.5  2000/06/07 15:10:22  ngorham
 *
 * More additions
 *
 * Revision 1.4  2000/06/07 08:29:47  ngorham
 *
 * More additions
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

#include <qmainwindow.h>
#include <qpopupmenu.h>
#include <qmenudata.h>
#include <qmenubar.h>
#include <qkeycode.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qsplitter.h>
#include <qmultilineedit.h>
#if (QT_VERSION>=300)
#include <qtextedit.h>
#endif
#include <sql.h>
#include <sqlext.h>
#include "odbctest.h"

#if (QT_VERSION<300)

QTextEdit::QTextEdit (QWidget *parent, const char *name )
    :QMultiLineEdit( parent, name )
{
    QMultiLineEdit::setMaxLines( 1 );
}

void QTextEdit::maxLines( int n )
{
    QMultiLineEdit::setMaxLines( n );
}

void QTextEdit::setMaxLength( int x )
{
    QMultiLineEdit::setMaxLineLength( 1 );
}

void QTextEdit::append( const char *str )
{
    QMultiLineEdit::append( str );
    setCursorPosition( numLines() + 1, 0 );
}

#endif

OutputWin::OutputWin( QWidget *parent , const char *name )
    :QTextEdit( parent, name )
{
#if (QT_VERSION<300)
    QTextEdit::maxLines( -1 );
#endif
}

void OutputWin::insertLineLimited( const char * str )
{
	QTextEdit::append( str );
}

void OutputWin::setMaxLines( int i )
{
	max_lines = i;
}

char * Handle::toStr( char * str )
{
	switch( type )
	{
		case SQL_HANDLE_ENV:
			sprintf( str, "henv: %p", handle );
			break;

		case SQL_HANDLE_DBC:
			sprintf( str, "hdbc: %p", handle );
			break;

		case SQL_HANDLE_STMT:
			sprintf( str, "hstmt: %p", handle );
			break;

		case SQL_HANDLE_DESC:
			sprintf( str, "hdesc: %p \"%s\"", handle, description.latin1());
			break;
	}

	return str;
};

const char * OdbcTest::return_as_text( int ret )
{
	switch( ret )
	{
		case SQL_SUCCESS:
			return "SQL_SUCCESS";

		case SQL_SUCCESS_WITH_INFO:
			return "SQL_SUCCESS_WITH_INFO";

		case SQL_ERROR:
			return "SQL_ERROR";

		case SQL_INVALID_HANDLE:
			return "SQL_INVALID_HANDLE";

		case SQL_STILL_EXECUTING:
			return "SQL_STILL_EXECUTING";

		case SQL_NEED_DATA:
			return "SQL_NEED_DATA";

		case SQL_NO_DATA:
			return "SQL_NO_DATA";

		default:
			return "Unknown Return ?";
	}
}

void OdbcTest::fill_list_box( attr_value *attr, QComboBox *lst )
{
	char txt[ 128 ];

	for ( int i = 0; attr[ i ].text; i ++ )
	{
		if ( strlen( attr[ i ].text ) > 0 )
		{
			if ( attr[ i ].version )
			{
				sprintf( txt, "%s=%d (%s)", attr[ i ].text, 
					attr[ i ].value, attr[ i ].version );
			}
			else
			{
				sprintf( txt, "%s=%d", attr[ i ].text, 
					attr[ i ].value );
			}
		}
		else
		{
			sprintf( txt, "%d", attr[ i ].value );
		}
		lst -> insertItem( txt, i );
	}
}

void OdbcTest::fill_list_box( attr_options *attr, QComboBox *lst )
{
	char txt[ 128 ];

	for ( int i = 0; attr[ i ].text; i ++ )
	{
		if ( strlen( attr[ i ].text ) > 0 )
		{
			if ( attr[ i ].version )
			{
				sprintf( txt, "%s=%d (%s)", attr[ i ].text, 
					attr[ i ].attr, attr[ i ].version );
			}
			else
			{
				sprintf( txt, "%s=%d", attr[ i ].text, 
					attr[ i ].attr );
			}
		}
		else
		{
			sprintf( txt, "%d", attr[ i ].attr );
		}
		lst -> insertItem( txt, i );
	}
}

SQLHANDLE OdbcTest::get_handle( int type )
{
	Handle *hand, *match = NULL;
	int i;
	char txt[ 128 ];

    for ( hand=listHandle.first(), i = 0;
		hand != 0; 
		hand=listHandle.next() )
	{
		if ( hand->getType() == type )
		{
			return hand->getHandle();
		}
	}

	return SQL_NULL_HANDLE;
}

Handle * OdbcTest::fill_handle_list( int type, QComboBox *lst )
{
	Handle *hand, *match = NULL;
	int i;
	char txt[ 128 ];

    for ( hand=listHandle.first(), i = 0;
		hand != 0; 
		hand=listHandle.next() )
	{
		hand->toStr( txt );
		if ( type < 0 )
		{
			lst->insertItem( txt, i ++ );
			match = hand;
		}
		else if ( hand->getType() == type )
		{
			lst->insertItem( txt, i ++ );
			match = hand;
		}
	}
	lst->insertItem( "SQL_NULL_HANDLE", i );

	return match;
}

Handle *OdbcTest::extract_handle_list( int type, QComboBox *lst )
{
	int index = lst -> currentItem(); 
	Handle *hand;
	int i;

	for ( i = 0, hand=listHandle.first();
		hand;
		hand=listHandle.next())
	{
		if ( type < 0 || hand->getType() == type )
		{
			if ( i == index )
			{
				break;
			}
			i ++;
		}
	}

	return hand;
}

void OdbcTest::dumpError( int type, SQLHANDLE hnd )
{
	SQLRETURN ret;
	SQLINTEGER count, i;
	char handle[ 30 ];

	switch( type )
	{
		case SQL_HANDLE_ENV:
			sprintf( handle, "henv %p", hnd );
			break;

		case SQL_HANDLE_DBC:
			sprintf( handle, "hdbc %p", hnd );
			break;

		case SQL_HANDLE_STMT:
			sprintf( handle, "hstmt %p", hnd );
			break;

		case SQL_HANDLE_DESC:
			sprintf( handle, "hdesc %p", hnd );
			break;

		default:
			strcpy( handle, "unknown handle" );
			break;
	}

	ret = SQLGetDiagField( type, hnd, 0, SQL_DIAG_NUMBER,
		&count, 0, NULL );

	if ( !SQL_SUCCEEDED( ret ))
	{
		out_win->insertLineLimited( "SQLGetDiagField( SQL_DIAG_NUMBER ) failed" );
		return;
	}

	for ( i = 1; i <= count; i ++ )
	{
		SQLCHAR sqlstate[ 6 ];
		SQLINTEGER native;
		SQLCHAR message_text[ SQL_MAX_MESSAGE_LENGTH ];
		char txt[ SQL_MAX_MESSAGE_LENGTH + 50 ];

		ret = SQLGetDiagRec( type, hnd, i, sqlstate,
			&native, message_text, sizeof( message_text ),
			NULL );

		if ( !SQL_SUCCEEDED( ret ))
	    {
			out_win->insertLineLimited( "SQLGetDiagRec() failed" );
			return;
		}

		sprintf( txt, "Diag(%s):%s:%d:%s",
			handle, sqlstate, native, message_text );

		out_win->insertLineLimited( txt );
	}
}

OdbcTest::OdbcTest( QWidget *parent, const char *name )
    : QMainWindow( parent, name )
{
    QPopupMenu *file = new QPopupMenu( this );
	int id;

    file->insertItem( "E&xit", qApp, SLOT(quit()), CTRL+Key_Q );

    QPopupMenu* diag = new QPopupMenu( this );
    diag->insertItem( "SQLGetDiag&Rec...", this, SLOT(sqlgetdiagrec()) );
    diag->insertItem( "SQLGetDiag&Field...", this, SLOT(sqlgetdiagfield()) );
    diag->insertItem( "SQL&Error...", this, SLOT(sqlerror()) );
    diag->insertSeparator();
    id = diag->insertItem( "Errors &All", this, SLOT(errorall()) );
	diag->setItemEnabled( id, FALSE );

    QPopupMenu* env = new QPopupMenu( this );
    env->insertItem( "SQL&AllocEnv...", this, SLOT(sqlallocenv()) );
    env->insertItem( "SQL&AllocHandle...", this, SLOT(sqlallochandle()) );
    env->insertItem( "SQLDataS&ources...", this, SLOT(sqldatasources()) );
    env->insertItem( "S&QLDrivers...", this, SLOT(sqldrivers()) );
    env->insertItem( "SQL&FreeEnv...", this, SLOT(sqlfreeenv()) );
    env->insertItem( "SQLF&reeHandle...", this, SLOT(sqlfreehandle()) );
    env->insertItem( "SQLE&ndTran...", this, SLOT(sqlendtran()) );
    env->insertItem( "SQL&Transact...", this, SLOT(sqltransact()) );
    env->insertSeparator();
    id = env->insertItem( "Data &Sources All", this, SLOT(datasourcesall()) );
	env->setItemEnabled( id, FALSE );
    id = env->insertItem( "Dri&vers All", this, SLOT(driversall()) );
	env->setItemEnabled( id, FALSE );

    QPopupMenu* conn = new QPopupMenu( this );
    conn->insertItem( "SQL&AllocConnect...", this, SLOT(sqlallocconnect()) );
    conn->insertItem( "SQL&BrowseConnect...", this, SLOT(sqlbrowseconnect()) );
    conn->insertItem( "SQL&Connect...", this, SLOT(sqlconnect()) );
    conn->insertItem( "SQLD&riverConnect...", this, SLOT(sqldriverconnect()) );
    conn->insertItem( "SQL&Disconnect...", this, SLOT(sqldisconnect()) );
    conn->insertItem( "SQL&FreeConnect...", this, SLOT(sqlfreeconnect()) );
    conn->insertItem( "S&QLGetFunctions...", this, SLOT(sqlgetfunctions()) );
    conn->insertItem( "SQLGet&Info...", this, SLOT(sqlgetinfo()) );
    conn->insertItem( "SQLN&ativeSQL...", this, SLOT(sqlnativesql()) );
	conn->setItemEnabled( id, FALSE );
    conn->insertSeparator();
    conn->insertItem( "F&ull Connect...", this, SLOT(fullconnect()), CTRL+Key_F );
    id = conn->insertItem( "Fu&ll Disconnect", this, SLOT(fulldisconnect()),
CTRL+Key_D );
	conn->setItemEnabled( id, FALSE );
    conn->insertItem( "G&et Info All", this, SLOT(getinfoall()) );
    id = conn->insertItem( "Get &Functions All", this, SLOT(getfunctionsall()) );
	conn->setItemEnabled( id, FALSE );

    QPopupMenu* desc = new QPopupMenu( this );
    desc->insertItem( "SQLCop&yDesc...", this, SLOT(sqlcopydesc()) );
    desc->insertItem( "SQLGetDesc&Field...", this, SLOT(sqlgetdescfield()) );
    desc->insertItem( "SQLG&etDescRec...", this, SLOT(sqlgetdescrec()) );
    id = desc->insertItem( "SQLSetDescF&ield...", this, SLOT(sqlsetdescfield()) );
	desc->setItemEnabled( id, FALSE );
    id = desc->insertItem( "SQLS&etDescRec...", this, SLOT(sqlsetdescrec()) );
	desc->setItemEnabled( id, FALSE );
    desc->insertSeparator();
    id = desc->insertItem( "Get Descriptors All", this, SLOT(getdescriptorsall()) );
	desc->setItemEnabled( id, FALSE );

    QPopupMenu* stmt = new QPopupMenu( this );
    stmt->insertItem( "SQL&AllocStmt...", this, SLOT(sqlallocstmt()) );
    id = stmt->insertItem( "S&QLBindParam...", this, SLOT(sqlbindparam()) );
	stmt->setItemEnabled( id, FALSE );
    id = stmt->insertItem( "SQL&BindParameter...", this, SLOT(sqlbindparameter()) );
	stmt->setItemEnabled( id, FALSE );
    stmt->insertItem( "SQL&Cancel...", this, SLOT(sqlcancel()) );
    stmt->insertItem( "SQLCl&oseCursor...", this, SLOT(sqlclosecursor()) );
    stmt->insertItem( "SQLDe&scribeParam...", this, SLOT(sqldescribeparam()) );
    stmt->insertItem( "SQL&Execute...", this, SLOT(sqlexecute()) );
    stmt->insertItem( "SQLExec&Direct...", this, SLOT(sqlexecdirect()) );
    stmt->insertItem( "SQL&FreeStmt...", this, SLOT(sqlfreestmt()) );
    stmt->insertItem( "SQL&GetCursorName...", this, SLOT(sqlgetcursorname()) );
    stmt->insertItem( "SQL&NumParams...", this, SLOT(sqlnumparams()) );
    stmt->insertItem( "SQLPara&mData...", this, SLOT(sqlparamdata()) );
    stmt->insertItem( "SQLPa&ramOptions...", this, SLOT(sqlparamoptions()) );
    stmt->insertItem( "SQL&Prepare...", this, SLOT(sqlprepare()) );
    stmt->insertItem( "SQLP&utData...", this, SLOT(sqlputdata()) );
    stmt->insertItem( "SQ&LSetCursorName...", this, SLOT(sqlsetcursorname()) );
    stmt->insertSeparator();
    id = stmt->insertItem( "F&ill Param...", this, SLOT(fillparam()) );
	stmt->setItemEnabled( id, FALSE );
    id = stmt->insertItem( "Sho&w Param...", this, SLOT(showparam()) );
	stmt->setItemEnabled( id, FALSE );
    id = stmt->insertItem( "S&how Cursor Settings...", this, SLOT(showcursorsettings()) );
	stmt->setItemEnabled( id, FALSE );

    QPopupMenu* attr = new QPopupMenu( this );
    attr->insertItem( "SQLSet&StmtAttr...", this, SLOT(sqlsetstmtattr()) );
    attr->insertItem( "SQLGetS&tmtAttr...", this, SLOT(sqlgetstmtattr()) );
    attr->insertItem( "SQLSetStmt&Option...", this, SLOT(sqlsetstmtoption()) );
    attr->insertItem( "SQLGetStmtOpt&ion...", this, SLOT(sqlgetstmtoption()) );
    attr->insertSeparator();
    attr->insertItem( "SQLSetConnect&Attr...", this, SLOT(sqlsetconnectattr()) );
    attr->insertItem( "SQLGetC&onnectAttr...", this, SLOT(sqlgetconnectattr()) );
    attr->insertItem( "SQLSetConnectO&ption...", this, SLOT(sqlsetconnectoption()) );
    attr->insertItem( "SQLGetC&onnectOp&tion...", this, SLOT(sqlgetconnectoption()) );
    attr->insertSeparator();
    attr->insertItem( "SQLSet&EnvAttr...", this, SLOT(sqlsetenvattr()) );
    attr->insertItem( "SQLGetE&nvAttr...", this, SLOT(sqlgetenvattr()) );
    attr->insertSeparator();
    id = attr->insertItem( "Set &Cursor Attributes...", this, SLOT(setcursoratributes()) );
	attr->setItemEnabled( id, FALSE );

    QPopupMenu* results = new QPopupMenu( this );
    id = results->insertItem( "SQL&BindCol...", this, SLOT(sqlbindcol()) );
	results->setItemEnabled( id, FALSE );
    results->insertItem( "SQLBul&kOperations...", this, SLOT(sqlbulkoperations()) );
    results->insertItem( "S&QLColAttributes...", this, SLOT(sqlcolattributes()) );
    results->insertItem( "SQLColAttribute...", this, SLOT(sqlcolattribute()) );
    results->insertItem( "SQL&DescribeCol...", this, SLOT(sqldescribecol()) );
    results->insertItem( "SQLE&xtendedFetch...", this, SLOT(sqlextendedfetch()) );
    results->insertItem( "SQL&Fetch...", this, SLOT(sqlfetch()) );
    results->insertItem( "SQLFetc&hScroll...", this, SLOT(sqlfetchscroll()) );
    results->insertItem( "SQL&GetData...", this, SLOT(sqlgetdata()) );
    results->insertItem( "SQL&MoreResults...", this, SLOT(sqlmoreresults()) );
    results->insertItem( "SQL&NumResultsCols...", this, SLOT(sqlnumresultscols()) );
    results->insertItem( "SQL&RowCount...", this, SLOT(sqlrowcount()) );
    results->insertItem( "SQL&SetPos...", this, SLOT(sqlsetpos()) );
    results->insertItem( "SQLS&etScrollOptions...", this, SLOT(sqlsetscrolloptions()) );

	results->insertSeparator();
    id = results->insertItem( "B&ind Col All", this, SLOT(bindcolall()) );
	results->setItemEnabled( id, FALSE );
    id = results->insertItem( "Describe C&ol All", this, SLOT(describecolall()) );
	results->setItemEnabled( id, FALSE );
    id = results->insertItem( "Fetch &All", this, SLOT(fetchall()) );
	results->setItemEnabled( id, FALSE );
    results->insertItem( "Get Data A&ll", this, SLOT(getdataall()) );
    id = results->insertItem( "Sho&w Bound Cols", this, SLOT(showboundcols()) );
	results->setItemEnabled( id, FALSE );
    id = results->insertItem( "Display Rowset", this, SLOT(displayrowset()) );
	results->setItemEnabled( id, FALSE );

    QPopupMenu* catalog = new QPopupMenu( this );
    catalog->insertItem( "SQL&Columns...", this, SLOT(sqlcolumns()) );
    catalog->insertItem( "SQL&ColumnPrivileges...", this, SLOT(sqlcolumnprivileges()) );
    catalog->insertItem( "SQL&GetTypeInfo...", this, SLOT(sqlgettypeinfo()) );
    catalog->insertItem( "SQL&ForeignKeys...", this, SLOT(sqlforeignkeys()) );
    catalog->insertItem( "SQL&PrimaryKeys...", this, SLOT(sqlprimarykeys()) );
    catalog->insertItem( "SQLP&rocedures...", this, SLOT(sqlprocedures()) );
    catalog->insertItem( "SQLProc&edureColumns...", this, SLOT(sqlprocedurecolumns()) );
    catalog->insertItem( "SQLSpecia&lColumns...", this, SLOT(sqlspecialcolumns()) );
    catalog->insertItem( "SQL&Statistics...", this, SLOT(sqlstatistics()) );
    catalog->insertItem( "SQL&Tables...", this, SLOT(sqltables()) );
    catalog->insertItem( "SQLT&ablePrivileges...", this, SLOT(sqltableprivileges()) );

    QPopupMenu* datasources = new QPopupMenu( this );
    datasources->insertItem( "SQL&ManageDataSources...", this, SLOT(sqlmanagedatasources()) );
    datasources->insertItem( "SQL&RemoveDefaultDataSource...", this, SLOT(sqlremovedefaultdatasource()) );
    datasources->insertItem( "SQL&ConfigDataSource...", this, SLOT(sqlconfigdatasource()) );
    datasources->insertItem( "SQ&LCreatDataSource...", this, SLOT(sqlcreatedatasource()) );
    datasources->insertItem( "SQL&ValidDSN...", this, SLOT(sqlvaliddsn()) );
    datasources->insertItem( "SQLR&emoveDSNFromIni...", this, SLOT(sqlremovedsnfromini()) );
    datasources->insertItem( "SQLWriteDSNTo&Ini...", this, SLOT(sqlwritedsntoini()) );

    QPopupMenu* drivers = new QPopupMenu( this );
    id = drivers->insertItem( "SQLRemo&veDrivers...", this, SLOT(sqlremovedrivers()) );
	drivers->setItemEnabled( id, FALSE );
    id = drivers->insertItem( "S&QLConfigDrivers...", this, SLOT(sqlconfigdrivers()) );
	drivers->setItemEnabled( id, FALSE );
    id = drivers->insertItem( "SQLI&nstallDriver...", this, SLOT(sqlinstalldriver()) );
	drivers->setItemEnabled( id, FALSE );
    id = drivers->insertItem( "SQLI&nstallDriverEx...", this, SLOT(sqlinstalldriverex()) );
	drivers->setItemEnabled( id, FALSE );
    id = drivers->insertItem( "SQLGetInstalledDrivers...", this, SLOT(sqlgetinstalleddrivers()) );
	drivers->setItemEnabled( id, FALSE );

    QPopupMenu* drivermanager = new QPopupMenu( this );
    id = drivermanager->insertItem( "SQL&RemoveDriverManager...", this, SLOT(sqlremovedrivermanager()) );
	drivermanager->setItemEnabled( id, FALSE );
    id = drivermanager->insertItem( "SQL&InstallDriverManager...", this, SLOT(sqlinstalldrivermanager()) );
	drivermanager->setItemEnabled( id, FALSE );

    QPopupMenu* filedsn = new QPopupMenu( this );
    id = filedsn->insertItem( "SQL&ReadFileDSN...", this, SLOT(sqlreadfiledsn()) );
	filedsn->setItemEnabled( id, FALSE );
    id = filedsn->insertItem( "SQL&WriteFileDSN...", this, SLOT(sqlwritefiledsn()) );
	filedsn->setItemEnabled( id, FALSE );

    QPopupMenu* profilestrings = new QPopupMenu( this );
    id = profilestrings->insertItem( "SQL&WritePrivateProfileString...", 
		this, SLOT(sqlwriteprivateprofilestring()) );
	profilestrings->setItemEnabled( id, FALSE );
    id = profilestrings->insertItem( "SQL&GetPrivateProfileString...", 
		this, SLOT(sqlgetprivateprofilestring()) );
	profilestrings->setItemEnabled( id, FALSE );

    QPopupMenu* translator = new QPopupMenu( this );
    id = translator->insertItem( "SQL&InstallTranslator...", 
		this, SLOT(sqlinstalltranslator()) );
	translator->setItemEnabled( id, FALSE );
    id = translator->insertItem( "SQLI&nstallTranslatorEx...", 
		this, SLOT(sqlinstalltranslatorex()) );
	translator->setItemEnabled( id, FALSE );
    id = translator->insertItem( "SQL&RemoveTranslator...", 
		this, SLOT(sqlremovetranslator()) );
	translator->setItemEnabled( id, FALSE );
    id = translator->insertItem( "SQL&GetTranslator...", 
		this, SLOT(sqlgettranslator()) );
	translator->setItemEnabled( id, FALSE );

    QPopupMenu* configmode = new QPopupMenu( this );
    id = configmode->insertItem( "SQL&SetConfigMode...", 
		this, SLOT(sqlsetconfigmode()) );
	configmode->setItemEnabled( id, FALSE );
    id = configmode->insertItem( "SQL&GetConfigMode...", 
		this, SLOT(sqlgetconfigmode()) );
	configmode->setItemEnabled( id, FALSE );

    QPopupMenu* installer = new QPopupMenu( this );
    id = installer->insertItem( "&Data Sources...", datasources );
	installer->setItemEnabled( id, FALSE );
    id = installer->insertItem( "Dri&vers...", drivers );
	installer->setItemEnabled( id, FALSE );
    id = installer->insertItem( "Driver&Manager...", drivermanager );
	installer->setItemEnabled( id, FALSE );
    id = installer->insertItem( "&File DSN...", filedsn );
	installer->setItemEnabled( id, FALSE );
    id = installer->insertItem( "&Profile Strings...", profilestrings );
	installer->setItemEnabled( id, FALSE );
    id = installer->insertItem( "&Translator...", translator );
	installer->setItemEnabled( id, FALSE );
    id = installer->insertItem( "C&onfig Mode...", configmode );
	installer->setItemEnabled( id, FALSE );

    QPopupMenu* tools = new QPopupMenu( this );
    id = tools->insertItem( "&Options..", this, SLOT(options()) );
	tools->setItemEnabled( id, FALSE );
    id = tools->insertItem( "T&race..", this, SLOT(trace()) );
	tools->setItemEnabled( id, FALSE );
    tools->insertSeparator();
    tools->insertItem( "Manage Test &Sources...", this, SLOT(manage_test()) );
    tools->insertItem( "&Manage Auto Tests...", this, SLOT(manage_auto_test()) );
    tools->insertItem( "&Manage Test &Groups...", this, SLOT(manage_test_groups()) );
    tools->insertItem( "Run Auto &Tests...", this, SLOT(run_auto_tests()) );

    QPopupMenu *help = new QPopupMenu( this );
    id = help->insertItem( "ODBC Test &Help", this, SLOT(testhelp()) );
	help->setItemEnabled( id, FALSE );
    id = help->insertItem( "API A&PI Help", this, SLOT(apihelp()) );
	help->setItemEnabled( id, FALSE );
    help->insertItem( "About ODBC Test...", this, SLOT(about()) );

    menu = new QMenuBar( this );

    menu->insertItem( "&File", file );
    menu->insertItem( "Dia&g", diag );
    menu->insertItem( "E&nv", env );
    menu->insertItem( "&Conn", conn );
    menu->insertItem( "&Desc", desc );
    menu->insertItem( "&Stmt", stmt );
    menu->insertItem( "&Attr", attr );
    menu->insertItem( "&Results", results );
    menu->insertItem( "Cata&log", catalog );
    menu->insertItem( "&Installer", installer );
    menu->insertItem( "&Tools", tools );
    menu->insertSeparator();
    menu->insertItem( "&Help", help );
    menu->setSeparator( QMenuBar::InWindowsStyle );

    this->setGeometry( 0, 0, 500, 250 );

	split = new QSplitter( QSplitter::Vertical, this, "main" );
	split -> setOpaqueResize( FALSE );
	setCentralWidget( split );
    in_win = new QMultiLineEdit( split );
    out_win = new OutputWin( split );
	out_win -> setReadOnly( TRUE );
	out_win -> setMaxLines( 1000 );

	listHandle.setAutoDelete( TRUE );
}

void OdbcTest::apihelp()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::testhelp()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::about()
{
    QMessageBox::about( this, "ODBC Test",
			"This program is part of the unixODBC SDK.\n"
			"It can be used to test the ODBC API.\n"
			"Based on the ODBC test program supplied by Microsoft.\n"
			"Written by Nick Gorham." );
}

void OdbcTest::resizeEvent( QResizeEvent * )
{
    // label->setGeometry( 20, rect().center().y()-20, width()-40, 40 );
}

const char *OdbcTest::int_type_as_string( SQLINTERVAL itype )
{
	switch ( itype )
	{
   		case SQL_IS_YEAR:
			return "SQL_IS_YEAR";

    	case SQL_IS_MONTH:
			return "SQL_IS_MONTH";

    	case SQL_IS_DAY:
			return "SQL_IS_DAY";

    	case SQL_IS_HOUR:
			return "SQL_IS_HOUR";

    	case SQL_IS_MINUTE:
			return "SQL_IS_MINUTE";

    	case SQL_IS_SECOND:
			return "SQL_IS_SECOND";

    	case SQL_IS_YEAR_TO_MONTH:
			return "SQL_IS_YEAR_TO_MONTH";

    	case SQL_IS_DAY_TO_HOUR :
			return "SQL_IS_DAY_TO_HOUR";

    	case SQL_IS_DAY_TO_MINUTE:
			return "SQL_IS_DAY_TO_MINUTE";

    	case SQL_IS_DAY_TO_SECOND:
			return "SQL_IS_DAY_TO_SECOND";

    	case SQL_IS_HOUR_TO_MINUTE:
			return "SQL_IS_HOUR_TO_MINUTE";

    	case SQL_IS_HOUR_TO_SECOND:
			return "SQL_IS_HOUR_TO_SECOND";

    	case SQL_IS_MINUTE_TO_SECOND:
			return "SQL_IS_MINUTE_TO_SECOND";
	}

    return "Unknown Type";
}	

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
	OdbcTest m;

    a.setMainWidget( &m );

	m.show();
    return a.exec();
}

Handle::Handle( int t, SQLHANDLE h, QString desc, SQLHANDLE stmt ) 
{ 
	type = t; 
	handle = h; 

    description = desc;
    stmt_handle = stmt;

    if ( stmt )
    {
        implicit = 1;
    }
    else
    {
        implicit = 0;
    }

    handle_list = NULL;

	bookmark_ptr = new char[ 256 ];
	param_bind_offset_ptr = &param_bind_offset;
	param_opt_ptr = &param_opt;
	param_status_ptr = &param_status;
	params_processed_ptr = &params_processed;
	row_bind_offset_ptr = &row_bind_offset;
	row_operation_ptr = &row_operation;
	row_status_ptr = &row_status;
	rows_fetched_ptr = &rows_fetched;

    row_array_size = 0;
    param_array_size = 0;
}

Handle::Handle( int t, SQLHANDLE h, QList<Handle> &list ) 
{ 
	type = t; 
	handle = h; 

    implicit = 0;
    description = "";
    stmt_handle = SQL_NULL_HANDLE;
    handle_list = &list;

	bookmark_ptr = new char[ 256 ];
	param_bind_offset_ptr = &param_bind_offset;
	param_opt_ptr = &param_opt;
	param_status_ptr = &param_status;
	params_processed_ptr = &params_processed;
	row_bind_offset_ptr = &row_bind_offset;
	row_operation_ptr = &row_operation;
	row_status_ptr = &row_status;
	rows_fetched_ptr = &rows_fetched;

    row_array_size = 0;
    param_array_size = 0;

    /*
     * extract any desc handles associated with a statement handle 
     */

    if ( type == SQL_HANDLE_STMT )
    {
        SQLRETURN ret;
        SQLHANDLE desc;

        ret = SQLGetStmtAttr( h, SQL_ATTR_APP_PARAM_DESC, &desc, 0, NULL );
        if ( SQL_SUCCEEDED( ret ))
		    list.append( new Handle( SQL_HANDLE_DESC, desc, "Implicit APD", h ));

        ret = SQLGetStmtAttr( h, SQL_ATTR_APP_ROW_DESC, &desc, 0, NULL );
        if ( SQL_SUCCEEDED( ret ))
		    list.append( new Handle( SQL_HANDLE_DESC, desc, "Implicit ARD", h ));

        ret = SQLGetStmtAttr( h, SQL_ATTR_IMP_PARAM_DESC, &desc, 0, NULL );
        if ( SQL_SUCCEEDED( ret ))
		    list.append( new Handle( SQL_HANDLE_DESC, desc, "Implicit IPD", h ));

        ret = SQLGetStmtAttr( h, SQL_ATTR_IMP_ROW_DESC, &desc, 0, NULL );
        if ( SQL_SUCCEEDED( ret ))
		    list.append( new Handle( SQL_HANDLE_DESC, desc, "Implicit IRD", h ));
    }
}

Handle::Handle( Handle &e ) 
{ 
	type = e.type; 
	handle = e.handle; 

    implicit = 0;
    description = "";
    stmt_handle = SQL_NULL_HANDLE;

	bookmark_ptr = new char[ 256 ];
	param_bind_offset_ptr = &param_bind_offset;
	param_opt_ptr = &param_opt;
	param_status_ptr = &param_status;
	params_processed_ptr = &params_processed;
	row_bind_offset_ptr = &row_bind_offset;
	row_operation_ptr = &row_operation;
	row_status_ptr = &row_status;
	rows_fetched_ptr = &rows_fetched;

    row_array_size = 0;
    param_array_size = 0;
};

Handle::~Handle()
{
    /*
     * remove the implicit descriptors if a stmt is removed
     */

    if ( type == SQL_HANDLE_STMT )
    {
	    Handle *hand, *match = NULL;
	    char txt[ 128 ];
        int changed = 0;

        do
        {
            changed = 0;
            
            for ( hand=handle_list -> first();
		        hand != 0; 
		        hand=handle_list -> next() )
	        {
                if ( hand->getType() == SQL_HANDLE_DESC && 
                        hand->getStmtHandle() == handle )
                {
                    handle_list -> remove( hand );

                    changed = 1;
                    break;
                }
		    }
        }
        while( changed );
	}
}
