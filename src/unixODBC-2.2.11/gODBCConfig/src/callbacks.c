#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>
#include <ctype.h>

#include <sql.h>
#include <odbcinst.h>
#include <odbcinstext.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

static char add_driver_name[ 257 ];
static char add_driver_description[ 257 ];
static char add_driver_driver[ 257 ];
static char add_driver_setup[ 257 ];
static char add_driver_fileusage[ 257 ];
static GtkWidget *clist1;
static GtkWidget *clist5;
static GtkWidget *clist6;
static int clist6_row;
static int clist5_row;
static int clist1_row;
static int driver_mode;

void
on_browse_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

static void
message_box( char *txt, void *mode )
{
  GtkWidget *messagebox1;
  GtkWidget *dialog_vbox11;
  GtkWidget *button66;
  GtkWidget *dialog_action_area11;

  messagebox1 = gnome_message_box_new ( txt,
                              mode, NULL);
  gtk_object_set_data (GTK_OBJECT (messagebox1), "messagebox1", messagebox1);
  gtk_window_set_modal (GTK_WINDOW (messagebox1), TRUE);
  gtk_window_set_policy (GTK_WINDOW (messagebox1), FALSE, FALSE, FALSE);
  gnome_dialog_set_close (GNOME_DIALOG (messagebox1), TRUE);

  dialog_vbox11 = GNOME_DIALOG (messagebox1)->vbox;
  gtk_object_set_data (GTK_OBJECT (messagebox1), "dialog_vbox11", dialog_vbox11);
  gtk_widget_show (dialog_vbox11);

  gnome_dialog_append_button (GNOME_DIALOG (messagebox1), GNOME_STOCK_BUTTON_OK);
  button66 = g_list_last (GNOME_DIALOG (messagebox1)->buttons)->data;
  gtk_widget_ref (button66);
  gtk_object_set_data_full (GTK_OBJECT (messagebox1), "button66", button66,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button66);
  GTK_WIDGET_SET_FLAGS (button66, GTK_CAN_DEFAULT);

  dialog_action_area11 = GNOME_DIALOG (messagebox1)->action_area;
  gtk_widget_ref (dialog_action_area11);
  gtk_object_set_data_full (GTK_OBJECT (messagebox1), "dialog_action_area11", dialog_action_area11,
                            (GtkDestroyNotify) gtk_widget_unref);

  gtk_widget_show (messagebox1);
}

static void 
fill_list( GtkCList *list, int mode )
{    
	char            szINI[ FILENAME_MAX + 1 ];
	char            szSectionNames[ 4095 ];
	int 			i;

	strcpy( szINI, "ODBC.INI" );
	memset( szSectionNames, 0, sizeof(szSectionNames) );
    SQLSetConfigMode( mode );

	if ( SQLGetPrivateProfileString( NULL, NULL, NULL, szSectionNames, 
							sizeof( szSectionNames ), szINI ) >= 0 )
	{
		gchar *array[ 3 ];
		char szDriver[ INI_MAX_OBJECT_NAME + 1 ];
		char szDescription[ INI_MAX_OBJECT_NAME + 1 ];
		char szSectionName[ INI_MAX_OBJECT_NAME + 1 ];

		for ( i = 0; 
			iniElement( szSectionNames, '\0', '\0', i, szSectionName, INI_MAX_OBJECT_NAME ) == INI_SUCCESS; 
			i++ )
		{
			/* 
			 * GET DRIVER AND
			 * DESCRIPTION
			 */

			szDriver[0] = '\0';
			szDescription[0] = '\0';

			SQLGetPrivateProfileString( szSectionName, "Driver",
				"", szDriver, INI_MAX_PROPERTY_VALUE, szINI );

			SQLGetPrivateProfileString( szSectionName, "Description",
				"", szDescription, INI_MAX_PROPERTY_VALUE, szINI);

			array[ 0 ] = szSectionName;
			array[ 1 ] = szDriver;
			array[ 2 ] = szDescription;

			gtk_clist_append( list, array );
		}
		SQLSetConfigMode( ODBC_BOTH_DSN);
	}
	else
	{
		char txt[ 1024 ];

		SQLSetConfigMode( ODBC_BOTH_DSN );
		sprintf( txt, _("Could not load %s"), szINI );
  		message_box( txt, GNOME_MESSAGE_BOX_ERROR );
	}
}


void
on_clist1_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
	clist1 = widget;
	clist1_row = -1;

	gtk_clist_clear( GTK_CLIST( widget ));
	fill_list( GTK_CLIST( widget ), ODBC_USER_DSN );
}


void
on_clist5_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
	clist5 = widget;
	clist5_row = -1;

	gtk_clist_clear( GTK_CLIST( widget ));
	fill_list( GTK_CLIST( widget ), ODBC_SYSTEM_DSN );
}


void
on_button38_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *d1;
	
	d1 = create_dialog1();
  	gtk_widget_show (d1);
}


void
on_button54_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( user_data );
}


void
on_button39_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *d2;
	
	d2 = create_dialog2();
  	gtk_widget_show (d2);
}


void
on_button55_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}


void
on_button56_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}


void
on_button40_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *d3;
	
	d3 = create_dialog3();
  	gtk_widget_show (d3);
}


void
on_button41_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *d4;
	
	d4 = create_dialog4();
  	gtk_widget_show (d4);
}


void
on_button57_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}


void
on_button43_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *d5;
	
	d5 = create_dialog5();
  	gtk_widget_show (d5);
}


void
on_button58_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}


void
on_button44_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *d6;
	
	d6 = create_dialog6();
  	gtk_widget_show (d6);
}


void
on_button59_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}


void
on_button42_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *d7;
	
	d7 = create_dialog7();
  	gtk_widget_show (d7);
}


void
on_button60_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}


void
on_button53_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *d8;
	
	d8 = create_dialog8();
  	gtk_widget_show (d8);
}


void
on_button61_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}

void
on_dialog1_close                       (GtkDialog       *dialog,
		                         gpointer         user_data)
{ 
	printf( "close\n" );
} 
  
void
on_dialog1_destroy                     (GtkObject       *object,
		                         gpointer         user_data)
{                   
	        gtk_exit (0);
	printf( "exit\n" );
}                   

void
on_clist6_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
	char    szINI[ FILENAME_MAX + 1 ];
	char 	szDriverName[INI_MAX_OBJECT_NAME+1];
	char	szPropertyName[INI_MAX_PROPERTY_NAME+1];
	char 	szDescription[INI_MAX_PROPERTY_VALUE+1];
	char 	szDriver[INI_MAX_PROPERTY_VALUE+1];
	char 	szSetup[INI_MAX_PROPERTY_VALUE+1];
	HINI	hIni;

	clist6 = widget;
	clist6_row = -1;

	gtk_clist_clear( GTK_CLIST( widget ));

	sprintf( szINI, "%s/odbcinst.ini", odbcinst_system_file_path());

	if ( iniOpen( &hIni, szINI, "#;", '[', ']', '=', TRUE ) != INI_ERROR )
	{
		gchar *array[ 4 ];

		iniObjectFirst( hIni );
		while ( iniObjectEOL( hIni ) == FALSE )
		{
			szDriverName[0] = '\0';
			szDescription[0] 	= '\0';
			szDriver[0] 	= '\0';
			szSetup[0] 		= '\0';
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
				if ( strncmp( szPropertyName, "DRIVER", INI_MAX_PROPERTY_NAME ) == 0 )
					iniValue( hIni, szDriver );
				if ( strncmp( szPropertyName, "SETUP", INI_MAX_PROPERTY_NAME ) == 0 )
					iniValue( hIni, szSetup );

				iniPropertyNext( hIni );
			}

			array[ 0 ] = szDriverName;
			array[ 1 ] = szDescription;
			array[ 2 ] = szDriver;
			array[ 3 ] = szSetup;

			gtk_clist_append( GTK_CLIST( widget ), array );

			iniObjectNext( hIni );
		}
		iniClose( hIni );
	}
	else
	{
		char txt[ 1024 ];

		SQLSetConfigMode( ODBC_BOTH_DSN );
		sprintf( txt, _("Could not load %s"), szINI );
  		message_box( txt, GNOME_MESSAGE_BOX_ERROR );
	}
}


void
on_button25_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *win;

	add_driver_name[ 0 ] = '\0';
	add_driver_description[ 0 ] = '\0';
	strcpy( add_driver_driver, odbcinst_system_file_path() );
	strcat( add_driver_driver, "/lib" );
	strcpy( add_driver_setup, odbcinst_system_file_path() );
	strcat( add_driver_setup, "/lib" );
	strcpy( add_driver_fileusage, "1" );

	win = create_window1();
  	gtk_widget_show (win);
}


void
on_button68_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	char szINI[ FILENAME_MAX + 1 ];
	char txt[ 256 ];

    sprintf( szINI, "%s/odbcinst.ini", odbcinst_system_file_path() );

	if ( SQLWritePrivateProfileString( add_driver_name, NULL, NULL, szINI ) == FALSE )
	{
		sprintf( txt, _("Could not write to (%s)"), szINI );
  		message_box( txt, GNOME_MESSAGE_BOX_ERROR );
	}
	else
	{
		SQLWritePrivateProfileString( add_driver_name, "Description", 
						add_driver_description, szINI );
		SQLWritePrivateProfileString( add_driver_name, "Driver", 
						add_driver_driver, szINI );
		SQLWritePrivateProfileString( add_driver_name, "Setup", 
						add_driver_setup, szINI );
		SQLWritePrivateProfileString( add_driver_name, "FileUsage", 
						add_driver_fileusage, szINI );

		on_clist6_realize( clist6, NULL );
	}
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}


void
on_entry2_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
	gchar *text = gtk_editable_get_chars( editable, 0, -1 );
	strcpy( add_driver_name, text );
	g_free( text );
}


void
on_button69_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}


void
on_entry3_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
	gchar *text = gtk_editable_get_chars( editable, 0, -1 );
	strcpy( add_driver_description, text );
	g_free( text );
}


void
on_entry4_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
	gchar *text = gtk_editable_get_chars( editable, 0, -1 );
	strcpy( add_driver_driver, text );
	g_free( text );
}


void
on_entry5_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
	gchar *text = gtk_editable_get_chars( editable, 0, -1 );
	strcpy( add_driver_setup, text );
	g_free( text );
}


void
on_entry6_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
	gchar *text = gtk_editable_get_chars( editable, 0, -1 );
	strcpy( add_driver_fileusage, text );
	g_free( text );
}


void
on_entry4_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
	gint pos = 0;

	gtk_editable_insert_text( GTK_EDITABLE( widget ), add_driver_driver, 
					strlen( add_driver_driver ), &pos );
}


void
on_entry5_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{

	gint pos = 0;

	gtk_editable_insert_text( GTK_EDITABLE( widget ), add_driver_setup, 
					strlen( add_driver_setup ), &pos );
}


void
on_entry6_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
	gint pos = 0;

	gtk_editable_insert_text( GTK_EDITABLE( widget ), add_driver_fileusage, 
					strlen( add_driver_fileusage ), &pos );
}



void
on_button26_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	if ( clist6_row < 0 )
	{
  		message_box( _("Select a driver to remove"), GNOME_MESSAGE_BOX_INFO );
	}
	else
	{
		gchar *ptr;
		CHAR szINI[ FILENAME_MAX + 1 ];
		char txt[ 256 ];

		gtk_clist_get_text( GTK_CLIST( user_data ), clist6_row,
						0, &ptr );

    	sprintf( szINI, "%s/odbcinst.ini", odbcinst_system_file_path() );

		if ( SQLWritePrivateProfileString( ptr, NULL, NULL, szINI ) == FALSE )
		{
			sprintf( txt, "Could not write to (%s)", szINI );
  			message_box( txt, GNOME_MESSAGE_BOX_ERROR );
		}

		on_clist6_realize( user_data, NULL );
	}
}


void
on_clist6_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
	clist6_row = row;
}


void
on_button27_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	if ( clist6_row < 0 )
	{
  		message_box( _("Select a driver to configure"), GNOME_MESSAGE_BOX_INFO );
	}
	else
	{
		gchar *ptr;
		char szINI[ FILENAME_MAX + 1 ];
		GtkWidget *win;

		gtk_clist_get_text( GTK_CLIST( clist6 ), clist6_row,
						0, &ptr );

    	sprintf( szINI, "%s/odbcinst.ini", odbcinst_system_file_path() );

		/*
		 * setup the fields
		 */

		strcpy( add_driver_name, ptr );

		SQLGetPrivateProfileString( add_driver_name, "Description", "",
						add_driver_description, sizeof( add_driver_description ), szINI );
		SQLGetPrivateProfileString( add_driver_name, "Driver", "",
						add_driver_driver, sizeof( add_driver_driver ), szINI );
		SQLGetPrivateProfileString( add_driver_name, "Setup", "",
						add_driver_setup, sizeof( add_driver_setup ), szINI );
		SQLGetPrivateProfileString( add_driver_name, "FileUsage", "",
						add_driver_fileusage, sizeof( add_driver_fileusage ), szINI );

		win = create_window2();
  		gtk_widget_show (win);
	}
}


void
on_entry2_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
	gint pos = 0;

	gtk_editable_insert_text( GTK_EDITABLE( widget ), add_driver_name, 
					strlen( add_driver_name ), &pos );
}


void
on_entry3_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
	gint pos = 0;

	gtk_editable_insert_text( GTK_EDITABLE( widget ), add_driver_description, 
					strlen( add_driver_description ), &pos );
}



void
on_clist1_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
	clist1_row = row;
}


void
on_clist5_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
	clist5_row = row;
}


void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *win;

	add_driver_name[ 0 ] = '\0';
	add_driver_description[ 0 ] = '\0';
	strcpy( add_driver_driver, odbcinst_system_file_path() );
	strcat( add_driver_driver, "/lib" );
	strcpy( add_driver_setup, odbcinst_system_file_path() );
	strcat( add_driver_setup, "/lib" );
	strcpy( add_driver_fileusage, "1" );

	driver_mode = ODBC_USER_DSN;

	win = create_window5();
  	gtk_widget_show (win);
}


void
on_clist7_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
	clist6 = widget;
	on_clist6_realize( widget, user_data );
}


void
on_button79_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	on_button25_clicked( button, user_data );
}


static void
on_add_dsn_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *dialog9 = GTK_WIDGET( user_data );
	GtkWidget *entry;
	gchar *driver_name;
	gchar *dsn_name;
	HODBCINSTPROPERTY	hFirstProperty	= NULL;
	HODBCINSTPROPERTY	hCurProperty	= NULL;
	char szINI[ FILENAME_MAX + 1 ];

	entry = gtk_object_get_data( GTK_OBJECT( dialog9 ), "dsn_value" );
	dsn_name = gtk_entry_get_text( GTK_ENTRY( entry ));

	entry = gtk_object_get_data( GTK_OBJECT( dialog9 ), "driver_value" );
	driver_name = gtk_entry_get_text( GTK_ENTRY( entry ));

	if ( strlen( dsn_name ) < 1 )
	{
		char txt[ 256 ];

		sprintf( txt, _("Enter a DSN name")  );
		message_box( txt, GNOME_MESSAGE_BOX_ERROR );
		return;
	}

	/*
	 * extract properties
	 */

	if ( ODBCINSTConstructProperties( driver_name, 
							&hFirstProperty ) != ODBCINST_SUCCESS )
	{
		char txt[ 256 ];

		sprintf( txt, _("Could not construct a property list for (%s)"), driver_name );
		message_box( txt, GNOME_MESSAGE_BOX_ERROR );
	}
	else
	{
		SQLSetConfigMode( driver_mode );

		strcpy( szINI, "ODBC.INI" );

		if ( SQLWritePrivateProfileString( dsn_name, NULL, NULL, szINI ) == FALSE )
		{
			char txt[ 256 ];
			sprintf( txt, _("Could not write to (%s)"), szINI );
  			message_box( txt, GNOME_MESSAGE_BOX_ERROR );
		}
		else
		{
			for ( hCurProperty = hFirstProperty->pNext; 
							hCurProperty != NULL; 
							hCurProperty = hCurProperty->pNext )
			{
				switch ( hCurProperty->nPromptType )
				{
					case ODBCINST_PROMPTTYPE_LABEL:
					{
						GtkWidget *label;
						char *str;
	
						label = gtk_object_get_data( GTK_OBJECT (dialog9), hCurProperty->szName );
	
						gtk_label_get( GTK_LABEL( label ), &str );
						SQLWritePrivateProfileString( dsn_name, 
										hCurProperty->szName, str, szINI );

					}
					break;
	
					case ODBCINST_PROMPTTYPE_LISTBOX:
					{
						GtkWidget *optionmenu_menu;
						GtkWidget *menuitem;
						gchar *ptr;
	
						optionmenu_menu = gtk_object_get_data( GTK_OBJECT (dialog9), hCurProperty->szName );
	
						menuitem = gtk_menu_get_active( GTK_MENU( optionmenu_menu ));
	
						ptr = gtk_widget_get_name( menuitem );

						SQLWritePrivateProfileString( dsn_name, 
										hCurProperty->szName, ptr, szINI );
					}
					break;
	
					case ODBCINST_PROMPTTYPE_COMBOBOX:
					case ODBCINST_PROMPTTYPE_FILENAME:
					default: /* PROMPTTYPE_TEXTEDIT AND PROMPTTYPE_TEXTEDIT_PASSWORD */
					{
						GtkWidget *entry;
						gchar *ptr;
	
						entry = gtk_object_get_data( GTK_OBJECT (dialog9), hCurProperty->szName );
						ptr = gtk_entry_get_text (GTK_ENTRY (entry));

						SQLWritePrivateProfileString( dsn_name, 
										hCurProperty->szName, ptr, szINI );
					}
					break;
				}
			}
		}
		ODBCINSTDestructProperties( &hFirstProperty );
		SQLSetConfigMode( ODBC_BOTH_DSN );
	}
	
	gtk_widget_destroy( dialog9 );

	if ( clist1 )
		on_clist1_realize( clist1, NULL );
	if ( clist5 )
		on_clist5_realize( clist5, NULL );
}


static void
on_add_dsn_cancel_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}

static void
add_dsn                    				(GtkButton       *button,
                                        gpointer         user_data,
										int mode )
{
	if ( clist6_row < 0 )
	{
  		message_box( _("Select a driver to Use"), GNOME_MESSAGE_BOX_INFO );
	}
	else
	{
		char *ptr;

        /*
         * can we call SQLConfigDataSource ?
         */

		gtk_clist_get_text( GTK_CLIST( clist6 ), clist6_row,
						0, &ptr );

		if ( SQLConfigDataSource(( HWND ) 1, mode,
                ptr, "" ))
		{
        }
		else
		{
			/*
		 	 * extract properties
		 	 */

			HODBCINSTPROPERTY	hFirstProperty	= NULL;
			HODBCINSTPROPERTY	hCurProperty	= NULL;

			if ( ODBCINSTConstructProperties( ptr, 
									&hFirstProperty ) != ODBCINST_SUCCESS )
			{
				char txt[ 256 ];

				sprintf( txt, _("Could not construct a property list for (%s)"), ptr );
  				message_box( txt, GNOME_MESSAGE_BOX_ERROR );
			}
			else
			{
  				GtkWidget *dialog9;
  				GtkWidget *dialog_vbox9;
  				GtkWidget *hbox2;
  				GtkWidget *table8;
  				GtkWidget *dialog_action_area9;
  				GtkWidget *button85;
  				GtkWidget *button87;
				GtkWidget *label;
				GtkWidget *entry;
				int slot;
  
  				dialog9 = gnome_dialog_new (NULL, NULL);
  				gtk_object_set_data (GTK_OBJECT (dialog9), "dialog9", dialog9);
  				gtk_window_set_policy (GTK_WINDOW (dialog9), FALSE, FALSE, FALSE);
  				gtk_window_set_modal (GTK_WINDOW (dialog9), TRUE);

  				dialog_vbox9 = GNOME_DIALOG (dialog9)->vbox;
  				gtk_object_set_data (GTK_OBJECT (dialog9), "dialog_vbox9", dialog_vbox9);
  				gtk_widget_show (dialog_vbox9);

  				hbox2 = gtk_hbox_new (FALSE, 0);
  				gtk_widget_ref (hbox2);
  				gtk_object_set_data_full (GTK_OBJECT (dialog9), "hbox2", hbox2,
                            				(GtkDestroyNotify) gtk_widget_unref);
  				gtk_widget_show (hbox2);
  				gtk_box_pack_start (GTK_BOX (dialog_vbox9), hbox2, TRUE, TRUE, 0);
				
  				table8 = gtk_table_new (50, 2, FALSE);
  				gtk_widget_ref (table8);
  				gtk_object_set_data_full (GTK_OBJECT (dialog9), "table8", table8,
                            				(GtkDestroyNotify) gtk_widget_unref);
  				gtk_widget_show (table8);
  				gtk_box_pack_start (GTK_BOX (hbox2), table8, TRUE, TRUE, 0);

				label = gtk_label_new (_("DSN"));
				gtk_label_set_justify( GTK_LABEL( label ), GTK_JUSTIFY_LEFT );

				gtk_widget_ref (label);
				gtk_object_set_data_full (GTK_OBJECT (dialog9), "dsn_name", 
							label, (GtkDestroyNotify) gtk_widget_unref);
					
				gtk_widget_show( label );
				gtk_table_attach (GTK_TABLE (table8), label, 0, 1, 0, 1,
                    			(GtkAttachOptions) (0),
                   				(GtkAttachOptions) (0), 0, 0);

				entry= gtk_entry_new ();
				gtk_widget_ref (entry);

				if ( mode == ODBC_CONFIG_DSN || mode == ODBC_CONFIG_SYS_DSN )
				{
					gtk_entry_set_editable( GTK_ENTRY( entry ), FALSE );
				}

				gtk_object_set_data_full (GTK_OBJECT (dialog9), 
								"dsn_value", entry,
								(GtkDestroyNotify) gtk_widget_unref);

				/*gtk_entry_set_text( GTK_ENTRY( entry ), ptr );*/

				gtk_widget_show (entry);
				gtk_table_attach (GTK_TABLE (table8), entry, 1, 2, 
								0, 1,
								(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
								(GtkAttachOptions) (0), 0, 0);

				entry= gtk_entry_new ();
				gtk_widget_ref (entry);
				gtk_entry_set_editable( GTK_ENTRY( entry ), FALSE );
				gtk_entry_set_text( GTK_ENTRY( entry ), ptr );

				gtk_object_set_data_full (GTK_OBJECT (dialog9), 
								"driver_value", entry,
								(GtkDestroyNotify) gtk_widget_unref);

				for ( hCurProperty = hFirstProperty->pNext, slot = 1; 
								hCurProperty != NULL; 
								hCurProperty = hCurProperty->pNext, slot ++ )
				{
  					label = gtk_label_new (_(hCurProperty->szName));
					gtk_label_set_justify( GTK_LABEL( label ), GTK_JUSTIFY_LEFT );

  					gtk_widget_ref (label);
  					gtk_object_set_data_full (GTK_OBJECT (dialog9), hCurProperty->szName, 
							label, (GtkDestroyNotify) gtk_widget_unref);
					
  					gtk_widget_show( label );
  					gtk_table_attach (GTK_TABLE (table8), label, 0, 1, slot, slot + 1,
                    			(GtkAttachOptions) (0),
                   				(GtkAttachOptions) (0), 0, 0);

					switch ( hCurProperty->nPromptType )
					{
					 	case ODBCINST_PROMPTTYPE_LABEL:
					  	{
  							GtkWidget *label;

  							label = gtk_label_new ( _(hCurProperty->szValue ));
  							gtk_widget_ref (label);

  							gtk_object_set_data_full (GTK_OBJECT (dialog9), 
											hCurProperty->szName, label,
                            				(GtkDestroyNotify) gtk_widget_unref);

  							gtk_widget_show (label);
  							gtk_table_attach (GTK_TABLE (table8), label, 1, 2, 
											slot, slot + 1,
                    						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    						(GtkAttachOptions) (0), 0, 0);
						}
						break;

						case ODBCINST_PROMPTTYPE_LISTBOX:
						{
							GtkWidget *optionmenu;
							GtkWidget *optionmenu_menu;
							GtkWidget *menuitem;
							char *ptr;
							int i;

  							optionmenu = gtk_option_menu_new ();
  							gtk_widget_ref (optionmenu);

  							gtk_widget_show (optionmenu);

  							optionmenu_menu = gtk_menu_new();

  							gtk_object_set_data(GTK_OBJECT (dialog9), 
											hCurProperty->szName,
											optionmenu_menu );

							ptr = hCurProperty->aPromptData[ i = 0 ];

							while( ptr )
							{
    							menuitem = gtk_menu_item_new_with_label (_(ptr));
								gtk_widget_set_name( menuitem, _(ptr));
	  							gtk_widget_show (menuitem);
  								gtk_menu_append (GTK_MENU (optionmenu_menu), menuitem);

								ptr = hCurProperty->aPromptData[ ++ i ];
							}

  							gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu), optionmenu_menu );
  							gtk_table_attach (GTK_TABLE (table8), optionmenu, 1, 2, 
												slot, slot + 1,
                    							(GtkAttachOptions) (GTK_FILL),
                    							(GtkAttachOptions) (GTK_FILL), 0, 0);
            			}
						break;

						case ODBCINST_PROMPTTYPE_COMBOBOX:
						{
							GtkWidget *combo1;
							GList *combo1_items = NULL;
  							GtkWidget *combo_entry1;
							char *ptr;
							int i;

  							combo1 = gtk_combo_new ();
  							gtk_widget_ref (combo1);
  							gtk_object_set_data_full (GTK_OBJECT (dialog9), "Combo", 
											combo1,
                            				(GtkDestroyNotify) gtk_widget_unref);
  							gtk_widget_show (combo1);

							ptr = hCurProperty->aPromptData[ i = 0 ];

							while( ptr )
							{
  								combo1_items = g_list_append (combo1_items, _(ptr));
								ptr = hCurProperty->aPromptData[ ++ i ];
							}

  							gtk_combo_set_popdown_strings (GTK_COMBO (combo1), combo1_items);
  							g_list_free (combo1_items);

  							combo_entry1 = GTK_COMBO (combo1)->entry;
  							gtk_widget_ref (combo_entry1);
  							gtk_object_set_data_full (GTK_OBJECT (dialog9), 
														hCurProperty->szName, 
														combo_entry1,
                            							(GtkDestroyNotify) gtk_widget_unref);
  							gtk_widget_show (combo_entry1);

							ptr = hCurProperty->aPromptData[ i = 0 ];
  							gtk_entry_set_text (GTK_ENTRY (combo_entry1), _(ptr));

  							gtk_table_attach (GTK_TABLE (table8), combo1, 1, 2, 
												slot, slot + 1,
                    							(GtkAttachOptions) (GTK_FILL),
                    							(GtkAttachOptions) (GTK_FILL), 0, 0);
						}
						break;

						case ODBCINST_PROMPTTYPE_FILENAME:
						{
							GtkWidget *hbox3;
							GtkWidget *entry;
							GtkWidget *button;

  							hbox3 = gtk_hbox_new (FALSE, 0);
  							gtk_widget_ref (hbox3);
  							gtk_object_set_data_full (GTK_OBJECT (dialog9), "hbox3", hbox3,
                            				(GtkDestroyNotify) gtk_widget_unref);
  							gtk_widget_show (hbox3);
  							gtk_table_attach (GTK_TABLE (table8), hbox3, 1, 2, 
												slot, slot + 1,
                    							(GtkAttachOptions) (GTK_FILL),
                    							(GtkAttachOptions) (GTK_FILL), 0, 0);

  							entry = gtk_entry_new ();
  							gtk_widget_ref (entry);
  							gtk_object_set_data_full (GTK_OBJECT (dialog9), hCurProperty->szName, 
														entry,
                            							(GtkDestroyNotify) gtk_widget_unref);
  							gtk_widget_show (entry);
  							gtk_box_pack_start (GTK_BOX (hbox3), entry, TRUE, TRUE, 0);
							
  							button = gtk_button_new_with_label (_("Browse"));
  							gtk_widget_ref (button);
  							gtk_object_set_data_full (GTK_OBJECT (dialog9), "button", button,
                            							(GtkDestroyNotify) gtk_widget_unref);
  							gtk_widget_show (button);
  							gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);

  							gtk_signal_connect (GTK_OBJECT (button), "clicked",
                      				GTK_SIGNAL_FUNC (on_browse_clicked),
                      				entry );
						}
						break;

						default: /* PROMPTTYPE_TEXTEDIT and PROMPTTYPE_TEXTEDIT_PASSWORD */
						{
  							GtkWidget *entry;

  							entry= gtk_entry_new ();
  							gtk_widget_ref (entry);

  							gtk_object_set_data_full (GTK_OBJECT (dialog9), 
											hCurProperty->szName, entry,
                            				(GtkDestroyNotify) gtk_widget_unref);

							gtk_entry_set_text( GTK_ENTRY( entry ), hCurProperty->szValue );

						if (hCurProperty->nPromptType == ODBCINST_PROMPTTYPE_TEXTEDIT_PASSWORD) 
                                                        gtk_entry_set_visibility(GTK_ENTRY(entry),false);

  							gtk_widget_show (entry);
  							gtk_table_attach (GTK_TABLE (table8), entry, 1, 2, 
											slot, slot + 1,
                    						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    						(GtkAttachOptions) (0), 0, 0);
						}
						break;
					}
				}
				ODBCINSTDestructProperties( &hFirstProperty );
				
  				dialog_action_area9 = GNOME_DIALOG (dialog9)->action_area;
  				gtk_object_set_data (GTK_OBJECT (dialog9), "dialog_action_area9", dialog_action_area9);
  				gtk_widget_show (dialog_action_area9);
  				gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area9), GTK_BUTTONBOX_END);
  				gtk_button_box_set_spacing (GTK_BUTTON_BOX (dialog_action_area9), 8);
				
  				gnome_dialog_append_button (GNOME_DIALOG (dialog9), GNOME_STOCK_BUTTON_OK);
  				button85 = g_list_last (GNOME_DIALOG (dialog9)->buttons)->data;
  				gtk_widget_ref (button85);
  				gtk_object_set_data_full (GTK_OBJECT (dialog9), "button85", button85,
                            				(GtkDestroyNotify) gtk_widget_unref);
  				gtk_widget_show (button85);
  				GTK_WIDGET_SET_FLAGS (button85, GTK_CAN_DEFAULT);

  				gnome_dialog_append_button (GNOME_DIALOG (dialog9), GNOME_STOCK_BUTTON_CANCEL);
  				button87 = g_list_last (GNOME_DIALOG (dialog9)->buttons)->data;
  				gtk_widget_ref (button87);
  				gtk_object_set_data_full (GTK_OBJECT (dialog9), "button87", button87,
                            (GtkDestroyNotify) gtk_widget_unref);
  				gtk_widget_show (button87);
  				GTK_WIDGET_SET_FLAGS (button87, GTK_CAN_DEFAULT);

				gtk_signal_connect (GTK_OBJECT (button85), "clicked",
						GTK_SIGNAL_FUNC (on_add_dsn_ok_clicked),
						dialog9 );

				gtk_signal_connect (GTK_OBJECT (button87), "clicked",
						GTK_SIGNAL_FUNC (on_add_dsn_cancel_clicked),
						dialog9 );
				
  				gtk_widget_show (dialog9);
			}
		}
		
		if ( user_data )
			gtk_widget_destroy( GTK_WIDGET( user_data ));
	}
}

static void
edit_dsn                   				(GtkButton       *button,
                                        gpointer         user_data,
										int mode )
{
	char constr[ 256 ];
	char szINI[ FILENAME_MAX + 1 ];
	char value[ 1024 ];
	GtkWidget *cl;
	int cr;

	if ( mode == ODBC_CONFIG_DSN )
	{
		cl = clist1;
		cr = clist1_row;
		driver_mode = ODBC_USER_DSN;
	}
	else
	{
		cl = clist5;
		cr = clist5_row;
		driver_mode = ODBC_SYSTEM_DSN;
	}

	if ( cr < 0 )
	{
  		message_box( _("Select a DSN to configure"), GNOME_MESSAGE_BOX_INFO );
	}
	else
	{
		char *dsn;
		char driver_name[ 256 ];

        /*
         * can we call SQLConfigDataSource ?
         */

		gtk_clist_get_text( GTK_CLIST( cl ), cr,
						0, &dsn );

	    SQLSetConfigMode( driver_mode );

		strcpy( szINI, "ODBC.INI" );

		SQLGetPrivateProfileString( dsn, "Driver", "", driver_name, sizeof( driver_name ), szINI );
		if ( strlen( driver_name ) < 1 )
		{
			message_box( _("Unable to find a Driver line for this entry"), GNOME_MESSAGE_BOX_INFO );
			return;
		}

		sprintf( constr, "DSN=%s", dsn );

		if ( SQLConfigDataSource(( HWND ) 1, mode,
                driver_name, constr ))
		{
        }
		else
		{
			HODBCINSTPROPERTY	hFirstProperty	= NULL;
			HODBCINSTPROPERTY	hCurProperty	= NULL;

			/*
		 	 * extract properties
		 	 */

			if ( ODBCINSTConstructProperties( driver_name, 
									&hFirstProperty ) != ODBCINST_SUCCESS )
			{
				char txt[ 256 ];

				sprintf( txt, _("Could not construct a property list for (%s)"), driver_name );
  				message_box( txt, GNOME_MESSAGE_BOX_ERROR );
			}
			else
			{
  				GtkWidget *dialog9;
  				GtkWidget *dialog_vbox9;
  				GtkWidget *hbox2;
  				GtkWidget *table8;
  				GtkWidget *dialog_action_area9;
  				GtkWidget *button85;
  				GtkWidget *button87;
				GtkWidget *label;
				GtkWidget *entry;
				int slot;
  
  				dialog9 = gnome_dialog_new (NULL, NULL);
  				gtk_window_set_modal (GTK_WINDOW (dialog9), TRUE);
  				gtk_object_set_data (GTK_OBJECT (dialog9), "dialog9", dialog9);
  				gtk_window_set_policy (GTK_WINDOW (dialog9), FALSE, FALSE, FALSE);

  				dialog_vbox9 = GNOME_DIALOG (dialog9)->vbox;
  				gtk_object_set_data (GTK_OBJECT (dialog9), "dialog_vbox9", dialog_vbox9);
  				gtk_widget_show (dialog_vbox9);

  				hbox2 = gtk_hbox_new (FALSE, 0);
  				gtk_widget_ref (hbox2);
  				gtk_object_set_data_full (GTK_OBJECT (dialog9), "hbox2", hbox2,
                            				(GtkDestroyNotify) gtk_widget_unref);
  				gtk_widget_show (hbox2);
  				gtk_box_pack_start (GTK_BOX (dialog_vbox9), hbox2, TRUE, TRUE, 0);
				
  				table8 = gtk_table_new (50, 2, FALSE);
  				gtk_widget_ref (table8);
  				gtk_object_set_data_full (GTK_OBJECT (dialog9), "table8", table8,
                            				(GtkDestroyNotify) gtk_widget_unref);
  				gtk_widget_show (table8);
  				gtk_box_pack_start (GTK_BOX (hbox2), table8, TRUE, TRUE, 0);

				label = gtk_label_new (_("DSN"));
				gtk_label_set_justify( GTK_LABEL( label ), GTK_JUSTIFY_LEFT );

				gtk_widget_ref (label);
				gtk_object_set_data_full (GTK_OBJECT (dialog9), "dsn_name", 
							label, (GtkDestroyNotify) gtk_widget_unref);
					
				gtk_widget_show( label );
				gtk_table_attach (GTK_TABLE (table8), label, 0, 1, 0, 1,
                    			(GtkAttachOptions) (0),
                   				(GtkAttachOptions) (0), 0, 0);

				entry= gtk_entry_new ();
				gtk_widget_ref (entry);

				gtk_entry_set_editable( GTK_ENTRY( entry ), FALSE );

				gtk_object_set_data_full (GTK_OBJECT (dialog9), 
								"dsn_value", entry,
								(GtkDestroyNotify) gtk_widget_unref);

				gtk_entry_set_text( GTK_ENTRY( entry ), dsn );

				gtk_widget_show (entry);
				gtk_table_attach (GTK_TABLE (table8), entry, 1, 2, 
								0, 1,
								(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
								(GtkAttachOptions) (0), 0, 0);

				entry= gtk_entry_new ();
				gtk_widget_ref (entry);
				gtk_entry_set_editable( GTK_ENTRY( entry ), FALSE );
				gtk_entry_set_text( GTK_ENTRY( entry ), driver_name );

				gtk_object_set_data_full (GTK_OBJECT (dialog9), 
								"driver_value", entry,
								(GtkDestroyNotify) gtk_widget_unref);

				for ( hCurProperty = hFirstProperty->pNext, slot = 1; 
								hCurProperty != NULL; 
								hCurProperty = hCurProperty->pNext, slot ++ )
				{
  					label = gtk_label_new (_(hCurProperty->szName));
					gtk_label_set_justify( GTK_LABEL( label ), GTK_JUSTIFY_LEFT );

  					gtk_widget_ref (label);
  					gtk_object_set_data_full (GTK_OBJECT (dialog9), hCurProperty->szName, 
							label, (GtkDestroyNotify) gtk_widget_unref);
					
  					gtk_widget_show( label );
  					gtk_table_attach (GTK_TABLE (table8), label, 0, 1, slot, slot + 1,
                    			(GtkAttachOptions) (0),
                   				(GtkAttachOptions) (0), 0, 0);

					SQLGetPrivateProfileString( dsn, hCurProperty->szName, "", 
									value, sizeof( value ), szINI );

					switch ( hCurProperty->nPromptType )
					{
					 	case ODBCINST_PROMPTTYPE_LABEL:
					  	{
  							GtkWidget *label;

  							label = gtk_label_new ( _( value ));
  							gtk_widget_ref (label);

  							gtk_object_set_data_full (GTK_OBJECT (dialog9), 
											hCurProperty->szName, label,
                            				(GtkDestroyNotify) gtk_widget_unref);

  							gtk_widget_show (label);
  							gtk_table_attach (GTK_TABLE (table8), label, 1, 2, 
											slot, slot + 1,
                    						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    						(GtkAttachOptions) (0), 0, 0);
						}
						break;

						case ODBCINST_PROMPTTYPE_LISTBOX:
						{
							GtkWidget *optionmenu;
							GtkWidget *optionmenu_menu;
							GtkWidget *menuitem;
							char *ptr;
							int i;

  							optionmenu = gtk_option_menu_new ();
  							gtk_widget_ref (optionmenu);

  							gtk_widget_show (optionmenu);

  							optionmenu_menu = gtk_menu_new();

  							gtk_object_set_data(GTK_OBJECT (dialog9), 
											hCurProperty->szName,
											optionmenu_menu );

							ptr = hCurProperty->aPromptData[ i = 0 ];

							while( ptr )
							{
								if ( strcmp( ptr, value ) == 0 )
								{
    								menuitem = gtk_menu_item_new_with_label (_(ptr));
									gtk_widget_set_name( menuitem, _(ptr));
	  								gtk_widget_show (menuitem);

  									gtk_menu_append (GTK_MENU (optionmenu_menu), menuitem );
								}

								ptr = hCurProperty->aPromptData[ ++ i ];
							}

							ptr = hCurProperty->aPromptData[ i = 0 ];

							while( ptr )
							{
								if ( strcmp( ptr, value ) != 0 )
								{
    								menuitem = gtk_menu_item_new_with_label (_(ptr));
									gtk_widget_set_name( menuitem, _(ptr));
	  								gtk_widget_show (menuitem);

  									gtk_menu_append (GTK_MENU (optionmenu_menu), menuitem );
								}

								ptr = hCurProperty->aPromptData[ ++ i ];
							}

  							gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu), optionmenu_menu );

  							gtk_table_attach (GTK_TABLE (table8), optionmenu, 1, 2, 
												slot, slot + 1,
                    							(GtkAttachOptions) (GTK_FILL),
                    							(GtkAttachOptions) (GTK_FILL), 0, 0);
            			}
						break;

						case ODBCINST_PROMPTTYPE_COMBOBOX:
						{
							GtkWidget *combo1;
							GList *combo1_items = NULL;
  							GtkWidget *combo_entry1;
							char *ptr;
							int i;

  							combo1 = gtk_combo_new ();
  							gtk_widget_ref (combo1);
  							gtk_widget_show (combo1);

							ptr = hCurProperty->aPromptData[ i = 0 ];

							while( ptr )
							{
  								combo1_items = g_list_append (combo1_items, _(ptr));
								ptr = hCurProperty->aPromptData[ ++ i ];
							}

  							gtk_combo_set_popdown_strings (GTK_COMBO (combo1), combo1_items);
  							g_list_free (combo1_items);

  							combo_entry1 = GTK_COMBO (combo1)->entry;
  							gtk_widget_ref (combo_entry1);
  							gtk_object_set_data_full (GTK_OBJECT (dialog9), 
														hCurProperty->szName, 
														combo_entry1,
                            							(GtkDestroyNotify) gtk_widget_unref);
  							gtk_widget_show (combo_entry1);

  							gtk_entry_set_text (GTK_ENTRY (combo_entry1), _(value));

  							gtk_table_attach (GTK_TABLE (table8), combo1, 1, 2, 
												slot, slot + 1,
                    							(GtkAttachOptions) (GTK_FILL),
                    							(GtkAttachOptions) (GTK_FILL), 0, 0);
						}
						break;

						case ODBCINST_PROMPTTYPE_FILENAME:
						{
							GtkWidget *hbox3;
							GtkWidget *entry;
							GtkWidget *button;

  							hbox3 = gtk_hbox_new (FALSE, 0);
  							gtk_widget_ref (hbox3);
  							gtk_object_set_data_full (GTK_OBJECT (dialog9), "hbox3", hbox3,
                            				(GtkDestroyNotify) gtk_widget_unref);
  							gtk_widget_show (hbox3);
  							gtk_table_attach (GTK_TABLE (table8), hbox3, 1, 2, 
												slot, slot + 1,
                    							(GtkAttachOptions) (GTK_FILL),
                    							(GtkAttachOptions) (GTK_FILL), 0, 0);

  							entry = gtk_entry_new ();
  							gtk_widget_ref (entry);
  							gtk_object_set_data_full (GTK_OBJECT (dialog9), hCurProperty->szName, 
														entry,
                            							(GtkDestroyNotify) gtk_widget_unref);

  							gtk_entry_set_text (GTK_ENTRY (entry), _(value));
  							gtk_widget_show (entry);
  							gtk_box_pack_start (GTK_BOX (hbox3), entry, TRUE, TRUE, 0);
							
  							button = gtk_button_new_with_label (_("Browse"));
  							gtk_widget_ref (button);
  							gtk_object_set_data_full (GTK_OBJECT (dialog9), "button", button,
                            							(GtkDestroyNotify) gtk_widget_unref);
  							gtk_widget_show (button);
  							gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);

  							gtk_signal_connect (GTK_OBJECT (button), "clicked",
                      				GTK_SIGNAL_FUNC (on_browse_clicked),
                      				entry );
						}
						break;

						default: /* PROMPTTYPE_TEXTEDIT and  PROMPTTYPE_TEXTEDIT_PASSWORD */
						{
  							GtkWidget *entry;

  							entry= gtk_entry_new ();
  							gtk_widget_ref (entry);

  							gtk_object_set_data_full (GTK_OBJECT (dialog9), 
											hCurProperty->szName, entry,
                            				(GtkDestroyNotify) gtk_widget_unref);

							gtk_entry_set_text( GTK_ENTRY( entry ), value );

						if (hCurProperty->nPromptType == ODBCINST_PROMPTTYPE_TEXTEDIT_PASSWORD) 
                                                        gtk_entry_set_visibility(GTK_ENTRY(entry),false);

  							gtk_widget_show (entry);
  							gtk_table_attach (GTK_TABLE (table8), entry, 1, 2, 
											slot, slot + 1,
                    						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    						(GtkAttachOptions) (0), 0, 0);
						}
						break;
					}
				}
				ODBCINSTDestructProperties( &hFirstProperty );
				
  				dialog_action_area9 = GNOME_DIALOG (dialog9)->action_area;
  				gtk_object_set_data (GTK_OBJECT (dialog9), "dialog_action_area9", dialog_action_area9);
  				gtk_widget_show (dialog_action_area9);
  				gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area9), GTK_BUTTONBOX_END);
  				gtk_button_box_set_spacing (GTK_BUTTON_BOX (dialog_action_area9), 8);
				
  				gnome_dialog_append_button (GNOME_DIALOG (dialog9), GNOME_STOCK_BUTTON_OK);
  				button85 = g_list_last (GNOME_DIALOG (dialog9)->buttons)->data;
  				gtk_widget_ref (button85);
  				gtk_object_set_data_full (GTK_OBJECT (dialog9), "button85", button85,
                            				(GtkDestroyNotify) gtk_widget_unref);
  				gtk_widget_show (button85);
  				GTK_WIDGET_SET_FLAGS (button85, GTK_CAN_DEFAULT);

  				gnome_dialog_append_button (GNOME_DIALOG (dialog9), GNOME_STOCK_BUTTON_CANCEL);
  				button87 = g_list_last (GNOME_DIALOG (dialog9)->buttons)->data;
  				gtk_widget_ref (button87);
  				gtk_object_set_data_full (GTK_OBJECT (dialog9), "button87", button87,
                            (GtkDestroyNotify) gtk_widget_unref);
  				gtk_widget_show (button87);
  				GTK_WIDGET_SET_FLAGS (button87, GTK_CAN_DEFAULT);

				gtk_signal_connect (GTK_OBJECT (button85), "clicked",
						GTK_SIGNAL_FUNC (on_add_dsn_ok_clicked),
						dialog9 );

				gtk_signal_connect (GTK_OBJECT (button87), "clicked",
						GTK_SIGNAL_FUNC (on_add_dsn_cancel_clicked),
						dialog9 );
				
  				gtk_widget_show (dialog9);
			}
		}

		if ( user_data )
			gtk_widget_destroy( GTK_WIDGET( user_data ));
	}
}

void
on_button82_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	if ( driver_mode == ODBC_USER_DSN )
		add_dsn( button, user_data, ODBC_ADD_DSN );
	else
		add_dsn( button, user_data, ODBC_ADD_SYS_DSN );
}


void
on_button83_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}


void
on_clist7_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
	clist6_row = row;
}

static void
on_browse_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkEntry *entry;

	entry = gtk_object_get_data( GTK_OBJECT (user_data), "entry" );

	gtk_entry_set_text( entry, gtk_file_selection_get_filename( GTK_FILE_SELECTION( user_data )));

	gtk_widget_destroy( GTK_WIDGET( user_data ));
}

static void
on_browse_cancel_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET( user_data ));
}

void
on_browse_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkEntry *entry = GTK_ENTRY( user_data );
  	GtkWidget *fileselection1;
  	GtkWidget *ok_button1;
  	GtkWidget *cancel_button1;
	gchar *txt;

	txt = gtk_entry_get_text( entry );

  	fileselection1 = gtk_file_selection_new (_("Select File"));
  	gtk_object_set_data (GTK_OBJECT (fileselection1), "fileselection1", fileselection1);
  	gtk_object_set_data (GTK_OBJECT (fileselection1), "entry", entry ); 
  	gtk_container_set_border_width (GTK_CONTAINER (fileselection1), 10);
  	gtk_window_set_modal (GTK_WINDOW (fileselection1), TRUE);

	gtk_file_selection_set_filename ( GTK_FILE_SELECTION (fileselection1), txt );

  	ok_button1 = GTK_FILE_SELECTION (fileselection1)->ok_button;
  	gtk_object_set_data (GTK_OBJECT (fileselection1), "ok_button1", ok_button1);
  	gtk_widget_show (ok_button1);
  	GTK_WIDGET_SET_FLAGS (ok_button1, GTK_CAN_DEFAULT);

  	cancel_button1 = GTK_FILE_SELECTION (fileselection1)->cancel_button;
  	gtk_object_set_data (GTK_OBJECT (fileselection1), "cancel_button1", cancel_button1);
  	gtk_widget_show (cancel_button1);
  	GTK_WIDGET_SET_FLAGS (cancel_button1, GTK_CAN_DEFAULT);

	gtk_signal_connect (GTK_OBJECT (ok_button1), "clicked",
			GTK_SIGNAL_FUNC (on_browse_ok_clicked),
			fileselection1 );

	gtk_signal_connect (GTK_OBJECT (cancel_button1), "clicked",
			GTK_SIGNAL_FUNC (on_browse_cancel_clicked),
			fileselection1 );

  	gtk_widget_show (fileselection1);
}


void
on_button16_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *win;

	add_driver_name[ 0 ] = '\0';
	add_driver_description[ 0 ] = '\0';
	strcpy( add_driver_driver, odbcinst_system_file_path() );
	strcat( add_driver_driver, "/lib" );
	strcpy( add_driver_setup, odbcinst_system_file_path() );
	strcat( add_driver_setup, "/lib" );
	strcpy( add_driver_fileusage, "1" );

	driver_mode = ODBC_SYSTEM_DSN;

	win = create_window5();
  	gtk_widget_show (win);
}


void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	edit_dsn( button, user_data, ODBC_CONFIG_DSN );
}


void
on_button18_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	edit_dsn( button, user_data, ODBC_CONFIG_SYS_DSN );
}


void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	if ( clist1_row < 0 )
	{
  		message_box( _("Select a DSN to Remove"), GNOME_MESSAGE_BOX_INFO );
	}
	else
	{
		char *dsn;
		char szINI[ FILENAME_MAX + 1 ];

		strcpy( szINI, "ODBC.INI" );

		gtk_clist_get_text( GTK_CLIST( clist1 ), clist1_row,
						0, &dsn );

		SQLSetConfigMode( ODBC_USER_DSN );
		if ( SQLWritePrivateProfileString( dsn, NULL, NULL, szINI ) == FALSE )
		{
			char txt[ 256 ];

			sprintf( txt, "Could not write property list for (%s)", dsn );
			message_box( txt, GNOME_MESSAGE_BOX_ERROR );
		}
		else
		{
			on_clist1_realize( clist1, NULL );
		}

		SQLSetConfigMode( ODBC_BOTH_DSN );
	}
}


void
on_button17_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	if ( clist5_row < 0 )
	{
  		message_box( _("Select a DSN to Remove"), GNOME_MESSAGE_BOX_INFO );
	}
	else
	{
		char *dsn;
		char szINI[ FILENAME_MAX + 1 ];

		strcpy( szINI, "ODBC.INI" );

		gtk_clist_get_text( GTK_CLIST( clist5 ), clist5_row,
						0, &dsn );

		SQLSetConfigMode( ODBC_SYSTEM_DSN );
		if ( SQLWritePrivateProfileString( dsn, NULL, NULL, szINI ) == FALSE )
		{
			char txt[ 256 ];

			sprintf( txt, _("Could not write property list for (%s)"), dsn );
			message_box( txt, GNOME_MESSAGE_BOX_ERROR );
		}
		else
		{
			on_clist5_realize( clist5, NULL );
		}

		SQLSetConfigMode( ODBC_BOTH_DSN );
	}
}

/*
 * get the current trace file
 */

void
on_entry1_realize                      (GtkWidget       *widget,
                                        gpointer         user_data)
{
	char            szINI[ FILENAME_MAX + 1 ];
	char            str[ FILENAME_MAX + 1 ];

	strcpy( szINI, "ODBCINST.INI" );

	if ( SQLGetPrivateProfileString( "ODBC", "TraceFile", "/tmp/sql.log", str, 
							sizeof( str ), szINI ) < 0 )
	{
		strcpy( str, "/tmp/sql.log" );
	}

	gtk_entry_set_text( GTK_ENTRY( widget ), str );
}


void
on_button51_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkEntry *entry = GTK_ENTRY( user_data );
	on_browse_clicked( button, entry );
}


void
on_Tracing_On_realize                  (GtkWidget       *widget,
                                        gpointer         user_data)
{
	char            szINI[ FILENAME_MAX + 1 ];
	char            str[ FILENAME_MAX + 1 ];
	int				state;

	strcpy( szINI, "ODBCINST.INI" );

	if ( SQLGetPrivateProfileString( "ODBC", "Trace", "No", str, 
							sizeof( str ), szINI ) < 0 )
	{
		strcpy( str, "No" );
	}

	state = TRUE;
	if ( toupper( str[ 0 ] ) == 'N' )
	{
		state = FALSE;
	}
	else if ( toupper( str[ 0 ] ) == 'O' && toupper( str[ 1 ] ) == 'F' )
	{
		state = FALSE;
	}

  	gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON( widget ), state );
}


void
on_button49_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  	GtkWidget *tracing_on;
  	GtkWidget *entry;
	int state;
	gchar *path;
	char szINI[ FILENAME_MAX + 1 ];

	tracing_on = gtk_object_get_data( GTK_OBJECT( user_data ), "tracing_on" );
	entry = gtk_object_get_data( GTK_OBJECT( user_data ), "entry1" );

	state = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( tracing_on ));
	path = gtk_entry_get_text( GTK_ENTRY( entry ));


	strcpy( szINI, "ODBCINST.INI" );

	if ( SQLWritePrivateProfileString( "ODBC", NULL, NULL, szINI ) == FALSE )
	{
		char txt[ 256 ];

		sprintf( txt, _("Could not write to %s"), szINI );
		message_box( txt, GNOME_MESSAGE_BOX_ERROR );
	}
	else
	{
		SQLWritePrivateProfileString( "ODBC", "Trace", 
						state ? "Yes" : "No", szINI );
		SQLWritePrivateProfileString( "ODBC", "TraceFile", 
						path, szINI );
	}
}

