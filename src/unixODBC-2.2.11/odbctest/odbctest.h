/****************************************************************************
 *  such slot OdbcTest::options()
 *  QObject::connec
** $Id: odbctest.h,v 1.2 2001/12/20 17:26:26 lurcher Exp $
**
** Copyright (C) 1992-1999 Troll Tech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef ODBCTEST_H
#define ODBCTEST_H

#include <stdio.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qsplitter.h>
#include <qmultilineedit.h>
#if (QT_VERSION>=300)
#include <qtextedit.h>
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include <sql.h>

/*
 * structure that defines the options and values
 */

typedef struct attr_value
{
	const char	*text;
	int 	value;
	const char 	*version;
	int		data_type;
} attr_value;

typedef struct attr_options
{
	const char	*text;
	int		attr;
	attr_value values[ 25 ];
	const char 	*version;
	int		data_type;
	int		is_bitmap;
	int		is_pointer;
} attr_options;

class Handle
{
public:
	Handle( int t, SQLHANDLE h, QList<Handle> &list );
	Handle( int t, SQLHANDLE h, QString desc = NULL, SQLHANDLE stmt = SQL_NULL_HANDLE );
	Handle( Handle &e );
	~Handle();
	char * toStr( char * str );
	SQLHANDLE getHandle( void ) { return handle; };
	SQLHANDLE getStmtHandle( void ) { return stmt_handle; };
	int getType( void ) { return type; };
	char *bookmark_ptr;

    SQLUINTEGER row_array_size;
    SQLUINTEGER param_array_size;

	SQLUINTEGER *param_bind_offset_ptr, param_bind_offset;
	SQLUSMALLINT *param_opt_ptr, param_opt;
	SQLUSMALLINT *param_status_ptr, param_status;
	SQLUINTEGER *params_processed_ptr, params_processed;
	SQLUINTEGER *row_bind_offset_ptr, row_bind_offset;
	SQLUSMALLINT *row_operation_ptr, row_operation;
	SQLUSMALLINT *row_status_ptr, row_status;
	SQLUINTEGER *rows_fetched_ptr, rows_fetched;

private:
	int type;
	SQLHANDLE handle;
    QString description;
    int implicit;
	SQLHANDLE stmt_handle;
    QList<Handle> *handle_list;
};

#if (QT_VERSION<300)

class QTextEdit : public QMultiLineEdit
{
public:
    QTextEdit (QWidget *, const char *);
    void setMaxLength( int x );
    void append( const char *str );
    void maxLines( int n );
};

#endif

class OutputWin : public QTextEdit
{
public:
	OutputWin( QWidget *parent = 0, const char *name = 0 );
	void insertLineLimited( const char * str );
	void setMaxLines( int );

private:
	int max_lines;
};

class OdbcTest : public QMainWindow
{
    Q_OBJECT

public:
    OdbcTest( QWidget *parent=0, const char *name=0 );
	QSplitter *split;
	QMultiLineEdit *in_win;
	OutputWin *out_win;
	QList<Handle> listHandle;
	const char *return_as_text( int ret );
	void fill_list_box( attr_value *attr, QComboBox *lst );
	void fill_list_box( attr_options *attr, QComboBox *lst );
	void dumpError( int type, SQLHANDLE );
	Handle *fill_handle_list( int type, QComboBox *lst );
	Handle *extract_handle_list( int type, QComboBox *lst );
	const char *int_type_as_string( SQLINTERVAL );
	SQLHANDLE get_handle( int type );
	
public slots:
    void about();
	void testhelp();
	void apihelp();
	void open();
	void saveas();
	void saveopt();
	void undo();
	void cut();
	void copy();
	void paste();
	void del();
	void clearow();
	void selectall();
	void wrap();
	void sqlgetdiagrec();
	void sqlgetdiagfield();
	void sqlerror();
	void errorall();
	void sqlallochandle();
	void sqldatasources();
	void sqldrivers();
	void sqlendtran();
	void sqlfreehandle();
	void datasourcesall();
	void driversall();
	void sqlbrowseconnect();
	void sqlconnect();
	void sqldriverconnect();
	void sqldisconnect();
	void sqlgetinfo();
	void getinfodbc( SQLHANDLE dbc = SQL_NULL_HANDLE );
	void sqlgetfunctions();
	void sqlnativesql();
	void fullconnect();
	void fulldisconnect();
	void getinfoall();
	void getfunctionsall();
	void sqlcopydesc();
	void sqlgetdescfield();
	void sqlgetdescrec();
	void sqlsetdescfield();
	void sqlsetdescrec();
	void getdescriptorsall();
	void sqlbindparameter();
	void sqlcancel();
	void sqlclosecursor();
	void sqldescribeparam();
	void sqlexecute();
	void sqlexecdirect();
	void sqlfreestmt();
	void sqlgetcursorname();
	void sqlnumparams();
	void sqlparamdata();
	void sqlparamoptions();
	void sqlprepare();
	void sqlputdata();
	void sqlsetcursorname();
	void fillparam();
	void showparam();
	void showcursorsettings();
	void sqlsetstmtattr();
	void sqlgetstmtattr();
	void sqlsetconnectattr();
	void sqlgetconnectattr();
	void sqlsetenvattr();
	void sqlgetenvattr();
	void setcursoratributes();
	void sqlbindcol();
	void sqlbulkoperations();
	void sqlcolattributes();
	void sqlcolattribute();
	void sqldescribecol();
	void sqlextendedfetch();
	void sqlfetch();
	void sqlfetchscroll();
	void sqlgetdata();
	void sqlmoreresults();
	void sqlnumresultscols();
	void sqlrowcount();
	void sqlsetpos();
	void sqlsetscrolloptions();
	void bindcolall();
	void describecolall();
	void fetchall();
	void getdataall();
	void getdatastmt( SQLHANDLE hstmt = SQL_NULL_HANDLE );
	void showboundcols();
	void displayrowset();
	void sqlcolumns();
	void sqlcolumnprivileges();
	void sqlgettypeinfo();
	void sqlforeignkeys();
	void sqlprimarykeys();
	void sqlprocedures();
	void sqlprocedurecolumns();
	void sqlspecialcolumns();
	void sqlstatistics();
	void sqltables();
	void sqltableprivileges();
	void sqlmanagedatasources();
	void sqlremovedefaultdatasource();
	void sqlconfigdatasource();
	void sqlcreatedatasource();
	void sqlvaliddsn();
	void sqlremovedsnfromini();
	void sqlwritedsntoini();
	void sqlremovedrivers();
	void sqlconfigdrivers();
	void sqlinstalldriver();
	void sqlinstalldriverex();
	void sqlgetinstalleddrivers();
	void sqlremovedrivermanager();
	void sqlinstalldrivermanager();
	void sqlreadfiledsn();
	void sqlwritefiledsn();
	void sqlwriteprivateprofilestring();
	void sqlgetprivateprofilestring();
	void sqlinstalltranslator();
	void sqlinstalltranslatorex();
	void sqlremovetranslator();
	void sqlgettranslator();
	void sqlsetconfigmode();
	void sqlgetconfigmode();
	void options();
	void trace();
    void sqlallocenv();
    void sqlfreeenv();
    void sqltransact();
    void sqlallocconnect();
    void sqlfreeconnect();
    void sqlallocstmt();
    void sqlbindparam();
    void sqlsetstmtoption();
    void sqlgetstmtoption();
    void sqlsetconnectoption();
    void sqlgetconnectoption();
    void manage_test();
    void manage_auto_test();
    void manage_test_groups();
    void run_auto_tests();

protected:
    void    resizeEvent( QResizeEvent * );

signals:
    void    explain( const QString& );

private:
    QMenuBar *menu;
    QLabel   *label;
};

#endif // ODBCTEST_H
