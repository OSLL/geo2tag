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
#include "classIndexs.h"

#include "keysilver2.xpm"

classIndexs::classIndexs( QListView *pParent, classCanvas *pCanvas, SQLHDBC hDbc, char *pszTable )
    : classNode( pParent, pCanvas )
{
	Init( hDbc, pszTable );
}

classIndexs::classIndexs( QListViewItem *pParent, classCanvas *pCanvas, SQLHENV hDbc, char *pszTable )
    : classNode( pParent, pCanvas )
{
	Init( hDbc, pszTable );
}

classIndexs::classIndexs( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc, char *pszTable )
    : classNode( pParent, pAfter, pCanvas )
{
	Init( hDbc, pszTable );
}

classIndexs::~classIndexs()
{
	listIndexs.clear();
}

void classIndexs::Init( SQLHDBC hDbc, char *pszTable )
{
	this->hDbc		= hDbc;
	this->qsTable	= pszTable;
	setText( 0, "Indexs" );
	setText( 1, "" );
	setText( 2, "" );
	listIndexs.setAutoDelete( TRUE );
	this->setPixmap( 0, QPixmap( keysilver2_xpm ) );
}

void classIndexs::setOpen( bool o )
{
    if ( o && !childCount() )
	{
		LoadIndexs();
    }
    QListViewItem::setOpen( o );
}

void classIndexs::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}

void classIndexs::LoadIndexs()
{
	SQLHSTMT        hstmt;
	SQLRETURN		nReturn             	= -1;
	SQLCHAR         szTableName[101]		= "";
	SQLCHAR         szIndexName[101]		= "";
	SQLCHAR         szPrevIndexName[101]	= "";
	int				bUnique					= 0;
	SQLCHAR         szDesc[101]				= "";
	QString         qsError;
	SQLLEN  		nIndicator				= 0;

	// CREATE A STATEMENT
	nReturn = SQLAllocStmt( hDbc, &hstmt );
	if ( nReturn != SQL_SUCCESS )
	{
		QMessageBox::warning( pCanvas, "Data Manager",  "Failed to SQLAllocStmt" );
		return;
	}

	// EXECUTE OUR SQL/CALL
	strcpy( (char *)szTableName, qsTable.data() );
	if ( SQL_SUCCESS != (nReturn=SQLStatistics( hstmt, 0, 0, 0, 0, szTableName, SQL_NTS, 0, 0 )) )
	{
		QMessageBox::warning( pCanvas, "Data Manager",  "Failed to SQLStatistics" );
		return;
	}

	// GET RESULTS
	nReturn = SQLFetch( hstmt );
	while ( nReturn == SQL_SUCCESS || nReturn == SQL_SUCCESS_WITH_INFO )
	{
		nReturn = SQLGetData( hstmt, 4, SQL_C_LONG, &bUnique, sizeof(bUnique), &nIndicator );
		if ( nReturn != SQL_SUCCESS || nIndicator == SQL_NULL_DATA || !bUnique )
			strcpy( (char *)szDesc, "" );
		else
			strcpy( (char *)szDesc, "UNIQUE" );

		nReturn = SQLGetData( hstmt, 6, SQL_C_CHAR, &szIndexName[0], sizeof(szIndexName), &nIndicator );
		if ( nReturn != SQL_SUCCESS || nIndicator == SQL_NULL_DATA )
			strcpy( (char *)szIndexName, "Unknown" );

		if ( strcmp( (const char*)szIndexName, (const char*)szPrevIndexName ) != 0 )
		{
			listIndexs.append( new classIndex( this, pCanvas, hDbc, (char *)szTableName, (char *)szIndexName, (char *)szDesc ) );
			strcpy( (char*)szPrevIndexName, (char*)szIndexName );
		}

		nReturn = SQLFetch( hstmt );
	}

	// FREE STATEMENT
	nReturn = SQLFreeStmt( hstmt, SQL_DROP );
	if ( nReturn != SQL_SUCCESS )
		QMessageBox::warning( pCanvas, "Data Manager",  "Failed to SQLFreeStmt" );

}

void classIndexs::selectionChanged( QListViewItem *p )
{
	classIndex	*pIndex;

	for ( pIndex = listIndexs.first(); pIndex != 0; pIndex = listIndexs.next() )
        pIndex->selectionChanged( p );

	if ( p == this )
	{
	}
}



