/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 30.NOV.00
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#include "classBrowse.h"

char *szHelpQuery = \
"Use this area to quickly specify a simple data filter.\n\n" \
"1. Leave the Expression cells blank to get all data in the table or enter filter Expression(s) to limit the amount of data returned. Expression examples; \n='MyText'\n>23\n" \
"2. Enter Y in the Show cells to get data in that column.\n" \
"3. Control-Click and drag column headers to rearrange the column order.\n" \
"4. Click the Run button to get your results.";

char *szHelpData = \
"This area contains the results from your Query. You can;\n" \
"1. Browse the data.\n" \
"2. Export the data in a variety of formats.\n\n" \
"Editing the data is not availible right *now*. Consider using the isql window (click on the Data Source) for this.";

classBrowse::classBrowse( SQLHDBC hDbc, QString qsTable, QWidget *parent, const char *name )
	: QWidget( parent, name, 0 )
{
    qsDataFileName = "";
    this->hDbc	 	= hDbc;
    this->qsTable	= qsTable;
 
    layoutMain = new QVBoxLayout( this );

    tableQuery = new QTable( 1, 1, this );
    tableQuery->setColumnMovingEnabled ( TRUE );
    tableQuery->setMinimumSize( 100, 100 );
    
    tableData = new QTable( 1, 1, this );
    tableData->setColumnMovingEnabled ( TRUE );


    labelFilter = new QLabel( "FILTER", this );
    labelFilter->setPalette( QPalette( QColor(lightGray) ) );
    labelFilter->setAlignment( AlignCenter );
    layoutMain->addWidget( labelFilter );

    layoutMain->addWidget( tableQuery );

    labelResults = new QLabel( "RESULTS", this );
    labelResults->setPalette( QPalette( QColor(lightGray) ) );
    labelResults->setAlignment( AlignCenter );
    layoutMain->addWidget( labelResults );

    layoutMain->addWidget( tableData, 10 );

    InitQuery();

	setMinimumSize( 50, 50 );
	setMaximumSize( 32767, 32767 );

    QWhatsThis::add( tableQuery, szHelpQuery );
    QWhatsThis::add( tableData, szHelpData );
}


classBrowse::~classBrowse()
{
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
    QString qsLabel;

    if ( qsDataFileName == "" )
        return SaveAs();

    // TRY TO SAVE THE FILE
    QFile hFile( qsDataFileName );

    if ( !hFile.open( IO_WriteOnly ) )
	    return FALSE;

    WriteHTML( &hFile, TRUE );

    hFile.close();
    
    qsLabel = "RESULTS (";
    qsLabel += qsDataFileName;
    qsLabel += ")";
    labelResults->setText( qsLabel );

    return TRUE;
}

bool classBrowse::SaveAs()      // DATA
{
    QString qsLabel;

    // LET USER PICK A FILE
    QString qsFile = QFileDialog::getSaveFileName( qsDataFileName );
    if ( qsFile.isNull() )
	    return FALSE;

    // TRY TO SAVE THE FILE
    QFile hFile( qsFile );

    if ( !hFile.open( IO_WriteOnly ) )
	    return FALSE;

    WriteHTML( &hFile, TRUE );

    hFile.close();

    qsDataFileName = qsFile;

    qsLabel = "RESULTS (";
    qsLabel += qsDataFileName;
    qsLabel += ")";
    labelResults->setText( qsLabel );
    
    return TRUE;
}

void classBrowse::InitQuery()
{
	SQLHSTMT        hstmt;
	SQLRETURN		nReturn             = -1;
	SQLCHAR         szTableName[101]	= "";
	SQLCHAR         szColumnName[101]	= "";
	SQLCHAR         szColumnType[101]	= "";
	QString         qsError;
    SQLLEN          nCols               = 0;
    SQLINTEGER      nCol                = 0;
    QHeader         *headerQuery;
    QHeader         *headerData;

	// CREATE A STATEMENT
	nReturn = SQLAllocStmt( hDbc, &hstmt );
	if ( nReturn != SQL_SUCCESS )
	{
		QMessageBox::warning( this, "Table browser",  "Failed to SQLAllocStmt" );
		return;
	}

	// EXECUTE OUR SQL/CALL
	strcpy( (char *)szTableName, qsTable.data() );

	if ( SQL_SUCCESS != (nReturn=SQLColumns( hstmt, 0, 0, 0, 0, szTableName, SQL_NTS, 0, 0 )) )
	{
		QMessageBox::warning( this, "Table browser",  "Failed to SQLColumns" );
		return;
	}

    // PREPARE THE TABLES
    nCol = 0;

    // not all drivers can do this 
    nCols = 0;
    SQLRowCount( hstmt, &nCols );
    if ( nCols <= 0 )
    {
        // work out the number of colums we need
        nCols = 0 ;
        while ( SQL_SUCCEEDED( SQLFetch( hstmt )))
            nCols ++;

        SQLFreeStmt( hstmt, SQL_CLOSE );

        // repeat the SQLTables call
        if ( SQL_SUCCESS != (nReturn=SQLColumns( hstmt, 0, 0, 0, 0, szTableName, SQL_NTS, 0, 0 )) )
        {
            QMessageBox::warning( this, "Table browser",  "Failed to SQLColumns" );
            return;
        }
    }
    tableQuery->setNumCols( nCols );
    tableQuery->setNumRows( 2 );
    tableData->setNumCols( nCols );
    tableData->setNumRows( 0 );
    headerQuery = tableQuery->verticalHeader();
    headerQuery->setLabel( 0, QString( tr("Expression") ) );
    headerQuery->setLabel( 1, QString( tr("Show") ) );
    headerQuery = tableQuery->horizontalHeader();
    headerData = tableData->horizontalHeader();

	// GET RESULTS
	nReturn = SQLFetch( hstmt );
	while ( (nReturn == SQL_SUCCESS || nReturn == SQL_SUCCESS_WITH_INFO) && nCol < nCols )
	{
		nReturn = SQLGetData( hstmt, SQLColumns_COLUMN_NAME, SQL_C_CHAR, &szColumnName[0], sizeof(szColumnName), 0 );
		if ( nReturn != SQL_SUCCESS )
			strcpy( (char *)szColumnName, "Unknown" );

        headerQuery->setLabel( nCol, QString((char*)szColumnName) );
        headerData->setLabel( nCol, QString((char*)szColumnName) );

		nReturn = SQLGetData( hstmt, SQLColumns_TYPE_NAME, SQL_C_CHAR, &szColumnType[0], sizeof(szColumnType), 0 );

		nReturn = SQLFetch( hstmt );
        nCol++;
	}

	// FREE STATEMENT
	nReturn = SQLFreeStmt( hstmt, SQL_DROP );
	if ( nReturn != SQL_SUCCESS )
		QMessageBox::warning( this, "Table browser",  "Failed to SQLFreeStmt" );

    SelectAllColumns();
}

void classBrowse::Exec()        // RUN QUERY
{
    SQLHSTMT    hStmt;
    SQLLEN      nRows;
    SWORD       nColumns;
	char	    szState[10];
	char	    szBuf[700];
	char	    szErrString[800];
	SQLRETURN   nRetVal;
    SQLCHAR     szSQL[32000];

//printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );

    if ( !CreateSQL() ) 
        return;
    
    ClearCells( tableData );

//    QMessageBox::information( (QWidget *)this, "Table browse", qsSQL );

	// CREATE A STATEMENT
    nRetVal = SQLAllocStmt( hDbc, &hStmt );
    if( SQL_SUCCESS != nRetVal )
    {
		QMessageBox::critical( (QWidget *)this, "Table browse", "Failed: SQLAllocStmt " );
        return;
    }

    strcpy( (char*)szSQL, qsSQL );

    if( SQL_SUCCESS != (nRetVal=SQLPrepare(hStmt, szSQL, SQL_NTS)) )
    {
        nRetVal = SQLError(SQL_NULL_HENV, hDbc, hStmt, (SQLCHAR*)szState, NULL, (SQLCHAR*)szBuf, 700, NULL );
    	if (nRetVal == SQL_SUCCESS)
    		QMessageBox::critical( (QWidget *)this, "Table browse", szBuf);
    	else
    		QMessageBox::critical( (QWidget *)this, "Table browse", "Failed: SQLPrepare " );
        SQLFreeStmt( hStmt, SQL_DROP );
		
        QMessageBox::information( (QWidget *)this, "Table browse", qsSQL );
        return;
    }

    // EXECUTE
    if( SQL_SUCCESS != (nRetVal=SQLExecute(hStmt)) )
    {
    	nRetVal = SQLError(SQL_NULL_HENV, hDbc, hStmt, (SQLCHAR*)szState, NULL, (SQLCHAR*)szBuf, 700, NULL );
    	if (nRetVal == SQL_SUCCESS)
    		QMessageBox::critical( (QWidget *)this, "Table browse", szBuf);
    	else
    		QMessageBox::critical( (QWidget *)this, "Table browse", "Failed: SQLExecute " );
        SQLFreeStmt( hStmt, SQL_DROP );
        return;
    }

    // GET NUMBER OF ROWS AFFECTED
    if ( (nRetVal = SQLRowCount( hStmt, &nRows )) != SQL_SUCCESS )
        nRows = 0;

    // Default Values
    if ( nRows == SQL_NO_ROW_NUMBER )
        nRows = 10000; // give a resonable number back
    // Note this code could be changed to use a ListView and therefore would not need to
    // know the result set size upfront....

    // GET NUMBER OF COLUMNS RETURNED
    if ( (nRetVal = SQLNumResultCols( hStmt, &nColumns )) != SQL_SUCCESS )
        nColumns = 0;

//printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );
    if ( nRows > 0 )
        tableData->setNumRows( nRows );

    tableData->setNumCols( nColumns );
//printf( "[PAH][%s][%d] nColumns=%d nRows=%d\n", __FILE__, __LINE__, nColumns, nRows );
//printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );

    // GET A RESULTS HEADER (column headers)
    ExecHeader( hStmt, nColumns );

    // GET A RESULTS BODY (data)
    nRows = ExecBody( hStmt, nColumns );

    // Show the correct number of rows to make the gui look correct
    if ( nRows > 0 )
        tableData->setNumRows( nRows );

    // FREE STATEMENT
    nRetVal = SQLFreeStmt( hStmt, SQL_DROP );
    if( SQL_SUCCESS != nRetVal )
		QMessageBox::critical( (QWidget *)this, "Table browse", "Failed: SQLFreeStmt " );

    //printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );
}

void classBrowse::ExecHeader( SQLHSTMT hStmt, SWORD nColumns )
{
	int				nCol;
	SQLCHAR			szColumnName[101]	= "";	
    QHeader         *header;
    
//printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );

    header = tableData->horizontalHeader();
	for( nCol = 0; nCol < nColumns; nCol++ )
	{
		SQLColAttribute( hStmt, nCol+1, SQL_DESC_LABEL, szColumnName, sizeof(szColumnName), 0, 0 );
        header->setLabel( nCol, QString( (char*)szColumnName ) );
	}

//printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );

}

int classBrowse::ExecBody( SQLHSTMT hStmt, SWORD nColumns )
{
    int         	nRetVal            	= 0;
    SQLUSMALLINT    nCol            	= 0;
	SQLLEN  		nIndicator;
	char			szColumn[300];
	int				nRow 				= 0;

//printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );

    // PROCESS ALL ROWS
    while( SQL_SUCCESS == (nRetVal=SQLFetch(hStmt)) || SQL_SUCCESS_WITH_INFO == nRetVal )
    {
		nRow++;
        // PROCESS ALL COLUMNS
        for( nCol = 1; nCol <= nColumns; nCol++ )
        {
            nRetVal = SQLGetData( hStmt, nCol, SQL_C_CHAR, (SQLPOINTER)szColumn, sizeof(szColumn)-1, &nIndicator );
            if ( nRetVal == SQL_SUCCESS && nIndicator != SQL_NULL_DATA )
                tableData->setText( nRow-1, nCol-1, QString( szColumn ) );
            else
                // Print a dash to represent NULL data as opposed to real blanks
                tableData->setText( nRow-1, nCol-1, QString( "-" ) );

        }
    }

//printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );
    return nRow ; // return the actual number of rows in the result set
}


bool classBrowse::CreateSQL()
{
    int nCol  = 0;
    int nCols = tableQuery->numCols();
    QHeader *header = tableQuery->horizontalHeader();
    QString qsColumns           = "";
    QString qsWhereExpressions  = "";
    QString qsAnd               = "";
    QString qsWhere             = " \nWHERE ";
    QString qsComma             = "";
    QString qsColumn            = "";
    QString qsExpression        = "";
    QString qsShow              = "";

//printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );

    for ( nCol=0; nCol<nCols; nCol++ ) 
    {
        qsColumn            = header->label( nCol );
        qsColumn.prepend('"').append('"') ;  // Quote column name in case it matches SQL keyword
        qsExpression        = tableQuery->text( QUERY_ROW_EXPRESSION, nCol );
        qsShow              = tableQuery->text( QUERY_ROW_SHOW, nCol );

        if ( qsShow && qsShow != "" )
        {
            qsColumns += qsComma;
            qsColumns += qsColumn;
            qsComma = ", ";
        }

        if ( qsExpression && qsExpression != "" ) 
        {
            qsWhereExpressions += qsWhere;
            qsWhereExpressions += qsAnd;
            qsWhereExpressions += qsColumn;
           qsWhereExpressions += " ";
            qsWhereExpressions += qsExpression;
            qsAnd = " \nAND ";
            qsWhere = "";
        }
    }
    
    if ( qsColumns == "" )
    {
		QMessageBox::warning( (QWidget *)this, "Table browse", "No columns to show. Please specify which columns you want to show." );
        return FALSE;
    }

    qsSQL = "SELECT \n";
    qsSQL += qsColumns; 
    qsSQL += "\nFROM ";
    qsSQL += qsTable;
    qsSQL += qsWhereExpressions;
// ORDER BY is not need here and could cause some queries to fail because of to many columns or
// the total byte size of the columns is to large.
// The columns will be ordered by the order they appear after the select.
// If the GUI is changed to allow different orders, some form of order by would be needed then.
//
//    qsSQL += " ORDER BY ";
//    qsSQL += qsColumns;

//printf( "[PAH][%s][%d]\n", __FILE__, __LINE__ );

    return TRUE;
}

void classBrowse::SelectAllColumns()
{
    int nCol  = 0;
    int nCols = tableQuery->numCols();
    QHeader *header;

    header = tableQuery->horizontalHeader();
    for ( nCol=0; nCol<nCols; nCol++ ) 
    {
        tableQuery->setText( QUERY_ROW_SHOW, nCol, "Y" );
    }
}

void classBrowse::UnSelectAllColumns()
{
    int nCol  = 0;
    int nCols = tableQuery->numCols();
    QHeader *header;

    header = tableQuery->horizontalHeader();
    for ( nCol=0; nCol<nCols; nCol++ ) 
    {
        tableQuery->setText( QUERY_ROW_SHOW, nCol, "" );
    }
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
        hFile->writeBlock( "<TD>", sizeof("<TD>")-1 );
        hFile->writeBlock( header->label( nCol ), header->label( nCol ).length() );
        hFile->writeBlock( "</TD>", sizeof("</TD>")-1 );
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


