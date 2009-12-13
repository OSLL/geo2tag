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
 **************************************************/

#include "CPropertiesFrame.h"

CPropertiesFrame::CPropertiesFrame( QWidget* parent, const char* name, HODBCINSTPROPERTY hTheFirstProperty )
	: QDialog( parent, name, true )
{
    setSizeGripEnabled( true );
    QGridLayout *playoutTop  = new QGridLayout( this, 2, 1 );
    playoutTop->setSpacing( 5 );
    playoutTop->setMargin( 5 );

    pProperties = new CProperties( this, 0, hTheFirstProperty );
    playoutTop->addWidget( pProperties, 0, 0 );

    connect( pProperties, SIGNAL(Ok()), this, SLOT(doOk()) );
    connect( pProperties, SIGNAL(Cancel()), this, SLOT(doCancel()) );

    doLoadState();
}

CPropertiesFrame::~CPropertiesFrame()
{
    doSaveState();
}

void CPropertiesFrame::doLoadState()
{
#if QT_VERSION>=300
    QSettings settings;

    int nW = settings.readNumEntry( "/unixODBC/CPropertiesFrame/w", geometry().width() );
    int nH = settings.readNumEntry( "/unixODBC/CPropertiesFrame/h", geometry().height() );
    resize( nW, nH );
#endif

}

void CPropertiesFrame::doSaveState()
{
#if QT_VERSION>=300
    QSettings settings;

    settings.writeEntry( "/unixODBC/CPropertiesFrame/w", width() );
    settings.writeEntry( "/unixODBC/CPropertiesFrame/h", height() );
#endif
}

void CPropertiesFrame::doOk()
{
    accept();
}

void CPropertiesFrame::doCancel()
{
    reject();
}



