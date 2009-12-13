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
#include "classLogin.h"

classLogin::classLogin( QWidget *pParent, SQLHDBC hDbc, char *pszDataSource, int nDataSourceType )
    : QDialog( pParent, "classLogin", TRUE )
{
    QLabel      *lblUID;
    QLabel      *lblPWD;
    QPushButton *pbOk;
    QPushButton *pbCancel;

    if ( nDataSourceType == classLogin::System )
        nDataSourceType = classLogin::System;
    else
        nDataSourceType = classLogin::User;

    qsDataSourceName    = pszDataSource;
    this->hDbc          = hDbc;

    setCaption( "Login - " + qsDataSourceName );

    lblUID      = new QLabel( "ID:", this  );
    lblPWD      = new QLabel( "PWD:", this  );
    txtUID      = new QLineEdit( this );
    txtPWD      = new QLineEdit( this );
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

classLogin::~classLogin()
{
}

void classLogin::pbOk_Clicked()
{
    SQLCHAR     szDSN[101];
    SQLCHAR     szUID[101];
    SQLCHAR     szPWD[101];
    SQLRETURN   nReturn;

    /* OK - Try to login */
    strncpy( (char *)szDSN, qsDataSourceName.data(), 100 );
    strncpy( (char *)szUID, txtUID->text(), 100 );
    strncpy( (char *)szPWD, txtPWD->text(), 100 );

    nReturn = SQLConnect( hDbc, szDSN, SQL_NTS, szUID, SQL_NTS, szPWD, SQL_NTS );
    if ( nReturn != SQL_SUCCESS && nReturn != SQL_SUCCESS_WITH_INFO )
    {
/* TIM - return error messages if provided */
        char    szState[10];
        char    szBuf[700];
        char    szErrString[800];
        SQLRETURN retcode;

        retcode = SQLError(SQL_NULL_HENV, hDbc, SQL_NULL_HSTMT,
                           (SQLCHAR*)szState, NULL, (SQLCHAR*)szBuf, 700, NULL);
        if (retcode == SQL_SUCCESS)
            QMessageBox::warning( this, "Data Manager", szBuf);
        else
/* END TIM */
            QMessageBox::warning( this, "Data Manager",  "Login failed\n\nThis may be for one of these reasons;\n1. invalid ID and Password\n2. invalid Data Source config\n3. improper installation" );
        return; 
    }

    /* Logged in so */
    accept();
}

void classLogin::pbCancel_Clicked()
{
    reject();
}
