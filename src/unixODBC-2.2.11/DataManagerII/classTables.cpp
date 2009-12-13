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

#include "classTables.h"
#include "classODBC.h"
#include "tables.xpm"

classTables::classTables( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc, const char *szLibrary )
    : classNode( pParent, pAfter, pCanvas ) , hDbc( hDbc ) , qsLibrary( szLibrary )
{
  setText( 0, qsLibrary.isEmpty() ? "Tables" : (const char*)qsLibrary );
  setText( 1, "LIBRARY  " );
  setText( 2, "" );
  listTables.setAutoDelete( TRUE );
  setPixmap( 0, QPixmap( tables_xpm ) );
  setExpandable( TRUE );
}

void classTables::setOpen( bool bOpen )
{
  if ( bOpen )
    LoadTables();
  else
    listTables.clear();

  QListViewItem::setOpen( bOpen );
}

void classTables::LoadTables()
{
  CursorScoper s(listView()) ;
  SQLRETURN    nReturn ;
  SQLCHAR      szTableName[MAX_COLUMN_WIDTH];
  SQLCHAR      szTableType[MAX_COLUMN_WIDTH];
  SQLCHAR      szTableRemarks[MAX_COLUMN_WIDTH];
  SQLLEN       nIndicatorName;
  SQLLEN       nIndicatorType;
  SQLLEN       nIndicatorRemarks;
  classTable   *pTable = NULL;

  // CREATE A STATEMENT
  StatementScoper stmt( hDbc ) ; if ( !stmt() ) return ;

  // EXECUTE OUR SQL/CALL
  if (!SQL_SUCCEEDED(nReturn=SQLTables( stmt(), 0, 0, (SQLCHAR*)qsLibrary.data(), SQL_NTS, 0, 0, 0, 0 ) ) )
    return my_msgBox( "classTables", "SQLTables", nReturn, NULL, NULL, stmt() ) ;

  SQLBindCol( stmt(), SQLTables_TABLE_NAME, SQL_C_CHAR, szTableName,    sizeof(szTableName),    &nIndicatorName    );
  SQLBindCol( stmt(), SQLTables_TABLE_TYPE, SQL_C_CHAR, szTableType,    sizeof(szTableType),    &nIndicatorType    );
  SQLBindCol( stmt(), SQLTables_REMARKS,    SQL_C_CHAR, szTableRemarks, sizeof(szTableRemarks), &nIndicatorRemarks );

  // GET RESULTS
  while ( SQL_SUCCEEDED(SQLFetch( stmt() ) ) )
  {
    if ( nIndicatorName == SQL_NULL_DATA )
      listTables.append( pTable = new classTable( this, pTable, pCanvas, hDbc, "Unknown" ) );
    else
      listTables.append( pTable = new classTable( this, pTable, pCanvas, hDbc, QString((char*)szTableName).stripWhiteSpace(), QString((char*)szTableType).stripWhiteSpace(), QString((char*)szTableRemarks).stripWhiteSpace(), qsLibrary ) );
  }
}

void classTables::selectionChanged( QListViewItem *p )
{
  for ( classTable *pTable = listTables.first(); pTable != 0; pTable = listTables.next() )
    pTable->selectionChanged( p );
}

