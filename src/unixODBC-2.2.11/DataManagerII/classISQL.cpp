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

#include "classISQL.h"
#include "classODBC.h"
#include <qwhatsthis.h>
#include <sqlucode.h>
#include <qlistbox.h>
#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif

char *szHelpSQL = \
"Use this area to type SQL statements for your query.\n" \
"For example:  SELECT * FROM TABLE1\n"
"You can cut/paste and save/load text in this area\n" ;

char *szHelpSQLSlider = \
"Use this slider to quickly access recent SQL statements.\n" ;

char *szHelpResults = \
"This area contains the output from your recent SQL query.\n" \
"You can cut/paste and save/load data in this area.\n" \
"NOTE: The max column size is 1024." ;

char *szHelpStatus = \
"This area contains status messages.\n" ;

classISQL::classISQL( SQLHDBC hDbc, QString qsDataSource, QWidget *parent, const char *name )
  : QWidget( parent, name ), hDbc( hDbc ), qsDataSource( qsDataSource.stripWhiteSpace() ), nSQL( 1 )
{
    QVBoxLayout *layoutMain = new QVBoxLayout( this );

    // SET FONT
    QFont qf( "Fixed", 12 );
    qf.setFixedPitch( TRUE );

    // TAB BAR
    pTabBar = new QTabBar( this );
    pTabBar->setGeometry( 0, 0, 150, 20 );
    pTabBar->addTab( new QTab( "SQL"     ) ) ;
    pTabBar->addTab( new QTab( "Results" ) ) ;
    connect( pTabBar, SIGNAL(selected(int)), SLOT(ChangeTextType(int)) );

    // SQL ENTRY GUI
    txtSQL = new QMultiLineEdit( this, "txtSQL" );
    txtSQL->setFocusPolicy( QWidget::StrongFocus );
    txtSQL->setBackgroundMode( QWidget::PaletteBase );
    txtSQL->insertLine( "" );
    txtSQL->setReadOnly( FALSE );
    txtSQL->setOverwriteMode( FALSE );
    txtSQL->setFont( qf );

    // SQL DATA RESULTS GUI
    txtResults = new QMultiLineEdit( this, "txtResults" );
    txtResults->setFocusPolicy( QWidget::StrongFocus );
    txtResults->setBackgroundMode( QWidget::PaletteBase );
    txtResults->insertLine( "" );
    txtResults->setReadOnly( FALSE );
    txtResults->setOverwriteMode( FALSE );
    txtResults->setFont( qf );
    txtResults->hide();

    // QUICK ACCESS SLIDER
    pSliderRecentSQL = new QSlider( QSlider::Horizontal, this );
    pSliderRecentSQL->setTickmarks( QSlider::Left );
    pSliderRecentSQL->setTickInterval( 1 );
    pSliderRecentSQL->setLineStep( 1 );
    pSliderRecentSQL->setPageStep( 1 );
    pSliderRecentSQL->setMinValue( nSQL );
    pSliderRecentSQL->setMaxValue( nSQL );
    pSliderRecentSQL->setValue( nSQL );
    connect( pSliderRecentSQL, SIGNAL(valueChanged(int)), SLOT(gotoHistoryItem(int)) );
    listSQL.append( "" );

    // STATUS LABEL
    QLabel *labelStatus = new QLabel( "STATUS", this );
    labelStatus->setAlignment( AlignCenter );

    // STATUS LISTBOX
    listStatus = new QListBox( this );
    listStatus->setSelectionMode( QListBox::NoSelection );

    // HELP TIPS
    QWhatsThis::add( txtSQL           , szHelpSQL       );
    QWhatsThis::add( pSliderRecentSQL , szHelpSQLSlider );
    QWhatsThis::add( txtResults       , szHelpResults   );
    QWhatsThis::add( labelStatus      , szHelpStatus    );
    QWhatsThis::add( listStatus       , szHelpStatus    );

    // LAYOUT GUI
    layoutMain->addWidget( pTabBar          );
    layoutMain->addWidget( txtSQL,     6    );
    layoutMain->addWidget( txtResults, 6    );
    layoutMain->addWidget( pSliderRecentSQL );
    layoutMain->addWidget( labelStatus      );
    layoutMain->addWidget( listStatus, 2    );

    // SETUP SIZE
    setMinimumSize( 50, 50 );
    setMaximumSize( 32767, 32767 );
    resize( parent->size() );
}

void classISQL::addStatus( const QString &statusMsg )
{
  if ( listStatus->count() == 300 )
    listStatus->removeItem( 0 ) ;

  listStatus->insertItem( statusMsg ) ;
  listStatus->setBottomItem( listStatus->count()-1 ) ;
}

void classISQL::ExecSQL(View view)
{
    CursorScoper s(this) ;
    QString      qsHorizSep;
    SQLLEN       nRowsAffected;
    SWORD        nColumns;
    SQLRETURN    nReturn;

    txtResults->clear();

    addStatus( QString().sprintf( "RUN: view=%s sql=%s", view == Text ? "Text" : view == TextDelimited ? "TextDelimited" : "HTML", txtSQL->text().simplifyWhiteSpace().data() ) ) ;

    // CREATE A STATEMENT
    StatementScoper stmt( hDbc ) ; if ( !stmt() ) return ;

    // PREPARE
    if (!SQL_SUCCEEDED(nReturn=SQLPrepare(stmt(), (SQLCHAR*)txtSQL->text().simplifyWhiteSpace().data(), SQL_NTS) ) )
      return my_msgBox( "classISQL", "SQLPrepare", nReturn, NULL, NULL, stmt(), txtSQL->text() ) ;

    // EXECUTE
    if (!SQL_SUCCEEDED(nReturn=SQLExecute( stmt() ) ) )
      return my_msgBox( "classISQL", "SQLExecute", nReturn, NULL, NULL, stmt(), txtSQL->text() ) ;

    // UPDATE HISTORY
    appendHistoryItem();

    // GET NUMBER OF COLUMNS RETURNED
    if (!SQL_SUCCEEDED(SQLNumResultCols( stmt(), &nColumns ) ) )
        nColumns = 0;

    txtResults->setAutoUpdate( FALSE );

    switch ( view )
    {
      case Text:
      {
        QString qsHorizSep;
        QArray<int> colWidths(nColumns) ;
        // GET A RESULTS HEADER (column headers)
        getResultsHeader( stmt(), nColumns, qsHorizSep, colWidths );
        // GET A RESULTS BODY (data)
        nRowsAffected = getResultsBody( stmt(), nColumns, qsHorizSep, colWidths );
      }
      break ;

      case TextDelimited:
      {
        int nCol ;
        QString qsLine ;
        QArray<bool> colChar(nColumns) ;

        // Get Column Names
        for ( nCol = 1; nCol <= nColumns; nCol++ )
        {
          SQLCHAR szColumnName[MAX_COLUMN_WIDTH];
          if (SQL_SUCCEEDED(SQLColAttribute( stmt(), nCol, SQL_DESC_LABEL, szColumnName, sizeof(szColumnName), 0, 0 ) ) )
            qsLine += QString("\"") + QString((const char *)szColumnName).stripWhiteSpace() + "\","  ;
          else
            qsLine += "\"ERR\"," ;

          // Determine if this column needs to have quotes around it
          SQLINTEGER nConciseType = 0 ;
          colChar[nCol-1]         = false ;
          if (SQL_SUCCEEDED(SQLColAttribute( stmt(), nCol, SQL_DESC_CONCISE_TYPE, 0, 0, 0, &nConciseType ) ) )
          {
            switch (nConciseType)
            {
              case SQL_CHAR:         // =1
              case SQL_VARCHAR:      // =12
              case SQL_LONGVARCHAR:  // =-1
              case SQL_WCHAR:        // =-8
              case SQL_WVARCHAR:     // =-9
              case SQL_WLONGVARCHAR: // =-10
                colChar[nCol-1] = true ;
              break ;

              default:
              break;
            }
          }
        }
        txtResults->insertLine( qsLine );

        // Get Data
        int nRow = 0;
        while ( SQL_SUCCEEDED(SQLFetch(stmt() ) ) )
        {
          nRow++;
          qsLine = "";
          // Process all columns
          for ( nCol = 1; nCol <= nColumns; nCol++ )
          {
            char szData[MAX_COLUMN_WIDTH];
            memset(szData, 0, sizeof(szData) ) ; // Handle broken drivers that don't properly null terminate
            SQLLEN nIndicator               = 0;
            if (SQL_SUCCEEDED(SQLGetData( stmt(), nCol, SQL_C_CHAR, (SQLPOINTER)szData, sizeof(szData), &nIndicator ) ) )
              qsLine += QString().sprintf("%s%s%s,", colChar[nCol-1] ? "\"" : "" , nIndicator != SQL_NULL_DATA ? QString(szData).stripWhiteSpace().data() : "", colChar[nCol-1] ? "\"" : "" ) ;
            else
              qsLine += QString("\"ERR\",")  ;
          }
          txtResults->insertLine( qsLine );
        }
        nRowsAffected = nRow ;

      }
      break ;

      case HTMLSource:
      {
        int nCol ;
        txtResults->insertLine( "<html>"  ) ;
        txtResults->insertLine( "<body>"  ) ;
        txtResults->insertLine( "<table border>" ) ;
        txtResults->insertLine( QString().sprintf("<caption>%s</caption>", txtSQL->text().data() ) ) ;
        // Get Column Names
        txtResults->insertLine( "  <tr>"    );
        SQLCHAR szColumnName[MAX_COLUMN_WIDTH] ;
        for ( nCol = 1; nCol <= nColumns; nCol++ )
        {
          if (SQL_SUCCEEDED(SQLColAttribute( stmt(), nCol, SQL_DESC_LABEL, szColumnName, sizeof(szColumnName), 0, 0 ) ) )
            txtResults->insertLine( QString().sprintf("    <th>%s</th>", QString((const char *)szColumnName).stripWhiteSpace().data() ) ) ;
          else
            txtResults->insertLine( "    <th>ERR</th>" ) ;
        }
        txtResults->insertLine( "  </tr>"   );

        // Get Data
        int nRow = 0;
        while ( SQL_SUCCEEDED(SQLFetch(stmt() ) ) )
        {
          nRow++;
          txtResults->insertLine( "  <tr>" );
          // Process all columns
          for ( nCol = 1; nCol <= nColumns; nCol++ )
          {
            char   szData[MAX_COLUMN_WIDTH] ;
            SQLLEN nIndicator = 0;
            memset(szData, 0, sizeof(szData) ) ; // Handle broken drivers that don't properly null terminate
            if (SQL_SUCCEEDED(SQLGetData( stmt(), nCol, SQL_C_CHAR, (SQLPOINTER)szData, sizeof(szData), &nIndicator ) ) )
              txtResults->insertLine( QString().sprintf("    <td>%s</td>", nIndicator != SQL_NULL_DATA ? QString((const char *)szData).stripWhiteSpace().data() : "" ) ) ;
            else
              txtResults->insertLine( "    <td>ERR</td>" ) ;
          }
          txtResults->insertLine( "  </tr>" );
        }

        txtResults->insertLine( "</table>" ) ;
        txtResults->insertLine( "</body>"  ) ;
        txtResults->insertLine( "</html>"  ) ;
        nRowsAffected = nRow ;
      }
      break ;
    }

    // UPDATE THE GUI
    pTabBar->setCurrentTab( 1 );
    txtResults->show();
    txtSQL->hide();
    txtResults->setAutoUpdate( TRUE );
    txtResults->repaint();
    addStatus( QString().sprintf( "RUN: %d rows and %d columns affected", nRowsAffected, nColumns ) ) ;
}

void classISQL::getResultsHeader( SQLHSTMT hStmt, SWORD nColumns, QString &qsHorizSep, QArray<int> &colWidths )
{
    QString qsColumnHeader;
    QString qsFill ; qsFill.fill('-', MAX_COLUMN_WIDTH);
    for ( int nCol = 1; nCol <= nColumns; nCol++ )
    {
        SQLLEN nMaxLength;
        SQLCHAR      szColumnName[MAX_COLUMN_WIDTH]; szColumnName[0] = 0;
        // Grab the column name and display size
        SQLColAttribute( hStmt, nCol, SQL_DESC_DISPLAY_SIZE, 0, 0, 0, &nMaxLength );
        SQLColAttribute( hStmt, nCol, SQL_DESC_LABEL, szColumnName, sizeof(szColumnName), 0, 0 ) ;
        QString qsColumnName( QString((const char *)szColumnName).stripWhiteSpace() ) ;
        // Calc the column width
        int nWidth = max( nMaxLength, qsColumnName.length() ) ;
        nWidth = min( nWidth, MAX_COLUMN_WIDTH );
        // Buld the formatted column
        qsHorizSep     += QString().sprintf( "+%-*.*s-" , nWidth, nWidth, qsFill.data()       ) ;
        qsColumnHeader += QString().sprintf( "| %-*.*s" , nWidth, nWidth, qsColumnName.data() ) ;
        colWidths[nCol-1] = nWidth ;
    }
    qsHorizSep += "+";
    qsColumnHeader += "|";

    txtResults->insertLine( qsHorizSep );
    txtResults->insertLine( qsColumnHeader );
    txtResults->insertLine( qsHorizSep );
}

int classISQL::getResultsBody( SQLHSTMT hStmt, SWORD nColumns, const QString &qsHorizSep, const QArray<int> &colWidths )
{
  int nRow = 0;

  // PROCESS ALL ROWS
  while ( SQL_SUCCEEDED(SQLFetch(hStmt) ) )
  {
    nRow++;
    QString qsLine;
    // PROCESS ALL COLUMNS
    for ( int nCol = 1; nCol <= nColumns; nCol++ )
    {
      SQLLEN nIndicator;
      char   szData[MAX_COLUMN_WIDTH]; szData[0] = 0;
      SQLRETURN nReturn = SQLGetData( hStmt, nCol, SQL_C_CHAR, (SQLPOINTER)szData, sizeof(szData), &nIndicator );
      // Grab the column data
      if ( SQL_SUCCEEDED(nReturn) && nIndicator != SQL_NULL_DATA )
        qsLine += QString().sprintf( "| %-*.*s", colWidths[nCol-1], colWidths[nCol-1], szData );
      else
        qsLine += QString().sprintf( "| %-*.*s", colWidths[nCol-1], colWidths[nCol-1], " "    );
    }
    txtResults->insertLine( qsLine + "|" );
  }
  txtResults->insertLine( qsHorizSep );
  return nRow ;
}

void classISQL::ChangeTextType( int nTab )
{
    if ( nTab == 0 )
    {
        pSliderRecentSQL->show();
        txtSQL->show();
        txtResults->hide();
    }
    else
    {
        pSliderRecentSQL->hide();
        txtSQL->hide();
        txtResults->show();
    }
}

void classISQL::gotoHistoryItem( int nValue )
{
    QValueList<QString>::Iterator it;

    // SAVE ANY CHANGES
    it      = listSQL.at( nSQL );
    (*it)   = txtSQL->text();

    // MOVE
    nSQL    = nValue;
    it      = listSQL.at( nSQL );
    txtSQL->setText( (*it) );

    // Lets keep it simple for now, previous sql can be executed again but not edited, force
    // the user to copy and paste to last sql for editing. Remember; its important to maintain
    // a history of executed statements, unchanged, so that the user can audit self.
    QPalette oPalette = txtResults->palette();

    if ( nSQL == pSliderRecentSQL->maxValue() )
    {
        txtSQL->setReadOnly( false );
        txtSQL->setPalette( oPalette );
    }
    else
    {
        txtSQL->setReadOnly( true );
        oPalette.setColor( QColorGroup::Text, txtResults->backgroundColor() );
        oPalette.setColor( QColorGroup::Base, txtResults->foregroundColor() );
        txtSQL->setPalette( oPalette );
    }
}

void classISQL::appendHistoryItem()
{
    QValueList<QString>::Iterator it;

    // SAVE ANY CHANGES
    it      = listSQL.at( nSQL );
    (*it)   = txtSQL->text();

    // ADD AS LAST & MOVE TO LAST
    listSQL.append( txtSQL->text() );
    pSliderRecentSQL->setMaxValue( pSliderRecentSQL->maxValue() + 1 );
    pSliderRecentSQL->setValue( pSliderRecentSQL->maxValue() );
}

void classISQL::NewSQL()
{
    pSliderRecentSQL->setValue( pSliderRecentSQL->maxValue() );
    txtSQL->clear();
    qsSQLFileName = "";
    pTabBar->setCurrentTab( 0 ) ;
    addStatus( "NEW: sql text cleared" ) ;
}

void classISQL::OpenSQL()
{
    QMultiLineEdit *txt;

    if ( pTabBar->currentTab() == 0 )
    {
      pSliderRecentSQL->setValue( pSliderRecentSQL->maxValue() );
      txt = txtSQL;
    }
    else
      txt = txtResults;

    // LET USER PICK A FILE
    QString qsFile = QFileDialog::getOpenFileName();
    if ( qsFile.isNull() )
      return;

    // TRY TO LOAD THE FILE
    QFile hFile( qsFile );
    if ( !hFile.open( IO_ReadOnly ) )
      return my_msgBox( "classISQL", "QFile.open", hFile.status(), NULL, NULL, NULL, qsFile ) ;

    txt->setAutoUpdate( FALSE );
    txt->clear();

    QTextStream t( &hFile );
    while ( !t.eof() )
      txt->append( t.readLine() ) ;
    hFile.close();

    txt->setAutoUpdate( TRUE );
    txt->repaint();

    if ( pTabBar->currentTab() == 0 )
      qsSQLFileName = qsFile;
    else
      qsResultsFileName = qsFile;

    addStatus( QString().sprintf( "OPEN: file %s opened", qsFile.data() ) ) ;
}

void classISQL::SaveSQL()
{
    QMultiLineEdit *txt        = pTabBar->currentTab() ? txtResults        : txtSQL        ;
    const QString  &qsFileName = pTabBar->currentTab() ? qsResultsFileName : qsSQLFileName ;

    if ( qsFileName.isEmpty() )
      return SaveAsSQL();

    // TRY TO SAVE THE FILE
    QFile hFile( qsFileName );
    if ( !hFile.open( IO_WriteOnly ) )
      return my_msgBox( "classISQL", "QFile.open", hFile.status(), NULL, NULL, NULL, qsFileName ) ;

    hFile.writeBlock( txt->text(), txt->text().length() );
    hFile.close();
    addStatus( QString().sprintf( "SAVE: file %s saved", qsFileName.data() ) ) ;
}

void classISQL::SaveAsSQL()
{
    QMultiLineEdit *txt        = pTabBar->currentTab() ? txtResults        : txtSQL        ;
    const QString  &qsFileName = pTabBar->currentTab() ? qsResultsFileName : qsSQLFileName ;

    // LET USER PICK A FILE
    QString qsFile = QFileDialog::getSaveFileName( qsFileName );
    if ( qsFile.isNull() )
        return;

    // TRY TO SAVE THE FILE
    QFile hFile( qsFile );
    if ( !hFile.open( IO_WriteOnly ) )
      return my_msgBox( "classISQL", "QFile.open", hFile.status(), NULL, NULL, NULL, qsFile ) ;

    hFile.writeBlock( txt->text(), txt->text().length() );
    hFile.close();

    // SAVE THE NEW FILE NAME
    if ( pTabBar->currentTab() == 0 )
      qsSQLFileName = qsFile;
    else
      qsResultsFileName = qsFile;

    addStatus( QString().sprintf( "SAVE SQL: file %s saved", qsFile.data() ) ) ;
}

