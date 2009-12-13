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

#include "classBrowseFrame.h"
#include "classBrowse.h"
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qmenubar.h>
#include <qpixmap.h>
#include <qwhatsthis.h>
#include "run.xpm"
#include "new.xpm"
#include "open.xpm"
#include "save.xpm"

classBrowseFrame::classBrowseFrame( SQLHDBC hDbc, const QString &qsTable, const QString &qsLibrary, QWidget *parent, const char *name )
        : QMainWindow( parent, name, 0 )
{
    // CLIENT AREA
    browse = new classBrowse( hDbc, qsTable, qsLibrary, this );
    setCentralWidget( browse );

    // SETUP MAIN MENU
    QMenuBar *menubarMain = new QMenuBar( this );

    QPopupMenu *pFile = new QPopupMenu();
    pFile->insertItem( QPixmap( new_xpm  ), tr("&Clear"   ), browse, SLOT(Clear()  ), CTRL+Key_C );
    pFile->insertItem( QPixmap( save_xpm ), tr("&Save"    ), browse, SLOT(Save()   ), CTRL+Key_S );
    pFile->insertItem( QPixmap( save_xpm ), tr("Save &As" ), browse, SLOT(SaveAs() ), CTRL+Key_A );
    pFile->insertItem( QPixmap( save_xpm ), tr("Save S&QL"), browse, SLOT(SaveSQL()), CTRL+Key_Q );
    menubarMain->insertItem( tr("&File"), pFile );

    QPopupMenu *pEdit = new QPopupMenu();
    pEdit->insertItem( tr("Select All Columns"),             browse, SLOT(SelectAllColumns()  ) );
    pEdit->insertItem( tr("Unselect All Columns"),           browse, SLOT(UnSelectAllColumns()) );
    pEdit->insertItem( tr("Clear All Column Sorting"),       browse, SLOT(UnSortAllColumns()  ) );
    pEdit->insertItem( tr("Clear All Column Expressions"),   browse, SLOT(UnExprAllColumns()  ) );
    menubarMain->insertItem( tr("&Filter"), pEdit );

    pResults = new QPopupMenu();
    pResults->insertItem( tr("Delete Selected Rows"),            browse, SLOT(DeleteRows()        ) );
    pResults->insertItem( tr("Insert Selected Rows"),            browse, SLOT(InsertRows()        ) );
    pResults->insertSeparator();
    pResults->insertItem( tr("Add blank row for inserting"),     browse, SLOT(AddRow()            ) );
    pResults->insertItem( tr("Copy selected row for inserting"), browse, SLOT(CopyRow()           ) );
    pResults->insertSeparator();
    nAutoRefresh = pResults->insertItem( tr("Auto refresh after Delete/Insert"), this, SLOT(autoRefresh() ) );
    pResults->setItemChecked( nAutoRefresh , true );
    menubarMain->insertItem( tr("&Results"), pResults );

    QString t ; t.sprintf("  [%s.%s]  ", qsLibrary.data(), qsTable.data() ) ;
    menubarMain->insertItem( t );

    menubarMain->setSeparator( QMenuBar::InWindowsStyle );

    // SETUP TOOLBAR
    QToolBar *toolbarMain = new QToolBar( this );
    addToolBar( toolbarMain, tr( "ToolBar" ), Top, FALSE );

    new QToolButton( QPixmap( new_xpm  ), QString(tr("Clear")), "", browse, SLOT(Clear()), toolbarMain );
    new QToolButton( QPixmap( save_xpm ), QString(tr("Save") ), "", browse, SLOT(Save() ), toolbarMain );
    new QToolButton( QPixmap( run_xpm  ), QString(tr("Run")  ), "", browse, SLOT(Exec() ), toolbarMain );
    QWhatsThis::whatsThisButton( toolbarMain );

    // RESIZE
    connect( parent, SIGNAL(changedSize(int,int)), SLOT(resize(int,int)) );
    resize( parent->size() );
    setMinimumSize( 50, 50 );
    setMaximumSize( 32767, 32767 );
}

void classBrowseFrame::resizeEvent( QResizeEvent *p )
{
  resize( p->size() );
}

void classBrowseFrame::autoRefresh()
{
  pResults->setItemChecked( nAutoRefresh, !pResults->isItemChecked( nAutoRefresh ) ) ;
  browse->AutoRefresh( pResults->isItemChecked( nAutoRefresh ) ) ;
}

