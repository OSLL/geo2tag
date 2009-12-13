#include "CStats.h"

#include "stats.xpm"

CStats::CStats( QWidget* parent, const char* name )
    : QWidget( parent, name, 0 )
{
    QVBoxLayout *   playoutTop;
    QBoxLayout *    playoutContent;
    QBoxLayout  *   playoutHelp;
    QFrame *        pframeHelp;
    QLabel *        plabelIcon;
    QLabel *        plabelHelp;

    playoutTop      = new QVBoxLayout( this, 5 );

    // main frame
    playoutContent  = new QHBoxLayout( playoutTop, 5 );
    pSummary        = new CStatSummary( this );
    pDetails        = new CStatDetails( this );
    playoutContent->addWidget( pSummary );
    playoutContent->addWidget( pDetails );

    // help frame
    pframeHelp      = new QFrame( this, "pframeHelp" );
    pframeHelp->setFrameStyle( QFrame::Box | QFrame::Raised );

    playoutTop->addWidget( pframeHelp );

    // help text
    playoutHelp     = new QHBoxLayout( pframeHelp, 5 );

    plabelIcon = new QLabel( pframeHelp, "Label_2" );
    plabelIcon->setPixmap( xpmStats );

    plabelHelp = new QLabel( pframeHelp, "Label_1" );
    plabelHelp->setText( "These are the number of active ODBC; environments, connections, statements and descriptors." );
    plabelHelp->setAlignment( AlignLeft | WordBreak );

    playoutHelp->addWidget( plabelIcon );
    playoutHelp->addWidget( plabelHelp, 10 );
}

CStats::~CStats()
{
}



