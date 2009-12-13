/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey   - pharvey@codebydesign.com
 **************************************************/
#ifndef classISQL_included
#define classISQL_included

#include <qwidget.h>
#include <qmenubar.h>
#if (QT_VERSION>=300)
#include <qcombobox.h>
#else
#include <qcombo.h>
#endif
#include <qslider.h>
#include <qtabbar.h>
#if (QT_VERSION>=300)
#else
#include <qmlined.h>
#endif
#include <qlabel.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qvaluelist.h>
#include <qmultilineedit.h>
#include <qlayout.h>
#include <qstring.h>
#if (QT_VERSION>=300)
#include <qmemarray.h>
#else
#include <qarray.h>
#endif

#include <sqlext.h>

#ifndef max
#define max( a, b ) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min( a, b ) (((a) > (b)) ? (b) : (a))
#endif

class classISQL : public QWidget
{
    Q_OBJECT

public:

    classISQL( SQLHDBC hDbc, QString qsDataSource, QWidget* parent = 0, const char* name = 0 );
    ~classISQL() {}

    enum View { Text=1, TextDelimited=2, HTMLSource=3 } ;

public slots:
    void ExecSQL( View );
    void NewSQL();
    void OpenSQL();
    void SaveSQL();
    void SaveAsSQL();

protected slots:
    void ChangeTextType( int nTab );
    void gotoHistoryItem( int nValue );

protected:
    QTabBar *           pTabBar;
    QSlider *           pSliderRecentSQL;
    QMultiLineEdit *    txtSQL;
    QMultiLineEdit *    txtResults;
    QListBox *          listStatus;
    SQLHDBC             hDbc;
    QString             qsDataSource;
    QString             qsResultsFileName;
    QString             qsSQLFileName;
    QValueList<QString> listSQL;
    int                 nSQL;

private:
    void getResultsHeader( SQLHSTMT hStmt, SWORD nColumns, QString &qsHorizSep, QArray<int> &colWidths );
    int getResultsBody( SQLHSTMT hStmt, SWORD nColumns, const QString &qsHorizSep, const QArray<int> &colWidths );
    void appendHistoryItem();
    void addStatus( const QString &statusMsg ) ;
};
#endif

