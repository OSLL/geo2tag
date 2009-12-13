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

#include "classDriver.h"
#include <qpixmap.h>
#include <ini.h>
#include <odbcinst.h>
#include "driver.xpm"

classDriver::classDriver( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, const char *pszDriverName )
    : classNode( pParent, pAfter, pCanvas )
{
  char szResults[9600] ;
  char szPropertyValue[INI_MAX_PROPERTY_VALUE+1] ;
  szResults[0]        = 0 ;
  szPropertyValue[0]  = 0 ;

  if ( pszDriverName != 0 )
    qsDriverName = pszDriverName;

  setPixmap( 0, QPixmap( driver_xpm ) );
  setExpandable( FALSE );

  if ( SQLGetPrivateProfileString((char*) qsDriverName.data(), "Description", "", szResults, sizeof(szResults), "odbcinst" ) > 0 )
    iniElement( szResults, 0, 0, 0, szPropertyValue, INI_MAX_PROPERTY_VALUE );

  setText( 0, qsDriverName );
  setText( 1, "DRIVER  " );
  setText( 2, szPropertyValue );
}

