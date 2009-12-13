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

#include "classSpecialColumns.h"
#include "classODBC.h"
#include <qpixmap.h>
#include "id.xpm"

classSpecialColumns::classSpecialColumns( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc, const char *pszTable, const char *pszLibrary )
    : classNode( pParent, pAfter, pCanvas ) , hDbc( hDbc ), qsTable( pszTable ), qsLibrary( pszLibrary )
{
  setText( 0, "SpecialColumns" );
  setText( 1, "" );
  setText( 2, "" );
  listColumns.setAutoDelete( TRUE );
  setPixmap( 0, QPixmap( id_xpm ) );
  setExpandable( TRUE );
}

void classSpecialColumns::setOpen( bool bOpen )
{
  if ( bOpen ) // Always refresh
    LoadColumns();
  else
    listColumns.clear() ;

  QListViewItem::setOpen( bOpen );
}

void classSpecialColumns::LoadColumns()
{
  CursorScoper s(listView()) ;
  SQLRETURN    nReturn ;
  SQLCHAR      szColumnName[MAX_COLUMN_WIDTH];
  SQLCHAR      szColumnType[MAX_COLUMN_WIDTH];
  SQLCHAR      szColumnSize[MAX_COLUMN_WIDTH];
  SQLCHAR      szRemarks   [MAX_COLUMN_WIDTH];
  QString      qsDesc ;
  classColumn  *pColumn = NULL ;

  // CREATE A STATEMENT
  StatementScoper stmt( hDbc ) ; if ( !stmt() ) return ;

  // EXECUTE OUR SQL/CALL
  if (!SQL_SUCCEEDED(nReturn=SQLSpecialColumns( stmt(), SQL_BEST_ROWID, 0, 0, (SQLCHAR*)qsLibrary.data(), SQL_NTS, (SQLCHAR*)qsTable.data(), SQL_NTS, SQL_SCOPE_SESSION, SQL_NULLABLE ) ) )
    return my_msgBox( "classSpecialColumns", "SQLSpecialColumns", nReturn, NULL, NULL, stmt() ) ;

  // GET RESULTS
  while (SQL_SUCCEEDED(SQLFetch( stmt() ) ) )
  {
    if (!SQL_SUCCEEDED(SQLGetData( stmt(), 2, SQL_C_CHAR, &szColumnName[0], sizeof(szColumnName), 0 ) ) )
      strcpy( (char *)szColumnName, "Unknown" );

    if (!SQL_SUCCEEDED(SQLGetData( stmt(), 4, SQL_C_CHAR, &szColumnType[0], sizeof(szColumnType), 0 ) ) )
      strcpy( (char *)szColumnType, "Unknown" );

    if (!SQL_SUCCEEDED(SQLGetData( stmt(), 5, SQL_C_CHAR, &szColumnSize[0], sizeof(szColumnSize), 0 ) ) )
      strcpy( (char *)szColumnSize, "Unknown" );

    qsDesc.sprintf("Len=%s", QString((char*)szColumnSize).stripWhiteSpace().data() ) ;
    listColumns.append( pColumn = new classColumn( this, pColumn, pCanvas, hDbc, QString((char*)szColumnName).stripWhiteSpace(), QString((char*)szColumnType).stripWhiteSpace(), qsDesc ) ) ;
  }
}

