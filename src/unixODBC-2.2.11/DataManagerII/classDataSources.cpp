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

#include "classDataSources.h"
#include "qpixmap.h"
#include <ini.h>
#include <odbcinst.h>
#include "datasourcesuser.xpm"
#include "datasourcessystem.xpm"

classDataSources::classDataSources( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, classODBC::DSType dataSourceType )
    : classNode( pParent, pAfter, pCanvas ), dataSourceType( dataSourceType )
{
  switch ( dataSourceType )
  {
    case classODBC::System :
      setText( 0, "System Data Sources" );
      setText( 1, "" );
      setText( 2, "" );
      setPixmap( 0, QPixmap( datasourcessystem_xpm ) );
    break;
    default:
      setText( 0, "User Data Sources" );
      setText( 1, "" );
      setText( 2, "" );
      setPixmap( 0, QPixmap( datasourcesuser_xpm ) );
   }

   pCanvas = pCanvas;
   listDataSources.setAutoDelete( TRUE );
   setExpandable( TRUE );
}

void classDataSources::setOpen( bool bOpen )
{
  if ( bOpen && !childCount() ) // Load only once
  {
    char szResults[9600];
    char szObjectName[INI_MAX_OBJECT_NAME+1];
    memset( szResults, 0, sizeof(szResults) );  // Results buffer must be cleared 
    classDataSource *pDataSource = NULL ;

    if ( dataSourceType == classODBC::User )
      SQLSetConfigMode( ODBC_USER_DSN );
    else
      SQLSetConfigMode( ODBC_SYSTEM_DSN );

    if ( SQLGetPrivateProfileString( 0, 0, 0, szResults, sizeof(szResults), 0 ) > 0 )
    {
      for ( int nElement = 0; iniElement( szResults, '\0', '\0', nElement, szObjectName, INI_MAX_OBJECT_NAME ) == INI_SUCCESS; nElement++ )
        listDataSources.append( pDataSource = new classDataSource( this, pDataSource, pCanvas, dataSourceType, szObjectName ) );
    }

    SQLSetConfigMode( ODBC_BOTH_DSN );
  }

  QListViewItem::setOpen( bOpen );
}

void classDataSources::selectionChanged( QListViewItem *p )
{
  classDataSource *pDataSource;

  for ( pDataSource = listDataSources.first(); pDataSource != 0; pDataSource = listDataSources.next() )
    pDataSource->selectionChanged( p );

}

