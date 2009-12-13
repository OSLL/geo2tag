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

#include "classIndexs.h"
#include "classODBC.h"  // my_msgBox
#include <qpixmap.h>
#include "keysilver2.xpm"

classIndexs::classIndexs( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc, const char *pszTable, const char *pszLibrary )
    : classNode( pParent, pAfter, pCanvas ), hDbc( hDbc ), qsTable( pszTable ), qsLibrary( pszLibrary )
{
  setText( 0, "Indexs" );
  setText( 1, "" );
  setText( 2, "" );
  listIndexs.setAutoDelete( TRUE );
  setPixmap( 0, QPixmap( keysilver2_xpm ) );
  setExpandable( TRUE );
}

void classIndexs::setOpen( bool bOpen )
{
  if ( bOpen ) // Always refresh
    LoadIndexs();
  else
    listIndexs.clear() ;

  QListViewItem::setOpen( bOpen );
}

void classIndexs::LoadIndexs()
{
  CursorScoper s(listView()) ;
  SQLRETURN    nReturn ;
  SQLCHAR      szIndexName[MAX_COLUMN_WIDTH];
  SQLCHAR      szColumnName[MAX_COLUMN_WIDTH];
  SQLCHAR      szUnique[MAX_COLUMN_WIDTH];
  int          bUnique;
  SQLLEN       nIndicator;
  QString      qsDesc ;
  classColumn  *pColumn = NULL ;

  // CREATE A STATEMENT
  StatementScoper stmt( hDbc ) ; if ( !stmt() ) return ;

  // EXECUTE OUR SQL/CALL
  if ( !SQL_SUCCEEDED(nReturn=SQLStatistics( stmt(), 0, 0, (SQLCHAR*)qsLibrary.data(), SQL_NTS, (SQLCHAR*)qsTable.data(), SQL_NTS, 0, 0 ) ) )
    return my_msgBox( "classIndexs", "SQLStatistics", nReturn, NULL, NULL, stmt() ) ;

  // GET RESULTS
  while ( SQL_SUCCEEDED( SQLFetch( stmt() ) ) )
  {
    nReturn = SQLGetData( stmt(), 9, SQL_C_CHAR, &szColumnName[0], sizeof(szColumnName), &nIndicator );
    if ( !SQL_SUCCEEDED(nReturn) || nIndicator == SQL_NULL_DATA )
      strcpy( (char *)szColumnName, "Unknown" );

    nReturn = SQLGetData( stmt(), 4, SQL_C_LONG, &bUnique, sizeof(bUnique), &nIndicator );
    if ( !SQL_SUCCEEDED(nReturn) || nIndicator == SQL_NULL_DATA || !bUnique )
      strcpy( (char *)szUnique, "NON-UNIQUE" );
    else
      strcpy( (char *)szUnique, "UNIQUE" );

    nReturn = SQLGetData( stmt(), 6, SQL_C_CHAR, &szIndexName[0], sizeof(szIndexName), &nIndicator );
    if ( !SQL_SUCCEEDED(nReturn) || nIndicator == SQL_NULL_DATA )
      strcpy( (char *)szIndexName, "Unknown" );

    qsDesc.sprintf("Column=%s %s", QString((char*)szColumnName).stripWhiteSpace().data(), (char*)szUnique ) ;
    listIndexs.append( pColumn = new classColumn( this, pColumn, pCanvas, hDbc, QString((char*)szIndexName).stripWhiteSpace(), "INDEX", qsDesc ) );
  }
}

