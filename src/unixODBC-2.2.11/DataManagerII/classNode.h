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
#ifndef classNode_included
#define classNode_included

#include "classCanvas.h"
#include <qlistview.h>

class classNode: public QListViewItem
{
public:
    classNode( QListView     *pParent,                        classCanvas *pCanvas );
    classNode( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas );
   ~classNode() {}

protected:
    classCanvas *pCanvas;

};
#endif

