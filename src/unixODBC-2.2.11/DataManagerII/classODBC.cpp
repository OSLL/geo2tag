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

#include "classODBC.h"
#include <qpixmap.h>
#include <classDrivers.h>
#include <classDataSources.h>
#include "ODBC.xpm"

// ***********************************
//   Common Utilities
// ***********************************

void my_msgBox( const QString & className,
             const QString & actionName,
             SQLRETURN rc,
             SQLHENV  hEnv,
             SQLHDBC  hDbc,
             SQLHSTMT hStmt,
             const QString & moreInfo,
             QMessageBox::Icon icon )
{
  QString caption = "DataManager - " + className + " - " + actionName ;
  QString text = actionName ;

  if ( rc )
    text += " failed, rc=" + QString::number(rc) ;

  if ( hEnv || hDbc || hStmt )
  {
    char szState[SQL_SQLSTATE_SIZE+1]    ; szState[0] = 0 ;
    char szBuf[SQL_MAX_MESSAGE_LENGTH+1] ; szBuf[0]   = 0 ;
    SQLINTEGER  sqlCode = 0;
    SQLSMALLINT length  = 0 ;
    int nRec = 0 ;
    while (SQL_SUCCEEDED(SQLGetDiagRec( hStmt ? SQL_HANDLE_STMT : hDbc ? SQL_HANDLE_DBC : SQL_HANDLE_ENV,
                                       hStmt ? hStmt           : hDbc ? hDbc           : hEnv,
                                       ++nRec,
                                       (SQLCHAR*)szState,
                                       &sqlCode,
                                       (SQLCHAR*)szBuf,
                                       sizeof(szBuf),
                                       &length) ) )
      text += QString().sprintf("\n[%d] SQLSTATE:%s SQLCODE:%d %s", nRec, szState, sqlCode, szBuf) ;
  }

  if ( !moreInfo.isEmpty() )
    text += "\n\nInfo:" + moreInfo ;

  QMessageBox(caption, text, icon, QMessageBox::Ok | QMessageBox::Default | QMessageBox::Escape, QMessageBox::NoButton, QMessageBox::NoButton ).exec() ;
}

// ***********************************
// ***********************************

classODBC::classODBC( QListView *pParent, classCanvas *pCanvas )
    : classNode( pParent, pCanvas ) , pDrivers ( NULL ) , pDataSourcesUser( NULL ) , pDataSourcesSystem( NULL )
{
  setPixmap( 0, QPixmap(xpmODBC) );
  setText( 0, "ODBC" );
  setText( 1, "" );
  setText( 2, "Open Database Connectivity" );
  setExpandable( TRUE );
}

void classODBC::setOpen( bool bOpen )
{
  if ( bOpen && !childCount() ) // Only create item once
  {
    // ADD CHILD NODES; only classODBC knows what they may be
    pDrivers           = new classDrivers    ( this, NULL,               pCanvas         );
    pDataSourcesSystem = new classDataSources( this, pDrivers,           pCanvas, System );
    pDataSourcesUser   = new classDataSources( this, pDataSourcesSystem, pCanvas, User   );
  }

  QListViewItem::setOpen( bOpen );
}

void classODBC::selectionChanged( QListViewItem *p )
{
  if ( pDataSourcesUser )   pDataSourcesUser->selectionChanged( p );
  if ( pDataSourcesSystem ) pDataSourcesSystem->selectionChanged( p );
}

