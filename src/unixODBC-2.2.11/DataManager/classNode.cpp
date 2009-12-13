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
#include "classNode.h"

classNode::classNode( QListView *pParent, classCanvas *pCanvas )
    : QListViewItem( pParent )
{
	Init( pCanvas);
}

classNode::classNode( QListViewItem *pParent, classCanvas *pCanvas )
    : QListViewItem( pParent )
{
	Init( pCanvas );
}

classNode::classNode( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas )
    : QListViewItem( pParent, pAfter )
{
	Init( pCanvas );
}

classNode::~classNode()
{
}

void classNode::Init( classCanvas *pCanvas )
{
	this->pCanvas = pCanvas;
	setText( 0, "Object" );
	setText( 1, "Type" );
	setText( 2, "Description" );
}

void classNode::setOpen( bool o )
{
    if ( o && !childCount() )
	{
    }
    QListViewItem::setOpen( o );
}

void classNode::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}

void classNode::selectionChanged ( QListViewItem * )
{
}





