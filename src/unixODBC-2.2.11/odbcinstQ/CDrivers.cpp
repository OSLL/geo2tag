/**************************************************
 * CDrivers
 *
 * see CDrivers.h for doco
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 31.JAN.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey		- pharvey@codebydesign.com
 **************************************************/
#include "CDrivers.h"

#include "driver.xpm"

CDrivers::CDrivers( QWidget* parent, const char* name )
: QWidget( parent, name )
{
  QVBoxLayout *playoutTop   = new QVBoxLayout( this, 5 );

  QHBoxLayout *playoutMain  = new QHBoxLayout( playoutTop );

  lvwDrivers = new QListView( this, "lvwDrivers" );
  lvwDrivers->setGeometry( 10, 10, 270, 190 );
  lvwDrivers->setMinimumSize( 50, 50 );
  lvwDrivers->setMaximumSize( 32767, 32767 );
  lvwDrivers->setFocusPolicy( QWidget::TabFocus );
  lvwDrivers->setBackgroundMode( QWidget::PaletteBackground );
  lvwDrivers->setFrameStyle( QFrame::Box | QFrame::Raised );
  lvwDrivers->setResizePolicy( QScrollView::Manual );
  lvwDrivers->setVScrollBarMode( QScrollView::Auto );
  lvwDrivers->setHScrollBarMode( QScrollView::Auto );
  lvwDrivers->setTreeStepSize( 20 );
  lvwDrivers->setMultiSelection( FALSE );
  lvwDrivers->setAllColumnsShowFocus( FALSE );
  lvwDrivers->setItemMargin( 1 );
  lvwDrivers->setRootIsDecorated( FALSE );
  lvwDrivers->addColumn( "Name", -1 );
  lvwDrivers->setColumnWidthMode( 0, QListView::Maximum );
  lvwDrivers->setColumnAlignment( 0, 1 );
  lvwDrivers->addColumn( "Description", -1 );
  lvwDrivers->setColumnWidthMode( 1, QListView::Maximum );
  lvwDrivers->setColumnAlignment( 1, 1 );
  lvwDrivers->addColumn( "Driver Lib", -1 );
  lvwDrivers->setColumnWidthMode( 2, QListView::Maximum );
  lvwDrivers->setColumnAlignment( 2, 1 );
  lvwDrivers->addColumn( "Setup Lib", -1 );
  lvwDrivers->setColumnWidthMode( 3, QListView::Maximum );
  lvwDrivers->setColumnAlignment( 3, 1 );

  playoutMain->addWidget( lvwDrivers, 10 );

  QVBoxLayout *playoutButtons = new QVBoxLayout( playoutMain, 5 );

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
  pframe->setGeometry( 10, 204, 380, 90 );
  pframe->setMinimumSize( 0, 0 );
  pframe->setMaximumSize( 32767, 32767 );
  pframe->setFocusPolicy( QWidget::NoFocus );
  pframe->setBackgroundMode( QWidget::PaletteBackground );
  pframe->setFrameStyle( QFrame::Box | QFrame::Raised );

  playoutTop->addWidget( pframe );
  QGridLayout *playoutHelp = new QGridLayout( pframe, 1, 2, 5 );

  QLabel* plabel1;
  plabel1 = new QLabel( pframe, "Label_1" );
  plabel1->setMinimumSize( 32, 32 );
//	plabel1->setMaximumSize( 32, 32 );
  plabel1->setPixmap( xpmDriver );

  QLabel* plabel2;
  plabel2 = new QLabel( pframe, "Label_2" );
  plabel2->setMinimumSize( 0, 0 );
  plabel2->setMaximumSize( 32767, 32767 );
  plabel2->setText( "These drivers facilitate communication between the Driver Manager and the data server. Many ODBC drivers can be downloaded from the Internet while others are obtained from your database vendor. Typically; you must be a root/adminstrator user to add drivers." );
  plabel2->setAlignment( AlignLeft | WordBreak );

  playoutHelp->addWidget( plabel1, 0, 0 );
  playoutHelp->addWidget( plabel2, 0, 1 );
  playoutHelp->setColStretch( 1, 10 );

  lvwDrivers->setAllColumnsShowFocus( true );

  connect( pbAdd, SIGNAL(clicked()), this, SLOT(Add()) );
  connect( pbRemove, SIGNAL(clicked()), this, SLOT(Delete()) );
  connect( pbConfigure, SIGNAL(clicked()), this, SLOT(Edit()) );

  Load();
}


CDrivers::~CDrivers()
{
  if ( hIni != NULL )
    iniClose( hIni );

}


void CDrivers::Add()
{
  QString       qsError         = "";

  CPropertiesFrame    *pProperties;
  HODBCINSTPROPERTY hFirstProperty  = NULL;
  HODBCINSTPROPERTY hCurProperty  = NULL;
  HODBCINSTPROPERTY hLastProperty;
  char        szINI[FILENAME_MAX+1];

  sprintf( szINI, "%s/odbcinst.ini", odbcinst_system_file_path() );

  // SET UP PROPERTIES LIST
  hFirstProperty            = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  memset( hFirstProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hFirstProperty->nPromptType     = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hFirstProperty->pNext       = NULL;
  hFirstProperty->bRefresh      = 0;
  hFirstProperty->hDLL        = NULL;
  hFirstProperty->pWidget       = NULL;
  hFirstProperty->pszHelp       = strdup( ODBC_HELP_DRIVER_NAME );
  hFirstProperty->aPromptData     = NULL;
  strncpy( hFirstProperty->szName, "Name", INI_MAX_PROPERTY_NAME );
  strcpy( hFirstProperty->szValue, "" );
  hLastProperty = hFirstProperty;

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType        = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hLastProperty->pNext          = NULL;
  hLastProperty->bRefresh         = 0;
  hLastProperty->hDLL           = NULL;
  hLastProperty->pWidget          = NULL;
  hLastProperty->pszHelp          = strdup( ODBC_HELP_DRIVER_DESC );
  hLastProperty->aPromptData        = NULL;
  strncpy( hLastProperty->szName, "Description", INI_MAX_PROPERTY_NAME );
  strcpy( hLastProperty->szValue, "" );

  /*
   * Don't have a deflib for 64 bit, the user may not want a driver entry, just a driver64
   */

#ifdef PLATFORM64
#undef DEFLIB_PATH
#endif

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType      = ODBCINST_PROMPTTYPE_FILENAME;
  hLastProperty->pszHelp        = strdup( ODBC_HELP_DRIVER_DRIVER );
  strncpy( hLastProperty->szName, "Driver", INI_MAX_PROPERTY_NAME );
#ifdef DEFLIB_PATH
  strncpy( hLastProperty->szValue, DEFLIB_PATH, INI_MAX_PROPERTY_VALUE );
#else
  strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );
#endif

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType      = ODBCINST_PROMPTTYPE_FILENAME;
  hLastProperty->pszHelp        = strdup( ODBC_HELP_DRIVER_DRIVER64 );
  strncpy( hLastProperty->szName, "Driver64", INI_MAX_PROPERTY_NAME );
#ifdef DEFLIB_PATH
  strncpy( hLastProperty->szValue, DEFLIB_PATH, INI_MAX_PROPERTY_VALUE );
#else
  strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );
#endif

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType      = ODBCINST_PROMPTTYPE_FILENAME;
  hLastProperty->pszHelp        = strdup( ODBC_HELP_DRIVER_SETUP );
  strncpy( hLastProperty->szName, "Setup", INI_MAX_PROPERTY_NAME );
#ifdef DEFLIB_PATH
  strncpy( hLastProperty->szValue, DEFLIB_PATH, INI_MAX_PROPERTY_VALUE );
#else
  strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );
#endif

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType      = ODBCINST_PROMPTTYPE_FILENAME;
  hLastProperty->pszHelp        = strdup( ODBC_HELP_DRIVER_SETUP64 );
  strncpy( hLastProperty->szName, "Setup64", INI_MAX_PROPERTY_NAME );
#ifdef DEFLIB_PATH
  strncpy( hLastProperty->szValue, DEFLIB_PATH, INI_MAX_PROPERTY_VALUE );
#else
  strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );
#endif

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType        = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hLastProperty->pNext          = NULL;
  hLastProperty->bRefresh         = 0;
  hLastProperty->hDLL           = NULL;
  hLastProperty->pWidget          = NULL;
  hLastProperty->pszHelp          = strdup( ODBC_HELP_DRIVER_USAGECOUNT );
  hLastProperty->aPromptData        = NULL;
  strncpy( hLastProperty->szName, "UsageCount", INI_MAX_PROPERTY_NAME );
  strcpy( hLastProperty->szValue, "1" );

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType        = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hLastProperty->pNext          = NULL;
  hLastProperty->bRefresh         = 0;
  hLastProperty->hDLL           = NULL;
  hLastProperty->pWidget          = NULL;
  hLastProperty->pszHelp          = strdup( ODBC_HELP_DRIVER_CPTIMEOUT );
  hLastProperty->aPromptData        = NULL;
  strncpy( hLastProperty->szName, "CPTimeout", INI_MAX_PROPERTY_NAME );

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType        = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hLastProperty->pNext          = NULL;
  hLastProperty->bRefresh         = 0;
  hLastProperty->hDLL           = NULL;
  hLastProperty->pWidget          = NULL;
  hLastProperty->pszHelp          = strdup( ODBC_HELP_DRIVER_CPREUSE );
  hLastProperty->aPromptData        = NULL;
  strncpy( hLastProperty->szName, "CPReuse", INI_MAX_PROPERTY_NAME );


  // ALLOW USER TO EDIT
  pProperties = new CPropertiesFrame( this, "Properties", hFirstProperty );
  pProperties->setCaption( "Driver Properties (new)" );
  if ( pProperties->exec() )
  {
    /* DELETE ENTIRE SECTION IF IT EXISTS (no entry given) */
    if ( SQLWritePrivateProfileString( hFirstProperty->szValue, NULL, NULL, szINI ) == FALSE )
    {
      delete pProperties;
      FreeProperties( &hFirstProperty );
      qsError.sprintf( "Could not write to (%s)", szINI );
      QMessageBox::information( this, "ODBC Config",  qsError );
      return;
    }

    /* ADD ENTRIES; SECTION CREATED ON FIRST CALL */
    for ( hCurProperty = hFirstProperty->pNext; hCurProperty != NULL; hCurProperty = hCurProperty->pNext )
    {
      SQLWritePrivateProfileString( hFirstProperty->szValue, hCurProperty->szName, hCurProperty->szValue, szINI );
    }
  }
  delete pProperties;
  FreeProperties( &hFirstProperty );

  // RELOAD (slow but safe)
  Load();
}

void CDrivers::Edit()
{
  QString       qsName          = "";
  QString       qsError         = "";

  CPropertiesFrame    *pProperties;
  HODBCINSTPROPERTY hFirstProperty  = NULL;
  HODBCINSTPROPERTY hCurProperty  = NULL;
  HODBCINSTPROPERTY hLastProperty;
  char        szINI[FILENAME_MAX+1];
  QListViewItem   *pListViewItem;

  sprintf( szINI, "%s/odbcinst.ini", odbcinst_system_file_path() );

  // HAS THE USER SELECTED SOMETHING
  pListViewItem = lvwDrivers->currentItem();
  if ( pListViewItem )
    qsName    = pListViewItem->text( 0 );
  else
  {
    QMessageBox::information( this, "ODBC Config",  "Please select a Driver from the list first." );
    return;
  }


  // SET UP PROPERTIES LIST STARTING WITH MANDATORY PROPERTIES
  hFirstProperty            = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  memset( hFirstProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hFirstProperty->nPromptType     = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hFirstProperty->pNext       = NULL;
  hFirstProperty->bRefresh      = 0;
  hFirstProperty->hDLL        = NULL;
  hFirstProperty->pWidget       = NULL;
  hFirstProperty->pszHelp       = strdup( ODBC_HELP_DRIVER_NAME );
  hFirstProperty->aPromptData     = NULL;
  strncpy( hFirstProperty->szName, "Name", INI_MAX_PROPERTY_NAME );
  strcpy( hFirstProperty->szValue, qsName.data() );
  hLastProperty = hFirstProperty;

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType        = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hLastProperty->pNext          = NULL;
  hLastProperty->bRefresh         = 0;
  hLastProperty->hDLL           = NULL;
  hLastProperty->pWidget          = NULL;
  hLastProperty->pszHelp          = strdup( ODBC_HELP_DRIVER_DESC );
  hLastProperty->aPromptData        = NULL;
  strncpy( hLastProperty->szName, "Description", INI_MAX_PROPERTY_NAME );
  strcpy( hLastProperty->szValue, "" );
  SQLGetPrivateProfileString((char*) qsName.data(), hLastProperty->szName, "", hLastProperty->szValue, sizeof(hLastProperty->szValue)-1, szINI );

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType      = ODBCINST_PROMPTTYPE_FILENAME;
  hLastProperty->pszHelp        = strdup( ODBC_HELP_DRIVER_DRIVER );
  strncpy( hLastProperty->szName, "Driver", INI_MAX_PROPERTY_NAME );
  strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );
  SQLGetPrivateProfileString((char*) qsName.data(), hLastProperty->szName, "", hLastProperty->szValue, sizeof(hLastProperty->szValue)-1, szINI );

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType      = ODBCINST_PROMPTTYPE_FILENAME;
  hLastProperty->pszHelp        = strdup( ODBC_HELP_DRIVER_DRIVER64 );
  strncpy( hLastProperty->szName, "Driver64", INI_MAX_PROPERTY_NAME );
  strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );
  SQLGetPrivateProfileString((char*) qsName.data(), hLastProperty->szName, "", hLastProperty->szValue, sizeof(hLastProperty->szValue)-1, szINI );

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType      = ODBCINST_PROMPTTYPE_FILENAME;
  hLastProperty->pszHelp        = strdup( ODBC_HELP_DRIVER_SETUP );
  strncpy( hLastProperty->szName, "Setup", INI_MAX_PROPERTY_NAME );
  strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );
  SQLGetPrivateProfileString((char*) qsName.data(), hLastProperty->szName, "", hLastProperty->szValue, sizeof(hLastProperty->szValue)-1, szINI );

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType      = ODBCINST_PROMPTTYPE_FILENAME;
  hLastProperty->pszHelp        = strdup( ODBC_HELP_DRIVER_SETUP64 );
  strncpy( hLastProperty->szName, "Setup64", INI_MAX_PROPERTY_NAME );
  strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );
  SQLGetPrivateProfileString((char*) qsName.data(), hLastProperty->szName, "", hLastProperty->szValue, sizeof(hLastProperty->szValue)-1, szINI );

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType        = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hLastProperty->pNext          = NULL;
  hLastProperty->bRefresh         = 0;
  hLastProperty->hDLL           = NULL;
  hLastProperty->pWidget          = NULL;
  hLastProperty->aPromptData        = NULL;
  hLastProperty->pszHelp          = strdup( ODBC_HELP_DRIVER_USAGECOUNT );
  strncpy( hLastProperty->szName, "UsageCount", INI_MAX_PROPERTY_NAME );
  strcpy( hLastProperty->szValue, "1" );
  SQLGetPrivateProfileString((char*) qsName.data(), hLastProperty->szName, "", hLastProperty->szValue, sizeof(hLastProperty->szValue)-1, szINI );

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType        = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hLastProperty->pNext          = NULL;
  hLastProperty->bRefresh         = 0;
  hLastProperty->hDLL           = NULL;
  hLastProperty->pWidget          = NULL;
  hLastProperty->pszHelp          = strdup( ODBC_HELP_DRIVER_CPTIMEOUT );
  hLastProperty->aPromptData        = NULL;
  strncpy( hLastProperty->szName, "CPTimeout", INI_MAX_PROPERTY_NAME );
  strcpy( hLastProperty->szValue, "0" );
  SQLGetPrivateProfileString((char*) qsName.data(), hLastProperty->szName, "", hLastProperty->szValue, sizeof(hLastProperty->szValue)-1, szINI );

  hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
  hLastProperty             = hLastProperty->pNext;
  memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
  hLastProperty->nPromptType        = ODBCINST_PROMPTTYPE_TEXTEDIT;
  hLastProperty->pNext          = NULL;
  hLastProperty->bRefresh         = 0;
  hLastProperty->hDLL           = NULL;
  hLastProperty->pWidget          = NULL;
  hLastProperty->pszHelp          = strdup( ODBC_HELP_DRIVER_CPREUSE );
  hLastProperty->aPromptData        = NULL;
  strncpy( hLastProperty->szName, "CPReuse", INI_MAX_PROPERTY_NAME );
  strcpy( hLastProperty->szValue, "0" );
  SQLGetPrivateProfileString((char*) qsName.data(), hLastProperty->szName, "", hLastProperty->szValue, sizeof(hLastProperty->szValue)-1, szINI );

  // ACCEPT ANY DRIVER SPECIFIC PROPERTIES THAT HAVE BEEN TYPED IN MANUALLY OR INSTALLED 
  // NOTE: This is a quick enhancement. Much room to optimize this func.
  {
    char  szDriverName[INI_MAX_OBJECT_NAME+1];
    char  szPropertyName[INI_MAX_PROPERTY_NAME+1];
    char  szPropertyNameUpper[INI_MAX_PROPERTY_NAME+1];
    char  szPropertyValue[INI_MAX_PROPERTY_VALUE+1];

    strcpy( szDriverName, qsName.data() );
    if ( iniOpen( &hIni, szINI, "#;", '[', ']', '=', TRUE ) != INI_ERROR )
    {
      if ( iniObjectSeek( hIni, szDriverName ) == INI_SUCCESS )
      {
        while ( iniPropertyEOL( hIni ) == FALSE )
        {
          iniProperty( hIni, szPropertyName );
          iniProperty( hIni, szPropertyNameUpper );
          iniToUpper( szPropertyNameUpper );

          if ( strncmp( szPropertyNameUpper, "NAME", INI_MAX_PROPERTY_NAME ) != 0 &&
               strncmp( szPropertyNameUpper, "DESCRIPTION", INI_MAX_PROPERTY_NAME ) != 0 &&
               strncmp( szPropertyNameUpper, "DRIVER", INI_MAX_PROPERTY_NAME ) != 0 &&
               strncmp( szPropertyNameUpper, "DRIVER64", INI_MAX_PROPERTY_NAME ) != 0 &&
               strncmp( szPropertyNameUpper, "SETUP", INI_MAX_PROPERTY_NAME ) != 0 &&
               strncmp( szPropertyNameUpper, "SETUP64", INI_MAX_PROPERTY_NAME ) != 0 &&
               strncmp( szPropertyNameUpper, "CPTIMEOUT", INI_MAX_PROPERTY_NAME ) != 0 &&
               strncmp( szPropertyNameUpper, "CPREUSE", INI_MAX_PROPERTY_NAME ) != 0 &&
//                         strncmp( szPropertyNameUpper, "DONTDLCLOSE", INI_MAX_PROPERTY_NAME ) != 0 &&
               strncmp( szPropertyNameUpper, "USAGECOUNT", INI_MAX_PROPERTY_NAME ) != 0  )
          {
            iniValue( hIni, szPropertyValue );

            hLastProperty->pNext        = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
            hLastProperty             = hLastProperty->pNext;
            memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
            hLastProperty->nPromptType      = ODBCINST_PROMPTTYPE_TEXTEDIT;
            hLastProperty->pNext        = NULL;
            hLastProperty->bRefresh       = 0;
            hLastProperty->hDLL         = NULL;
            hLastProperty->pWidget        = NULL;
            hLastProperty->pszHelp        = strdup( ODBC_HELP_DRIVER_UNKNOWN );
            hLastProperty->aPromptData      = NULL;
            strncpy( hLastProperty->szName, szPropertyName, INI_MAX_PROPERTY_NAME );
            strncpy( hLastProperty->szValue, szPropertyValue, sizeof(hLastProperty->szValue)-1 );
          }
          iniPropertyNext( hIni );
        }
      }
    }
  }

  // ALLOW USER TO EDIT
  pProperties = new CPropertiesFrame( this, "Properties", hFirstProperty );
  pProperties->setCaption( "Driver Properties (edit)" );
  if ( pProperties->exec() )
  {
    /* DELETE ENTIRE SECTION IF IT EXISTS (no entry given) */
    if ( SQLWritePrivateProfileString((char*) qsName.ascii(), NULL, NULL, szINI ) == FALSE )
    {
      delete pProperties;
      FreeProperties( &hFirstProperty );
      qsError.sprintf( "Could not write to (%s)", szINI );
      QMessageBox::information( this, "ODBC Config",  qsError );
      return;
    }

    /* ADD ENTRIES; SECTION CREATED ON FIRST CALL */
    for ( hCurProperty = hFirstProperty->pNext; hCurProperty != NULL; hCurProperty = hCurProperty->pNext )
    {
      SQLWritePrivateProfileString( hFirstProperty->szValue, hCurProperty->szName, hCurProperty->szValue, szINI );
    }
  }
  delete pProperties;
  FreeProperties( &hFirstProperty );

  // RELOAD (slow but safe)
  Load();
}

void CDrivers::Delete()
{
  QListViewItem   *pListViewItem;
  char        szINI[FILENAME_MAX+1];
  char        *pszName;
  QString       qsError;
  DWORD       nErrorCode;
  char        szErrorMsg[FILENAME_MAX+1];

  sprintf( szINI, "%s/odbcinst.ini", odbcinst_system_file_path() );

  // GET SELECT DATA SOURCE NAME
  pListViewItem = lvwDrivers->currentItem();
  if ( pListViewItem )
  {
    pszName = (char *)pListViewItem->text( 0 ).ascii();
  }
  else
  {
    QMessageBox::information( this, "ODBC Config",  "Please select a Driver from the list first" );
    return;
  }

  // DELETE ENTIRE SECTION IF IT EXISTS (given NULL entry)
  if ( SQLWritePrivateProfileString( pszName, NULL, NULL, szINI ) == FALSE )
  {
    qsError.sprintf( "Could not write property list for (%s)", pszName );
    QMessageBox::information( this, "ODBC Config",  qsError );
    while ( SQLInstallerError( 1, &nErrorCode, szErrorMsg, FILENAME_MAX, NULL ) == SQL_SUCCESS )
      QMessageBox::information( this, "ODBC Config",  szErrorMsg );
  }

  // RELOAD (slow but safe)
  Load();
}



void CDrivers::Load()
{
  lvwDrivers->clear();

  /***************************************
   * METHOD 1 - Using Driver Manager
   ***************************************/


  /***************************************
   * METHOD 2 - Using ODBCINST lib (also see SQLGetInstalledDrivers)
   ***************************************/
  /*
  QListViewItem	*pListViewItem;
  QString			qsError;
  DWORD			nErrorCode;
  char   			szErrorMsg[101];

  char			szINI[FILENAME_MAX+1];
  char			szSectionNames[4096];
  char			szSectionName[INI_MAX_OBJECT_NAME+1];
  int				nElement;
  
  char 			szDriverName[INI_MAX_OBJECT_NAME+1];
  char 			szDescription[INI_MAX_PROPERTY_VALUE+1];
  char 			szDriver[INI_MAX_PROPERTY_VALUE+1];
  char 			szSetup[INI_MAX_PROPERTY_VALUE+1];

    sprintf( szINI, "%s/odbcinst.ini", odbcinst_system_file_path() );

  memset( szSectionNames, 0, sizeof(szSectionNames) );
  if ( SQLGetPrivateProfileString( NULL, NULL, NULL, szSectionNames, 4090, szINI ) >= 0 )
  {
    for ( nElement = 0; iniElement( szSectionNames, '\0', '\0', nElement, szSectionName, INI_MAX_OBJECT_NAME ) == INI_SUCCESS ; nElement++ )
    {
      szDriverName[0]		= '\0';
      szDescription[0]	= '\0';
      szDriver[0]			= '\0';
      szSetup[0]			= '\0';
      SQLGetPrivateProfileString( szSectionName, "Driver", "", szDriverName, INI_MAX_PROPERTY_VALUE, szINI );
      SQLGetPrivateProfileString( szSectionName, "Description", "", szDescription, INI_MAX_PROPERTY_VALUE, szINI );
      SQLGetPrivateProfileString( szSectionName, "Driver", "", szDriver, INI_MAX_PROPERTY_VALUE, szINI );
      SQLGetPrivateProfileString( szSectionName, "Setup", "", szSetup, INI_MAX_PROPERTY_VALUE, szINI );
      pListViewItem = new QListViewItem( lvwDrivers, szDriverName, szDescription, szDriver, szSetup );
    }
  }
  else
  {
    qsError.sprintf( "Could not load %s", szINI );
    QMessageBox::information(	this, "ODBC Config",  qsError );
    while ( SQLInstallerError( 1, &nErrorCode, szErrorMsg, 100, NULL ) == SQL_SUCCESS )
      QMessageBox::information( this, "ODBC Config",  szErrorMsg );
  }
  */

  /***************************************
   * METHOD 3 - Using INI lib
   ***************************************/
  char      szDriverName[INI_MAX_OBJECT_NAME+1];
  char      szPropertyName[INI_MAX_PROPERTY_NAME+1];
  char      szDescription[INI_MAX_PROPERTY_VALUE+1];
  char      szDriver[INI_MAX_PROPERTY_VALUE+1];
  char      szDriver64[INI_MAX_PROPERTY_VALUE+1];
  char      szSetup[INI_MAX_PROPERTY_VALUE+1];
  char      szSetup64[INI_MAX_PROPERTY_VALUE+1];
  QListViewItem *pListViewItem;
  QString     qsError;

  sprintf( szINI, "%s/odbcinst.ini", odbcinst_system_file_path());

  if ( iniOpen( &hIni, szINI, "#;", '[', ']', '=', TRUE ) != INI_ERROR )
  {
    iniObjectFirst( hIni );
    while ( iniObjectEOL( hIni ) == FALSE )
    {
      szDriverName[0] = '\0';
      szDescription[0]  = '\0';
      szDriver[0]   = '\0';
      szDriver64[0]   = '\0';
      szSetup64[0]   = '\0';
      szSetup[0]    = '\0';
      iniObject( hIni, szDriverName );
      iniPropertyFirst( hIni );
      if ( strcmp( szDriverName, "ODBC" ) == 0 )
      {
        iniObjectNext( hIni );
        continue;
      }

      while ( iniPropertyEOL( hIni ) == FALSE )
      {
        iniProperty( hIni, szPropertyName );
        iniToUpper( szPropertyName );

        if ( strncmp( szPropertyName, "DESCRIPTION", INI_MAX_PROPERTY_NAME ) == 0 )
          iniValue( hIni, szDescription );
#ifdef PLATFORM64
	if ( strncmp( szPropertyName, "DRIVER64", INI_MAX_PROPERTY_NAME ) == 0 )
          		iniValue( hIni, szDriver64 );
	if ( strncmp( szPropertyName, "DRIVER", INI_MAX_PROPERTY_NAME ) == 0 )
          		iniValue( hIni, szDriver );

        if ( strncmp( szPropertyName, "SETUP64", INI_MAX_PROPERTY_NAME ) == 0 )
          iniValue( hIni, szSetup64 );
        if ( strncmp( szPropertyName, "SETUP", INI_MAX_PROPERTY_NAME ) == 0 )
          iniValue( hIni, szSetup );
#else
        if ( strncmp( szPropertyName, "DRIVER", INI_MAX_PROPERTY_NAME ) == 0 )
          iniValue( hIni, szDriver );

        if ( strncmp( szPropertyName, "SETUP", INI_MAX_PROPERTY_NAME ) == 0 )
          iniValue( hIni, szSetup );
#endif

        iniPropertyNext( hIni );
      }

#ifdef PLATFORM64
      if ( szDriver64[ 0 ] != '\0' )
      {
	      strcpy( szDriver, szDriver64 );
      }
      if ( szSetup64[ 0 ] != '\0' )
      {
	      strcpy( szSetup, szSetup64 );
      }
#endif

      pListViewItem = new QListViewItem( lvwDrivers, szDriverName, szDescription, szDriver, szSetup );
      iniObjectNext( hIni );
    }
  }
  else
  {
    qsError.sprintf( "Could not open system file at %s", szINI );
    QMessageBox::information( this, "ODBC Config",  qsError );
  }

}

void CDrivers::FreeProperties( HODBCINSTPROPERTY *hFirstProperty )
{
  HODBCINSTPROPERTY hNextProperty;
  HODBCINSTPROPERTY hCurProperty;

  /* SANITY CHECKS */
  if ( (*hFirstProperty) == NULL )
    return;

  /* FREE MEMORY */
  for ( hCurProperty = (*hFirstProperty); hCurProperty != NULL; hCurProperty = hNextProperty )
  {
    hNextProperty = hCurProperty->pNext;

    /* FREE ANY PROMPT DATA (ie pick list options and such) */
    if ( hCurProperty->aPromptData != NULL )
      free( hCurProperty->aPromptData );

    /* FREE OTHER STUFF */
    if ( hCurProperty->pszHelp != NULL )
      free( hCurProperty->pszHelp );

    free( hCurProperty );
  }
  (*hFirstProperty) = NULL;
}

