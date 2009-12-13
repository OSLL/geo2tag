/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 31.JAN.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 * Nick Gorham      - nick@easysoft.com
 **************************************************/
#include "CFileDSN.h"

#include "dsn-file.xpm"

void CFileDSN::NewDir()
{
    QString dir;

    path = pFileList->GetDir();

    dir = "Current Path : " + path;

	dirlab->setText( dir );
    SQLWritePrivateProfileString( "ODBC", "FileDSNPath", (char*)path.ascii(), "odbcinst.ini" );
}

CFileDSN::CFileDSN( QWidget* parent, const char* name, QString *cwd )
	: QWidget( parent, name )
{
	QVBoxLayout	*playoutTop		= new QVBoxLayout( this, 5 );
    QHBoxLayout	*playoutMain	= new QHBoxLayout( playoutTop );
	QVBoxLayout	*playoutLeft	= new QVBoxLayout( playoutMain, 5 );
	QHBoxLayout	*playoutHead	= new QHBoxLayout( playoutLeft, 3 );

    if ( !cwd )
    {
        char szFilePath[ 256 ];
        char szFileName[ 256 ];

        sprintf( szFileName, "%s/ODBCDataSources", odbcinst_system_file_path());
        SQLGetPrivateProfileString( "ODBC", "FileDSNPath", szFileName, szFilePath, sizeof( szFilePath ), "odbcinst.ini" );
        path.sprintf( "%s", szFilePath );
    }
    else
    {
        path = *cwd;
    }

    QString dir;

    dir = "Current Path : " + path;

	dirlab=new QLabel( dir, this, "cdir" );
    dirlab->setGeometry( 10, 10, 300, 20 );
//	dirlab->setMaximumSize( 290, 20 );

	pFileList = new CFileList( this, "pFileList" );
	pFileList->setGeometry( 10, 60, 270, 190 );
	pFileList->setMinimumSize( 50, 50 );
	pFileList->setMaximumSize( 32767, 32767 );

    playoutHead->addWidget( dirlab, 10 );
	dirlab->setAlignment( AlignLeft );

    playoutLeft->addWidget( pFileList, 10 );

	pbDir = new QPushButton( this, "pbDir" );
	pbDir->setGeometry( 10, 10, 100, 35 );
	pbDir->setMinimumSize( 0, 0 );
	pbDir->setMaximumSize( 90, 400 );
	pbDir->setFocusPolicy( QWidget::TabFocus );
	pbDir->setBackgroundMode( QWidget::PaletteBackground );
	pbDir->setText( "&Set Dir..." );
	pbDir->setAutoRepeat( FALSE );
	pbDir->setAutoResize( FALSE );

    playoutHead->addWidget( pbDir, 10 );

	QVBoxLayout	*playoutButtons	= new QVBoxLayout( playoutMain, 5 );

	pbAdd = new QPushButton( this, "pbAdd" );
	pbAdd->setGeometry( 290, 10, 100, 30 );
	pbAdd->setMinimumSize( 0, 0 );
	pbAdd->setMaximumSize( 32767, 32767 );
	pbAdd->setFocusPolicy( QWidget::TabFocus );
	pbAdd->setBackgroundMode( QWidget::PaletteBackground );
	pbAdd->setText( "A&dd..." );
	pbAdd->setAutoRepeat( FALSE );
	pbAdd->setAutoResize( FALSE );

    playoutButtons->addWidget( pbAdd );

	pbRemove = new QPushButton( this, "pbRemove" );
	pbRemove->setGeometry( 290, 50, 100, 30 );
	pbRemove->setMinimumSize( 0, 0 );
	pbRemove->setMaximumSize( 32767, 32767 );
	pbRemove->setFocusPolicy( QWidget::TabFocus );
	pbRemove->setBackgroundMode( QWidget::PaletteBackground );
	pbRemove->setText( "&Remove" );
	pbRemove->setAutoRepeat( FALSE );
	pbRemove->setAutoResize( FALSE );

    playoutButtons->addWidget( pbRemove );

	pbConfigure = new QPushButton( this, "pbConfigure" );
	pbConfigure->setGeometry( 290, 90, 100, 30 );
	pbConfigure->setMinimumSize( 0, 0 );
	pbConfigure->setMaximumSize( 32767, 32767 );
	pbConfigure->setFocusPolicy( QWidget::TabFocus );
	pbConfigure->setBackgroundMode( QWidget::PaletteBackground );
	pbConfigure->setText( "&Configure..." );
	pbConfigure->setAutoRepeat( FALSE );
	pbConfigure->setAutoResize( FALSE );

    playoutButtons->addWidget( pbConfigure );

    playoutButtons->addStretch( 10 );

	QFrame *pframe;
	pframe = new QFrame( this, "Frame_2" );
	pframe->setGeometry( 10, 210, 380, 80 );
	pframe->setMinimumSize( 0, 0 );
	pframe->setMaximumSize( 32767, 32767 );
	pframe->setFrameStyle( QFrame::Box | QFrame::Raised );

    playoutTop->addWidget( pframe );
    QGridLayout *playoutHelp = new QGridLayout( pframe, 1, 2, 5 );

	QLabel* plabel1;
	plabel1 = new QLabel( pframe, "Label_1" );
	plabel1->setMinimumSize( 32, 32 );
//	plabel1->setMaximumSize( 32, 32 );
	plabel1->setPixmap( xpmDSN_File );

	QLabel* plabel2;
	plabel2 = new QLabel( pframe, "Label_2" );
	plabel2->setMinimumSize( 0, 0 );
	plabel2->setMaximumSize( 32767, 32767 );
	plabel2->setText( "An ODBC File data source can be stored on a file server to be shared among many users on a network. All users should have required drivers installed on their machine." );
	plabel2->setAlignment( AlignLeft | WordBreak );

	playoutHelp->addWidget( plabel1, 0, 0 );
    playoutHelp->addWidget( plabel2, 0, 1 );
    playoutHelp->setColStretch( 1, 10 );

    pFileList->Load( &path );

	connect( pbDir, SIGNAL(clicked()), pFileList, SLOT(NewDir()) );
	connect( pbDir, SIGNAL(clicked()), SLOT(NewDir()) );
	connect( pbAdd, SIGNAL(clicked()), pFileList, SLOT(Add()) );
	connect( pbRemove, SIGNAL(clicked()), pFileList, SLOT(Delete()) );
	connect( pbConfigure, SIGNAL(clicked()), pFileList, SLOT(Edit()) );
}

CFileDSN::~CFileDSN()
{
}

