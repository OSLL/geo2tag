/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#include "classTables.h"

#include "tables.xpm"

classTables::classTables( QListView *pParent, classCanvas *pCanvas, SQLHDBC hDbc )
    : classNode( pParent, pCanvas )
{
	Init( hDbc );
}

classTables::classTables( QListViewItem *pParent, classCanvas *pCanvas, SQLHENV hDbc )
    : classNode( pParent, pCanvas )
{
	Init( hDbc );
}

classTables::classTables( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc )
    : classNode( pParent, pAfter, pCanvas )
{
	Init( hDbc );
}

classTables::~classTables()
{
	listTables.clear();
}

void classTables::Init( SQLHDBC hDbc )
{
	this->hDbc		= hDbc;
	setText( 0, "Tables" );
	setText( 1, "" );
	setText( 2, "" );
	listTables.setAutoDelete( TRUE );
	this->setPixmap( 0, QPixmap( tables_xpm ) );
}

void classTables::setOpen( bool bOpen )
{
    QListViewItem::setOpen( bOpen );
    listView()->setSelected( listView()->selectedItem(), false );
    if ( bOpen )
    {
		LoadTables();
//        setSelected( true );
    }
    else
        listTables.clear();
}

void classTables::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}

void classTables::LoadTables()
{
	SQLHSTMT        hstmt;
	SQLRETURN		nReturn             = -1;
	SQLCHAR         szTableName[101]    = "";
	SQLCHAR         szTableType[101]    = "";
	SQLCHAR         szTableRemarks[301] = "";
	QString         qsError;
	SQLLEN		    nIndicatorName;
	SQLLEN		    nIndicatorType;
	SQLLEN		    nIndicatorRemarks;
    classTable      *pTable = NULL;

	// CREATE A STATEMENT
	nReturn = SQLAllocStmt( hDbc, &hstmt );
	if ( nReturn != SQL_SUCCESS )
	{
		QMessageBox::warning( pCanvas, "Data Manager",  "Failed to SQLAllocStmt" );
		return;
	}

	// EXECUTE OUR SQL/CALL
	if ( SQL_SUCCESS != (nReturn=SQLTables( hstmt, 0, 0, 0, 0, 0, 0, 0, 0 )) )
	{
		QMessageBox::warning( pCanvas, "Data Manager",  "Failed to SQLTables" );
		return;
	}

	SQLBindCol( hstmt, SQLTables_TABLE_NAME, SQL_C_CHAR, szTableName, sizeof(szTableName), &nIndicatorName );
    SQLBindCol( hstmt, SQLTables_TABLE_TYPE, SQL_C_CHAR, szTableType, sizeof(szTableType), &nIndicatorType );
    SQLBindCol( hstmt, SQLTables_REMARKS, SQL_C_CHAR, szTableRemarks, sizeof(szTableRemarks), &nIndicatorRemarks );
	// GET RESULTS
	nReturn = SQLFetch( hstmt );
	while ( nReturn == SQL_SUCCESS || nReturn == SQL_SUCCESS_WITH_INFO )
	{
		if ( nIndicatorName == SQL_NULL_DATA )
			listTables.append( pTable = new classTable( this, pTable, pCanvas, hDbc, "Unknown" ) );
		else
			listTables.append( pTable = new classTable( this, pTable, pCanvas, hDbc, (char *)szTableName, (char*)szTableType, (char*)szTableRemarks ) );

        szTableName[0]      = '\0';
        szTableType[0]      = '\0';
        szTableRemarks[0]   = '\0';
		nReturn = SQLFetch( hstmt );
	}

	// FREE STATEMENT
	nReturn = SQLFreeStmt( hstmt, SQL_DROP );
	if ( nReturn != SQL_SUCCESS )
		QMessageBox::warning( pCanvas, "Data Manager",  "Failed to SQLFreeStmt" );

}

void classTables::selectionChanged( QListViewItem *p )
{
	classTable	*pTable;

	for ( pTable = listTables.first(); pTable != 0; pTable = listTables.next() )
        pTable->selectionChanged( p );

	if ( p == this )
	{
	}
}

