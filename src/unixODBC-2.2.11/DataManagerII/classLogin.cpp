/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey         - pharvey@codebydesign.com
 **************************************************/

#include "classLogin.h"
#include <qpushbutton.h>
#include <qpixmap.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include "LinuxODBC.xpm"

#include <classODBC.h>

classLogin::classLogin( QWidget *pParent, ConnectionScoper &dbc, const QString &dataSource )
    : QDialog( pParent, "classLogin", TRUE ) , dbc( dbc ) , qsDataSourceName( dataSource )
{
    QLabel      *lblUID;
    QLabel      *lblPWD;
    QPushButton *pbOk;
    QPushButton *pbCancel;

    setCaption( "Login - " + qsDataSourceName );
    setIcon( QPixmap( LinuxODBC_xpm ) );

    lblUID      = new QLabel( "ID:", this  );
    lblPWD      = new QLabel( "PWD:", this  );
    txtUID      = new QLineEdit( "", this );
    txtPWD      = new QLineEdit( "", this );
    pbOk        = new QPushButton( "&Ok", this  );
    pbCancel    = new QPushButton( "&Cancel", this  );

    lblUID->setGeometry( 5, 10, 30, 25 );
    lblPWD->setGeometry( 5, 40, 30, 25 );
    txtUID->setGeometry( 35, 10, 200, 25 );
    txtPWD->setGeometry( 35, 40, 200, 25 );
    pbOk->setGeometry( 35, 70, 100, 25 );
    pbCancel->setGeometry( 135, 70, 100, 25 );

    txtUID->setText( ((struct passwd *)getpwuid(getuid()))->pw_name );
    txtPWD->setEchoMode( QLineEdit::Password );

    connect( pbOk, SIGNAL(clicked()), this, SLOT(pbOk_Clicked()) );
    connect( pbCancel, SIGNAL(clicked()), this, SLOT(pbCancel_Clicked()) );

    pbOk->setDefault( TRUE );
    txtPWD->setFocus();
}

void classLogin::pbOk_Clicked()
{
  if ( dbc.connect( qsDataSourceName, txtUID->text(), txtPWD->text() ) )
  {
    // Clear security values
    txtUID->setText( "") ;
    txtPWD->setText( "") ;
    accept();
  }
}

void classLogin::pbCancel_Clicked()
{
  reject();
}
