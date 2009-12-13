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
#include "CFileSelector.h"

CFileSelector::CFileSelector( QWidget* parent, const char* name )
	: QWidget( parent, name )
{
	QBoxLayout 		*pTopLayout		= new QHBoxLayout( this );

	pLineEdit 	= new QLineEdit( this );
	pButton		= new QPushButton( ">", this );
	pButton->setMaximumSize( 20,20 );

    pTopLayout->addWidget( pLineEdit, 2 );
    pTopLayout->addWidget( pButton, 0 );

    pTopLayout->activate();

	connect( pButton, SIGNAL(clicked()), this, SLOT(pButton_Clicked()) );
}

CFileSelector::~CFileSelector()
{
}

void CFileSelector::pButton_Clicked()
{
	QString qsFile( QFileDialog::getOpenFileName( pLineEdit->text()) );
    if ( qsFile.isNull() )
	    return;

	pLineEdit->setText( qsFile );
}


