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
#ifndef classNode_included
#define classNode_included

#include <qlistview.h>
#include <qmessagebox.h>

#include "classCanvas.h"

class classNode: public QListViewItem
{
public:
    classNode( QListView 		*pParent, classCanvas *pCanvas );
    classNode( QListViewItem 	*pParent, classCanvas *pCanvas );
    classNode( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas );
    ~classNode();

	virtual void Init( classCanvas *pCanvas );
    virtual void setOpen( bool );
    virtual void setup();
	virtual void selectionChanged ( QListViewItem * );

protected:
	classCanvas	*pCanvas;
	
};

#endif

