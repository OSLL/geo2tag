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
#ifndef classDrivers_included
#define classDrivers_included

#include <ini.h>
#include <odbcinst.h>
#include <sqlext.h>
#include <qptrlist.h>
#include "classNode.h"
#include "classDriver.h"

class classDrivers: public classNode
{
public:
    classDrivers( QListView 		*pParent, classCanvas *pCanvas, SQLHENV hEnv = 0 );
    classDrivers( QListViewItem 	*pParent, classCanvas *pCanvas, SQLHENV hEnv = 0 );
    classDrivers( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hEnv = 0 );
	~classDrivers();

    void setOpen( bool );
    void setup();
	void selectionChanged ( QListViewItem * );

private:
	QList<classDriver> listDrivers;
	SQLHENV	hEnv;

	void Init( SQLHENV	hEnv );
};

#endif

