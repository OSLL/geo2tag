#include "CStatSummary.h"

#include "info.xpm"

CStatSummary::CStatSummary( QWidget* parent, const char* name )
	: QWidget( parent, name, 0 )
{
	QBoxLayout	*playoutTop		= new QVBoxLayout( this, 5 );

    QGridLayout *playoutSliders = new QGridLayout( playoutTop, 3, 4 );
    
    nSliderMax = 10;

    pEnv = new QLabel( "0", this );
    pCon = new QLabel( "0", this );
    pSta = new QLabel( "0", this );
    pDes = new QLabel( "0", this );

	playoutSliders->addWidget( pEnv, 0, 0 );
	playoutSliders->addWidget( pCon, 0, 1 );
	playoutSliders->addWidget( pSta, 0, 2 );
	playoutSliders->addWidget( pDes, 0, 3 );

    pEnvSlider = new QSlider( this );
    pConSlider = new QSlider( this );
    pStaSlider = new QSlider( this );
    pDesSlider = new QSlider( this );

//    pEnvSlider->setTickmarks( QSlider::Both );
//    pConSlider->setTickmarks( QSlider::Both );
//    pStaSlider->setTickmarks( QSlider::Both );
//    pDesSlider->setTickmarks( QSlider::Both );

    pEnvSlider->setMinValue( 0-nSliderMax );
    pConSlider->setMinValue( 0-nSliderMax );
    pStaSlider->setMinValue( 0-nSliderMax );
    pDesSlider->setMinValue( 0-nSliderMax );

    pEnvSlider->setMaxValue( 0 );
    pConSlider->setMaxValue( 0 );
    pStaSlider->setMaxValue( 0 );
    pDesSlider->setMaxValue( 0 );

	playoutSliders->addWidget( pEnvSlider, 1, 0 );
	playoutSliders->addWidget( pConSlider, 1, 1 );
	playoutSliders->addWidget( pStaSlider, 1, 2 );
	playoutSliders->addWidget( pDesSlider, 1, 3 );

    QLabel *p1 = new QLabel( "Env", this );
    QLabel *p2 = new QLabel( "Con", this );
    QLabel *p3 = new QLabel( "Sta", this );
    QLabel *p4 = new QLabel( "Des", this );

	playoutSliders->addWidget( p1, 2, 0 );
	playoutSliders->addWidget( p2, 2, 1 );
	playoutSliders->addWidget( p3, 2, 2 );
	playoutSliders->addWidget( p4, 2, 3 );

	resize( 400,300 );
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );

    // SHARED MEM
    hStats = 0;

    // TIMER
    pTimer = new QTimer( this );
    connect( pTimer, SIGNAL(timeout()), SLOT(showStats()) );
    pTimer->start( 700, FALSE );
}

CStatSummary::~CStatSummary()
{
    uodbc_close_stats( hStats );
}


void CStatSummary::showStats()
{
    if ( this->isVisible() )
    {
        if ( !hStats )
            if ( uodbc_open_stats( &hStats, UODBC_STATS_READ ) != 0 );

        if ( uodbc_get_stats( hStats, -1, aStats, 4 ) == 4 )
        {
            QString qs;
            
            if ( aStats[0].value.l_value > nSliderMax )
                nSliderMax = aStats[0].value.l_value;
            if ( aStats[1].value.l_value > nSliderMax )
                nSliderMax = aStats[1].value.l_value;
            if ( aStats[2].value.l_value > nSliderMax )
                nSliderMax = aStats[2].value.l_value;
            if ( aStats[3].value.l_value > nSliderMax )
                nSliderMax = aStats[3].value.l_value;
    
            qs.sprintf( "%d", aStats[0].value.l_value );
            pEnv->setText( qs );
            pEnvSlider->setMinValue( 0-nSliderMax );
            pEnvSlider->setValue( 0-aStats[0].value.l_value );
            
            qs.sprintf( "%d", aStats[1].value.l_value );
            pCon->setText( qs );
            pConSlider->setMinValue( 0-nSliderMax );
            pConSlider->setValue( 0-aStats[1].value.l_value );
    
            qs.sprintf( "%d", aStats[2].value.l_value );
            pSta->setText( qs );
            pStaSlider->setMinValue( 0-nSliderMax );
            pStaSlider->setValue( 0-aStats[2].value.l_value );
    
            qs.sprintf( "%d", aStats[3].value.l_value );
            pDes->setText( qs );
            pDesSlider->setMinValue( 0-nSliderMax );
            pDesSlider->setValue( 0-aStats[3].value.l_value );
        }
    }
/*
    else
    {
        char szError[501];
        szError[0] = '\0';
        pEnv->setText( uodbc_stats_error( szError, sizeof(szError)-1 ) );
    }
*/    
}

