#include "CSplashDialog.h"

#include "Splash.xpm"

CSplashDialog::CSplashDialog( QWidget *pwidgetParent )
    : QDialog( pwidgetParent, 0, true, Qt::WStyle_Customize | Qt::WStyle_NoBorder )
{
#if defined( OSXHEADER )
    setErasePixmap( xpmSplash );
    QGridLayout *pgridlayoutTop = new QGridLayout( this, 2, 1, 0 );
    QGridLayout *pgridlayout    = new QGridLayout( 1, 2, 0 );

    pgridlayoutTop->addLayout( pgridlayout, 1, 0 );
	
    pgridlayoutTop->setRowStretch( 0, 10 );
    pgridlayout->setColStretch( 0, 10 );
	
    pcheckbox = new QCheckBox( "Never show again", this );
    QPushButton *ppushbutton = new QPushButton( "Ok", this );
	
    pgridlayout->addWidget( pcheckbox, 0, 0 );
    pgridlayout->addWidget( ppushbutton, 0, 1 );

    connect( ppushbutton, SIGNAL(clicked()), SLOT(accept()) );

    resize( 360, 240 );
#endif
}

CSplashDialog::~CSplashDialog()
{
#if defined( OSXHEADER )
    if ( pcheckbox->isChecked() )
    {
        QSettings settings;
        settings.writeEntry( "/unixODBC/NoSplash", 1 );
    }
#endif
}


