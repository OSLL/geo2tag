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
#ifndef classDataSources_included
#define classDataSources_included

#include <qpixmap.h>
#include <ini.h>
#include <odbcinst.h>
#include <sqlext.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif
#include "classNode.h"
#include "classDataSource.h"

class classDataSources: public classNode
{
public:
    classDataSources( QListView 		*pParent, classCanvas *pCanvas, int nDataSourceType = User, SQLHENV hEnv = 0 );
    classDataSources( QListViewItem 	*pParent, classCanvas *pCanvas, int nDataSourceType = User, SQLHENV hEnv = 0 );
    classDataSources( QListViewItem 	*pParent, QListViewItem *pAfter, classCanvas *pCanvas, int nDataSourceType = User, SQLHENV hEnv = 0 );
	~classDataSources();

	enum DataSourceTypes
	{
		User,
		System
	};
    void setOpen( bool bOpen );
    void setup();
	void selectionChanged( QListViewItem * );

private:
	QList<classDataSource> listDataSources;
	int 	nDataSourceType;
	SQLHENV hEnv;

	void Init( int nDataSourceType, SQLHENV hEnv );
};

#endif

