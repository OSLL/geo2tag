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
 * $Id: tools.cpp,v 1.6 2003/12/01 16:37:17 lurcher Exp $
 *
 * $Log: tools.cpp,v $
 * Revision 1.6  2003/12/01 16:37:17  lurcher
 *
 * Fix a bug in SQLWritePrivateProfileString
 *
 * Revision 1.5  2003/11/13 15:12:54  lurcher
 *
 * small change to ODBCConfig to have the password field in the driver
 * properties hide the password
 * Make both # and ; comments in ini files
 *
 * Revision 1.4  2003/09/29 08:56:10  lurcher
 *
 * Fix warning dialogs
 * Check for headers before using them
 *
 * Revision 1.3  2002/01/21 18:00:51  lurcher
 *
 * Assorted fixed and changes, mainly UNICODE/bug fixes
 *
 * Revision 1.2  2001/12/04 16:46:19  lurcher
 *
 * Allow the Unix Domain Socket to be set from the ini file (DSN)
 * Make the DataManager browser work with drivers that don't support
 * SQLRowCount
 * Make the directory selection from odbctest work simplier
 *
 * Revision 1.1.1.1  2001/10/17 16:40:31  lurcher
 *
 * First upload to SourceForge
 *
 * Revision 1.11  2001/10/01 17:24:53  nick
 *
 * Assorted fixes
 *
 * Revision 1.10  2001/05/31 16:05:55  nick
 *
 * Fix problems with postgres closing local sockets
 * Make odbctest build with QT 3 (it doesn't work due to what I think are bugs
 * in QT 3)
 * Fix a couple of problems in the cursor lib
 *
 * Revision 1.9  2001/05/09 11:56:47  nick
 *
 * Add support for libtool 1.4
 *
 * Revision 1.8  2001/04/17 16:29:39  nick
 *
 * More checks and autotest fixes
 *
 * Revision 1.7  2001/04/17 12:57:11  nick
 *
 * Extra for AutoTest
 *
 * Revision 1.6  2001/04/16 22:35:10  nick
 *
 * More tweeks to the AutoTest code
 *
 * Revision 1.5  2001/04/16 15:41:24  nick
 *
 * Fix some problems calling non existing error funcs
 *
 * Revision 1.4  2001/04/14 19:00:34  nick
 *
 * Add support for ODBC Test handles, and catch a few problems with
 * empty Gator.ini
 *
 * Revision 1.3  2001/04/14 10:42:03  nick
 *
 * Extra work on the autotest feature of odbctest
 *
 * Revision 1.2  2001/04/12 17:43:36  nick
 *
 * Change logging and added autotest to odbctest
 *
 * Revision 1.1.1.1  2000/09/04 16:42:53  nick
 * Imported Sources
 *
 * Revision 1.1  2000/05/04 17:04:48  ngorham
 *
 * Initial commit
 *
 *
 **********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ltdl.h>
#include <autotest.h>
#include <qapplication.h>
#include <qmessagebox.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include <qlistview.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qheader.h>
#include "autotest.h"
#include "tools.h"
#include "odbctest.h"
#include "ini.h"

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#define setqbit(lpa, pos)	\
	(lpa[((pos) / CQBITS)] |= (1 << ((pos) - (CQBITS * ((pos) / CQBITS)))))
#define SETBIT(p1,p2) setqbit(p1,(p2)-1)

class prop
{
  public:
    prop( const char *sname = "", const char *svalue = "" )
    {
        _name = sname;
        _value = svalue;
    }

    const char *name( void )
    {
        return _name;
    }

    const char *value( void )
    {
        return _value;
    }

  private:
    QString _name, _value;
};

class section
{
  public:
    section( const char *sname = ""  )
    {
        name = sname;
        entries.clear();
    }

    void append( prop *ent )
    {   
        entries.append( ent );
    }

    const char *get_section( void )
    {
        return name;
    }

    prop *first( void )
    {
        return entries.first();
    }

    prop *next( void )
    {
        return entries.next();
    }

    void remove( prop *p )
    {
        entries.remove( p );
    }

    void clear( void )
    {
        entries.clear();
    }

  private:
    QString name;
    QList <prop> entries;
};

QList<section> ini_list;

//
// time func
// 

const char *get_time_str( void )
{
    static char str[ 128 ];
    time_t tim;
    struct tm *tm;

    tim = time( NULL );

    tm = localtime( &tim ); 

    sprintf( str, "%02d:%02d:%02d", tm -> tm_hour, tm -> tm_min, tm -> tm_sec );

    return str;
}

//
// callback function
//

static OdbcTest *static_odbctest;

static void post_to_odbctest( lpSERVERINFO lps )
{
    lps -> szBuff[ lps -> cBuff ] = '\0';

    if ( lps -> fScreen )
    {
        static_odbctest -> out_win -> insertLineLimited( lps -> szBuff );
    }
    if ( lps -> fLog )
    {
        FILE *fp = fopen( lps -> szLogFile, "a+" );
        if ( fp )
        {
            fputs( lps -> szBuff, fp );
            fputs( "\n", fp );
            fclose( fp );
        }
    }

    lps -> cBuff = 0;
}

static void print_to_odbctest( lpSERVERINFO lps, char *str, int log )
{
    static_odbctest -> out_win -> insertLineLimited( str );
    if ( log && lps -> fLog )
    {
        FILE *fp = fopen( lps -> szLogFile, "a+" );
        if ( fp )
        {
            fputs( str, fp );
            fputs( "\n", fp );
            fclose( fp );
        }
    }
}

static int show_message_box( int style, const char *title, const char *msg )
{
    int button0, button1, button2;

    button0 = QMessageBox::NoButton;
    button1 = QMessageBox::NoButton;
    button2 = QMessageBox::NoButton;

    switch ( style & (MB_OK | MB_ABORTRETRYIGNORE | MB_OKCANCEL | MB_RETRYCANCEL | MB_YESNO | MB_YESNOCANCEL) )
    {
      case MB_ABORTRETRYIGNORE:
        button0 = QMessageBox::Abort;
        button1 = QMessageBox::Retry;
        button2 = QMessageBox::Ignore;
        break;
      case MB_OKCANCEL:
        button0 = QMessageBox::Ok;
        button1 = QMessageBox::Cancel;
        break;
      case MB_RETRYCANCEL:
        button0 = QMessageBox::Retry;
        button1 = QMessageBox::Cancel;
        break;
      case MB_YESNO:
        button0 = QMessageBox::Yes;
        button1 = QMessageBox::No;
        break;
      case MB_YESNOCANCEL:
        button0 = QMessageBox::Yes;
        button1 = QMessageBox::No;
        button2 = QMessageBox::Cancel;
        break;
      default:
        button0 = QMessageBox::Ok;
        break;
    }

    switch ( style & (MB_DEFBUTTON1 | MB_DEFBUTTON2 | MB_DEFBUTTON3 ) ) 
    {
      case MB_DEFBUTTON1:
      default:
        button0 |= QMessageBox::Default;
        break;
      case MB_DEFBUTTON2:
        button1 |= QMessageBox::Default;
        break;
      case MB_DEFBUTTON3:
        button2 |= QMessageBox::Default;
        break;
    }

    QMessageBox::Icon icon = QMessageBox::NoIcon ;
    switch ( style & (MB_ICONINFORMATION | MB_ICONQUESTION | MB_ICONSTOP ) ) 
    {
      case MB_ICONINFORMATION :
        icon = QMessageBox::Information;
        break;
      case MB_ICONQUESTION:
        icon = QMessageBox::Warning;
        break;
      case MB_ICONSTOP:
        icon = QMessageBox::Critical;
        break;
      default:
        icon = QMessageBox::NoIcon;
        break;
    }

    QMessageBox mb( title,
                    msg,
                    icon,
                    button0,
                    button1,
                    button2 );

    return mb.exec();
}

extern "C" {
static int callback_function( lpSERVERINFO lps, BOOL fForce,
        BOOL fMsgBox, UINT style, LPTSTR szTitle, const char *msg )
{
    if ( fMsgBox )
    {
        return show_message_box( style, szTitle, msg );
    }
    else
    {
        int line_out = 0;
        /*
         * copy from msg to szBuff, looking for \n
         */

        while( *msg )
        {
            if ( *msg == '\n' )
            {
                post_to_odbctest( lps );
            }
            else if ( isprint( *msg ))
            {
                if ( lps -> cBuff < sizeof( lps -> szBuff ) - 1 )
                {
                    lps -> szBuff[ lps -> cBuff ] = *msg;
                    lps -> cBuff ++;
                }
            }
            msg ++;
        }

        if ( fForce && lps -> cBuff > 0 )
        {
            post_to_odbctest( lps );
        }
    }

    return 0;
}
}

//
// Find a section
//

section *find_section( const char *txt )
{
    for ( section *ent = ini_list.first(); ent != 0; ent = ini_list.next())
    {
        if ( strcmp( ent -> get_section(), txt ) == 0 )
        {
            return ent;
        }
    }

    return NULL;
}

//
// Put information back into ini file
//

void replace_ini_list( OdbcTest *parent )
{
	HINI hIni;
	char szPropertyValue[INI_MAX_PROPERTY_VALUE+1];
    char szINI[ODBC_FILENAME_MAX+1];

    sprintf( szINI, "%s/%s", SYSTEM_FILE_PATH, "Gator.ini" ); 
	if ( iniOpen( &hIni, szINI, "#;", '[', ']', '=', TRUE ) != INI_ERROR )
	{
        for ( section *ent = ini_list.first(); ent != 0; ent = ini_list.next())

        {
            iniObjectSeekSure( hIni, (char*)ent -> get_section());
            iniObjectDelete( hIni );

            iniObjectSeekSure( hIni, (char*)ent -> get_section());

            for ( prop *prop = ent->first(); prop != 0; prop = ent -> next())
            {
                iniPropertyInsert( hIni, (char*)prop -> name(), (char*)prop -> value());
            }

            iniCommit( hIni );
        }
	}
    else
    {
        char msg[ 256 ];

        sprintf( msg, "Unable to open INI file (%s)", szINI );
        QMessageBox::critical( parent, "OdbcTest", msg );
    }

    ini_list.clear();
}

void remove_section( OdbcTest *parent, section *section )
{
	HINI hIni;
	char szPropertyValue[INI_MAX_PROPERTY_VALUE+1];
    char szINI[ODBC_FILENAME_MAX+1];

    sprintf( szINI, "%s/%s", SYSTEM_FILE_PATH, "Gator.ini" ); 
	if ( iniOpen( &hIni, szINI, "#;", '[', ']', '=', TRUE ) != INI_ERROR )
	{
        iniObjectSeekSure( hIni, (char*)section -> get_section());
        iniObjectDelete( hIni );
        iniCommit( hIni );
	}
    else
    {
        char msg[ 256 ];

        sprintf( msg, "Unable to open INI file (%s)", szINI );
        QMessageBox::critical( parent, "OdbcTest", msg );
    }
}

//
// extract the information from the ini file
//

void init_ini_list( OdbcTest *parent )
{
	HINI hIni;
	char szObject[INI_MAX_OBJECT_NAME+1];
	char szPropertyName[INI_MAX_PROPERTY_NAME+1];
	char szPropertyValue[INI_MAX_PROPERTY_VALUE+1];
    char szINI[ODBC_FILENAME_MAX+1];

    ini_list.clear();

    sprintf( szINI, "%s/%s", SYSTEM_FILE_PATH, "Gator.ini" ); 
	if ( iniOpen( &hIni, szINI, "#;", '[', ']', '=', TRUE ) != INI_ERROR )
	{
		iniObjectFirst( hIni );
		while ( iniObjectEOL( hIni ) == FALSE )
		{
			szObject[0]         = '\0';
			szPropertyName[0] 	= '\0';
			szPropertyValue[0] 	= '\0';
			iniObject( hIni, szObject );
			iniPropertyFirst( hIni );

            section *s = new section( szObject );

            ini_list.append( s );

            while ( iniPropertyEOL( hIni ) == FALSE )
            {
                iniProperty( hIni, szPropertyName );
                iniValue( hIni, szPropertyValue );

                prop *p = new prop( szPropertyName, szPropertyValue );

                s -> append( p );

                iniPropertyNext( hIni );
            }
			iniObjectNext( hIni );
		}
	}
	else
	{
        FILE *fp;

        //
        // empty list
        //
        sprintf( szINI, "%s/%s", SYSTEM_FILE_PATH, "Gator.ini" ); 
        fp = fopen( szINI, "w+" );
        if ( fp )
        {
            //
            // Create the sections
            //
            fprintf( fp, "[SQL_DRIVERS]\n\n[GROUPS]\n\n" );
            fclose( fp );
        }
        else
        {
            char msg[ 256 ];

            sprintf( msg, "Unable to open INI file (%s)", szINI );
            QMessageBox::critical( parent, "OdbcTest", msg );
        }
	}

    //
    // if empty create base groups
    //

    section *s = find_section( "SQL_DRIVERS" );
    if ( !s )
    {
        section *s = new section( "SQL_DRIVERS" );
        ini_list.append( s );
    }
    s = find_section( "GROUPS" );
    if ( !s )
    {
        section *s = new section( "GROUPS" );
        ini_list.append( s );
    }
    s = find_section( "Auto Tests" );
    if ( !s )
    {
        section *s = new section( "Auto Tests" );
        ini_list.append( s );
    }
}

void fill_dsn_list( OdbcTest *parent, QComboBox *box )
{
	SQLHANDLE henv;
	SQLRETURN ret;
	SQLCHAR dsn_txt[ 128 ];

    box -> clear();

	/*
	 * allocate a handle if required
	 */

    ret = SQLAllocEnv( &henv );

    if ( !SQL_SUCCEEDED( ret ))
    {
        parent -> out_win -> insertLineLimited( "SQLAllocHandle fails allocating a environment handle" );
        return;
    }

	ret = SQLDataSources( henv, 
		SQL_FETCH_FIRST,
		dsn_txt,
		sizeof( dsn_txt ),
		NULL,
		NULL,
		0,
		NULL );

	while ( SQL_SUCCEEDED( ret ))
	{
		box -> insertItem(( char * ) dsn_txt );
		ret = SQLDataSources( henv, 
			SQL_FETCH_NEXT,
			dsn_txt,
			sizeof( dsn_txt ),
			NULL,
			NULL,
			0,
			NULL );
	}

	/*
	 * remove the temp henv
	 */

    SQLFreeEnv( henv );
}

void set_dsn_list( QComboBox *box, const char *str )
{
    int index;
    QString qstr;

    for ( index = 0; index < box->count(); index ++ )
    {
        qstr = box->text( index );
        if ( qstr == str )
        {
            box->setCurrentItem( index );
            break;
        }
    }
}

void dNewSource::Ok()
{
    QString name = source -> text();
    prop *p;

    //
    // check if its allready there
    //
    //
    section *s = find_section( "SQL_DRIVERS" );
    if ( s )
    {
        for ( p = s->first(); p != 0; p = s -> next())
        {
            if ( strcmp( p -> name(), name ) == 0 )
            {
                char msg[ 128 ];

                sprintf( msg, "Source (%s) already defined", name.ascii());
                QMessageBox::critical( this, "OdbcTest", msg );
                return;
            }
        }

        p = new prop( name.ascii(), "Installed" );

        s -> append( p );

        parent_test -> test_source -> insertItem( p -> name());
        parent_test -> Activated( p -> name());
    }
}

dNewSource::dNewSource( OdbcTest *parent, QString name, dManageTest *ptest )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;
    parent_test = ptest;

    cancel = new QPushButton( "Close", this );
    cancel->setGeometry( 200,50, 70,25 );

    ok = new QPushButton( "Ok", this );
    ok->setGeometry( 110,50, 70,25 );

	source = new QLineEdit( this, "Test Source" );
	source -> setGeometry( 100, 20, 250, 20 );
    source -> setMaxLength( 128 );

	l_dsn = new QLabel( "Test Source:", this );
    l_dsn -> setGeometry( 10, 20, 90, 20 );

    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
}

dNewSource::~dNewSource()
{
    delete cancel;
    delete ok;
    delete source;
    delete l_dsn;
}

void dManageTest::Activated( const QString &str )
{
    section *s = find_section( str );

    if ( s )
    {
        for ( prop *prop = s->first(); prop != 0; prop = s -> next())
        {
            if ( strcmp( prop -> name(), "SERVER0" ) == 0 )
            {
                set_dsn_list( dsn, prop->value());
            }
            else if ( strcmp( prop -> name(), "LOGIN0" ) == 0 )
            {
                uid -> setText( prop -> value());
            }
            else if ( strcmp( prop -> name(), "PASSWORD0" ) == 0 )
            {
                pwd -> setText( prop -> value());
            }
            else if ( strcmp( prop -> name(), "KEYWORDS" ) == 0 )
            {
                kw -> setText( prop -> value());
            }
        }
    }
    else
    {
        uid -> clear();
        pwd -> clear();
        kw -> clear();
    }
}

void dManageTest::Activated( int val )
{
    if ( test_source -> count() > 0 )
    {
    	Activated( test_source -> text( val ));
    }
}

void dManageTest::Ok()
{
    //
    // find driver name
    // 
    QString driver = test_source -> currentText();

	if ( !driver )
	{
		return;
	}

    //
    // Get section
    // 

    section *s = find_section( driver.ascii());
    if ( s )
    {
        //
        // remove all its entries
        //
        s -> clear();
    }
    else
    {
        s = new section( driver.ascii());
        ini_list.append( s );
    }

    //
    // Add the entries
    //
    
    prop *p = new prop( "SERVER0", dsn -> currentText());
    s -> append( p );
    p = new prop( "LOGIN0", uid -> text());
    s -> append( p );
    p = new prop( "PASSWORD0", pwd -> text());
    s -> append( p );
    p = new prop( "KEYWORDS", kw -> text());
    s -> append( p );
}

void dManageTest::NewSource()
{
	dNewSource *dlg = new dNewSource( this -> odbctest, "New Test Sources", this );

	dlg -> exec();

	delete dlg;

    //
    // fill up combo box
    //
    
    test_source -> clear();
    
    section *s = find_section( "SQL_DRIVERS" );
    int last = 0;

    if ( s )
    {
        prop *p;

        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            test_source -> insertItem( p -> name());
            last ++;
        }
    }
    Activated( last - 1 );
    test_source -> setCurrentItem( last - 1 );
}

void dManageTest::DelSource()
{
    if ( test_source -> count() == 0 )
    {
        return;
    }

    QString driver = test_source -> currentText();

    char msg [128];

    sprintf( msg, "Delete the test source %s ?", driver.ascii());

    if ( QMessageBox::information( this, "OdbcTest",
                msg,
                "&Delete", "&Cancel", 0,
                0, 1 ) == 0 )
    {
        //
        // Remove it from the SQL_DRIVERS section
        //
        section *s = find_section( "SQL_DRIVERS" );

        if ( s )
        {
            prop *p;

            for ( p = s -> first(); 
                    p != 0; 
                    p = s -> next())
            {
                if ( strcmp( p -> name(), driver.ascii()) == 0 )
                {
                    s -> remove( p );
                    break;
                }
            }
        }

        //
        // remove the section
        //
        s = find_section( driver.ascii());
        if ( s )
        {
            ini_list.removeRef( s );
            remove_section( odbctest, s );
        }

        //
        // fill up combo box
        //
        
        test_source -> clear();
        
        s = find_section( "SQL_DRIVERS" );

        if ( s )
        {
            prop *p;

            for ( p = s -> first(); 
                    p != 0; 
                    p = s -> next())
            {
                test_source -> insertItem( p -> name());
            }
        }
        Activated( 0 );
        test_source -> setCurrentItem( 0 );
    }
}

dManageTest::dManageTest( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    init_ini_list( parent );

    close = new QPushButton( "Close", this );
    close->setGeometry( 270,80, 70,25 );

    nw = new QPushButton( "New", this );
    nw->setGeometry( 270,110, 70,25 );

    del = new QPushButton( "Delete", this );
    del->setGeometry( 270,140, 70,25 );

	test_source = new QComboBox( FALSE, this, "test Source" );
	test_source -> setGeometry( 100, 20, 250, 20 );

	l_ts = new QLabel( "Test Source:", this );
    l_ts -> setGeometry( 10, 20, 80, 20 );

	param = new QButtonGroup( "Connect Parameters", this );
    param -> setGeometry( 10, 60, 240, 125 );

	dsn = new QComboBox( FALSE, this, "dsn" );
	dsn -> setGeometry( 80, 90, 150, 20 );

	l_dsn = new QLabel( "DSN:", this );
    l_dsn -> setGeometry( 20, 90, 60, 20 );

	uid = new QLineEdit( this, "uid" );
	uid -> setGeometry( 80, 120, 150, 20 );
    uid -> setMaxLength( 128 );

	l_uid = new QLabel( "UID:", this );
    l_uid -> setGeometry( 20, 120, 60, 20 );

	pwd = new QLineEdit( this, "pwd" );
	pwd -> setGeometry( 80, 150, 150, 20 );
    pwd -> setMaxLength( 128 );

	l_pwd = new QLabel( "PWD:", this );
    l_pwd -> setGeometry( 20, 150, 60, 20 );

	kw = new QLineEdit( this, "Keywords" );
	kw -> setGeometry( 80, 200, 250, 20 );
    kw -> setMaxLength( 128 );

	l_kw = new QLabel( "Keywords:", this );
    l_kw -> setGeometry( 10, 200, 60, 20 );

    connect( close, SIGNAL(clicked()), SLOT(Ok()) );
    connect( close, SIGNAL(clicked()), SLOT(accept()) );

    connect( nw, SIGNAL(clicked()), SLOT(NewSource()) );

    connect( del, SIGNAL(clicked()), SLOT(DelSource()) );

    //
    // fill up combo box
    //
    
    section *s = find_section( "SQL_DRIVERS" );

    if ( s )
    {
        prop *p;

        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            test_source -> insertItem( p -> name());
        }
    }

    //
    // fill the list of DSN's
    //

    fill_dsn_list( odbctest, dsn );

    //
    // set up other fields
    //
    Activated( 0 );
	connect( test_source, SIGNAL(activated(const QString &)), 
            this, SLOT( Activated(const QString &)));
}

dManageTest::~dManageTest()
{
    replace_ini_list( odbctest );
	delete close;
	delete nw;
	delete del;
    delete test_source;
    delete dsn;
    delete l_dsn;
    delete l_ts;
    delete pwd;
    delete l_pwd;
    delete kw;
    delete l_kw;
    delete param;
}

void dManageAutoTest::Ok()
{
}

void dManageAutoTest::SetPath( QString &qs )
{
    SetPath( qs.ascii());
}

void dManageAutoTest::SetPath( const char *path )
{
    s_from -> setText( path );

    //
    // extract any libs from the path
    //
   
    lib_list -> clear();

    QDir q_d( path );
    if ( q_d.exists())
    {
        q_d.setFilter( QDir::Files | QDir::Hidden );
        for ( int i=0; i < q_d.count(); i++ )
        {
            lib_list -> insertItem( q_d[i] );
        }
    }
}

void dManageAutoTest::Add()
{
    int index = lib_list -> currentItem();

    if ( index >= 0 )
    {
        QListBoxItem *lbi = lib_list -> item( index );
        const char *name = lbi -> text();

        QDir q_d( s_from->text());

        QString path = q_d.filePath ( lbi->text());

        /*
         * initialize libtool
         */

        lt_dlinit();

        lt_dlhandle handle = lt_dlopen( path.ascii());

        if ( !handle )
        {
            char msg[ 256 ];

            sprintf( msg, "Unable to open file %s", lt_dlerror());
            QMessageBox::critical( odbctest, "OdbcTest", msg );
        }
        else
        {
            //
            // Try and extract the symbols
            //
            void *pfAutoTestDesc, *pfAutoTestFunc;
            BOOL (*pfAutoTestName)(LPSTR,UINT*);

            pfAutoTestName = (BOOL(*)(LPSTR,UINT*))lt_dlsym( handle, "AutoTestName" );
            pfAutoTestDesc = lt_dlsym( handle, "AutoTestDesc" );
            pfAutoTestFunc = lt_dlsym( handle, "AutoTestFunc" );


            if ( !pfAutoTestName &&
                    !pfAutoTestDesc && 
                    !pfAutoTestFunc )
            {
                char msg[ 256 ];

                sprintf( msg, "Could not find one or more of AutoTestName, AutoTestDesc, or AutoTestFunc in auto test\n%s", path.ascii());

                QMessageBox::critical( odbctest, "OdbcTest", msg );
                lt_dlclose( handle );
                return;
            }

            char test_name[ AUTO_MAX_TEST_NAME + 1 ];
            UINT count;

            if ( pfAutoTestName )
            {
                //
                // get the test name
                //
                if ( !pfAutoTestName( test_name, &count ))
                {
                    char msg[ 256 ];

                    sprintf( msg, "AutoTestName returned FALSE in %s", path.ascii());

                    QMessageBox::critical( odbctest, "OdbcTest", msg );
                    lt_dlclose( handle );
                    return;
                }
            }
            else
            {
                char msg[ 256 ];

                sprintf( msg, "AutoTestName not exported from %s", path.ascii());

                QMessageBox::critical( odbctest, "OdbcTest", msg );
                lt_dlclose( handle );
                return;
            }

            //
            // if we are here, we can add it
            //
            //
            section *s = find_section( test_name );

            if ( s )
            {
                char msg[ 256 ];

                sprintf( msg, "Auto test '%s'already installed", test_name );

                QMessageBox::information( odbctest, "OdbcTest", msg );
                lt_dlclose( handle );
                return;
            }

            char number[ 64 ];
            s = find_section( "Auto Tests" );

            sprintf( number, "%d", count );
            prop *p = new prop( test_name, number );
            s -> append( p );

            s = new section( test_name );
            ini_list.append( s );

            p = new prop( "DLL", path.ascii());
            s -> append( p );

            lt_dlclose( handle );

            s = find_section( "Auto Tests" );

            test_list -> clear();

            if ( s )
            {
                prop *p;

                for ( p = s -> first(); 
                        p != 0; 
                        p = s -> next())
                {
                    test_list -> insertItem( p -> name());
                }
            }
            test_list -> setCurrentItem( 0 );

            ListSelect( test_name );
        }
    }
}

void dManageAutoTest::From()
{
	QFileDialog *dlg = new QFileDialog( this, "fred", TRUE );

    dlg -> setMode( QFileDialog::Directory );

    if ( dlg -> exec() == QDialog::Accepted )
    {
        QString result = dlg -> selectedFile();
        SetPath( result );
    }

	delete dlg;
}

void dManageAutoTest::ListSelect( const QString &name )
{
    section *s = find_section( name.ascii());

    if ( s )
    {
        s_name -> setText( name.ascii());
        s_lib -> setText( "" );

        for ( prop *prop = s->first(); prop != 0; prop = s -> next())
        {
            if ( strcmp( prop -> name(), "DLL" ) == 0 )
            {
                s_lib -> setText( prop->value());
            }
        }
    }
    else
    {
        s_name -> setText( "" );
        s_lib -> setText( "" );
    }
}

void dManageAutoTest::Remove()
{
  int index = test_list -> currentItem();

  if ( index >= 0 )
  {
    QString autotest = test_list->text(index);

    //
    // remove it from the Auto Test section
    //

    section *s = find_section( "Auto Tests" );

    if ( s )
    {
        prop *p;

        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            if ( strcmp( p -> name(), autotest.ascii()) == 0 )
            {
                s -> remove( p );
                break;
            }
        }
    }

    //
    // remove the section
    //
    
    s = find_section( autotest );
    if ( s )
    {
        ini_list.removeRef( s );
        remove_section( odbctest, s );
    }

    //
    // update combo box
    //
    
    s = find_section( "Auto Tests" );

    s_name -> setText( "" );
    s_lib -> setText( "" );

    test_list -> clear();

    if ( s )
    {
        prop *p;
        int first = 1;

        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            test_list -> insertItem( p -> name());
            if ( first )
            {
                ListSelect( p -> name());
                first = 0;
            }
        }
    }
    test_list -> setCurrentItem( 0 );
  }
}

dManageAutoTest::dManageAutoTest( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    init_ini_list( parent );

    from = new QPushButton( "From", this );
    from->setGeometry( 10,18, 70,25 );

	s_from = new QLabel( "Path", this );
    s_from -> setGeometry( 100, 20, 300, 20 );

    add = new QPushButton( "Add >", this );
    add->setGeometry( 180,90, 70,25 );

    remove = new QPushButton( "< Remove", this );
    remove->setGeometry( 180,130, 70,25 );

    close = new QPushButton( "Close", this );
    close->setGeometry( 180,200, 70,25 );

	lib_list = new QListBox( this, "Lib List" );
    lib_list -> setGeometry( 10, 80, 150, 160 );

	test_list = new QListBox( this, "Test List" );
    test_list -> setGeometry( 270, 80, 250, 160 );

	l_avail = new QLabel( "Available Test Libs:", this );
    l_avail -> setGeometry( 10, 55, 200, 20 );

	l_lib = new QLabel( "Installed Auto Tests:", this );
    l_lib -> setGeometry( 270, 55, 200, 20 );

	l_name = new QLabel( "Name:", this );
    l_name -> setGeometry( 10, 255, 200, 20 );

	l_so = new QLabel( "Lib:", this );
    l_so -> setGeometry( 10, 280, 200, 20 );

	s_name = new QLabel( "NAME", this );
    s_name -> setGeometry( 70, 255, 200, 20 );

	s_lib = new QLabel( "LIB", this );
    s_lib -> setGeometry( 70, 280, 300, 20 );

    connect( close, SIGNAL(clicked()), SLOT(Ok()) );
    connect( close, SIGNAL(clicked()), SLOT(accept()) );
    connect( from, SIGNAL(clicked()), SLOT(From()) );
    connect( add,  SIGNAL(clicked()), SLOT(Add()) );
    connect( remove,  SIGNAL(clicked()), SLOT(Remove()) );

    //
    // fill up combo box
    //
    
    section *s = find_section( "Auto Tests" );

    if ( s )
    {
        prop *p;

        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            test_list -> insertItem( p -> name());
        }
    }

    //
    // Setup the path, initially to the current directory
    //

    getcwd( curr_dir, sizeof( curr_dir ));
    SetPath( curr_dir );

    connect( test_list,  SIGNAL(highlighted( const QString &)), SLOT(ListSelect(const QString &)) );

    test_list -> setCurrentItem( 0 );
}

dManageAutoTest::~dManageAutoTest()
{
    replace_ini_list( odbctest );
	delete close;
	delete add;
	delete remove;
    delete from;
    delete s_from;
    delete lib_list;
    delete test_list;
    delete l_avail;
    delete l_lib;
    delete l_name;
    delete l_so;
    delete s_name;
    delete s_lib;
}

void dNewGroup::Ok()
{
    QString name = source -> text();
    prop *p;

    //
    // check if its allready there
    //
    //
    section *s = find_section( "GROUPS" );
    if ( s )
    {
        for ( p = s->first(); p != 0; p = s -> next())
        {
            if ( strcmp( p -> name(), name ) == 0 )
            {
                char msg[ 128 ];

                sprintf( msg, "Group (%s) already defined", name.ascii());
                QMessageBox::critical( this, "OdbcTest", msg );
                return;
            }
        }

        p = new prop( name.ascii(), "Installed" );
        s -> append( p );

        s = new section( name );
        ini_list.append( s );

        parent_test -> group -> insertItem( p -> name());
        parent_test->group->setCurrentItem( parent_test->group->count() - 1 );
        parent_test -> Activated( p -> name());
    }
}

dNewGroup::dNewGroup( OdbcTest *parent, QString name, dManageTestGroup *ptest )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;
    parent_test = ptest;

    cancel = new QPushButton( "Close", this );
    cancel->setGeometry( 200,50, 70,25 );

    ok = new QPushButton( "Ok", this );
    ok->setGeometry( 110,50, 70,25 );

	source = new QLineEdit( this, "Test Group" );
	source -> setGeometry( 100, 20, 250, 20 );
    source -> setMaxLength( 128 );

	l_dsn = new QLabel( "Test Group:", this );
    l_dsn -> setGeometry( 10, 20, 90, 20 );

    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
}

dNewGroup::~dNewGroup()
{
    delete cancel;
    delete ok;
    delete source;
    delete l_dsn;
}

void dManageTestGroup::Ok()
{
}

//
// for a given test, look at each entry in the auto test section, see if it 
// is in the group section, if so it goes in the selected, else the installed list
//

void dManageTestGroup::update_test_lists( void )
{
    section *s = find_section( "Auto Tests" );
    QString current_text = group->currentText();
    section *s_group = find_section( current_text );

    auto_list->clear();
    sauto_list->clear();

    if ( s )
    {
        prop *p;

        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            prop *atprop;
            int found = 0;

            for ( atprop = s_group -> first();
                    atprop != 0;
                    atprop = s_group -> next())
            {
                if ( strcmp( atprop -> name(), p -> name()) == 0 )
                {
                    found = 1;
                    break;
                }
            }
            if ( found )
            {
                sauto_list->insertItem( p->name());
            }
            else
            {
                auto_list->insertItem( p->name());
            }
        }
    }
}

void dManageTestGroup::Activated( const QString &str )
{
    update_test_lists();
}

void dManageTestGroup::Activated( int val )
{
    if ( group -> count() > 0 )
    	Activated( group -> text( val ));
}

void dManageTestGroup::Add()
{
    int index = auto_list -> currentItem();
    QString current_text = group->currentText();

    if ( index >= 0 )
    {
        QListBoxItem *lbi = auto_list -> item( index );

        section *s = find_section( current_text );
        prop *p = new prop( lbi -> text(), "Installed" );
        s -> append( p );

        update_test_lists();
    }
}

void dManageTestGroup::Remove()
{
    int index = sauto_list -> currentItem();
    QString current_group = group->currentText();

    if ( index >= 0 )
    {
        QListBoxItem *lbi = sauto_list -> item( index );
        section *s = find_section( current_group );
        prop *p;

        //
        // find element
        //
        for ( p = s->first(); p != 0; p = s -> next())
        {
            if ( strcmp( p -> name(), lbi -> text()) == 0 )
            {
                s -> remove( p );
                break;
            }
        }
        update_test_lists();
    }
}

void dManageTestGroup::New()
{
	dNewGroup *dlg = new dNewGroup( this -> odbctest, "New Test Group", this );

	dlg -> exec();

	delete dlg;
}

void dManageTestGroup::Delete()
{
    if ( group -> count() == 0 )
    {
        return;
    }

    QString grp = group -> currentText();

    char msg [128];

    sprintf( msg, "Delete the test group %s ?", grp.ascii());

    if ( QMessageBox::information( this, "OdbcTest",
                msg,
                "&Delete", "&Cancel", 0,
                0, 1 ) == 0 )
    {
        //
        // Remove it from the GROUPS section
        //
        section *s = find_section( "GROUPS" );

        if ( s )
        {
            prop *p;

            for ( p = s -> first(); 
                    p != 0; 
                    p = s -> next())
            {
                if ( strcmp( p -> name(), grp.ascii()) == 0 )
                {
                    s -> remove( p );
                    break;
                }
            }
        }

        //
        // remove the section
        //
        s = find_section( grp.ascii());
        if ( s )
        {
            ini_list.removeRef( s );
            remove_section( odbctest, s );
        }

        //
        // fill up combo box
        //
        
        group -> clear();
        
        s = find_section( "GROUPS" );

        if ( s )
        {
            prop *p;

            for ( p = s -> first(); 
                    p != 0; 
                    p = s -> next())
            {
                group -> insertItem( p -> name());
            }
        }
        group -> setCurrentItem( 0 );
        Activated( 0 );
    }
}

dManageTestGroup::dManageTestGroup( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    init_ini_list( parent );

    nw = new QPushButton( "New", this );
    nw->setGeometry( 300,15, 70,25 );

    del = new QPushButton( "Delete", this );
    del->setGeometry( 380,15, 70,25 );

    add = new QPushButton( "Add >", this );
    add->setGeometry( 280,90, 70,25 );

    remove = new QPushButton( "< Remove", this );
    remove->setGeometry( 280,130, 70,25 );

    close = new QPushButton( "Close", this );
    close->setGeometry( 280,200, 70,25 );

	group = new QComboBox( FALSE, this, "group" );
	group -> setGeometry( 100, 15, 180, 20 );

	l_group = new QLabel( "Test Group:", this );
    l_group -> setGeometry( 10, 15, 60, 20 );

	l_auto = new QLabel( "Installed Auto Tests:", this );
    l_auto -> setGeometry( 10, 60, 160, 20 );

	l_sauto = new QLabel( "Selected Auto Tests:", this );
    l_sauto -> setGeometry( 370, 60, 160, 20 );

	auto_list = new QListBox( this, "Auto List" );
    auto_list -> setGeometry( 10, 80, 250, 160 );

	sauto_list = new QListBox( this, "Selected Auto List" );
    sauto_list -> setGeometry( 370, 80, 250, 160 );

    connect( close, SIGNAL(clicked()), SLOT(Ok()) );
    connect( close, SIGNAL(clicked()), SLOT(accept()) );
    connect( add, SIGNAL(clicked()), SLOT(Add()) );
    connect( remove, SIGNAL(clicked()), SLOT(Remove()) );
    connect( nw, SIGNAL(clicked()), SLOT(New()) );
    connect( del, SIGNAL(clicked()), SLOT(Delete()) );

    //
    // fill up combo box
    //
    
    section *s = find_section( "GROUPS" );

    if ( s )
    {
        prop *p;

        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            group -> insertItem( p -> name());
        }
    }

    Activated( 0 );
	connect( group, SIGNAL(activated(const QString &)), 
            this, SLOT( Activated(const QString &)));
}

dManageTestGroup::~dManageTestGroup()
{
    replace_ini_list( odbctest );
	delete close;
	delete add;
	delete remove;
    delete nw;
    delete del;
    delete group;
    delete l_group;
    delete l_auto;
    delete l_sauto;
    delete auto_list;
    delete sauto_list;
}

MYQListViewItem::MYQListViewItem( QListView * parent, MYQListViewItem *after,
        QString label1, QString label2 )
    :QListViewItem( parent, after, label1, label2 )
{
    test_index = 0;
    test_group = label1;
    test_name = QString::null;
    test_name = QString::null;
}

MYQListViewItem::MYQListViewItem( MYQListViewItem * parent, QString label1, QString label2,
         int index, QString _test_name )
    :QListViewItem( parent, label1, label2 )
{
    test_index = index;
    test_source = _test_name;
    test_name = label1;
}

MYQListViewItem::MYQListViewItem( MYQListViewItem * parent, MYQListViewItem *after,
            QString label1, QString label2, int index, QString _test_name )
    :QListViewItem( parent, after, label1, label2 )
{
    test_index = index;
    test_source = _test_name;
    test_name = label1;
}



MYQListViewItem* MYQListViewItem::firstChild()
{
    return (MYQListViewItem*) QListViewItem::firstChild();
}

MYQListViewItem * MYQListViewItem::parent()
{
    return (MYQListViewItem*) QListViewItem::parent();
}

MYQListViewItem * MYQListViewItem::nextSibling()
{
    return (MYQListViewItem*) QListViewItem::nextSibling();
}

void MYQListViewItem::set_down( BOOL state )
{
    if ( firstChild() )
    {
        firstChild()->set_down( state );
    }

    if ( nextSibling())
    {
        nextSibling()->set_down( state );
    }

    setSelected( state );
    repaint();
}

void MYQListViewItem::activate()
{
    int state = !isSelected();

    if ( state )
    {
        MYQListViewItem *p = this;

        while ((p = p->parent()))
        {
            p -> setSelected( state );
            p -> repaint();
        }
    }

    if ( firstChild() )
    {
        firstChild()->set_down( state );
    }
}

void dRunAutoTests::Ok()
{
    //
    // use each selected source with all selected tests
    //
    int i;
    QString group_name;
    SERVERINFO server_info;
    char msg[ 1024 ];

    //
    // set up callback, handle
    //
    static_odbctest = odbctest;

    for ( i = 0; i < sources -> count(); i ++ )
    {
        if ( sources -> isSelected( i ))
        {
            MYQListViewItem *group;

            QString src = sources -> text( i );

            // walk tree
        
            group = (MYQListViewItem *) tests -> firstChild();
            while( group )
            {
                if ( group -> isSelected())
                {
                    sprintf( msg, "Now executing Group %s on source %s",
                            group->group().ascii(), src.ascii());
                    print_to_odbctest( &server_info, msg, 0 );

                    section *src_section;

                    if ( strcmp( src.ascii(), "ODBC Test Handles" ) == 0 )
                    {
                        src_section = NULL;
                        server_info.henv = odbctest->get_handle( SQL_HANDLE_ENV );
                        server_info.hdbc = odbctest->get_handle( SQL_HANDLE_DBC );
                        server_info.hstmt = odbctest->get_handle( SQL_HANDLE_STMT );

                    }
                    else
                    {
                        src_section = find_section( src );
                        server_info.henv = NULL;
                        server_info.hdbc = NULL;
                        server_info.hstmt = NULL;
                    }

                    server_info.szSource[ 0 ] = '\0';
                    server_info.szValidServer0[ 0 ] = '\0';
                    server_info.szValidLogin0[ 0 ] = '\0';
                    server_info.szValidPassword0[ 0 ] = '\0';
                    server_info.szKeywords[ 0 ] = '\0';
                    server_info.cErrors = 0;

                    if ( b_debug -> isChecked())
                        server_info.fDebug = 1;
                    else
                        server_info.fDebug = 0;

                    if ( b_isolate -> isChecked())
                        server_info.fIsolate = 1;
                    else
                        server_info.fIsolate = 0;

                    if ( b_screen -> isChecked())
                        server_info.fScreen = 1;
                    else
                        server_info.fScreen = 0;

                    server_info.vCursorLib = cursor_state;

                    if ( b_log_file -> isChecked())
                    {
                        strcpy( server_info.szLogFile, l_log->text());
                        server_info.fLog = 1;
                    }
                    else
                    {
                        server_info.szLogFile[ 0 ] = '\0';
                        server_info.fLog = 0;
                    }

                    server_info.hLoadedInst = NULL;

                    if ( src_section )
                    {
                        for ( prop *prop = src_section->first(); prop != 0; prop = src_section -> next())
                        {
                            if ( strcmp( prop -> name(), "SERVER0" ) == 0 )
                            {
                                strcpy( server_info.szValidServer0, prop -> value());
                            }
                            else if ( strcmp( prop -> name(), "LOGIN0" ) == 0 )
                            {
                                strcpy( server_info.szValidLogin0, prop -> value());
                            }
                            else if ( strcmp( prop -> name(), "PASSWORD0" ) == 0 )
                            {
                                strcpy( server_info.szValidPassword0, prop -> value());
                            }
                            else if ( strcmp( prop -> name(), "KEYWORDS" ) == 0 )
                            {
                                strcpy( server_info.szKeywords, prop -> value());
                            }
                        }
                    }

                    strcpy( server_info.szSource, src.ascii());
                    server_info.cBuff = 0;

                    sprintf( msg, "Keywords: %s", server_info.szKeywords );
                    print_to_odbctest( &server_info, msg, 0 );

                    MYQListViewItem *sect = group -> firstChild();

                    while( sect )
                    {
                        if ( sect -> isSelected())
                        {
                            sprintf( msg, "Now Executing Auto Test: %s", sect->name().ascii());
                            print_to_odbctest( &server_info, msg, 1 );
                            sprintf( msg, "Source: %s", src.ascii());
                            print_to_odbctest( &server_info, msg, 1 );
                            sprintf( msg, "Time started %s", get_time_str());
                            print_to_odbctest( &server_info, msg, 1 );

                            // run tests
                            // open driver
                           
                            section *s = find_section( sect->name());

                            /*
                             * initialize libtool
                             */

                            lt_dlinit();

                            if ( s )
                            {
                                for ( prop *prop = s->first(); prop != 0; prop = s -> next())
                                {
                                    if ( strcmp( prop -> name(), "DLL" ) == 0 )
                                    {
                                        lt_dlhandle handle = lt_dlopen( prop -> value());
                                        //
                                        // open the lib
                                        //
                                        if ( handle )
                                        {
                                            void (*pfAutoTestFunc)(lpSERVERINFO);
                                            BOOL (*pfAutoTestName)(LPSTR,UINT*);

                                            pfAutoTestFunc = (void(*)(lpSERVERINFO))lt_dlsym( handle, "AutoTestFunc" );
                                            pfAutoTestName = (BOOL(*)(LPSTR,UINT*))lt_dlsym( handle, "AutoTestName" );

                                            if ( !pfAutoTestFunc )
                                            {
                                                sprintf( msg, "Can't Find AutoTestFunc in %s", prop -> value());
                                                print_to_odbctest( &server_info, msg, 1 );
                                            }
                                            else if ( !pfAutoTestName )
                                            {
                                                sprintf( msg, "Can't Find AutoTestName in %s", prop -> value());
                                                print_to_odbctest( &server_info, msg, 1 );
                                            }
                                            else
                                            {
                                                char test_name[ AUTO_MAX_TEST_NAME + 1 ];
                                                UINT count;

                                                if ( !pfAutoTestName( test_name, &count ))
                                                {
                                                    sprintf( msg, "AutoTestName returns FALSE" );
                                                    print_to_odbctest( &server_info, msg, 1 );
                                                }
                                                else
                                                {
                                                    int size;
                                                    
                                                    size = count / (sizeof(unsigned int)*8);
                                                    size ++;

                                                    server_info.rglMask = (UINT FAR *)calloc( sizeof(unsigned int), size );

                                                    server_info.hwnd = (void*)callback_function;

                                                    // now we are ready
                                                    if ( b_isolate -> isChecked())
                                                    {
                                                        MYQListViewItem *test = sect -> firstChild();

                                                        while( test )
                                                        {
                                                            if ( test -> isSelected())
                                                            {
                                                                SETBIT(server_info.rglMask, test->index());
                                                                pfAutoTestFunc(&server_info);
                                                            }
                                                            test = test -> nextSibling();
                                                        }
                                                    }
                                                    else
                                                    {
                                                        MYQListViewItem *test = sect -> firstChild();

                                                        memset( server_info.rglMask, 0, sizeof(unsigned int) * size );
                                                        while( test )
                                                        {
                                                            if ( test -> isSelected())
                                                            {
                                                                SETBIT(server_info.rglMask, test->index());
                                                            }
                                                            test = test -> nextSibling();
                                                        }
                                                        pfAutoTestFunc(&server_info);
                                                    }
                                                    sprintf( msg, "Error Count: %d", server_info.cErrors);
                                                    print_to_odbctest( &server_info, msg, 1 );

                                                    free( server_info.rglMask );
                                                }
                                            }
                                            lt_dlclose( handle );
                                        }
                                        else
                                        {
                                            sprintf( msg, "Can't open %s", prop -> value());
                                            print_to_odbctest( &server_info, msg, 1 );
                                        }
                                    }
                                }
                            }

                            sprintf( msg, "Time finished %s", get_time_str());
                            print_to_odbctest( &server_info, msg, 1 );
                        }
                        sect = sect -> nextSibling();
                    }
                }
                group = group -> nextSibling();
            }
        }
    }
}

void dRunAutoTests::Log()
{
	QFileDialog *dlg = new QFileDialog( this, "fred", TRUE );

    dlg -> setMode( QFileDialog::AnyFile );
    dlg -> setSelection( l_log -> text());

    if ( dlg -> exec() == QDialog::Accepted )
    {
        QString result = dlg -> selectedFile();
        l_log -> setText( result );
    }

	delete dlg;
} 

void dRunAutoTests::LogChanged( int state )
{
    if ( state )
    {
        log_file -> setEnabled( TRUE );
        l_log -> setEnabled( TRUE );
    }
    else
    {
        log_file -> setEnabled( FALSE );
        l_log -> setEnabled( FALSE );
    }
}

void dRunAutoTests::CursorChanged( int state )
{
    if ( state == 0 )
    {
        cursor_state = SQL_CUR_USE_DRIVER;
    }
    else if ( state == 1 )
    {
        cursor_state = SQL_CUR_USE_IF_NEEDED;
    }
    else if ( state == 1 )
    {
        cursor_state = SQL_CUR_USE_ODBC;
    }
}

void dRunAutoTests::add_auto_test( const char * test_name, MYQListViewItem *top, MYQListViewItem **item )
{
    section *s = find_section( test_name );

    if ( s )
    {
        for ( prop *prop = s->first(); prop != 0; prop = s -> next())
        {
            if ( strcmp( prop -> name(), "DLL" ) == 0 )
            {
                /*
                 * initialize libtool
                 */

                lt_dlinit();

                //
                // open the lib
                //
                lt_dlhandle handle = lt_dlopen( prop -> value());
                if ( handle )
                {
                    void *pfAutoTestFunc;
                    BOOL (*pfAutoTestName)(LPSTR,UINT*);
                    BOOL (*pfAutoTestDesc)(UWORD,LPSTR,LPSTR);

                    pfAutoTestName = (BOOL(*)(LPSTR,UINT*))lt_dlsym( handle, "AutoTestName" );
                    pfAutoTestDesc = (BOOL(*)(UWORD,LPSTR,LPSTR))lt_dlsym( handle, "AutoTestDesc" );
                    pfAutoTestFunc = lt_dlsym( handle, "AutoTestFunc" );


                    if ( !pfAutoTestName ||
                            !pfAutoTestDesc ||
                            !pfAutoTestFunc )
                    {
                        lt_dlclose( handle );
                        return;
                    }

                    char test_name[ AUTO_MAX_TEST_NAME + 1 ];
                    char func_name[ AUTO_MAX_TESTCASE_NAME + 1 ];
                    char test_desc[ AUTO_MAX_TESTDESC_NAME + 1 ];
                    UINT count;

                    //
                    // get the test name
                    //
                    if ( !pfAutoTestName( test_name, &count ))
                    {
                        lt_dlclose( handle );
                        return;
                    }

                    *item = new MYQListViewItem( top, *item, test_name );  
                    MYQListViewItem *after = NULL;

                    for ( int i = 1; i <= count; i ++ )
                    {

                        if( pfAutoTestDesc( i, func_name, test_desc ))
                        {
                            if ( after )
                            {
                                MYQListViewItem *test = new MYQListViewItem( *item, after, 
                                        func_name, test_desc, i, test_name );  
                                after = test;
                            }
                            else
                            {
                                MYQListViewItem *test = new MYQListViewItem( *item, func_name, test_desc, 
                                        i, test_name );  
                                after = test;
                            }
                        }
                    }

                    lt_dlclose( handle );
                }
            }
        }
    }
}

void dRunAutoTests::TestsChanged()
{
    int i, ok1 = 0, ok2 = 0;
    MYQListViewItem *group;

    for ( i = 0; i < sources -> count(); i ++ )
    {
        if ( sources -> isSelected( i ))
        {
            ok1 = 1;
            break;
        }
    }

    group = (MYQListViewItem*) tests -> firstChild();
    while( group )
    {
        if ( group -> isSelected())
        {
            ok2 = 1;
            break;
        }
        group = group -> nextSibling();
    }

    if ( ok1 && ok2 )
    {
        ok -> setEnabled( TRUE );
    }
    else
    {
        ok -> setEnabled( FALSE );
    }
}

dRunAutoTests::dRunAutoTests( OdbcTest *parent, QString name )
        : QDialog( parent, name, TRUE )
{
	setCaption( name );

	odbctest = parent;

    init_ini_list( parent );

	l_tests = new QLabel( "Auto Tests:", this );
    l_tests -> setGeometry( 10, 15, 60, 20 );

    tests = new QListView( this, "auto tests" );
    tests -> setGeometry( 10, 40, 300, 150 );
    tests -> addColumn( "Tests", -1 );
    tests -> setRootIsDecorated( TRUE );
    tests -> setMultiSelection( TRUE );
    tests -> setSorting( -1 );
    tests -> header()->hide();

	l_sources = new QLabel( "Test Sources:", this );
    l_sources -> setGeometry( 320, 15, 80, 20 );

	sources = new QListBox( this, "Test Sources" );
    sources -> setGeometry( 320, 40, 150, 220 );
    sources -> setSelectionMode( QListBox::Multi );

	output = new QButtonGroup( "Output", this );
    output -> setGeometry( 10, 200, 90, 80 );

	b_log_file = new QCheckBox( "Log File", output, "Log File" );
	b_log_file -> setGeometry( 10, 20, 70, 20 );

	b_screen = new QCheckBox( "Screen", output, "Screen" );
	b_screen -> setGeometry( 10, 50, 70, 20 );
    b_screen -> setChecked( TRUE );

	options = new QButtonGroup( "Options", this );
    options -> setGeometry( 110, 200, 200, 80 );

	b_debug = new QCheckBox( "Debug", options, "Debug" );
	b_debug -> setGeometry( 10, 20, 80, 20 );

	b_isolate = new QCheckBox( "Isolate tests", options, "Isolate" );
	b_isolate -> setGeometry( 10, 50, 100, 20 );

	b_cursor = new QCheckBox( "Cursor Library", options, "Cursor Library" );
	b_cursor -> setGeometry( 90, 20, 100, 20 );
    b_cursor -> setTristate( TRUE );

    run_list = new QPushButton( "Run List...", this );
	run_list -> setGeometry( 10, 290, 70, 25 );
    run_list -> setEnabled( FALSE );

	rlist = new QComboBox( FALSE, this, "Run List" );
	rlist -> setGeometry( 100, 290, 150, 20 );
	rlist -> insertItem( "<Default>", 0 );
    rlist -> setEnabled( FALSE );

    ok = new QPushButton( "Ok", this );
	ok -> setGeometry( 320, 290, 70, 25 );
    ok -> setEnabled( FALSE );

    cancel = new QPushButton( "Cancel", this );
	cancel -> setGeometry( 400, 290, 70, 25 );

    log_file = new QPushButton( "Log File...", this );
	log_file -> setGeometry( 10, 320, 70, 25 );
    log_file -> setEnabled( FALSE );

	l_log = new QLabel( "auto.log", this );
    l_log -> setGeometry( 89, 320, 380, 20 );
    l_log -> setEnabled( FALSE );

    connect( ok, SIGNAL(clicked()), SLOT(Ok()) );
    connect( ok, SIGNAL(clicked()), SLOT(accept()) );

    connect( cancel, SIGNAL(clicked()), SLOT(reject()) );

    connect( log_file, SIGNAL(clicked()), SLOT(Log()) );

    connect( b_log_file, SIGNAL(stateChanged(int)), SLOT(LogChanged(int)));

    cursor_state = SQL_CUR_USE_DRIVER;
    connect( b_cursor, SIGNAL(stateChanged(int)), SLOT(CursorChanged(int)));

    connect( sources, SIGNAL(selectionChanged()), SLOT(TestsChanged()));
    connect( tests, SIGNAL(selectionChanged()), SLOT(TestsChanged()));

    //
    // Fill test sources
    //
    section *s = find_section( "SQL_DRIVERS" );
    if ( s )
    {
        prop *p;

        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            sources -> insertItem( p -> name());
        }
    }
    sources -> insertItem( "ODBC Test Handles" );


    MYQListViewItem *top = NULL;  
    MYQListViewItem *last_test = NULL;  

    //
    // fill auto tests
    //

    s = find_section( "Auto Tests" );

    if ( s )
    {
        prop *p;

        top = new MYQListViewItem( tests, top, "All" );  
        
        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            add_auto_test( p -> name(), top, &last_test );
        }
    }

    //
    // fill individual tests
    //
    s = find_section( "GROUPS" );
    if ( s )
    {
        prop *p, *p1;

        for ( p = s -> first(); 
                p != 0; 
                p = s -> next())
        {
            section *s1 = find_section( p -> name() );
            if ( s1 )
            {
                top = new MYQListViewItem( tests, top, p -> name());  
                last_test = NULL ;
                for ( p1 = s1-> first(); 
                        p1 != 0; 
                        p1 = s1-> next())
                {
                    add_auto_test( p1 -> name(), top, &last_test );
                }
            }
        }
    }
}

dRunAutoTests::~dRunAutoTests()
{
    replace_ini_list( odbctest );
    delete l_tests;
    delete tests;
    delete l_sources;
    delete sources;
    delete output;
    delete options;
    delete run_list;
    delete rlist;
    delete ok;
    delete cancel;
    delete log_file;
    delete l_log;
}

void OdbcTest::options()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::trace()
{
    QMessageBox::about( this, "ODBC Test",
			"Not yet implemented" );
}

void OdbcTest::manage_test()
{
	dManageTest *dlg = new dManageTest( this, "Manage Test Sources" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::manage_auto_test()
{
	dManageAutoTest *dlg = new dManageAutoTest( this, "Manage Auto Test" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::manage_test_groups()
{
	dManageTestGroup *dlg = new dManageTestGroup( this, "Manage Test Groups" );

	dlg -> exec();

	delete dlg;
}

void OdbcTest::run_auto_tests()
{
	dRunAutoTests *dlg = new dRunAutoTests( this, "Run Auto Tests" );

	dlg -> exec();

	delete dlg;
}
