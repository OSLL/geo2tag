#include "CCredits.h"

#include "NoFlag.xpm"
#include "Canada.xpm"
#include "GreatBritain.xpm"

CCredits::CCredits( QWidget* parent, const char* name )
	: QDialog( parent, name, true )
{
    setSizeGripEnabled( true );
// Qt::WStyle_Customize | Qt::WStyle_NormalBorder /*Qt::WStyle_DialogBorder*/
// Qt::WType_Dialog | Qt::WShowModal

    QGridLayout	*layoutTop  = new QGridLayout( this, 2, 1 );
    layoutTop->setSpacing( 5 );

    QListBox    *list       = new QListBox( this );

    setCaption( "unixODBC - Credits" );

    new QListBoxPixmap( list, QPixmap( xpmGreatBritain ), QString( "Nick Gorham - Current Project Lead, Driver Manager, gODBCConfig, odbctest, many fixs" ) );
    new QListBoxPixmap( list, QPixmap( xpmCanada ), QString( "Peter Harvey - Original Project Lead, support libs, ODBCConfig, DataManager, isql, odbcinst" ) );
    new QListBoxPixmap( list, QPixmap( xpmCanada ), QString( "Jon Pounder" ) );
    new QListBoxPixmap( list, QPixmap( xpmGreatBritain ), QString( "Martin Evans" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Lars Doelle" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Manush Dodunekov" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Scott Courtney" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Greg Bentz" ) );
    new QListBoxPixmap( list, QPixmap( xpmCanada ), QString( "Shandy J. Brown" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Mark Hessling" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Charles Morrison" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Holger Bischoff" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Charles Overbeck" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Murray Todd Williams" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Jim Ziegler" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Thomas Langen" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Nikolai Afanasiev" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Ralf Fassel" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Tim Roepken" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Zoltan Boszormenyi" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Murad Nayal" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Michael Koch" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Dmitriy Yusupov" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Alex Hornby" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Steve Gilbert" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Max Khon" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Jay Q. Cai" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Bill Bouma" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Steffen Dettmer" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Jens Schlegel" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Venu Anuganti" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Tomas Zellerin" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "James Dugal" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Simon Pepping" ) );

    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Bard Hustveit" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Ola Sundell" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Christian Werner" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Martin Edlman" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Dave Berry" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Holger Schurig" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Martin Ediman" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Jon Kåre Hellan" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Trond Eivind Glomsrød" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Jürgen Pfeifer" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Jason Crummack" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Bojnourdi Kaikavous" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Martin Hobbs" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Gary Bunting" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Patrice Favre" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "John C. Rood" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Martin Lacko" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Mikko Vierula" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Paul Richardson" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Samuel Cote" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Christian Jullien" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Mark Vanderwiel" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Jeff Garzik" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Keith Woodard" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Steven M. Schultz" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Joel W. Reed" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Jay Van Vark" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Rick Flower" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Artiom Morozov" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Bill Medland" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Per I. Mathisen" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Emile Heitor" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "John L Miller" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Brian Harris" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Craig A Berry" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Stefan Radman" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Jay Cai" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Steve Langasek" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "Stuart Coupe" ) );
    new QListBoxPixmap( list, QPixmap( xpmNoFlag ), QString( "David Brown" ) );

    layoutTop->addWidget( list, 0, 0 );
    layoutTop->setRowStretch( 0, 10 );

    //
	QGridLayout	*layoutButtons = new QGridLayout( layoutTop, 1, 2 );
    layoutButtons->setSpacing( 5 );
    layoutButtons->setMargin( 5 );
    QPushButton *ppushbuttonOk = new QPushButton( "Ok", this );
    layoutButtons->addWidget( ppushbuttonOk, 0, 1 );
    layoutButtons->setColStretch( 0, 10 );

    connect( ppushbuttonOk, SIGNAL(clicked()), SLOT(accept()) );

    resize( 600, 300 );
}

CCredits::~CCredits()
{
}



