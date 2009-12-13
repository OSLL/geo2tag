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
#include <qfile.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qpixmap.h>
#include <qlayout.h>
#if (QT_VERSION>=300)
#include <qpushbutton.h>
#else
#include <qpushbt.h>
#endif
#include <qkeycode.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qvaluelist.h>
#include <qmultilineedit.h>

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
    ~classISQL();

    void resizeEvent( QResizeEvent * );

public slots:
    void ExecSQL();
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
    SQLHDBC             hDbc;
    QString             qsDataSource;
    QString             qsResultsFileName;
    QString             qsSQLFileName;
    QValueList<QString> listSQL;
    int                 nSQL;

private:
    int getResultsHeader( SQLHSTMT hStmt, SWORD nColumns, QString *pqsHorizSep );
    int getResultsBody( SQLHSTMT hStmt, SWORD nColumns );
    void setTextType( int nIndex );
    void appendHistoryItem();
};

#endif


