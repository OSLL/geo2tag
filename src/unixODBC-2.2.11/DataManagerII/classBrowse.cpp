/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 30.NOV.00
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey         - pharvey@codebydesign.com
 **************************************************/

#include "classBrowse.h"
#include "classODBC.h"
#include <qheader.h>
#include <qwhatsthis.h>
#include <qfiledialog.h>
#include <qvaluelist.h>
#include <qlayout.h>
#include <qpixmap.h>
#include "LinuxODBC.xpm"

#define MAX_ROWS_IN_TABLE 25000 // give a resonable number back

// WhatsThis help strings
char *szHelpQuery = \
"Use this area to quickly specify a simple data filter.\n\n" \
"1. Leave the Expression cells blank to get all data in the table or enter filter Expression(s) to limit the amount of data returned. Expression examples; \n='MyText'\n>23\n" \
"2. Enter (Y)es in the Show cells to get data in that column.\n" \
"3. Enter (A)scending or (D)escending in the Sort cells to get sorted column data.\n"\
"4. Control-Click and drag column headers to rearrange the column order.\n" \
"5. Click the Run button to get your results.\n" \
"NOTE: The max number of rows returned is 25000 and \n" \
"the max column size is 1024." ;

char *szHelpData = \
"This area contains the results from your Query. You can:\n\n" \
"1. Browse the data.\n" \
"2. Export the data in HTML format\n" \
"3. Delete rows of existing data\n" \
"4. Insert new rows of data\n" \
"5. Add a blank row for inserting later\n" \
"6. Copy an existing row for inserting later\n\n" \
"NOTE: The insert and delete operatoins are performed with\n" \
"the current columns shown in the result table\n" ;


char *szHelpStatus2 = \
"This area contains status messages.\n" ;

classBrowse::classBrowse( SQLHDBC hDbc, const QString& qsTable, const QString& qsLibrary, QWidget *parent, const char *name )
        : QWidget( parent, name, 0 ) , hDbc( hDbc ) , qsTable( qsTable ) , qsLibrary( qsLibrary ) , bAutoRefresh( true )
{
    // VERTICAL LAYOUT
    QVBoxLayout *layoutMain = new QVBoxLayout( this );

    // FILTER LABEL
    labelFilter = new QLabel( "FILTER" , this );
    labelFilter->setPalette( QPalette( QColor(lightGray) ) );
    labelFilter->setAlignment( AlignCenter );

    // FILTER TABLE
    tableQuery = new QTable( 3, 1, this );
    tableQuery->setColumnMovingEnabled ( TRUE );
    tableQuery->setMinimumSize( 78, 105 );
    tableQuery->setLeftMargin( 84 ) ;
    QHeader * headerQuery = tableQuery->verticalHeader();
    headerQuery->setLabel( 0, QString( tr("Expression") ));
    headerQuery->setLabel( 1, QString( tr("Show"      ) ));
    headerQuery->setLabel( 2, QString( tr("Sort"      ) ));

    // DATA LABEL
    labelResults = new QLabel( "RESULTS", this );
    labelResults->setPalette( QPalette( QColor(lightGray) ) );
    labelResults->setAlignment( AlignCenter );

    // DATA TABLE
    tableData = new myQTable( 0, 0, this );
    tableData->setSorting( TRUE ) ;
    tableData->setLeftMargin( 60 ) ;

    // STATUS LABEL
    QLabel *labelStatus = new QLabel( "STATUS", this );
    labelStatus->setAlignment( AlignCenter );

    // STATUS LISTBOX
    listStatus = new QListBox( this );
    listStatus->setSelectionMode( QListBox::NoSelection );

    // LAYOUT
    layoutMain->addWidget( labelFilter  );
    layoutMain->addWidget( tableQuery   );
    layoutMain->addWidget( labelResults );
    layoutMain->addWidget( tableData, 6 );
    layoutMain->addWidget( labelStatus  );
    layoutMain->addWidget( listStatus, 2);

    InitQuery();

    setMinimumSize( 50, 50 );
    setMaximumSize( 32767, 32767 );

    QWhatsThis::add( tableQuery,   szHelpQuery   );
    QWhatsThis::add( labelFilter,  szHelpQuery   );
    QWhatsThis::add( tableData,    szHelpData    );
    QWhatsThis::add( labelResults, szHelpData    );
    QWhatsThis::add( labelStatus,  szHelpStatus2 );
    QWhatsThis::add( listStatus,   szHelpStatus2 );
}

void classBrowse::addStatus( const QString &statusMsg )
{
  if ( listStatus->count() == 300 )
    listStatus->removeItem( 0 ) ;

  listStatus->insertItem( statusMsg ) ;
  listStatus->setBottomItem( listStatus->count()-1 ) ;
}

int classBrowse::GetSelectedRows( QString & qsRowsSelected, ListRows & listRows )
{
  listRows.clear() ;
  qsRowsSelected = "" ;
  int nRows = tableData->numRows();

  // Build a nice string to confirm the delete
  for (int r = 0 ; r < nRows ; r++ )
  {
    if ( tableData->isRowSelected( r, TRUE ) )
    {
      listRows += r ;
      qsRowsSelected += QString::number(r+1) + "," ;
    }
  }
  qsRowsSelected.remove( qsRowsSelected.length() - 1, 1 ) ; // cleanup last comma
  return listRows.count() ;
}

void classBrowse::AutoRefresh( bool b )
{
  bAutoRefresh = b ;
}

bool classBrowse::OkWithUser( const QString &shortDesc, const QString &qsQuestion )
{
  QMessageBox qBox("DataManager - classBrowse - " + shortDesc,    // caption
    qsQuestion,                                                   // text
    QMessageBox::Information,                                     // icon
    QMessageBox::Yes,                                             // button 1
    QMessageBox::No | QMessageBox::Escape | QMessageBox::Default, // button 2
    QMessageBox::NoButton ) ;                                     // button 3
  qBox.setIconPixmap( QPixmap( LinuxODBC_xpm ) );
  return QMessageBox::Yes == qBox.exec() ;
}

int classBrowse::RowAction( const QString &shortDesc, const QString &sql, const ListRows &listRows, SQLRETURN &nReturn )
{
  CursorScoper s(this) ;
  nReturn = SQL_SUCCESS ;
  int nRowsAffected = 0 ;
  int nCols = tableData->numCols();

  addStatus( QString().sprintf("%s: SQL=%s", shortDesc.upper().data(), sql.data() ) ) ;

  // LOOP THROUGH THE ROWS
  ListRows::ConstIterator it ;
  for (it = listRows.begin(); it != listRows.end() && SQL_SUCCEEDED(nReturn) ; it++ )
  {
    int nRow = *it ;
    addStatus( QString().sprintf("%s: row %d in progress...", shortDesc.upper().data(), nRow+1) ) ;

    // CREATE A STATEMENT
    StatementScoper stmt( hDbc ) ; if ( !stmt() ) return 0 ;

    // PREPARE
    if (!SQL_SUCCEEDED(nReturn=SQLPrepare( stmt(), (SQLCHAR*)sql.data(), SQL_NTS ) ) )
      my_msgBox( "classBrowse", "SQLPrepare", nReturn, NULL, NULL, stmt(), sql ) ;

    // BIND PARAMETERS
    // TODO might need to add call to SQLDescribeCols to get the host data type for the Bind call.
    //      most drivers ignore this unneeded field.
    QString *parmValues = new QString[ nCols ];

    for (int nCol = 0; nCol < nCols ; nCol++ )
    {
      parmValues[nCol] = tableData->text(nRow, nCol) ;
      if (!SQL_SUCCEEDED(nReturn=SQLBindParameter( stmt(), nCol+1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, parmValues[nCol].length(), 0, (void*)parmValues[nCol].data(), 0, 0 ) ) )
        my_msgBox( "classBrowse", "SQLBindParameter", nReturn, NULL, NULL, stmt(), sql ) ;
    }

    // EXECUTE
    if (!SQL_SUCCEEDED(nReturn=SQLExecute( stmt() ) ) )
      my_msgBox( "classBrowse", "SQLExecute", nReturn, NULL, NULL, stmt(), sql ) ;

    // GET NUMBER OF ROWS AFFECTED
    SQLLEN nRows = 0 ;
    if (!SQL_SUCCEEDED(SQLRowCount( stmt(), &nRows ) ) )
      nRows = 0 ;

    // DO WE NEED TO FETCH THE DATA TO COUNT THE ROWS?
    if ( nRows == SQL_NO_ROW_NUMBER )
      for ( nRows = 0 ; SQL_SUCCEEDED(SQLFetch(stmt())); nRows++ ) ;

    nRowsAffected += nRows;
  }

  if (SQL_SUCCEEDED(nReturn))
    addStatus( QString().sprintf("%s: %d row(s) completed", shortDesc.upper().data(), nRowsAffected) ) ;
  else
    addStatus( QString().sprintf("%s: %d row(s), ERROR rc=%d, %d row(s) affected", shortDesc.upper().data(), listRows.count(), nReturn, nRowsAffected) ) ;

  return nRowsAffected ;
}

bool classBrowse::CheckRowsAffected(const QString &shortDesc, const ListRows &listRows )
{
  // Build SQL statement with parameter markers
  // SELECT COL1, COL2 FROM LIBRARY.TABLE WHERE COL1=? AND COL2=?
  QString sql("SELECT ");
  int nCols = tableData->numCols();
  for (int nCol = 0; nCol < nCols ; nCol++ ) sql += tableData->horizontalHeader()->label(nCol) + (nCol != nCols-1 ? ", " : " ") ;
  sql += "FROM ";
  if (!qsLibrary.isEmpty())
    sql += qsLibrary + "." ;
  sql += qsTable + " WHERE ";
  for (int nCol = 0; nCol < nCols ; nCol++ ) sql += tableData->horizontalHeader()->label(nCol) + "=?" + (nCol != nCols-1 ? " AND " : " ") ;

  // LOOP THROUGH THE ROWS
  SQLRETURN nReturn ;
  ListRows::ConstIterator it ;
  for (it = listRows.begin(); it != listRows.end(); it++ )
  {
    ListRows singleRow ; singleRow += *it ;
    int nRowsAffected = RowAction( "check", sql, singleRow, nReturn ) ;

    // Did an error occur?
    if (!SQL_SUCCEEDED(nReturn) )
      return FALSE ;

    // Found a possible dup row, give the user at least one warning
    if ( nRowsAffected > 1 )
      return OkWithUser( shortDesc, QString().sprintf("WARNING! For row %d, the affected row count (%d) is greated then 1.\n", *it+1, nRowsAffected ) +
        "This usually means there are rows with duplicate data in them.\n"
        "ALL DUPLICATE ROWS WILL BE AFFECTED.\n\n"
        "Do you REALLY want to continue?" ) ;
  }
  return TRUE ;
}

void classBrowse::DeleteRows()
{
  QString qsRowsSelected ;
  ListRows listRows ;

  // No work to do?
  if (0 == GetSelectedRows( qsRowsSelected, listRows ))
    return ;

  // Confirm delete with user
  if (!OkWithUser( "table row delete", QString().sprintf("Do you want to delete the following %d row(s)?\nROWS: ", listRows.count()) + qsRowsSelected ))
    return ;

  // Double check the action to be sure user knows about duplicates that might be affected
  if (!CheckRowsAffected("table row delete", listRows ))
    return ;

  // Build SQL statement with parameter markers
  // DELETE FROM LIBRARY.TABLE WHERE COL1=? AND COL2=?
  QString sql("DELETE FROM ");
  if (!qsLibrary.isEmpty())
    sql += qsLibrary + "." ;
  sql += qsTable + " WHERE ";
  int nCols = tableData->numCols();
  for (int nCol = 0; nCol < nCols ; nCol++ ) sql += tableData->horizontalHeader()->label(nCol) + "=?" + (nCol != nCols-1 ? " AND " : " ") ;

  // Execute the statement
  SQLRETURN nReturn ;
  RowAction( "delete", sql, listRows, nReturn ) ;

  if ( SQL_SUCCEEDED(nReturn) && bAutoRefresh )
    Exec() ;
}

void classBrowse::InsertRows()
{
  QString qsRowsSelected ;
  ListRows listRows ;

  // No work to do?
  if (0 == GetSelectedRows( qsRowsSelected, listRows ))
    return ;

  // Confirm delete with user
  if (!OkWithUser( "table row insert", QString().sprintf("Do you want to insert the following %d row(s)?\nROWS: ", listRows.count()) + qsRowsSelected ))
    return ;

  // Build SQL statement with parameter markers
  // INSERT INTO LIBRARY.TABLE ( COL1, COL2 ) VALUES( ?, ? )
  QString sql("INSERT INTO ");
  if (!qsLibrary.isEmpty())
    sql += qsLibrary + "." ;
  sql += qsTable + " ( ";
  int nCols = tableData->numCols();
  for (int nCol = 0; nCol < nCols ; nCol++ ) sql += tableData->horizontalHeader()->label(nCol) + (nCol != nCols-1 ? ", " : " ") ;
  sql += ") VALUES ( ";
  for (int nCol = 0; nCol < nCols ; nCol++ ) sql += QString( "?" ) + (nCol != nCols-1 ? ", " : " ") ;
  sql += ") ";

  // Execute the statement
  SQLRETURN nReturn ;
  RowAction( "insert", sql, listRows, nReturn ) ;

  if ( SQL_SUCCEEDED(nReturn) && bAutoRefresh )
    Exec() ;
}

int classBrowse::AddRow()
{
  // Add the Row
  int nRows = tableData->numRows();
  tableData->setNumRows(nRows+1);
  tableData->verticalHeader()->setLabel( nRows, QString().sprintf("*%d", nRows+1) ) ;

  // Scroll down to the new row
  tableData->ensureCellVisible( nRows, 0 ) ;

  addStatus( QString().sprintf("ADD: row %d added", nRows+1) ) ;
  return nRows ;
}

int classBrowse::CopyRow()
{
  QString qsRowsSelected ;
  ListRows listRows ;

  // No work to do?
  if (1 != GetSelectedRows( qsRowsSelected, listRows )) return 0 ;

  // Add a new row
  int nRows = AddRow();

  // Copy the contents of the selected row
  int nCols = tableData->numCols();
  for (int nCol = 0; nCol < nCols ; nCol++ ) tableData->setText( nRows, nCol, tableData->text( *listRows.begin(), nCol) ) ;

  // Scroll down to the new copy
  tableData->ensureCellVisible( nRows, 0 ) ;

  addStatus( QString().sprintf("COPY: row %d copied to new row %d", *listRows.begin()+1, nRows+1) ) ;
  return nRows ;
}

void classBrowse::resizeEvent( QResizeEvent *p )
{
    resize( p->size() );
}

void classBrowse::Clear()         // CLEAR QUERY AND DATA
{
    ClearCells( tableData );
    InitQuery();
}

bool classBrowse::Save()        // DATA
{
    labelResults->setText("RESULTS" );

    if ( qsDataFileName.isEmpty() )
      return SaveAs();

    // TRY TO SAVE THE FILE
    QFile hFile( qsDataFileName );
    if ( !hFile.open( IO_WriteOnly ) )
    {
      my_msgBox( "classBrowse", "QFile.open", hFile.status(), NULL, NULL, NULL, qsDataFileName ) ;
      return FALSE;
    }
    WriteHTML( &hFile, TRUE );
    hFile.close();

    labelResults->setText( QString().sprintf("RESULTS (%s)", qsDataFileName.data() ) );
    addStatus( QString().sprintf("SAVE: results saved as %s", qsDataFileName.data() ) ) ;

    return TRUE;
}

bool classBrowse::SaveAs()      // DATA
{
    // LET USER PICK A FILE
    QString fileName = QFileDialog::getSaveFileName( qsDataFileName );
    if ( fileName.isEmpty() )
      return FALSE;
    else
    {
      qsDataFileName = fileName ;
      return Save();
    }
}

bool classBrowse::SaveSQL()        // SAVE QUERY
{
    labelFilter->setText( "FILTER (%s)" );

    QString fileName = QFileDialog::getSaveFileName( qsSQLFileName );
    if (fileName.isEmpty())
      return FALSE;

    // TRY TO SAVE THE FILE
    QFile hFile( fileName );
    if ( !hFile.open( IO_WriteOnly ) )
    {
      my_msgBox( "classBrowse", "QFile.open", hFile.status(), NULL, NULL, NULL, fileName ) ;
      return FALSE;
    }
    QString qsSQL;
    CreateSQL( qsSQL ) ;
    hFile.writeBlock( qsSQL.data(), qsSQL.length() ) ;
    hFile.close();

    qsSQLFileName = fileName ;
    labelFilter->setText( QString().sprintf("FILTER (%s)", qsSQLFileName.data() ) );
    addStatus( QString().sprintf("SAVE SQL: sql filter saved as %s", qsSQLFileName.data() ) ) ;

    return TRUE;
}

void classBrowse::InitQuery()
{
    SQLRETURN       nReturn ;
    SQLCHAR         szColumnName[MAX_COLUMN_WIDTH];
    SQLLEN          nCols             = 0;
    int             nCol              = 0;
    QHeader         *headerQuery = tableQuery->horizontalHeader();

    // CREATE A STATEMENT
    StatementScoper stmt( hDbc ) ; if ( !stmt() ) return ;

    // EXECUTE
    if (!SQL_SUCCEEDED(nReturn=SQLColumns( stmt(), 0, 0, (SQLCHAR*)qsLibrary.data(), SQL_NTS, (SQLCHAR*)qsTable.data(), SQL_NTS, 0, 0 ) ) )
      return my_msgBox( "classBrowse", "SQLColumns", nReturn, NULL, NULL, stmt() ) ;

    // GET NUMBER OF COLUMNS AFFECTED
    if (!SQL_SUCCEEDED(SQLRowCount( stmt(), &nCols ) ) )
      nCols = 0;

    // Default Value, since some drivers do not handle RowCount up front for performance reasons
    if ( nCols == SQL_NO_ROW_NUMBER )
      nCols = 1000; // give a resonable number back

    tableQuery->setNumCols( nCols );

    // GET RESULTS
    while ( SQL_SUCCEEDED(SQLFetch( stmt() ) )  && nCol < nCols )
    {
      if (!SQL_SUCCEEDED(SQLGetData( stmt(), SQLColumns_COLUMN_NAME, SQL_C_CHAR, &szColumnName[0], sizeof(szColumnName), 0 ) ) )
        strcpy( (char *)szColumnName, "Unknown" );

      headerQuery->setLabel( nCol, QString((char*)szColumnName).stripWhiteSpace() );
      nCol++;
    }

    // Show the correct number of columns to make the gui look correct
    tableQuery->setNumCols( nCol );

    SelectAllColumns();
}

void classBrowse::Exec()        // RUN QUERY
{
    CursorScoper s(this) ;
    SQLLEN      nRows;
    SWORD       nColumns;
    SQLRETURN   nReturn;
    QString     qsSQL ;

    if (!CreateSQL( qsSQL ) )
      return ;

    ClearCells( tableData );

    // CREATE A STATEMENT
    StatementScoper stmt( hDbc ) ; if ( !stmt() ) return ;

    // PREPATE
    if (!SQL_SUCCEEDED(nReturn=SQLPrepare(stmt(), (SQLCHAR*)qsSQL.data(), SQL_NTS) ) )
      return my_msgBox( "classBrowse", "SQLPrepare", nReturn, NULL, NULL, stmt(), qsSQL ) ;

    // EXECUTE
    addStatus( QString().sprintf( "RUN: filter sql=%s", qsSQL.data() ) ) ;
    if(!SQL_SUCCEEDED(nReturn=SQLExecute(stmt()) ) )
      return my_msgBox( "classBrowse", "SQLExecute", nReturn, NULL, NULL, stmt(), qsSQL ) ;

    // GET NUMBER OF ROWS AFFECTED
    if (!SQL_SUCCEEDED(SQLRowCount( stmt(), &nRows ) ) )
      nRows = 0;

    // Default Value, since some drivers do not handle RowCount up front for performance reasons
    if ( nRows == SQL_NO_ROW_NUMBER )
      nRows = MAX_ROWS_IN_TABLE; // give a resonable number back

    tableData->setNumRows( nRows );

    // GET NUMBER OF COLUMNS RETURNED
    if (!SQL_SUCCEEDED(SQLNumResultCols( stmt(), &nColumns ) ) )
      nColumns = 0;

    tableData->setNumCols( nColumns );

    // GET A RESULTS HEADER (column headers)
    ExecHeader( stmt(), nColumns );

    // GET A RESULTS BODY (data)
    nRows = ExecBody( stmt(), nColumns );

    // Show the correct number of rows to make the gui look correct
    tableData->setNumRows( nRows );

    // Update the status
    addStatus( QString().sprintf( "RUN: %d rows and %d columns affected", nRows, nColumns ) ) ;
}

void classBrowse::ExecHeader( SQLHSTMT hStmt, SWORD nColumns )
{
    for( int nCol = 0; nCol < nColumns; nCol++ )
    {
      SQLCHAR szColumnName[MAX_COLUMN_WIDTH]; szColumnName[0] = 0;
      SQLColAttribute( hStmt, nCol+1, SQL_DESC_LABEL, szColumnName, sizeof(szColumnName), 0, 0 );
      tableData->horizontalHeader()->setLabel( nCol, QString( (char*)szColumnName ).stripWhiteSpace() );
    }
}

int classBrowse::ExecBody( SQLHSTMT hStmt, SWORD nColumns )
{
  int nRow = 0;

  // PROCESS ALL ROWS
  while( SQL_SUCCEEDED(SQLFetch(hStmt) ) && nRow < MAX_ROWS_IN_TABLE )
  {
    nRow++;
    // PROCESS ALL COLUMNS
    for( int nCol = 1; nCol <= nColumns; nCol++ )
    {
      SQLLEN nIndicator;
      char   szData[MAX_COLUMN_WIDTH]; szData[0] = 0 ;
      SQLRETURN nReturn = SQLGetData( hStmt, nCol, SQL_C_CHAR, (SQLPOINTER)szData, sizeof(szData), &nIndicator );
      if ( SQL_SUCCEEDED(nReturn) && nIndicator != SQL_NULL_DATA )
        tableData->setText( nRow-1, nCol-1, QString( szData ).stripWhiteSpace() );
      else
        tableData->setText( nRow-1, nCol-1, " " );

      tableData->verticalHeader()->setLabel( nRow-1, QString::number( nRow ) ) ;
    }
  }
  return nRow ; // return the actual number of rows in the result set
}

bool classBrowse::CreateSQL( QString& qsSQL )
{
    int nCols = tableQuery->numCols();
    QString qsColumns ;
    QString qsWhereExpressions ;
    QString qsAnd ;
    QString qsComma1 ;
    QString qsComma2 ;
    QString qsSorts ;

    for (int nCol=0; nCol<nCols; nCol++ )
    {
        QString qsColumn     = tableQuery->horizontalHeader()->label( nCol );
        qsColumn.prepend('"').append('"') ;  // Quote column name in case it matches SQL keyword
        QString qsExpression = tableQuery->text( QUERY_ROW_EXPRESSION, nCol );
        QString qsShow       = tableQuery->text( QUERY_ROW_SHOW, nCol ).upper();
        QString qsSort       = tableQuery->text( QUERY_ROW_SORT, nCol ).upper();

        if ( qsShow.startsWith("Y") )
        {
            qsColumns += qsComma1;
            qsColumns += qsColumn;
            qsComma1 = ", ";
        }

        if ( !qsSort.isEmpty() )
        {
            qsSorts += qsComma2;
            qsSorts += qsColumn;
            qsSorts += qsSort.startsWith("D") ? " DESC " : " ASC " ;
            qsComma2 = ", ";
        }

        if ( !qsExpression.isEmpty() )
        {
            qsWhereExpressions += qsAnd;
            qsWhereExpressions += qsColumn;
            qsWhereExpressions += " ";
            qsWhereExpressions += qsExpression;
            qsAnd = " AND ";
        }
    }

    if ( qsColumns.isEmpty() )
    {
        QMessageBox::warning( (QWidget *)this, "Table browse", "No columns to show. Please specify which columns you want to show." );
        return false;
    }

    qsSQL = "SELECT ";
    qsSQL += qsColumns;
    qsSQL += " FROM ";
    if (!qsLibrary.isEmpty())
      qsSQL += qsLibrary + "." ;
    qsSQL += qsTable;
    if (!qsWhereExpressions.isEmpty()) qsSQL += " WHERE "    + qsWhereExpressions;
    if (!qsSorts.isEmpty()           ) qsSQL += " ORDER BY " + qsSorts;

    return true;
}

void classBrowse::ChangeAllColumns( int row, const char * text )
{
  int nCol  = 0;
  int nCols = tableQuery->numCols();
  QHeader *header;

  header = tableQuery->horizontalHeader();
  for ( nCol=0; nCol<nCols; nCol++ )
    tableQuery->setText( row, nCol, text );
}

void classBrowse::SelectAllColumns()
{
  ChangeAllColumns( QUERY_ROW_SHOW, "YES" ) ;
}

void classBrowse::UnSelectAllColumns()
{
  ChangeAllColumns( QUERY_ROW_SHOW, "" ) ;
}

void classBrowse::UnSortAllColumns()
{
  ChangeAllColumns( QUERY_ROW_SORT, "" ) ;
}

void classBrowse::UnExprAllColumns()
{
  ChangeAllColumns( QUERY_ROW_EXPRESSION, "" ) ;
}

void classBrowse::WriteHTML( QFile *hFile, bool bPage )
{
    int nCol  = 0;
    int nCols = tableData->numCols();
    int nRow  = 0;
    int nRows = tableData->numRows();
    QHeader *header;

    header = tableData->horizontalHeader();

    if ( bPage )
        hFile->writeBlock( "<HTML><BODY>", sizeof("<HTML><BODY>")-1 );

    hFile->writeBlock( "<TABLE>", sizeof("<TABLE>")-1 );

    // HEADER
    hFile->writeBlock( "\n<TR>", sizeof("\n<TR>")-1 );
    for ( nCol=0; nCol<nCols; nCol++ )
    {
        hFile->writeBlock( "<TH>", sizeof("<TH>")-1 );
        hFile->writeBlock( header->label( nCol ), header->label( nCol ).length() );
        hFile->writeBlock( "</TH>", sizeof("</TH>")-1 );
    }
    hFile->writeBlock( "</TR>", sizeof("</TR>")-1 );

    // BODY
    for ( nRow=0; nRow<nRows; nRow++ )
    {
        hFile->writeBlock( "\n<TR>", sizeof("\n<TR>")-1 );
        for ( nCol=0; nCol<nCols; nCol++ )
        {
            hFile->writeBlock( "\n<TD>", sizeof("\n<TD>")-1 );
            hFile->writeBlock( tableData->text( nRow, nCol ), tableData->text( nRow, nCol ).length() );
            hFile->writeBlock( "</TD>", sizeof("</TD>")-1 );
        }
        hFile->writeBlock( "</TR>", sizeof("</TR>")-1 );
    }

    hFile->writeBlock( "\n</TABLE>", sizeof("\n</TABLE>")-1 );

    if ( bPage )
        hFile->writeBlock( "</BODY></HTML>", sizeof("</BODY></HTML>")-1 );
}



void classBrowse::ClearCells( QTable *table )
{
    int nCol  = 0;
    int nCols = table->numCols();
    int nRow  = 0;
    int nRows = table->numRows();

    for ( nRow=0; nRow<nRows; nRow++ )
    {
        for ( nCol=0; nCol<nCols; nCol++ )
        {
            table->clearCell( nRow, nCol );
        }
    }
}

