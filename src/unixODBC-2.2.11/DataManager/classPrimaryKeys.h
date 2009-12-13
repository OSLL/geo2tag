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
#ifndef classPrimaryKeys_included
#define classPrimaryKeys_included

#include <qstring.h>
#include <sqlext.h>
#include <qmessagebox.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include "classNode.h"

class classPrimaryKeys: public classNode
{
public:
    classPrimaryKeys( QListView 		*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0 );
    classPrimaryKeys( QListViewItem 	*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0 );
    classPrimaryKeys( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0 );
	~classPrimaryKeys();

    void setOpen( bool );
    void setup();
	void selectionChanged( QListViewItem * );

private:
	SQLHDBC	hDbc;
	QString	qsTable;

	void Init( SQLHDBC hDbc, char *pszTable );
	void LoadColumns();

};

#endif

