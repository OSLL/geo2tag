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

#include "classTable.h"
#include "classODBC.h"
#include <qpixmap.h>
#include "table.xpm"

classTable::classTable( QListViewItem *pParent, QListViewItem *pAfter, classCanvas *pCanvas, SQLHENV hDbc, const char *pszTable, const char *pszType, const char *pszDescription, const char* pszLibrary )
  : classNode( pParent, pAfter, pCanvas ), hDbc( hDbc ), qsTable( pszTable ), qsLibrary( pszLibrary ), pPrimaryKeys( NULL ), pIndexs( NULL ), pSpecialColumns( NULL ), pBrowse( NULL )
{
  setText( 0, pszTable       );
  setText( 1, pszType        );
  setText( 2, pszDescription );
  listColumns.setAutoDelete( TRUE );
  setPixmap( 0, QPixmap( table_xpm ) );
  setExpandable( TRUE );
}

classTable::~classTable()
{
  Fini();
}

void classTable::Fini()
{
  listColumns.clear();
  delete pPrimaryKeys;
  delete pIndexs;
  delete pSpecialColumns;
  delete pBrowse;
  pPrimaryKeys    = NULL ;
  pIndexs         = NULL ;
  pSpecialColumns = NULL ;
  pBrowse         = NULL ;
}

void classTable::setOpen( bool bOpen )
{
  if ( bOpen ) // Always refresh
  {
    LoadColumns();
    pSpecialColumns = new classSpecialColumns( this, NULL,            pCanvas, hDbc, qsTable, qsLibrary );
    pPrimaryKeys    = new classPrimaryKeys(    this, pSpecialColumns, pCanvas, hDbc, qsTable, qsLibrary );
    pIndexs         = new classIndexs(         this, pPrimaryKeys,    pCanvas, hDbc, qsTable, qsLibrary );
    selectionChanged( this ) ;
  }
  else
  {
    Fini();
  }

  QListViewItem::setOpen( bOpen );
}

void classTable::LoadColumns()
{
  CursorScoper s(listView()) ;
  SQLRETURN    nReturn ;
  SQLCHAR      szColumnName[MAX_COLUMN_WIDTH];
  SQLCHAR      szColumnType[MAX_COLUMN_WIDTH];
  SQLCHAR      szColumnSize[MAX_COLUMN_WIDTH];
  SQLCHAR      szRemarks   [MAX_COLUMN_WIDTH];
  classColumn  *pColumn = NULL;
  QString      qsDesc ;

  // CREATE A STATEMENT
  StatementScoper stmt( hDbc ) ; if ( !stmt() ) return ;

  // EXECUTE OUR SQL/CALL
  if (!SQL_SUCCEEDED(nReturn=SQLColumns( stmt(), 0, 0, (SQLCHAR*)qsLibrary.data(), SQL_NTS, (SQLCHAR*)qsTable.data(), SQL_NTS, 0, 0 ) ) )
    return my_msgBox( "classTable", "SQLColumns", nReturn, NULL, NULL, stmt() ) ;

  // GET RESULTS
  while (SQL_SUCCEEDED(SQLFetch( stmt() ) ) )
  {
    if (!SQL_SUCCEEDED(SQLGetData( stmt(), SQLColumns_COLUMN_NAME,   SQL_C_CHAR, &szColumnName[0], sizeof(szColumnName), 0 ) ) )
      strcpy( (char *)szColumnName, "Unknown" );

    if (!SQL_SUCCEEDED(SQLGetData( stmt(), SQLColumns_TYPE_NAME,     SQL_C_CHAR, &szColumnType[0], sizeof(szColumnType), 0 ) ) )
      strcpy( (char *)szColumnType, "Unknown" );

    if (!SQL_SUCCEEDED(SQLGetData( stmt(), SQLColumns_COLUMN_SIZE,   SQL_C_CHAR, &szColumnSize[0], sizeof(szColumnSize), 0 ) ) )
      strcpy( (char *)szColumnSize, "Unknown" );

    if (!SQL_SUCCEEDED(SQLGetData( stmt(), SQLColumns_REMARKS,       SQL_C_CHAR, &szRemarks[0],    sizeof(szRemarks),    0 ) ) )
      strcpy( (char *)szRemarks, "Unknown" );

    qsDesc.sprintf("Len=%s Desc=%s", QString((char*)szColumnSize).stripWhiteSpace().data(), QString((char*)szRemarks).stripWhiteSpace().data() ) ;
    listColumns.append( pColumn = new classColumn( this, pColumn, pCanvas, hDbc, QString((char*)szColumnName).stripWhiteSpace(), QString((char*)szColumnType).stripWhiteSpace(), QString(qsDesc).stripWhiteSpace() ) ) ;
  }
}

void classTable::selectionChanged( QListViewItem *p )
{
  // Determine if the selected item is a child of this table using the y coord, is there a better way?
  QListViewItem *n = nextSibling() ;
  int siblingPos = n ? n->itemPos() : 999999 ;
  int selectPos  = p ? p->itemPos() : 0      ;
  int thisPos    =        itemPos()          ;

  if ( thisPos <= selectPos && selectPos < siblingPos )
  {
    if ( !pBrowse )
      pBrowse = new classBrowseFrame( hDbc, qsTable, qsLibrary, pCanvas );
    pBrowse->show();
  }
  else
  {
    if ( pBrowse )
      pBrowse->hide();
  }
}

