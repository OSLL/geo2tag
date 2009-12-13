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
#ifndef classTables_included
#define classTables_included

#include <stdio.h>
#include <qstring.h>
#include <sqlext.h>
#include <qmessagebox.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include "classNode.h"
#include "classTable.h"

class classTables: public classNode
{
public:
    classTables( QListView 		*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0 );
    classTables( QListViewItem 	*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0 );
    classTables( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc = 0 );
	~classTables();

    void setOpen( bool bOpen );
    void setup();
	void selectionChanged( QListViewItem * );

private:
	QList<classTable> listTables;
	SQLHDBC	hDbc;

	void Init( SQLHDBC hDbc );
	void LoadTables();

};

#endif

