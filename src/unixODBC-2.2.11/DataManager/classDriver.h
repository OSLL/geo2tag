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
#ifndef classDriver_included
#define classDriver_included

#include <qstring.h>
#include <ini.h>
#include <odbcinst.h>
#include <sqlext.h>
#include "classNode.h"

class classDriver: public classNode
{
public:
    classDriver( QListView 		*pParent, classCanvas *pCanvas,	char *pszDriverName = 0, SQLHENV	hEnv = 0 );
    classDriver( QListViewItem 	*pParent, classCanvas *pCanvas,	char *pszDriverName = 0, SQLHENV	hEnv = 0 );
    classDriver( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas,	char *pszDriverName = 0, SQLHENV	hEnv = 0 );
	~classDriver();

    void setOpen( bool );
    void setup();
	void selectionChanged ( QListViewItem * );

private:
	QString	qsDriverName;
	SQLHENV	hEnv;

	void Init( char *pszDriverName, SQLHENV hEnv );

};

#endif

