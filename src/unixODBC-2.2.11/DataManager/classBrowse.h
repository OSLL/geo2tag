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
#ifndef classBrowse_included
#define classBrowse_included

#include <qwidget.h>
#include <qlayout.h>
#include <qtable.h>
#include <qheader.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qwmatrix.h>
#include <qwhatsthis.h>
#include <qfile.h>
#include <qfiledialog.h>

#include <sqlext.h>

#define QUERY_ROW_EXPRESSION 0
#define QUERY_ROW_SHOW 1

class classBrowse : public QWidget
{
    Q_OBJECT

public:

    classBrowse( SQLHDBC hDbc, QString qsTable, QWidget* parent = 0, const char* name = 0 );
    ~classBrowse();

    QString qsDataFileName;
    void resizeEvent( QResizeEvent * );
    
public slots:
    void Clear();         // CLEAR QUERY AND DATA
    bool Save();        // SAVE DATA
    bool SaveAs();      // SAVE DATA AS
    void Exec();        // RUN QUERY
    void SelectAllColumns();
    void UnSelectAllColumns();
    void WriteHTML( QFile *hFile, bool bPage );

protected:
    QVBoxLayout     *layoutMain;
    QTable          *tableQuery;
    QTable          *tableData;
	SQLHDBC 		hDbc;
    QString     	qsTable;
    QString         qsSQL;
    QLabel          *labelFilter;
    QLabel          *labelResults;

    void InitQuery();
    void ExecHeader( SQLHSTMT hStmt, SWORD nColumns );
    int  ExecBody( SQLHSTMT hStmt, SWORD nColumns );
    bool CreateSQL();
    void ClearCells( QTable *table );
};

#endif



