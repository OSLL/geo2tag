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
#ifndef classColumn_included
#define classColumn_included

#include <qstring.h>
#include <sqlext.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include "classNode.h"

class classColumn: public classNode
{
public:
    classColumn( QListView 		*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszName = 0, char *pszType = 0, char *pszDesc = 0 );
    classColumn( QListViewItem 	*pParent, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszName = 0, char *pszType = 0, char *pszDesc = 0 );
    classColumn( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHDBC hDbc = 0, char *pszName = 0, char *pszType = 0, char *pszDesc = 0 );
	~classColumn();

    void setOpen( bool );
    void setup();
	void selectionChanged ( QListViewItem * );

private:
	SQLHDBC	hDbc;
	QString	qsName;

	void Init( SQLHDBC hDbc, char *pszName, char *pszType, char *pszDesc );
};

#endif

