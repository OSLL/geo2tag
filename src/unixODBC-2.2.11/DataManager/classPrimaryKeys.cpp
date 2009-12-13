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
#include "classPrimaryKeys.h"

#include "keyred.xpm"

classPrimaryKeys::classPrimaryKeys( QListView *pParent, classCanvas *pCanvas, SQLHDBC hDbc, char *pszTable )
    : classNode( pParent, pCanvas )
{
	Init( hDbc, pszTable );
}

classPrimaryKeys::classPrimaryKeys( QListViewItem *pParent, classCanvas *pCanvas, SQLHENV hDbc, char *pszTable )
    : classNode( pParent, pCanvas )
{
	Init( hDbc, pszTable );
}

classPrimaryKeys::classPrimaryKeys( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc, char *pszTable )
    : classNode( pParent, pAfter, pCanvas )
{
	Init( hDbc, pszTable );
}

classPrimaryKeys::~classPrimaryKeys()
{
}

void classPrimaryKeys::Init( SQLHDBC hDbc, char *pszTable )
{
	this->hDbc		= hDbc;
	this->qsTable	= pszTable;
	setText( 0, "PrimaryKeys" );
	setText( 1, "" );
	setText( 2, "" );
	this->setPixmap( 0, QPixmap( keyred_xpm ) );
}

void classPrimaryKeys::setOpen( bool o )
{
    if ( o && !childCount() )
	{
		LoadColumns();
    }
    QListViewItem::setOpen( o );
}

void classPrimaryKeys::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}

void classPrimaryKeys::LoadColumns()
{
}

void classPrimaryKeys::selectionChanged( QListViewItem * )
{
}

