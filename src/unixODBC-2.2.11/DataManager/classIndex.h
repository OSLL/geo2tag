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
#ifndef classIndex_included
#define classIndex_included

#include <qptrlist.h>
#include <qstring.h>
#include <sqlext.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include "classNode.h"
#include "classColumn.h"

class classIndex: public classNode
{
public:
    classIndex( QListView 		*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0, char *pszIndex = 0, char *pszDesc = 0 );
    classIndex( QListViewItem 	*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0, char *pszIndex = 0, char *pszDesc = 0 );
    classIndex( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0, char *pszIndex = 0, char *pszDesc = 0 );
	~classIndex();

    void setOpen( bool );
    void setup();
	void selectionChanged( QListViewItem * );

private:
	SQLHDBC	hDbc;
	QString	qsTable;
	QString	qsIndex;
	QList<classColumn>	listColumns;

	void Init( SQLHDBC hDbc, char *pszTable, char *pszIndex, char *pszDesc );
	void LoadColumns();
};

#endif

