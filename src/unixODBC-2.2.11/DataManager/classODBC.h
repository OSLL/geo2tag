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
#ifndef classODBC_included
#define classODBC_included

#include <qmessagebox.h>
#include <sqlext.h>
#include "classNode.h"
#include "classDrivers.h"
#include "classDataSources.h"

class classODBC: public classNode
{
public:
    classODBC( QListView 		*pParent, classCanvas *pCanvas );
    classODBC( QListViewItem 	*pParent, classCanvas *pCanvas );
    ~classODBC();

    void setOpen( bool );
    void setup();
	void selectionChanged ( QListViewItem * );

private:
	SQLHENV				hEnv;
	classDrivers		*pDrivers;
	classDataSources	*pDataSourcesUser;
	classDataSources	*pDataSourcesSystem;

	void Init();
};

#endif

