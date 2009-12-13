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
#ifndef classDataSource_included
#define classDataSource_included

#include <qstring.h>
#include <qmessagebox.h>
#include <ini.h>
#include <odbcinst.h>
#include <sqlext.h>
#include <qpixmap.h>
#include "classNode.h"
#include "classLogin.h"
#include "classTables.h"
#include "classConnectionFrame.h"

class classDataSource: public classNode
{
public:
    classDataSource( QListView 		*pParent, classCanvas *pCanvas, int nDataSourceType = User,	char *pszDataSourceName = 0, SQLHENV	hEnv = 0 );
    classDataSource( QListViewItem 	*pParent, classCanvas *pCanvas, int nDataSourceType = User,	char *pszDataSourceName = 0, SQLHENV	hEnv = 0 );
    classDataSource( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, int nDataSourceType = User,	char *pszDataSourceName = 0, SQLHENV	hEnv = 0 );
	~classDataSource();

	enum DataSourceTypes
	{
		User,
		System
	};
    void setOpen( bool bOpen );
    void setup();
	void selectionChanged ( QListViewItem * );

private:
	classTables *           pTables;
	classConnectionFrame *  pConnectionFrame;
	bool	                bLoggedIn;
	QString	                qsDataSourceName;
	int 	                nDataSourceType;
	SQLHENV	                hEnv;
	SQLHDBC	                hDbc;

	void Init( int nDataSourceType, char *pszDataSourceName, SQLHENV hEnv );
};

#endif

