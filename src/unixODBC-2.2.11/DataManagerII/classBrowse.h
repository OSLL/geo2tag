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
#ifndef classBrowse_included
#define classBrowse_included

#include <qwidget.h>
#include <qtable.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qvaluelist.h>
#include <qfile.h>
#if (QT_VERSION>=300)
#include <qmemarray.h>
#else
#include <qarray.h>
#endif

#include <sqlext.h>

// Typesdefs
typedef QValueList<int> ListRows ;

// QTable subclass that will sort columns with whole row support
class myQTable : public QTable
{
public:
  myQTable( int r, int c, QWidget * w = 0, const char * n = 0 ) : QTable( r, c, w, n ) {} ;
  void sortColumn( int c, bool s, bool w) { QTable::sortColumn( c, s, TRUE ) ; }
};

#define QUERY_ROW_EXPRESSION 0
#define QUERY_ROW_SHOW       1
#define QUERY_ROW_SORT       2

class classBrowse : public QWidget
{
    Q_OBJECT

public:

    classBrowse( SQLHDBC hDbc, const QString &qsTable, const QString &qsLibrary, QWidget* parent = 0, const char* name = 0 );
    ~classBrowse() {};

    QString qsDataFileName;
    QString qsSQLFileName;
    void resizeEvent( QResizeEvent * );

public slots:
    void Clear();       // CLEAR QUERY AND DATA
    bool Save();        // SAVE DATA
    bool SaveAs();      // SAVE DATA AS
    bool SaveSQL();     // SAVE QUERY
    void Exec();        // RUN QUERY
    void SelectAllColumns();
    void UnSelectAllColumns();
    void UnSortAllColumns();
    void UnExprAllColumns();
    void DeleteRows();
    void InsertRows();
    int  AddRow();
    int  CopyRow();
    void AutoRefresh( bool );
    void WriteHTML( QFile *hFile, bool bPage );

protected:
    QTable          *tableQuery;
    QTable          *tableData;
    QLabel          *labelFilter;
    QLabel          *labelResults;
    QListBox        *listStatus;
    SQLHDBC         hDbc;
    QString         qsLibrary;
    QString         qsTable;
    bool            bAutoRefresh;

    void InitQuery();
    void ExecHeader( SQLHSTMT hStmt, SWORD nColumns );
    int  ExecBody( SQLHSTMT hStmt, SWORD nColumns );
    bool CreateSQL(QString & qsSQL);
    void ClearCells( QTable *table );
    void ChangeAllColumns( int row, const char * text );
    int  GetSelectedRows( QString &qsRowsSelected, ListRows &listRows ) ;
    bool OkWithUser( const QString &shortDesc, const QString &qsQuestion ) ;
    int  RowAction( const QString &shortDesc, const QString &sql, const ListRows &listRows, SQLRETURN &nReturn ) ;
    bool CheckRowsAffected(const QString &shortDesc, const ListRows &listRows ) ;
    void addStatus( const QString &statusMsg ) ;

};
#endif

