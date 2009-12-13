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
#include "classBrowseFrame.h"

#include "run.xpm"
#include "new.xpm"
#include "open.xpm"
#include "save.xpm"

classBrowseFrame::classBrowseFrame( SQLHDBC hDbc, QString qsTable, QWidget *parent, const char *name )
	: QMainWindow( parent, name, 0 )
{
    QString         qsCaption;

    this->hDbc	 	= hDbc;
    this->qsTable	= qsTable;
    qsCaption.sprintf( "Browse %s", (const char *)qsTable );
    setCaption( qsCaption );
    
    // CLIENT AREA
    browse = new classBrowse( hDbc, qsTable, this ); 
    setCentralWidget( browse );

    // SETUP MAIN MENU
    menubarMain = new QMenuBar( this );
    menubarMain->setFrameStyle( QFrame::NoFrame );

    QPopupMenu *pFile = new QPopupMenu();
    pFile->insertItem( QPixmap( new_xpm ), tr("Clear"),  this, SLOT(Clear()) );
    pFile->insertItem( QPixmap( save_xpm ), tr("&Save"), this, SLOT(Save()), CTRL+Key_S );
    pFile->insertItem( tr("Save &As"), this, SLOT(SaveAs()), CTRL+Key_A );
    menubarMain->insertItem( tr("&File"), pFile );
    
    QPopupMenu *pEdit = new QPopupMenu();
    pEdit->insertItem( tr("Se&lect All Columns"),  browse, SLOT(SelectAllColumns()) );
    pEdit->insertItem( tr("&Unselect All Columns"),  browse, SLOT(UnSelectAllColumns()) );
    menubarMain->insertItem( tr("&Edit"), pEdit );
    
    menubarMain->setSeparator( QMenuBar::InWindowsStyle );

    // SETUP TOOLBAR
    toolbarMain = new QToolBar( this );
    addToolBar( toolbarMain, tr( "ToolBar" ), Top, FALSE );

    QToolButton *toolbutton = new QToolButton( QPixmap( new_xpm ), QString(tr("Clear")), QString(""), this, SLOT(Clear()), toolbarMain );
    toolbutton = new QToolButton( QPixmap( save_xpm ), QString(tr("Save")), QString(""), this, SLOT(Save()), toolbarMain );
    toolbutton = new QToolButton( QPixmap( run_xpm ), QString(tr("Run")), QString(""), this, SLOT(Exec()), toolbarMain );
    QWhatsThis::whatsThisButton ( toolbarMain );

    // RESIZE
	connect( parent, SIGNAL(changedSize(int,int)), SLOT(resize(int,int)) );
	resize( parent->size() );
	setMinimumSize( 50, 50 );
	setMaximumSize( 32767, 32767 );
}


classBrowseFrame::~classBrowseFrame()
{
}

void classBrowseFrame::resizeEvent( QResizeEvent *p )
{
    resize( p->size() );
}


void classBrowseFrame::Clear()         // CLEAR QUERY AND DATA
{
    browse->Clear();
}

void classBrowseFrame::Save()        // DATA
{
    browse->Save();
}

void classBrowseFrame::SaveAs()      // DATA
{
    browse->SaveAs();
}

void classBrowseFrame::Exec()        // RUN QUERY
{
    browse->Exec();
}


