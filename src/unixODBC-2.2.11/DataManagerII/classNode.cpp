/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey         - pharvey@codebydesign.com
 **************************************************/

#include "classNode.h"
#include "classCanvas.h"
#include <qlistview.h>

classNode::classNode( QListView *pParent, classCanvas *pCanvas )
    : pCanvas( pCanvas) , QListViewItem( pParent )
{
}

classNode::classNode( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas )
    : pCanvas( pCanvas) , QListViewItem( pParent, pAfter )
{
}

