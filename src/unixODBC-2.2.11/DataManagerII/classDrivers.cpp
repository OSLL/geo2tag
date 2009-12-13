/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey         - pharvey@codebydesign.com
 **************************************************/

#include "classDrivers.h"
#include "classODBC.h"
#include <qpixmap.h>
#include <sqlext.h>
#include "drivers.xpm"

classDrivers::classDrivers( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas )
    : classNode( pParent, pAfter, pCanvas )
{
  listDrivers.setAutoDelete( TRUE );
  setText( 0, "Drivers" );
  setText( 1, "" );
  setText( 2, "" );
  setPixmap( 0, QPixmap( drivers_xpm ) );
  setExpandable( TRUE );
}

void classDrivers::setOpen( bool bOpen )
{
  SQLRETURN nReturn;
  SQLCHAR   szDriverDesc[1024];
  classDriver *pDriver = NULL ;

  /********************************
   * METHOD 1 - Using Driver Manager
   ********************************/
  if ( bOpen && !childCount() ) // Load only once
  {
    EnvironmentScoper env;
    nReturn = SQLDrivers( env(), SQL_FETCH_FIRST, szDriverDesc, sizeof(szDriverDesc)-1, 0, 0, -1, 0 );
    while ( nReturn == SQL_SUCCESS )
    {
      listDrivers.append( pDriver = new classDriver( this, pDriver, pCanvas, (char*)szDriverDesc ) );
      nReturn = SQLDrivers( env(), SQL_FETCH_NEXT, szDriverDesc, sizeof(szDriverDesc)-1, 0, 0, -1, 0 );
    }
  }

  /********************************
   * METHOD 2 - Using odbcinst
   ********************************/
  /*
  int  nElement;
  char szResults[9600];
  char szObjectName[INI_MAX_OBJECT_NAME+1];

  if ( bOpen && !childCount() )
  {
    // ADD CHILD NODES; only classDataSources knows what they may be
    szResults[0] = '\0';
    if ( SQLGetInstalledDrivers( szResults, 9600, 0 ) == TRUE )
    {
       for ( nElement = 0; iniElement( szResults, '\0', '\0', nElement, szObjectName, INI_MAX_OBJECT_NAME ) == INI_SUCCESS; nElement++ )
         listDrivers.append( new classDriver( this, pCanvas, szObjectName, hEnv ) );
    }
  }
  */

  QListViewItem::setOpen( bOpen );
}

