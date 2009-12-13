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

#include "CProperties.h"

#ifdef HAVE_STRCASECMP
#ifdef HAVE_STRINGS
#include <strings.h>
#endif
#else
int strcasecmp( char *, char * );
#endif

#include "checkOk.xpm"
#include "checkCancel.xpm"

CProperties::CProperties( QWidget* parent, const char* name, HODBCINSTPROPERTY hTheFirstProperty )
	: QMainWindow( parent, name, 0 )
{
    HODBCINSTPROPERTY 	hProperty;
	int				    nProperty;

    pMainWidget     = new QWidget( this );
    setCentralWidget( pMainWidget );

	pTopLayout		= new QVBoxLayout( pMainWidget );

    // SETUP TOOLBAR
    toolbarMain = new QToolBar( this );
    addToolBar( toolbarMain, tr( "ToolBar" ), Top, FALSE );
    new QToolButton( QPixmap( checkOk_xpm ), QString(tr("Save and Exit")), QString(""), this, SLOT(pbOk_Clicked()), toolbarMain );
    new QToolButton( QPixmap( checkCancel_xpm ), QString(tr("Cancel any changes and Exit")), QString(""), this, SLOT(pbCancel_Clicked()), toolbarMain );
    QWhatsThis::whatsThisButton ( toolbarMain );

	/* PROPERTIES */
    hFirstProperty = hTheFirstProperty;
	for ( nProperties = 0, hProperty = hFirstProperty; hProperty != NULL; hProperty = hProperty->pNext )
	{
		nProperties++;
	}

	pGridLayout = new QGridLayout( nProperties, 2, 2 );
	pTopLayout->addLayout( pGridLayout );
	pGridLayout->setColStretch ( 0, 0 );
	pGridLayout->setColStretch ( 1, 1 );

	for ( nProperty = 0, hProperty = hFirstProperty; hProperty != NULL; nProperty++, hProperty = hProperty->pNext )
	{
        QLabel *pLabel = new QLabel( pMainWidget );
		// 1ST COLUMN IS ALWAYS A LABEL CONTAINING THE PROPERTY NAME
        if ( hProperty->nPromptType != ODBCINST_PROMPTTYPE_HIDDEN )
        {
            if ( hProperty->pszHelp )
                QWhatsThis::add( pLabel, hProperty->pszHelp );
            pLabel->setLineWidth( 1 );
            pLabel->setText( hProperty->szName );
            pLabel->setMinimumSize( pLabel->sizeHint() );
            pLabel->setFixedHeight( pLabel->sizeHint().height() );
            pGridLayout->addWidget( pLabel, nProperty, 0 );
        }

		// 2ND COLUMN IS WHERE THE USER ENTERS DATA SO CREATE A WIDGET THAT IS MEANINGFULL
		switch ( hProperty->nPromptType )
		{
		case ODBCINST_PROMPTTYPE_LABEL:
			{
				QLabel *pLabel2 = new QLabel( pMainWidget );
                if ( hProperty->pszHelp )
                    QWhatsThis::add( pLabel2, hProperty->pszHelp );
				pLabel2->setFrameStyle( QFrame::Box | QFrame::Sunken );
				pLabel2->setLineWidth( 1 );
				pLabel2->setText( hProperty->szValue );
				pLabel2->setMinimumSize( pLabel2->sizeHint() );
				pLabel2->setFixedHeight( pLabel2->sizeHint().height() );
				pGridLayout->addWidget( pLabel2, nProperty, 1 );
                hProperty->pWidget = pLabel2;
                if ( hProperty->pszHelp ) QToolTip::add( pLabel2, hProperty->pszHelp );
			}
			break;
		case ODBCINST_PROMPTTYPE_LISTBOX:
			{
				QComboBox *pComboBox = new QComboBox( pMainWidget );
                if ( hProperty->pszHelp )
                    QWhatsThis::add( pComboBox, hProperty->pszHelp );
				pComboBox->insertStrList( (const char **)hProperty->aPromptData );
				pComboBox->setMinimumSize( pComboBox->sizeHint() );
				pComboBox->setFixedHeight( pComboBox->sizeHint().height() );
				pGridLayout->addWidget( pComboBox, nProperty, 1 );
                hProperty->pWidget = pComboBox;
                if ( hProperty->pszHelp ) QToolTip::add( pComboBox, hProperty->pszHelp );
                setCurrentItem( pComboBox, hProperty->szValue );
            }
			break;
		case ODBCINST_PROMPTTYPE_COMBOBOX:
			{
				QComboBox *pComboBox = new QComboBox( true, pMainWidget );
                if ( hProperty->pszHelp )
                    QWhatsThis::add( pComboBox, hProperty->pszHelp );
				pComboBox->insertStrList( (const char **)hProperty->aPromptData );
                pComboBox->setEditText( hProperty->szValue );
				pComboBox->setMinimumSize( pComboBox->sizeHint() );
				pComboBox->setFixedHeight( pComboBox->sizeHint().height() );
				pGridLayout->addWidget( pComboBox, nProperty, 1 );
                hProperty->pWidget = pComboBox;
                if ( hProperty->pszHelp ) QToolTip::add( pComboBox, hProperty->pszHelp );
			}
			break;
		case ODBCINST_PROMPTTYPE_FILENAME:
			{
                CFileSelector *pFileSelector = new CFileSelector( pMainWidget );
                if ( hProperty->pszHelp )
                    QWhatsThis::add( pFileSelector, hProperty->pszHelp );
				pFileSelector->pLineEdit->setText( hProperty->szValue );
				pGridLayout->addWidget( pFileSelector, nProperty, 1 );
                hProperty->pWidget = pFileSelector;
                if ( hProperty->pszHelp ) QToolTip::add( pFileSelector, hProperty->pszHelp );
			}
			break;
        case ODBCINST_PROMPTTYPE_HIDDEN:
			delete pLabel;
            break;
		default: // PROMPTTYPE_TEXTEDIT and PROMPTTYPE_TEXTEDIT_PASSWORD
			{
				QLineEdit *pLineEdit = new QLineEdit( pMainWidget );
                if ( hProperty->pszHelp )
                    QWhatsThis::add( pLineEdit, hProperty->pszHelp );
				pLineEdit->setText( hProperty->szValue );
				pLineEdit->setMinimumHeight( pLineEdit->sizeHint().height() );
				pLineEdit->setFixedHeight( pLineEdit->sizeHint().height() );
		if (hProperty->nPromptType == ODBCINST_PROMPTTYPE_TEXTEDIT_PASSWORD)
			pLineEdit->setEchoMode( QLineEdit::Password ) ;
				pGridLayout->addWidget( pLineEdit, nProperty, 1 );
				pLabel->setBuddy( pLineEdit );
                hProperty->pWidget = pLineEdit;
                if ( hProperty->pszHelp ) QToolTip::add( pLineEdit, hProperty->pszHelp );
			}
		}
	}
	/* SPACER */
	QLabel *pSpacer = new QLabel( pMainWidget );
	pTopLayout->addWidget( pSpacer, 11 );	

	pTopLayout->activate();
    pMainWidget->show();

}


CProperties::~CProperties()
{
}

/*
void resizeEvent( QResizeEvent *p )
{
	pTopLayout->resize( p->size() );
}
*/

void CProperties::pbOk_Clicked()
{
    HODBCINSTPROPERTY 	hProperty;


	// COLLECT ALL VALUES
	for ( hProperty = hFirstProperty; hProperty != NULL; hProperty = hProperty->pNext )
	{
        if ( hProperty == hFirstProperty )
        {
            // 
            // check name is filled in
            //
            if ( ((QLineEdit *)hProperty->pWidget)->text().isEmpty())
            {
                return;
            }
        }

		switch ( hProperty->nPromptType )
		{
		case ODBCINST_PROMPTTYPE_LABEL:
			{
				strncpy( hProperty->szValue, ((QLabel *)hProperty->pWidget)->text(), INI_MAX_PROPERTY_VALUE );
			}
			break;
		case ODBCINST_PROMPTTYPE_LISTBOX:
		case ODBCINST_PROMPTTYPE_COMBOBOX:
			{
				strncpy( hProperty->szValue, ((QComboBox *)hProperty->pWidget)->currentText(), INI_MAX_PROPERTY_VALUE );
			}
			break;
		case ODBCINST_PROMPTTYPE_FILENAME:
			{
				strncpy( hProperty->szValue, ((CFileSelector *)hProperty->pWidget)->pLineEdit->text(), INI_MAX_PROPERTY_VALUE );
			}
			break;
		case ODBCINST_PROMPTTYPE_HIDDEN:
            break;
		default: // PROMPTTYPE_TEXTEDIT and PROMPTTYPE_TEXTEDIT_PASSWORD
			{
				strncpy( hProperty->szValue, ((QLineEdit *)hProperty->pWidget)->text(), INI_MAX_PROPERTY_VALUE );
			}
		}
	} // for

	// LET CALLER KNOW WHAT TO DO NEXT
    emit Ok();
}

void CProperties::pbCancel_Clicked()
{
    emit Cancel();
}

void CProperties::setCurrentItem( QComboBox *pComboBox, char *pszItem )
{
    QString stringItem;
	int n 			= 0;
	int nCurItem 	= 0;

    stringItem = pszItem;
	nCurItem = pComboBox->currentItem();
	for ( n=0; n < pComboBox->count(); n++ )
	{
		pComboBox->setCurrentItem( n );
		if ( stringItem == pComboBox->currentText() )
		{
			return;
		}
	}

	pComboBox->setCurrentItem( nCurItem );
}

