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
#include "classTable.h"
#include "table.xpm"

classTable::classTable( QListView *pParent, classCanvas *pCanvas, SQLHDBC hDbc, char *pszName, char *pszType, char *pszDescription )
    : classNode( pParent, pCanvas )
{
	Init( hDbc, pszName, pszType, pszDescription );
}

classTable::classTable( QListViewItem *pParent, classCanvas *pCanvas, SQLHENV hDbc, char *pszName, char *pszType, char *pszDescription )
    : classNode( pParent, pCanvas )
{
	Init( hDbc, pszName, pszType, pszDescription );
}

classTable::classTable( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc, char *pszName, char *pszType, char *pszDescription )
    : classNode( pParent, pAfter, pCanvas )
{
	Init( hDbc, pszName, pszType, pszDescription );
}

classTable::~classTable()
{
    Fini();
}

void classTable::Init( SQLHDBC hDbc, char *pszName, char *pszType, char *pszDescription )
{
	this->hDbc		= hDbc;
	this->qsName	= pszName;
	setText( 0, qsName );
	setText( 1, pszType );
	setText( 2, pszDescription );
	sortChildItems( 1, FALSE );
	listColumns.setAutoDelete( TRUE );
    pPrimaryKeys	= 0;
	pIndexs 		= 0;
	pSpecialColumns	= 0;
    pBrowse         = 0;
	this->setPixmap( 0, QPixmap( table_xpm ) );
}

void classTable::Fini()
{
    listColumns.clear();
    if ( pPrimaryKeys ) delete pPrimaryKeys;
    if ( pIndexs ) delete pIndexs;
    if ( pSpecialColumns ) delete pSpecialColumns;
    if ( pBrowse ) delete pBrowse;
    pPrimaryKeys	= 0;
	pIndexs 		= 0;
	pSpecialColumns	= 0;
    pBrowse         = 0;
}

void classTable::setOpen( bool bOpen )
{
    QListViewItem::setOpen( bOpen );
    listView()->setSelected( listView()->selectedItem(), false );
    if ( bOpen )
    {
		LoadColumns();
		pPrimaryKeys	= new classPrimaryKeys( this, pCanvas, hDbc, (char *)qsName.data() );
		pIndexs 		= new classIndexs( this, pCanvas, hDbc, (char *)qsName.data() );
		pSpecialColumns = new classSpecialColumns( this, pCanvas, hDbc, (char *)qsName.data() );
//        listView()->setSelected( listView()->selectedItem(), false );
//        setSelected( true );
    }
    else
    {
        Fini();
    }
}

void classTable::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}

void classTable::LoadColumns()
{
	SQLHSTMT        hstmt;
	SQLRETURN		nReturn             = -1;
	SQLCHAR         szTableName[101]	= "";
	SQLCHAR         szColumnName[101]	= "";
	SQLCHAR         szColumnType[101]	= "";
	SQLCHAR         szDescription[301]	= "";
	QString         qsError;
    classColumn     *pColumn = NULL;

	// CREATE A STATEMENT
	nReturn = SQLAllocStmt( hDbc, &hstmt );
	if ( nReturn != SQL_SUCCESS )
	{
		QMessageBox::warning( pCanvas, "Data Manager",  "Failed to SQLAllocStmt" );
		return;
	}

	// EXECUTE OUR SQL/CALL
	strcpy( (char *)szTableName, qsName.data() );

	if ( SQL_SUCCESS != (nReturn=SQLColumns( hstmt, 0, 0, 0, 0, szTableName, SQL_NTS, 0, 0 )) )
	{
		QMessageBox::warning( pCanvas, "Data Manager",  "Failed to SQLColumns" );
		return;
	}

	// GET RESULTS
	nReturn = SQLFetch( hstmt );
	while ( nReturn == SQL_SUCCESS || nReturn == SQL_SUCCESS_WITH_INFO )
	{
        szDescription[0] = '\0';

		nReturn = SQLGetData( hstmt, SQLColumns_COLUMN_NAME, SQL_C_CHAR, &szColumnName[0], sizeof(szColumnName), 0 );
		if ( nReturn != SQL_SUCCESS )
			strcpy( (char *)szColumnName, "Unknown" );

		nReturn = SQLGetData( hstmt, SQLColumns_TYPE_NAME, SQL_C_CHAR, &szColumnType[0], sizeof(szColumnType), 0 );

		listColumns.append( pColumn = new classColumn( this, pColumn, pCanvas, hDbc, (char *)szColumnName, (char*)szColumnType, (char*)szDescription ) );

		nReturn = SQLFetch( hstmt );
	}

	// FREE STATEMENT
	nReturn = SQLFreeStmt( hstmt, SQL_DROP );
	if ( nReturn != SQL_SUCCESS )
		QMessageBox::warning( pCanvas, "Data Manager",  "Failed to SQLFreeStmt" );

}

void classTable::selectionChanged( QListViewItem *p )
{
	classColumn	*pColumn;

	for ( pColumn = listColumns.first(); pColumn != 0; pColumn = listColumns.next() )
        pColumn->selectionChanged( p );

	if ( p == this )
    {
        if ( !pBrowse ) 
            pBrowse = new classBrowseFrame( hDbc, qsName, pCanvas );
        pBrowse->show();
    }
	else 
    {
        if ( pBrowse )
    		pBrowse->hide();
    }
}



