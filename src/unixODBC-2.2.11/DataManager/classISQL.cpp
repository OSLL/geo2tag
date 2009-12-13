/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#include "classISQL.h"

#define MAX_COLUMN_WIDTH 300   /* see szColumn */

classISQL::classISQL( SQLHDBC hDbc, QString qsDataSource, QWidget *parent, const char *name )
: QWidget( parent, name )
{
    QString         qsCaption;
    QFont           qf( "Fixed", 12 );

    qf.setFixedPitch( TRUE );

    this->hDbc                  = hDbc;
    this->qsDataSource          = qsDataSource.stripWhiteSpace();
    qsSQLFileName               = "";
    qsResultsFileName           = "";

    qsCaption.sprintf( "ISQL %s", (const char *)qsDataSource );

    setCaption( qsCaption );

    txtSQL = new QMultiLineEdit( this, "txtSQL" );
    txtSQL->setFocusPolicy( QWidget::StrongFocus );
    txtSQL->setBackgroundMode( QWidget::PaletteBase );
    txtSQL->insertLine( "" );
    txtSQL->setReadOnly( FALSE );
    txtSQL->setOverwriteMode( FALSE );
    txtSQL->setFont( qf );

    pTabBar     = new QTabBar( this );
    pTabBar->setGeometry( 0, 0, 150, 20 );

    QTab *pTab  = new QTab( "SQL" );
    pTabBar->addTab( pTab );

    pTab        = new QTab( "Results" );
    pTabBar->addTab( pTab );

    connect( pTabBar, SIGNAL(selected(int)), SLOT(ChangeTextType(int)) );

    txtResults = new QMultiLineEdit( this, "txtResults" );
    txtResults->setFocusPolicy( QWidget::StrongFocus );
    txtResults->setBackgroundMode( QWidget::PaletteBase );
    txtResults->insertLine( "" );
    txtResults->setReadOnly( FALSE );
    txtResults->setOverwriteMode( FALSE );
    txtResults->setFont( qf );

    listSQL.append( QString("") );
    nSQL = 1;

    pSliderRecentSQL = new QSlider( QSlider::Vertical, this );
    pSliderRecentSQL->setTickmarks( QSlider::Left );
    pSliderRecentSQL->setTickInterval( 1 );
    pSliderRecentSQL->setLineStep( 1 );
    pSliderRecentSQL->setPageStep( 1 );
    pSliderRecentSQL->setMinValue( nSQL );
    pSliderRecentSQL->setMaxValue( nSQL );
    pSliderRecentSQL->setValue( nSQL );

    connect( pSliderRecentSQL, SIGNAL(valueChanged(int)), SLOT(gotoHistoryItem(int)) );

    resize( 590,400 );
    setMinimumSize( 50, 50 );
    setMaximumSize( 32767, 32767 );

    // SETUP MAIN MENU
    txtResults->hide();

    resize( parent->width(), parent->height() );
}


classISQL::~classISQL()
{
}

void classISQL::ExecSQL()
{
    SQLHSTMT    hStmt;
    QString     qsHorizSep;
    SQLLEN      nRowsAffected;
    SWORD       nColumns;
    int         iRC                 = 0;
/* TIM - return error messages if provided */
    char    szState[10];
    char    szBuf[700];
    char    szErrString[800];
    SQLRETURN retcode;

    // CREATE A STATEMENT
    iRC = SQLAllocStmt( hDbc, &hStmt );
    if ( SQL_SUCCESS != iRC )
        QMessageBox::critical( (QWidget *)this, "Data Manager", "Failed: SQLAllocStmt " );

    if ( SQL_SUCCESS != (iRC=SQLPrepare(hStmt, (SQLCHAR*)(txtSQL->text().data()), SQL_NTS)) )
    {
        retcode = SQLError(SQL_NULL_HENV, hDbc, hStmt, (SQLCHAR*)szState, NULL, (SQLCHAR*)szBuf, 700, NULL);
        if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
            QMessageBox::critical( (QWidget *)this, "Data Manager", szBuf);
        else
            QMessageBox::critical( (QWidget *)this, "Data Manager", "Failed: SQLPrepare " );
    }

    // EXECUTE
    iRC = SQLExecute( hStmt );
    if ( iRC != SQL_SUCCESS )
    {
        retcode = SQLError( 0, 0, hStmt, (SQLCHAR*)szState, NULL, (SQLCHAR*)szBuf, 700, NULL);
        if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
            QMessageBox::critical( (QWidget *)this, "Data Manager", szBuf);
        else
            QMessageBox::critical( (QWidget *)this, "Data Manager", "Failed: SQLExecute " );
    }

    if ( iRC != SQL_SUCCESS && iRC != SQL_SUCCESS_WITH_INFO )
        return;

    // UPDATE HISTORY
    appendHistoryItem();

    // GET NUMBER OF ROWS AFFECTED
// Wait until we know the exact number. SQLRowCount is not supported by all drivers and should not
// be used.
//    nRowsAffected = -1;
//    if ( (iRC = SQLRowCount( hStmt, &nRowsAffected )) != SQL_SUCCESS )
//        nRowsAffected = -1;

    // GET NUMBER OF COLUMNS RETURNED
    if ( (iRC = SQLNumResultCols( hStmt, &nColumns )) != SQL_SUCCESS )
        nColumns = -1;

    txtResults->setAutoUpdate( FALSE );
    txtResults->clear();

    // GET A RESULTS HEADER (column headers)
    getResultsHeader( hStmt, nColumns, &qsHorizSep );
    // GET A RESULTS BODY (data)
    nRowsAffected = getResultsBody( hStmt, nColumns );

    // FREE STATEMENT
    iRC = SQLFreeStmt( hStmt, SQL_DROP );
    if ( SQL_SUCCESS != iRC )
        QMessageBox::critical( (QWidget *)this, "Data Manager", "Failed: SQLFreeStmt " );

    pTabBar->setCurrentTab( 1 );
    txtResults->show();
    txtSQL->hide();

    txtResults->append( qsHorizSep );
    qsHorizSep.sprintf( "\n\n%d rows affected", nRowsAffected );
    txtResults->append( qsHorizSep );

    txtResults->setAutoUpdate( TRUE );
    txtResults->repaint();

}

int classISQL::getResultsHeader( SQLHSTMT hStmt, SWORD nColumns, QString
                                 *pqsHorizSep )
{
    QString         qsColumnHeader      = "";
    QString         qsColumn            = "";
    QString         qsColumnName        = "";
    int             nCol;
    SQLLEN    		nMaxLength          = 10;
    SQLCHAR         szColumnName[101]   = "";   

    *pqsHorizSep = "";
    for ( nCol = 1; nCol <= nColumns; nCol++ )
    {
        int nWidth;

        SQLColAttribute( hStmt, nCol, SQL_DESC_DISPLAY_SIZE, 0, 0, 0, &nMaxLength );
        SQLColAttribute( hStmt, nCol, SQL_DESC_LABEL, szColumnName, sizeof(szColumnName), 0, 0 );
        qsColumnName = (const char *)szColumnName;
        nWidth = max( nMaxLength, qsColumnName.length() );
        nWidth = min( nWidth, MAX_COLUMN_WIDTH );
        qsColumn.fill( '-', nWidth );
        qsColumn += "-";
        *pqsHorizSep += ( "+" + qsColumn );
        qsColumn.sprintf( "| %-*s", nWidth, szColumnName );
        qsColumnHeader += qsColumn;
    }
    *pqsHorizSep += "+\n";
    qsColumnHeader += "|\n";

    txtResults->append( *pqsHorizSep );
    txtResults->append( qsColumnHeader );
    txtResults->append( *pqsHorizSep );

    return qsColumnHeader.length();
}

int classISQL::getResultsBody( SQLHSTMT hStmt, SWORD nColumns )
{
    QString         qsLine;
    QString         qsColumn;
    QString         qsColumnName        = "";
    int             iRC                 = 0;
    SQLUSMALLINT    nCol                = 0;
    SQLLEN          nIndicator;
    char            szColumn[MAX_COLUMN_WIDTH+1];
    int             nRow                = 0;
    SQLLEN    		nMaxLength          = 10;
    SQLCHAR         szColumnName[101];  

    // PROCESS ALL ROWS
    while ( SQL_SUCCESS == (iRC=SQLFetch(hStmt)) || SQL_SUCCESS_WITH_INFO == iRC )
    {
        int nWidth;

        nRow++;
        qsLine = "";
        // PROCESS ALL COLUMNS
        for ( nCol = 1; nCol <= nColumns; nCol++ )
        {
            SQLColAttribute( hStmt, nCol, SQL_DESC_LABEL, szColumnName, sizeof(szColumnName), 0, 0 );
            SQLColAttribute( hStmt, nCol, SQL_DESC_DISPLAY_SIZE, 0, 0, 0, &nMaxLength );
            qsColumnName = (const char *)szColumnName;
            iRC = SQLGetData( hStmt, nCol, SQL_C_CHAR, (SQLPOINTER)szColumn, sizeof(szColumn), &nIndicator );

            /*
             * this is to handle a broken driver not null terminated
             * truncated strings
             */
            szColumn[MAX_COLUMN_WIDTH] = '\0';
            nWidth = max( nMaxLength, qsColumnName.length() );
            nWidth = min( nWidth, MAX_COLUMN_WIDTH );
            if ( iRC == SQL_SUCCESS && nIndicator != SQL_NULL_DATA )
                qsColumn.sprintf( "| %-*s", nWidth, szColumn );
            else
                // Print a dash to represent NULL data as opposed to real blanks
                qsColumn.sprintf( "| %-*s", nWidth, "-" );

            qsLine += qsColumn;
        }
        qsLine += "|\n";
        txtResults->append( qsLine );
    }
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
    setCaption( qsDataSource );
    qsSQLFileName = "";
    qsResultsFileName = "";

    setTextType( 0 );
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
        return;

    txt->setAutoUpdate( FALSE );
    txt->clear();

    QTextStream t( &hFile );
    while ( !t.eof() )
    {
        QString s = t.readLine();
        txt->append( s );
    }
    hFile.close();

    txt->setAutoUpdate( TRUE );
    txt->repaint();

    if ( pTabBar->currentTab() == 0 )
        qsSQLFileName = qsFile;
    else
        qsResultsFileName = qsFile;

    setTextType( 0 );
}

void classISQL::SaveSQL()
{
    QMultiLineEdit  *txt;
    QString         qsFileName;

    if ( pTabBar->currentTab() == 0 )
    {
        txt = txtSQL;
        qsFileName = qsSQLFileName;
    }
    else
    {
        txt = txtResults;
        qsFileName = qsResultsFileName;
    }

    if ( qsFileName == "" )
    {
        SaveAsSQL();
        return;
    }

    // TRY TO SAVE THE FILE
    QFile hFile( qsFileName );

    if ( !hFile.open( IO_WriteOnly ) )
        return;

    hFile.writeBlock( txt->text(), txt->text().length() );
    hFile.close();

}

void classISQL::SaveAsSQL()
{
    QMultiLineEdit  *txt;
    QString         qsFileName;

    if ( pTabBar->currentTab() == 0 )
    {
        txt = txtSQL;
        qsFileName = qsSQLFileName;
    }
    else
    {
        txt = txtResults;
        qsFileName = qsResultsFileName;
    }

    // LET USER PICK A FILE
    QString qsFile = QFileDialog::getSaveFileName( qsFileName );
    if ( qsFile.isNull() )
        return;

    // TRY TO SAVE THE FILE
    QFile hFile( qsFile );

    if ( !hFile.open( IO_WriteOnly ) )
        return;

    hFile.writeBlock( txt->text(), txt->text().length() );
    hFile.close();

    if ( pTabBar->currentTab() == 0 )
    {
        qsSQLFileName = qsFile;
    }
    else
    {
        qsResultsFileName = qsFile;
    }

}

void classISQL::setTextType( int nIndex )
{
    pTabBar->setCurrentTab( nIndex );
    if ( nIndex == 0 )
    {
        txtResults->hide();
        pSliderRecentSQL->show();
        txtSQL->show();
    }
    else
    {
        pSliderRecentSQL->hide();
        txtSQL->hide();
        txtResults->show();
    }
}

void classISQL::resizeEvent( QResizeEvent *p )
{
    int nMarg       = 0;
    int nYOffset    = pTabBar->height();
    int nW          = 0;
    int nH          = 0;

    resize( p->size() );
    nW = p->size().width() - nMarg * 2;
    nH = p->size().height() - nMarg * 2;

    pSliderRecentSQL->setGeometry( nMarg, nYOffset, 20, nH - nYOffset );
    txtSQL->setGeometry( nMarg + pSliderRecentSQL->width(), nYOffset + nMarg, nW - pSliderRecentSQL->width(), nH - nYOffset );
    txtResults->setGeometry( nMarg, nYOffset + nMarg, nW, nH - nYOffset );
}


