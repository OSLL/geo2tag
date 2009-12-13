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
#include "classColumn.h"
#include "column.xpm"


classColumn::classColumn( QListView *pParent, classCanvas *pCanvas, SQLHDBC hDbc, char *pszName, char *pszType, char *pszDesc )
    : classNode( pParent, pCanvas )
{
	Init( hDbc, pszName, pszType, pszDesc );
}

classColumn::classColumn( QListViewItem *pParent, classCanvas *pCanvas, SQLHENV hDbc, char *pszName, char *pszType, char *pszDesc )
    : classNode( pParent, pCanvas )
{
	Init( hDbc, pszName, pszType, pszDesc );
}

classColumn::classColumn( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc, char *pszName, char *pszType, char *pszDesc )
    : classNode( pParent, pAfter, pCanvas )
{
	Init( hDbc, pszName, pszType, pszDesc );
}

classColumn::~classColumn()
{
}

void classColumn::Init( SQLHDBC hDbc, char *pszName, char *pszType, char *pszDesc )
{
	this->hDbc		= hDbc;
	this->qsName	= pszName;
	setText( 0, qsName );
	setText( 1, pszType );
	setText( 2, pszDesc );
	setPixmap( 0, QPixmap( column_xpm ) );
}

void classColumn::setOpen( bool o )
{
    if ( o && !childCount() )
	{
		setExpandable( FALSE );
    }
    QListViewItem::setOpen( o );
}

void classColumn::setup()
{
    setExpandable( FALSE );
    QListViewItem::setup();
}


void classColumn::selectionChanged( QListViewItem *p )
{
	if ( p == this )
	{
	}
}


