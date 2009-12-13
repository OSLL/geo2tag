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
#ifndef classSpecialColumns_included
#define classSpecialColumns_included

#include <qstring.h>
#include <sqlext.h>
#include <qmessagebox.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include "classNode.h"
#include "classColumn.h"

class classSpecialColumns: public classNode
{
public:
    classSpecialColumns( QListView 		*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0 );
    classSpecialColumns( QListViewItem 	*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0 );
    classSpecialColumns( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszTable = 0 );
	~classSpecialColumns();

    void setOpen( bool );
    void setup();
	void selectionChanged( QListViewItem * );

private:
	QList<classColumn>	listColumns;
	SQLHDBC				hDbc;
	QString				qsTable;

	void Init( SQLHDBC hDbc, char *pszTable );
	void LoadColumns();

};

#endif

