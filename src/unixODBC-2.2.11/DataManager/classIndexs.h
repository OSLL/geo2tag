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
#ifndef classIndexs_included
#define classIndexs_included

#include <qstring.h>
#include <sqlext.h>
#include <qmessagebox.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include "classNode.h"
#include "classIndex.h"


class classIndexs: public classNode
{
public:
    classIndexs( QListView 		*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0 );
    classIndexs( QListViewItem 	*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0 );
    classIndexs( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0 );
	~classIndexs();

    void setOpen( bool );
    void setup();
	void selectionChanged( QListViewItem * );

private:
	QList<classIndex>	listIndexs;
	SQLHDBC				hDbc;
	QString				qsTable;

	void Init( SQLHDBC hDbc, char *pszTable );
	void LoadIndexs();

};

#endif

