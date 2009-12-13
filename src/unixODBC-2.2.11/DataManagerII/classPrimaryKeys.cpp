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

#include "classPrimaryKeys.h"
#include "classODBC.h"
#include <qpixmap.h>
#include "keyred.xpm"

classPrimaryKeys::classPrimaryKeys( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc, const char *pszTable, const char *pszLibrary )
    : classNode( pParent, pAfter, pCanvas ) , hDbc( hDbc ), qsTable( pszTable ), qsLibrary( pszLibrary )
{
  setText( 0, "PrimaryKeys" );
  setText( 1, "" );
  setText( 2, "" );
  listColumns.setAutoDelete( TRUE );
  setPixmap( 0, QPixmap( keyred_xpm ) );
  setExpandable( TRUE );
}

void classPrimaryKeys::setOpen( bool bOpen )
{
  if ( bOpen ) // Always refresh
    LoadColumns();
  else
    listColumns.clear() ;

  QListViewItem::setOpen( bOpen );
}

void classPrimaryKeys::LoadColumns()
{
  CursorScoper s(listView()) ;
  SQLRETURN    nReturn ;
  SQLCHAR      szColumnName[MAX_COLUMN_WIDTH];
  SQLCHAR      szKeyName[MAX_COLUMN_WIDTH];
  QString      qsDesc ;
  classColumn  *pColumn = NULL ;

  // CREATE A STATEMENT
  StatementScoper stmt( hDbc ) ; if ( !stmt() ) return ;

  // EXECUTE OUR SQL/CALL
  if (!SQL_SUCCEEDED(nReturn=SQLPrimaryKeys( stmt(), 0, 0, (SQLCHAR*)qsLibrary.data(), SQL_NTS, (SQLCHAR*)qsTable.data(), SQL_NTS ) ) )
    return my_msgBox( "classPrimaryKeys", "SQLPrimaryKeys", nReturn, NULL, NULL, stmt() ) ;

  // GET RESULTS
  while (SQL_SUCCEEDED(SQLFetch( stmt() ) ) )
  {
    if (!SQL_SUCCEEDED(SQLGetData( stmt(), 4, SQL_C_CHAR, &szColumnName[0], sizeof(szColumnName), 0 ) ) )
      strcpy( (char *)szColumnName, "Unknown" );

    if (!SQL_SUCCEEDED(SQLGetData( stmt(), 6, SQL_C_CHAR, &szKeyName[0], sizeof(szKeyName), 0 ) ) )
      strcpy( (char *)szKeyName, "Unknown" );

    qsDesc.sprintf("Desc=%s", QString((char*)szKeyName).stripWhiteSpace().data() ) ;
    listColumns.append( pColumn = new classColumn( this, pColumn, pCanvas, hDbc, QString((char*)szColumnName).stripWhiteSpace(), "KEY", qsDesc ) );
  }
}

