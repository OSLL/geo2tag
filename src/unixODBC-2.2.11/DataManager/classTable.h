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
#ifndef classTable_included
#define classTable_included

#include <stdio.h>

#include <qstring.h>
#include <sqlext.h>
#include <qmessagebox.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include <qpixmap.h>
#include "classNode.h"
#include "classColumn.h"
#include "classPrimaryKeys.h"
#include "classSpecialColumns.h"
#include "classIndexs.h"
#include "classBrowseFrame.h"

class classTable: public classNode
{
public:
    classTable( QListView 		*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszName = 0, char *pszType = 0, char *pszDescription = 0 );
    classTable( QListViewItem 	*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszName = 0, char *pszType = 0, char *pszDescription = 0 );
    classTable( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszName = 0, char *pszType = 0, char *pszDescription = 0 );
	~classTable();

    void setOpen( bool bOpen );
    void setup();
	void selectionChanged( QListViewItem * );

private:
	QList<classColumn>	listColumns;
	classPrimaryKeys	*pPrimaryKeys;
    classSpecialColumns	*pSpecialColumns;
	classIndexs			*pIndexs;
	SQLHDBC	            hDbc;
	QString	            qsName;
	classBrowseFrame    *pBrowse;

	void Init( SQLHDBC hDbc, char *pszName, char *pszType, char *pszDescription );
    void Fini();
	void LoadColumns();
};

#endif

