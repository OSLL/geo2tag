#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gnome.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include <odbcconfig.h>
#include <interface.h>

static void
gtk_odbcconfig_class_init( GtkOdbcConfigClass *class )
{
	GtkObjectClass *object_class;

	object_class = (GtkObjectClass*) class;
}

GtkWidget *
gtk_odbcconfig_new()
{
	GtkOdbcConfig *odbc;

	odbc = gtk_type_new( GTK_TYPE_ODBCCONFIG );

	return GTK_WIDGET( odbc );
}

static void 
gtk_odbcconfig_init( GtkOdbcConfig *odbc )
{
  odbc->gODBCConfig = gtk_vbox_new( FALSE, 10 );
  gtk_widget_ref (odbc->gODBCConfig);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "gODBCConfig", 
				  			odbc->gODBCConfig,
                            (GtkDestroyNotify) gtk_widget_unref);

  gtk_container_add(GTK_CONTAINER(odbc),odbc->gODBCConfig);
  gtk_widget_show (odbc->gODBCConfig);

  odbc->notebook1 = gtk_notebook_new ();
  gtk_widget_ref (odbc->notebook1);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "notebook1", 
				  			odbc->notebook1,
                            (GtkDestroyNotify) gtk_widget_unref);

  gtk_widget_show (odbc->notebook1);
  gtk_container_add (GTK_CONTAINER (odbc->gODBCConfig), odbc->notebook1);

  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "notebook1", 
				  			odbc->notebook1,
                            (GtkDestroyNotify) gtk_widget_unref);

  odbc->fixed2 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed2);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed2", 
				  			odbc->fixed2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed2);
  gtk_container_add (GTK_CONTAINER (odbc->notebook1), odbc->fixed2);

  odbc->button2 = gtk_button_new_with_label (_("Add"));
  gtk_widget_ref (odbc->button2);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button2", odbc->button2,
                            (GtkDestroyNotify) gtk_widget_unref);

  gtk_widget_show (odbc->button2);
  gtk_fixed_put (GTK_FIXED (odbc->fixed2), odbc->button2, 312, 16);
  gtk_widget_set_uposition (odbc->button2, 312, 16);
  gtk_widget_set_usize (odbc->button2, 88, 32);

  odbc->button3 = gtk_button_new_with_label (_("Remove"));
  gtk_widget_ref (odbc->button3);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button3", odbc->button3,
                            (GtkDestroyNotify) gtk_widget_unref);

  gtk_widget_show (odbc->button3);
  gtk_fixed_put (GTK_FIXED (odbc->fixed2), odbc->button3, 312, 56);
  gtk_widget_set_uposition (odbc->button3, 312, 56);
  gtk_widget_set_usize (odbc->button3, 88, 32);

  odbc->button4 = gtk_button_new_with_label (_("Configure..."));
  gtk_widget_ref (odbc->button4);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button4", odbc->button4,
                            (GtkDestroyNotify) gtk_widget_unref);

  gtk_widget_show (odbc->button4);
  gtk_fixed_put (GTK_FIXED (odbc->fixed2), odbc->button4, 312, 96);
  gtk_widget_set_uposition (odbc->button4, 312, 96);
  gtk_widget_set_usize (odbc->button4, 88, 32);

  odbc->frame1 = gtk_frame_new (NULL);
  gtk_widget_ref (odbc->frame1);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "frame1", odbc->frame1,
                            (GtkDestroyNotify) gtk_widget_unref);

  gtk_widget_show (odbc->frame1);
  gtk_fixed_put (GTK_FIXED (odbc->fixed2), odbc->frame1, 0, 216);
  gtk_widget_set_uposition (odbc->frame1, 0, 216);
  gtk_widget_set_usize (odbc->frame1, 408, 72);

  odbc->fixed5 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed5);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed5", odbc->fixed5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed5);
  gtk_container_add (GTK_CONTAINER (odbc->frame1), odbc->fixed5);

  odbc->label24 = gtk_label_new (_("User data source configuration is stored in your home directory. This allows you configure data access without having to be system administrator"));
  gtk_widget_ref (odbc->label24);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label24", odbc->label24,
                            (GtkDestroyNotify) gtk_widget_unref);

  gtk_widget_show (odbc->label24);
  gtk_fixed_put (GTK_FIXED (odbc->fixed5), odbc->label24, 72, 8);
  gtk_widget_set_uposition (odbc->label24, 72, 8);
  gtk_widget_set_usize (odbc->label24, 312, 56);
  gtk_label_set_line_wrap (GTK_LABEL (odbc->label24), TRUE);

  odbc->pixmap3 = create_pixmap (odbc->gODBCConfig, "gODBCConfig/person.xpm", FALSE);
  gtk_widget_ref (odbc->pixmap3);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "pixmap3", odbc->pixmap3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->pixmap3);
  gtk_fixed_put (GTK_FIXED (odbc->fixed5), odbc->pixmap3, 8, 8);
  gtk_widget_set_uposition (odbc->pixmap3, 8, 8);
  gtk_widget_set_usize (odbc->pixmap3, 48, 48);

  odbc->scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (odbc->scrolledwindow2);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "scrolledwindow2", odbc->scrolledwindow2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->scrolledwindow2);
  gtk_fixed_put (GTK_FIXED (odbc->fixed2), odbc->scrolledwindow2, -1, -1);
  gtk_widget_set_uposition (odbc->scrolledwindow2, 0, 0);
  gtk_widget_set_usize (odbc->scrolledwindow2, 304, 208);

  odbc->clist1 = gtk_clist_new (3);
  gtk_widget_ref (odbc->clist1);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "clist1", odbc->clist1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->clist1);
  gtk_container_add (GTK_CONTAINER (odbc->scrolledwindow2), odbc->clist1);
  gtk_widget_set_usize (odbc->clist1, 304, 208);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist1), 0, 81);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist1), 1, 77);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist1), 2, 80);
  gtk_clist_column_titles_show (GTK_CLIST (odbc->clist1));
  gtk_clist_set_shadow_type (GTK_CLIST (odbc->clist1), GTK_SHADOW_NONE);

  odbc->Name = gtk_label_new (_("Name"));
  gtk_widget_ref (odbc->Name);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "Name", odbc->Name,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->Name);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist1), 0, odbc->Name);

  odbc->Desc = gtk_label_new (_("Description"));
  gtk_widget_ref (odbc->Desc);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "Desc", odbc->Desc,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->Desc);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist1), 1, odbc->Desc);

  odbc->Driver = gtk_label_new (_("Driver"));
  gtk_widget_ref (odbc->Driver);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "Driver", odbc->Driver,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->Driver);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist1), 2, odbc->Driver);

  odbc->User = gtk_label_new (_("User DSN"));
  gtk_widget_ref (odbc->User);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "User", odbc->User,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->User);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (odbc->notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (odbc->notebook1), 0), odbc->User);

  odbc->fixed13 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed13);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed13", odbc->fixed13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed13);
  gtk_container_add (GTK_CONTAINER (odbc->notebook1), odbc->fixed13);

  odbc->button16 = gtk_button_new_with_label (_("Add"));
  gtk_widget_ref (odbc->button16);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button16", odbc->button16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button16);
  gtk_fixed_put (GTK_FIXED (odbc->fixed13), odbc->button16, 312, 16);
  gtk_widget_set_uposition (odbc->button16, 312, 16);
  gtk_widget_set_usize (odbc->button16, 88, 32);

  odbc->button17 = gtk_button_new_with_label (_("Remove"));
  gtk_widget_ref (odbc->button17);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button17", odbc->button17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button17);
  gtk_fixed_put (GTK_FIXED (odbc->fixed13), odbc->button17, 312, 56);
  gtk_widget_set_uposition (odbc->button17, 312, 56);
  gtk_widget_set_usize (odbc->button17, 88, 32);

  odbc->button18 = gtk_button_new_with_label (_("Configure..."));
  gtk_widget_ref (odbc->button18);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button18", odbc->button18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button18);
  gtk_fixed_put (GTK_FIXED (odbc->fixed13), odbc->button18, 312, 96);
  gtk_widget_set_uposition (odbc->button18, 312, 96);
  gtk_widget_set_usize (odbc->button18, 88, 32);

  odbc->frame4 = gtk_frame_new (NULL);
  gtk_widget_ref (odbc->frame4);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "frame4", odbc->frame4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->frame4);
  gtk_fixed_put (GTK_FIXED (odbc->fixed13), odbc->frame4, 0, 216);
  gtk_widget_set_uposition (odbc->frame4, 0, 216);
  gtk_widget_set_usize (odbc->frame4, 408, 72);

  odbc->fixed14 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed14);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed14", odbc->fixed14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed14);
  gtk_container_add (GTK_CONTAINER (odbc->frame4), odbc->fixed14);

  odbc->label37 = gtk_label_new (_("System data sources are shared among all users of this machine.These data sources may also be used by system services. Only the administrator can configure system data sources."));
  gtk_widget_ref (odbc->label37);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label37", odbc->label37,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label37);
  gtk_fixed_put (GTK_FIXED (odbc->fixed14), odbc->label37, 72, 8);
  gtk_widget_set_uposition (odbc->label37, 72, 8);
  gtk_widget_set_usize (odbc->label37, 312, 56);
  gtk_label_set_line_wrap (GTK_LABEL (odbc->label37), TRUE);

  odbc->pixmap7 = create_pixmap (odbc->gODBCConfig, "gODBCConfig/server.xpm", FALSE);
  gtk_widget_ref (odbc->pixmap7);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "pixmap7", odbc->pixmap7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->pixmap7);
  gtk_fixed_put (GTK_FIXED (odbc->fixed14), odbc->pixmap7, 8, 8);
  gtk_widget_set_uposition (odbc->pixmap7, 8, 8);
  gtk_widget_set_usize (odbc->pixmap7, 48, 48);

  odbc->scrolledwindow3 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (odbc->scrolledwindow3);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "scrolledwindow3", odbc->scrolledwindow3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->scrolledwindow3);
  gtk_fixed_put (GTK_FIXED (odbc->fixed13), odbc->scrolledwindow3, -1, -1);
  gtk_widget_set_uposition (odbc->scrolledwindow3, 0, 0);
  gtk_widget_set_usize (odbc->scrolledwindow3, 304, 208);

  odbc->clist5 = gtk_clist_new (3);
  gtk_widget_ref (odbc->clist5);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "clist5", odbc->clist5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->clist5);
  gtk_container_add (GTK_CONTAINER (odbc->scrolledwindow3), odbc->clist5);
  gtk_widget_set_usize (odbc->clist5, 304, 208);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist5), 0, 80);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist5), 1, 80);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist5), 2, 80);
  gtk_clist_column_titles_show (GTK_CLIST (odbc->clist5));

  odbc->label34 = gtk_label_new (_("Name"));
  gtk_widget_ref (odbc->label34);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label34", odbc->label34,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label34);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist5), 0, odbc->label34);

  odbc->label35 = gtk_label_new (_("Description"));
  gtk_widget_ref (odbc->label35);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label35", odbc->label35,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label35);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist5), 1, odbc->label35);

  odbc->label36 = gtk_label_new (_("Driver"));
  gtk_widget_ref (odbc->label36);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label36", odbc->label36,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label36);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist5), 2, odbc->label36);

  odbc->System = gtk_label_new (_("System DSN"));
  gtk_widget_ref (odbc->System);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "System", odbc->System,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->System);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (odbc->notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (odbc->notebook1), 1), odbc->System);

  odbc->fixed4 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed4);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed4", odbc->fixed4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed4);
  gtk_container_add (GTK_CONTAINER (odbc->notebook1), odbc->fixed4);

  odbc->fixed8 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed8);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed8", odbc->fixed8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed8);
  gtk_fixed_put (GTK_FIXED (odbc->fixed4), odbc->fixed8, -1, -1);
  gtk_widget_set_uposition (odbc->fixed8, 0, 0);
  gtk_widget_set_usize (odbc->fixed8, 0, 0);

  odbc->fixed15 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed15);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed15", odbc->fixed15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed15);
  gtk_fixed_put (GTK_FIXED (odbc->fixed4), odbc->fixed15, -1, -1);
  gtk_widget_set_uposition (odbc->fixed15, 0, 0);
  gtk_widget_set_usize (odbc->fixed15, 0, 0);

  odbc->fixed20 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed20);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed20", odbc->fixed20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed20);
  gtk_fixed_put (GTK_FIXED (odbc->fixed4), odbc->fixed20, -1, -1);
  gtk_widget_set_uposition (odbc->fixed20, 0, 0);
  gtk_widget_set_usize (odbc->fixed20, 0, 0);

  odbc->button25 = gtk_button_new_with_label (_("Add"));
  gtk_widget_ref (odbc->button25);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button25", odbc->button25,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button25);
  gtk_fixed_put (GTK_FIXED (odbc->fixed20), odbc->button25, 312, 16);
  gtk_widget_set_uposition (odbc->button25, 312, 16);
  gtk_widget_set_usize (odbc->button25, 88, 32);

  odbc->button26 = gtk_button_new_with_label (_("Remove"));
  gtk_widget_ref (odbc->button26);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button26", odbc->button26,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button26);
  gtk_fixed_put (GTK_FIXED (odbc->fixed20), odbc->button26, 312, 56);
  gtk_widget_set_uposition (odbc->button26, 312, 56);
  gtk_widget_set_usize (odbc->button26, 88, 32);

  odbc->button27 = gtk_button_new_with_label (_("Configure..."));
  gtk_widget_ref (odbc->button27);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button27",odbc-> button27,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button27);
  gtk_fixed_put (GTK_FIXED (odbc->fixed20), odbc->button27, 312, 96);
  gtk_widget_set_uposition (odbc->button27, 312, 96);
  gtk_widget_set_usize (odbc->button27, 88, 32);

  odbc->frame7 = gtk_frame_new (NULL);
  gtk_widget_ref (odbc->frame7);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "frame7", odbc->frame7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->frame7);
  gtk_fixed_put (GTK_FIXED (odbc->fixed20), odbc->frame7, 0, 216);
  gtk_widget_set_uposition (odbc->frame7, 0, 216);
  gtk_widget_set_usize (odbc->frame7, 408, 72);

  odbc->fixed21 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed21);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed21", odbc->fixed21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed21);
  gtk_container_add (GTK_CONTAINER (odbc->frame7), odbc->fixed21);

  odbc->label52 = gtk_label_new (_("These drivers facilitate communication between the Driver Manager and the data server. Many ODBC drivers  for Linux can be downloaded from the Internet while others are obtained from your database vendor."));
  gtk_widget_ref (odbc->label52);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label52", odbc->label52,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label52);
  gtk_fixed_put (GTK_FIXED (odbc->fixed21), odbc->label52, 72, 8);
  gtk_widget_set_uposition (odbc->label52, 72, 8);
  gtk_widget_set_usize (odbc->label52, 312, 56);
  gtk_label_set_line_wrap (GTK_LABEL (odbc->label52), TRUE);

  odbc->pixmap10 = create_pixmap (odbc->gODBCConfig, "gODBCConfig/driver.xpm", FALSE);
  gtk_widget_ref (odbc->pixmap10);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "pixmap10", odbc->pixmap10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->pixmap10);
  gtk_fixed_put (GTK_FIXED (odbc->fixed21), odbc->pixmap10, 8, 8);
  gtk_widget_set_uposition (odbc->pixmap10, 8, 8);
  gtk_widget_set_usize (odbc->pixmap10, 48, 48);

  odbc->scrolledwindow4 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (odbc->scrolledwindow4);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "scrolledwindow4", odbc->scrolledwindow4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->scrolledwindow4);
  gtk_fixed_put (GTK_FIXED (odbc->fixed20), odbc->scrolledwindow4, -1, -1);
  gtk_widget_set_uposition (odbc->scrolledwindow4, 0, 0);
  gtk_widget_set_usize (odbc->scrolledwindow4, 304, 208);

  odbc->clist6 = gtk_clist_new (4);
  gtk_widget_ref (odbc->clist6);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "clist6", odbc->clist6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->clist6);
  gtk_container_add (GTK_CONTAINER (odbc->scrolledwindow4), odbc->clist6);
  gtk_widget_set_usize (odbc->clist6, 304, 208);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist6), 0, 80);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist6), 1, 80);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist6), 2, 80);
  gtk_clist_set_column_width (GTK_CLIST (odbc->clist6), 3, 80);
  gtk_clist_column_titles_show (GTK_CLIST (odbc->clist6));

  odbc->label117 = gtk_label_new (_("Name"));
  gtk_widget_ref (odbc->label117);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label117", odbc->label117,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label117);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist6), 0, odbc->label117);

  odbc->label118 = gtk_label_new (_("Description"));
  gtk_widget_ref (odbc->label118);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label118", odbc->label118,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label118);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist6), 1, odbc->label118);

  odbc->label119 = gtk_label_new (_("Driver Lib"));
  gtk_widget_ref (odbc->label119);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label119", odbc->label119,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label119);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist6), 2, odbc->label119);

  odbc->label120 = gtk_label_new (_("Setup Lib"));
  gtk_widget_ref (odbc->label120);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label120", odbc->label120,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label120);
  gtk_clist_set_column_widget (GTK_CLIST (odbc->clist6), 3, odbc->label120);

  odbc->Drivers = gtk_label_new (_("Drivers"));
  gtk_widget_ref (odbc->Drivers);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "Drivers", odbc->Drivers,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->Drivers);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (odbc->notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (odbc->notebook1), 2), odbc->Drivers);

  odbc->fixed29 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed29);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed29", odbc->fixed29,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed29);
  gtk_container_add (GTK_CONTAINER (odbc->notebook1), odbc->fixed29);

  odbc->button49 = gnome_stock_button (GNOME_STOCK_BUTTON_APPLY);
  gtk_widget_ref (odbc->button49);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button49", odbc->button49,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button49);
  gtk_fixed_put (GTK_FIXED (odbc->fixed29), odbc->button49, 304, 160);
  gtk_widget_set_uposition (odbc->button49, 304, 160);
  gtk_widget_set_usize (odbc->button49, 88, 32);

  odbc->frame9 = gtk_frame_new (NULL);
  gtk_widget_ref (odbc->frame9);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "frame9", odbc->frame9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->frame9);
  gtk_fixed_put (GTK_FIXED (odbc->fixed29), odbc->frame9, 0, 216);
  gtk_widget_set_uposition (odbc->frame9, 0, 216);
  gtk_widget_set_usize (odbc->frame9, 408, 72);

  odbc->fixed30 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed30);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed30", odbc->fixed30,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed30);
  gtk_container_add (GTK_CONTAINER (odbc->frame9), odbc->fixed30);

  odbc->label73 = gtk_label_new (_("Tracing allows you to create logs of the calls to ODBC drivers. Great for support people, or to aid you in debugging applications.\nYou must be 'root' to set"));
  gtk_widget_ref (odbc->label73);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label73", odbc->label73,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label73);
  gtk_fixed_put (GTK_FIXED (odbc->fixed30), odbc->label73, 72, 0);
  gtk_widget_set_uposition (odbc->label73, 72, 0);
  gtk_widget_set_usize (odbc->label73, 312, 64);
  gtk_label_set_line_wrap (GTK_LABEL (odbc->label73), TRUE);

  odbc->pixmap12 = create_pixmap (odbc->gODBCConfig, "gODBCConfig/trace.xpm", FALSE);
  gtk_widget_ref (odbc->pixmap12);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "pixmap12", odbc->pixmap12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->pixmap12);
  gtk_fixed_put (GTK_FIXED (odbc->fixed30), odbc->pixmap12, 8, 8);
  gtk_widget_set_uposition (odbc->pixmap12, 8, 8);
  gtk_widget_set_usize (odbc->pixmap12, 48, 48);

  odbc->entry1 = gtk_entry_new ();
  gtk_widget_ref (odbc->entry1);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "entry1", odbc->entry1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->entry1);
  gtk_fixed_put (GTK_FIXED (odbc->fixed29), odbc->entry1, 96, 80);
  gtk_widget_set_uposition (odbc->entry1, 96, 80);
  gtk_widget_set_usize (odbc->entry1, 208, 24);

  odbc->button51 = gtk_button_new_with_label (_("Browse"));
  gtk_widget_ref (odbc->button51);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button51", odbc->button51,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button51);
  gtk_fixed_put (GTK_FIXED (odbc->fixed29), odbc->button51, 312, 80);
  gtk_widget_set_uposition (odbc->button51, 312, 80);
  gtk_widget_set_usize (odbc->button51, 72, 24);

  odbc->label74 = gtk_label_new (_("Trace File"));
  gtk_widget_ref (odbc->label74);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label74", odbc->label74,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label74);
  gtk_fixed_put (GTK_FIXED (odbc->fixed29), odbc->label74, 16, 80);
  gtk_widget_set_uposition (odbc->label74, 16, 80);
  gtk_widget_set_usize (odbc->label74, 72, 24);

  odbc->tracing_on = gtk_check_button_new_with_label (_("Tracing On"));
  gtk_widget_ref (odbc->tracing_on);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "tracing_on", odbc->tracing_on,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->tracing_on);
  gtk_fixed_put (GTK_FIXED (odbc->fixed29), odbc->tracing_on, 24, 32);
  gtk_widget_set_uposition (odbc->tracing_on, 24, 32);
  gtk_widget_set_usize (odbc->tracing_on, 99, 24);

  odbc->Tracing = gtk_label_new (_("Tracing"));
  gtk_widget_ref (odbc->Tracing);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "Tracing", odbc->Tracing,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->Tracing);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (odbc->notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (odbc->notebook1), 3), odbc->Tracing);

  odbc->fixed24 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed24);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed24", odbc->fixed24,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed24);
  gtk_container_add (GTK_CONTAINER (odbc->notebook1), odbc->fixed24);

  odbc->frame8 = gtk_frame_new (NULL);
  gtk_widget_ref (odbc->frame8);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "frame8", odbc->frame8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->frame8);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->frame8, 0, 200);
  gtk_widget_set_uposition (odbc->frame8, 0, 200);
  gtk_widget_set_usize (odbc->frame8, 408, 88);

  odbc->fixed25 = gtk_fixed_new ();
  gtk_widget_ref (odbc->fixed25);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "fixed25", odbc->fixed25,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->fixed25);
  gtk_container_add (GTK_CONTAINER (odbc->frame8), odbc->fixed25);

  odbc->label61 = gtk_label_new (_("Open DataBase Connectivity (ODBC) was developed to be an Open and portable standard for accessing data. unixODBC implements this standard for Linux/UNIX."));
  gtk_widget_ref (odbc->label61);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label61", odbc->label61,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label61);
  gtk_fixed_put (GTK_FIXED (odbc->fixed25), odbc->label61, 72, 0);
  gtk_widget_set_uposition (odbc->label61, 72, 0);
  gtk_widget_set_usize (odbc->label61, 328, 48);
  gtk_label_set_line_wrap (GTK_LABEL (odbc->label61), TRUE);

  odbc->label62 = gtk_label_new (_("http://www.unixodbc.org"));
  gtk_widget_ref (odbc->label62);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "label62", odbc->label62,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->label62);
  gtk_fixed_put (GTK_FIXED (odbc->fixed25), odbc->label62, 104, 48);
  gtk_widget_set_uposition (odbc->label62, 104, 48);
  gtk_widget_set_usize (odbc->label62, 144, 32);
  gtk_label_set_justify (GTK_LABEL (odbc->label62), GTK_JUSTIFY_LEFT);

  odbc->button52 = gtk_button_new_with_label (_("Database System"));
  gtk_widget_ref (odbc->button52);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button52", odbc->button52,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button52);
  gtk_fixed_put (GTK_FIXED (odbc->fixed25), odbc->button52, 24, 152);
  gtk_widget_set_uposition (odbc->button52, 24, 152);
  gtk_widget_set_usize (odbc->button52, 112, 32);

  odbc->button53 = gtk_button_new_with_label (_("Credits"));
  gtk_widget_ref (odbc->button53);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button53", odbc->button53,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button53);
  gtk_fixed_put (GTK_FIXED (odbc->fixed25), odbc->button53, 304, 56);
  gtk_widget_set_uposition (odbc->button53, 304, 56);
  gtk_widget_set_usize (odbc->button53, 88, 22);

  odbc->pixmap11 = create_pixmap (odbc->gODBCConfig, "gODBCConfig/info.xpm", FALSE);
  gtk_widget_ref (odbc->pixmap11);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "pixmap11", odbc->pixmap11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->pixmap11);
  gtk_fixed_put (GTK_FIXED (odbc->fixed25), odbc->pixmap11, 16, 16);
  gtk_widget_set_uposition (odbc->pixmap11, 16, 16);
  gtk_widget_set_usize (odbc->pixmap11, 48, 48);

  odbc->button38 = gtk_button_new_with_label (_("Application"));
  gtk_widget_ref (odbc->button38);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button38", odbc->button38,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button38);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->button38, 24, 8);
  gtk_widget_set_uposition (odbc->button38, 24, 8);
  gtk_widget_set_usize (odbc->button38, 112, 32);

  odbc->button39 = gtk_button_new_with_label (_("Driver Manager"));
  gtk_widget_ref (odbc->button39);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button39", odbc->button39,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button39);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->button39, 24, 56);
  gtk_widget_set_uposition (odbc->button39, 24, 56);
  gtk_widget_set_usize (odbc->button39, 112, 32);

  odbc->button40 = gtk_button_new_with_label (_("Driver"));
  gtk_widget_ref (odbc->button40);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button40", odbc->button40,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button40);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->button40, 24, 104);
  gtk_widget_set_uposition (odbc->button40, 24, 104);
  gtk_widget_set_usize (odbc->button40, 112, 32);

  odbc->button41 = gtk_button_new_with_label (_("Database System"));
  gtk_widget_ref (odbc->button41);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button41", odbc->button41,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button41);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->button41, 24, 152);
  gtk_widget_set_uposition (odbc->button41, 24, 152);
  gtk_widget_set_usize (odbc->button41, 112, 32);

  odbc->button42 = gtk_button_new_with_label (_("odbcinst.ini"));
  gtk_widget_ref (odbc->button42);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button42", odbc->button42,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button42);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->button42, 280, 104);
  gtk_widget_set_uposition (odbc->button42, 280, 104);
  gtk_widget_set_usize (odbc->button42, 112, 32);

  odbc->button43 = gtk_button_new_with_label (_("odbc.ini"));
  gtk_widget_ref (odbc->button43);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button43", odbc->button43,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button43);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->button43, 152, 56);
  gtk_widget_set_uposition (odbc->button43, 152, 56);
  gtk_widget_set_usize (odbc->button43, 112, 32);

  odbc->button44 = gtk_button_new_with_label (_("Config"));
  gtk_widget_ref (odbc->button44);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "button44", odbc->button44,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->button44);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->button44, 280, 56);
  gtk_widget_set_uposition (odbc->button44, 280, 56);
  gtk_widget_set_usize (odbc->button44, 112, 32);

  odbc->vseparator4 = gtk_vseparator_new ();
  gtk_widget_ref (odbc->vseparator4);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "vseparator4", odbc->vseparator4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->vseparator4);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->vseparator4, 72, 40);
  gtk_widget_set_uposition (odbc->vseparator4, 72, 40);
  gtk_widget_set_usize (odbc->vseparator4, 16, 16);

  odbc->vseparator5 = gtk_vseparator_new ();
  gtk_widget_ref (odbc->vseparator5);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "vseparator5", odbc->vseparator5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->vseparator5);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->vseparator5, 72, 88);
  gtk_widget_set_uposition (odbc->vseparator5, 72, 88);
  gtk_widget_set_usize (odbc->vseparator5, 16, 16);

  odbc->vseparator6 = gtk_vseparator_new ();
  gtk_widget_ref (odbc->vseparator6);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "vseparator6", odbc->vseparator6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->vseparator6);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->vseparator6, 72, 136);
  gtk_widget_set_uposition (odbc->vseparator6, 72, 136);
  gtk_widget_set_usize (odbc->vseparator6, 16, 16);

  odbc->vseparator7 = gtk_vseparator_new ();
  gtk_widget_ref (odbc->vseparator7);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "vseparator7", odbc->vseparator7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->vseparator7);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->vseparator7, 328, 88);
  gtk_widget_set_uposition (odbc->vseparator7, 328, 88);
  gtk_widget_set_usize (odbc->vseparator7, 16, 16);

  odbc->hseparator2 = gtk_hseparator_new ();
  gtk_widget_ref (odbc->hseparator2);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "hseparator2", odbc->hseparator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->hseparator2);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->hseparator2, 136, 64);
  gtk_widget_set_uposition (odbc->hseparator2, 136, 64);
  gtk_widget_set_usize (odbc->hseparator2, 16, 16);

  odbc->hseparator3 = gtk_hseparator_new ();
  gtk_widget_ref (odbc->hseparator3);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "hseparator3", odbc->hseparator3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->hseparator3);
  gtk_fixed_put (GTK_FIXED (odbc->fixed24), odbc->hseparator3, 264, 64);
  gtk_widget_set_uposition (odbc->hseparator3, 264, 64);
  gtk_widget_set_usize (odbc->hseparator3, 16, 16);

  odbc->About = gtk_label_new (_("About"));
  gtk_widget_ref (odbc->About);
  gtk_object_set_data_full (GTK_OBJECT (odbc->gODBCConfig), "About", odbc->About,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (odbc->About);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (odbc->notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (odbc->notebook1), 4), odbc->About);

  gtk_signal_connect (GTK_OBJECT (odbc->button2), "clicked",
                      GTK_SIGNAL_FUNC (on_button2_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button3), "clicked",
                      GTK_SIGNAL_FUNC (on_button3_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button4), "clicked",
                      GTK_SIGNAL_FUNC (on_button4_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->clist1), "realize",
                      GTK_SIGNAL_FUNC (on_clist1_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->clist1), "select_row",
                      GTK_SIGNAL_FUNC (on_clist1_select_row),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button16), "clicked",
                      GTK_SIGNAL_FUNC (on_button16_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button17), "clicked",
                      GTK_SIGNAL_FUNC (on_button17_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button18), "clicked",
                      GTK_SIGNAL_FUNC (on_button18_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->clist5), "realize",
                      GTK_SIGNAL_FUNC (on_clist5_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->clist5), "select_row",
                      GTK_SIGNAL_FUNC (on_clist5_select_row),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button25), "clicked",
                      GTK_SIGNAL_FUNC (on_button25_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button26), "clicked",
                      GTK_SIGNAL_FUNC (on_button26_clicked),
                      odbc->clist6);
  gtk_signal_connect (GTK_OBJECT (odbc->button27), "clicked",
                      GTK_SIGNAL_FUNC (on_button27_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->clist6), "realize",
                      GTK_SIGNAL_FUNC (on_clist6_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->clist6), "select_row",
                      GTK_SIGNAL_FUNC (on_clist6_select_row),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button49), "clicked",
                      GTK_SIGNAL_FUNC (on_button49_clicked),
                      odbc->gODBCConfig);
  gtk_signal_connect (GTK_OBJECT (odbc->entry1), "realize",
                      GTK_SIGNAL_FUNC (on_entry1_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button51), "clicked",
                      GTK_SIGNAL_FUNC (on_button51_clicked),
                      odbc->entry1);
  gtk_signal_connect (GTK_OBJECT (odbc->tracing_on), "realize",
                      GTK_SIGNAL_FUNC (on_Tracing_On_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button53), "clicked",
                      GTK_SIGNAL_FUNC (on_button53_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button38), "clicked",
                      GTK_SIGNAL_FUNC (on_button38_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button39), "clicked",
                      GTK_SIGNAL_FUNC (on_button39_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button40), "clicked",
                      GTK_SIGNAL_FUNC (on_button40_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button41), "clicked",
                      GTK_SIGNAL_FUNC (on_button41_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button42), "clicked",
                      GTK_SIGNAL_FUNC (on_button42_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button43), "clicked",
                      GTK_SIGNAL_FUNC (on_button43_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc->button44), "clicked",
                      GTK_SIGNAL_FUNC (on_button44_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (odbc), "destroy",
                      GTK_SIGNAL_FUNC (on_dialog1_destroy),
                      NULL);
}

GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *label79;
  GtkWidget *hbox1;
  GtkWidget *vbox7;
  GtkWidget *pixmap14;
  GtkWidget *label75;
  GtkWidget *label78;
  GtkWidget *label77;
  GtkWidget *dialog_action_area1;
  GtkWidget *button54;

  dialog1 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog1", dialog1);
  gtk_window_set_title (GTK_WINDOW (dialog1), _("gODBCConfig - Appication"));
  GTK_WINDOW (dialog1)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_modal (GTK_WINDOW (dialog1), TRUE);
  gtk_window_set_policy (GTK_WINDOW (dialog1), TRUE, TRUE, FALSE);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog_vbox1", dialog_vbox1);
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  label79 = gtk_label_new ("");
  gtk_widget_ref (label79);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label79", label79,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label79);
  gtk_box_pack_start (GTK_BOX (vbox1), label79, FALSE, FALSE, 0);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  vbox7 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox7);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "vbox7", vbox7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox7);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox7, FALSE, FALSE, 0);

  pixmap14 = create_pixmap (dialog1, "gODBCConfig/info.xpm", FALSE);
  gtk_widget_ref (pixmap14);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "pixmap14", pixmap14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap14);
  gtk_box_pack_start (GTK_BOX (vbox7), pixmap14, TRUE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixmap14), 16, 0);

  label75 = gtk_label_new (_("The Application communicates with the Driver Manager using the standard ODBC calls.\n\nThe application does not care; where the data is stored, how it is stored, or even how the system is configured to access the data.\n\nThe Application only needs to know the data source name (DSN)\n\nThe Application is not hard wired to a particular database system. This allows the user to select a different database system using the ODBCConfig Tool."));
  gtk_widget_ref (label75);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label75", label75,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label75);
  gtk_box_pack_start (GTK_BOX (hbox1), label75, FALSE, FALSE, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label75), TRUE);

  label78 = gtk_label_new ("");
  gtk_widget_ref (label78);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label78", label78,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label78);
  gtk_box_pack_start (GTK_BOX (hbox1), label78, FALSE, FALSE, 0);
  gtk_widget_set_usize (label78, 16, -2);

  label77 = gtk_label_new ("");
  gtk_widget_ref (label77);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label77", label77,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label77);
  gtk_box_pack_start (GTK_BOX (vbox1), label77, FALSE, FALSE, 0);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog_action_area1", dialog_action_area1);
  gtk_widget_show (dialog_action_area1);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);

  button54 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button54);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "button54", button54,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button54);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), button54, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button54), "clicked",
                      GTK_SIGNAL_FUNC (on_button54_clicked),
                      dialog1);

  return dialog1;
}

GtkWidget*
create_dialog2 (void)
{
  GtkWidget *dialog2;
  GtkWidget *dialog_vbox2;
  GtkWidget *table1;
  GtkWidget *label80;
  GtkWidget *label81;
  GtkWidget *label83;
  GtkWidget *label84;
  GtkWidget *vbox8;
  GtkWidget *pixmap15;
  GtkWidget *dialog_action_area2;
  GtkWidget *button55;

  dialog2 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog2), "dialog2", dialog2);
  gtk_window_set_title (GTK_WINDOW (dialog2), _("gODBCConfig - Driver Manager"));
  GTK_WINDOW (dialog2)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_modal (GTK_WINDOW (dialog2), TRUE);
  gtk_window_set_policy (GTK_WINDOW (dialog2), TRUE, TRUE, FALSE);

  dialog_vbox2 = GTK_DIALOG (dialog2)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog2), "dialog_vbox2", dialog_vbox2);
  gtk_widget_show (dialog_vbox2);

  table1 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table1);
  gtk_object_set_data_full (GTK_OBJECT (dialog2), "table1", table1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox2), table1, TRUE, TRUE, 0);

  label80 = gtk_label_new (_("The Driver Manager carries out a number of functions, such as:\n1. Resolve data source names via odbcinst lib)\n2. Loads any required drivers\n3. Calls the drivers exposed functions to communicate with the database. Some functionality, such as listing all Data Source, is only present in the Driver Manager or via odbcinst lib)."));
  gtk_widget_ref (label80);
  gtk_object_set_data_full (GTK_OBJECT (dialog2), "label80", label80,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label80);
  gtk_table_attach (GTK_TABLE (table1), label80, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label80), TRUE);

  label81 = gtk_label_new (_(" "));
  gtk_widget_ref (label81);
  gtk_object_set_data_full (GTK_OBJECT (dialog2), "label81", label81,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label81);
  gtk_table_attach (GTK_TABLE (table1), label81, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  label83 = gtk_label_new ("");
  gtk_widget_ref (label83);
  gtk_object_set_data_full (GTK_OBJECT (dialog2), "label83", label83,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label83);
  gtk_table_attach (GTK_TABLE (table1), label83, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label83, 16, -2);

  label84 = gtk_label_new ("");
  gtk_widget_ref (label84);
  gtk_object_set_data_full (GTK_OBJECT (dialog2), "label84", label84,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label84);
  gtk_table_attach (GTK_TABLE (table1), label84, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  vbox8 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox8);
  gtk_object_set_data_full (GTK_OBJECT (dialog2), "vbox8", vbox8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox8);
  gtk_table_attach (GTK_TABLE (table1), vbox8, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  pixmap15 = create_pixmap (dialog2, "gODBCConfig/info.xpm", FALSE);
  gtk_widget_ref (pixmap15);
  gtk_object_set_data_full (GTK_OBJECT (dialog2), "pixmap15", pixmap15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap15);
  gtk_box_pack_start (GTK_BOX (vbox8), pixmap15, TRUE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixmap15), 16, 0);

  dialog_action_area2 = GTK_DIALOG (dialog2)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog2), "dialog_action_area2", dialog_action_area2);
  gtk_widget_show (dialog_action_area2);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area2), 10);

  button55 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button55);
  gtk_object_set_data_full (GTK_OBJECT (dialog2), "button55", button55,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button55);
  gtk_box_pack_start (GTK_BOX (dialog_action_area2), button55, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button55), "clicked",
                      GTK_SIGNAL_FUNC (on_button55_clicked),
                      dialog2);

  return dialog2;
}

GtkWidget*
create_dialog3 (void)
{
  GtkWidget *dialog3;
  GtkWidget *dialog_vbox3;
  GtkWidget *table2;
  GtkWidget *label86;
  GtkWidget *label87;
  GtkWidget *label88;
  GtkWidget *label89;
  GtkWidget *vbox9;
  GtkWidget *pixmap16;
  GtkWidget *dialog_action_area3;
  GtkWidget *button56;

  dialog3 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog3), "dialog3", dialog3);
  gtk_window_set_title (GTK_WINDOW (dialog3), _("ODBCConfig - Drivers"));
  GTK_WINDOW (dialog3)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_modal (GTK_WINDOW (dialog3), TRUE);
  gtk_window_set_policy (GTK_WINDOW (dialog3), TRUE, TRUE, FALSE);

  dialog_vbox3 = GTK_DIALOG (dialog3)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog3), "dialog_vbox3", dialog_vbox3);
  gtk_widget_show (dialog_vbox3);

  table2 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table2);
  gtk_object_set_data_full (GTK_OBJECT (dialog3), "table2", table2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table2);
  gtk_box_pack_start (GTK_BOX (dialog_vbox3), table2, TRUE, TRUE, 0);

  label86 = gtk_label_new ("");
  gtk_widget_ref (label86);
  gtk_object_set_data_full (GTK_OBJECT (dialog3), "label86", label86,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label86);
  gtk_table_attach (GTK_TABLE (table2), label86, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label86, 16, -2);

  label87 = gtk_label_new ("");
  gtk_widget_ref (label87);
  gtk_object_set_data_full (GTK_OBJECT (dialog3), "label87", label87,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label87);
  gtk_table_attach (GTK_TABLE (table2), label87, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  label88 = gtk_label_new (_("The ODBC Drivers contain code specific to a Database System and provides a set of callable functions to the Driver Manager.\nDrivers may implement some database functionality when it is required by ODBC and is not present in the Database System.\nDrivers may also translate data types.\n\nODBC Drivers can be obtained from the Internet or directly from the Database vendor.\n\nCheck http://www.unixodbc.org for drivers"));
  gtk_widget_ref (label88);
  gtk_object_set_data_full (GTK_OBJECT (dialog3), "label88", label88,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label88);
  gtk_table_attach (GTK_TABLE (table2), label88, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label88), TRUE);

  label89 = gtk_label_new ("");
  gtk_widget_ref (label89);
  gtk_object_set_data_full (GTK_OBJECT (dialog3), "label89", label89,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label89);
  gtk_table_attach (GTK_TABLE (table2), label89, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  vbox9 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox9);
  gtk_object_set_data_full (GTK_OBJECT (dialog3), "vbox9", vbox9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox9);
  gtk_table_attach (GTK_TABLE (table2), vbox9, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  pixmap16 = create_pixmap (dialog3, "gODBCConfig/info.xpm", FALSE);
  gtk_widget_ref (pixmap16);
  gtk_object_set_data_full (GTK_OBJECT (dialog3), "pixmap16", pixmap16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap16);
  gtk_box_pack_start (GTK_BOX (vbox9), pixmap16, TRUE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixmap16), 16, 0);

  dialog_action_area3 = GTK_DIALOG (dialog3)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog3), "dialog_action_area3", dialog_action_area3);
  gtk_widget_show (dialog_action_area3);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area3), 10);

  button56 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button56);
  gtk_object_set_data_full (GTK_OBJECT (dialog3), "button56", button56,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button56);
  gtk_box_pack_start (GTK_BOX (dialog_action_area3), button56, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button56), "clicked",
                      GTK_SIGNAL_FUNC (on_button56_clicked),
                      dialog3);

  return dialog3;
}

GtkWidget*
create_dialog4 (void)
{
  GtkWidget *dialog4;
  GtkWidget *dialog_vbox4;
  GtkWidget *table3;
  GtkWidget *label90;
  GtkWidget *label92;
  GtkWidget *label93;
  GtkWidget *label94;
  GtkWidget *vbox10;
  GtkWidget *pixmap17;
  GtkWidget *dialog_action_area4;
  GtkWidget *button57;

  dialog4 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog4), "dialog4", dialog4);
  gtk_window_set_title (GTK_WINDOW (dialog4), _("ODBCConfig - Database Systems"));
  GTK_WINDOW (dialog4)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_modal (GTK_WINDOW (dialog4), TRUE);
  gtk_window_set_policy (GTK_WINDOW (dialog4), TRUE, TRUE, FALSE);

  dialog_vbox4 = GTK_DIALOG (dialog4)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog4), "dialog_vbox4", dialog_vbox4);
  gtk_widget_show (dialog_vbox4);

  table3 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table3);
  gtk_object_set_data_full (GTK_OBJECT (dialog4), "table3", table3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table3);
  gtk_box_pack_start (GTK_BOX (dialog_vbox4), table3, TRUE, TRUE, 0);

  label90 = gtk_label_new (_("Perhaps the most common type of Database System today is an SQL Server\n\nSQL Servers with Heavy Functionality\n  ADABAS-D\n  Empress\n  Sybase - www.sybase.com\n  Oracle - www.oracle.com\n\nSQL Servers with Lite Functionality\n  MiniSQL\n  MySQL\n  Solid\n\nThe Database System may be running on the local machine or on a remote machine. It may also store its information in a variety of ways. This does not matter to an ODBC application because the Driver Manager and the Driver provides a consistent interface to the Database System."));
  gtk_widget_ref (label90);
  gtk_object_set_data_full (GTK_OBJECT (dialog4), "label90", label90,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label90);
  gtk_table_attach (GTK_TABLE (table3), label90, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label90), TRUE);

  label92 = gtk_label_new ("");
  gtk_widget_ref (label92);
  gtk_object_set_data_full (GTK_OBJECT (dialog4), "label92", label92,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label92);
  gtk_table_attach (GTK_TABLE (table3), label92, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label92, 16, -2);

  label93 = gtk_label_new ("");
  gtk_widget_ref (label93);
  gtk_object_set_data_full (GTK_OBJECT (dialog4), "label93", label93,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label93);
  gtk_table_attach (GTK_TABLE (table3), label93, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  label94 = gtk_label_new ("");
  gtk_widget_ref (label94);
  gtk_object_set_data_full (GTK_OBJECT (dialog4), "label94", label94,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label94);
  gtk_table_attach (GTK_TABLE (table3), label94, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  vbox10 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox10);
  gtk_object_set_data_full (GTK_OBJECT (dialog4), "vbox10", vbox10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox10);
  gtk_table_attach (GTK_TABLE (table3), vbox10, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  pixmap17 = create_pixmap (dialog4, "gODBCConfig/info.xpm", FALSE);
  gtk_widget_ref (pixmap17);
  gtk_object_set_data_full (GTK_OBJECT (dialog4), "pixmap17", pixmap17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap17);
  gtk_box_pack_start (GTK_BOX (vbox10), pixmap17, TRUE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixmap17), 16, 0);

  dialog_action_area4 = GTK_DIALOG (dialog4)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog4), "dialog_action_area4", dialog_action_area4);
  gtk_widget_show (dialog_action_area4);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area4), 10);

  button57 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button57);
  gtk_object_set_data_full (GTK_OBJECT (dialog4), "button57", button57,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button57);
  gtk_box_pack_start (GTK_BOX (dialog_action_area4), button57, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button57), "clicked",
                      GTK_SIGNAL_FUNC (on_button57_clicked),
                      dialog4);

  return dialog4;
}

GtkWidget*
create_dialog5 (void)
{
  GtkWidget *dialog5;
  GtkWidget *dialog_vbox5;
  GtkWidget *table4;
  GtkWidget *label95;
  GtkWidget *label96;
  GtkWidget *label98;
  GtkWidget *label99;
  GtkWidget *vbox11;
  GtkWidget *pixmap18;
  GtkWidget *dialog_action_area5;
  GtkWidget *button58;

  dialog5 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog5), "dialog5", dialog5);
  gtk_window_set_title (GTK_WINDOW (dialog5), _("ODBCConfig - odbc.ini"));
  GTK_WINDOW (dialog5)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_modal (GTK_WINDOW (dialog5), TRUE);
  gtk_window_set_policy (GTK_WINDOW (dialog5), TRUE, TRUE, FALSE);

  dialog_vbox5 = GTK_DIALOG (dialog5)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog5), "dialog_vbox5", dialog_vbox5);
  gtk_widget_show (dialog_vbox5);

  table4 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table4);
  gtk_object_set_data_full (GTK_OBJECT (dialog5), "table4", table4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table4);
  gtk_box_pack_start (GTK_BOX (dialog_vbox5), table4, TRUE, TRUE, 0);

  label95 = gtk_label_new (_("This is the main configuration file for ODBC.\nIt contains Data Source configuration.\n\nIt is used by the Driver Manager to determine, from a given Data Source Name, such things as the name of the Driver.\n\nIt is a simple text file but is configured using the ODBCConfig tool.\nThe User data sources are typically stored in ~/.odbc.ini while the System data sources are stored in /etc/odbc.ini\n"));
  gtk_widget_ref (label95);
  gtk_object_set_data_full (GTK_OBJECT (dialog5), "label95", label95,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label95);
  gtk_table_attach (GTK_TABLE (table4), label95, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label95), TRUE);

  label96 = gtk_label_new ("");
  gtk_widget_ref (label96);
  gtk_object_set_data_full (GTK_OBJECT (dialog5), "label96", label96,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label96);
  gtk_table_attach (GTK_TABLE (table4), label96, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label96, 16, -2);

  label98 = gtk_label_new ("");
  gtk_widget_ref (label98);
  gtk_object_set_data_full (GTK_OBJECT (dialog5), "label98", label98,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label98);
  gtk_table_attach (GTK_TABLE (table4), label98, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  label99 = gtk_label_new ("");
  gtk_widget_ref (label99);
  gtk_object_set_data_full (GTK_OBJECT (dialog5), "label99", label99,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label99);
  gtk_table_attach (GTK_TABLE (table4), label99, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  vbox11 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox11);
  gtk_object_set_data_full (GTK_OBJECT (dialog5), "vbox11", vbox11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox11);
  gtk_table_attach (GTK_TABLE (table4), vbox11, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  pixmap18 = create_pixmap (dialog5, "gODBCConfig/info.xpm", FALSE);
  gtk_widget_ref (pixmap18);
  gtk_object_set_data_full (GTK_OBJECT (dialog5), "pixmap18", pixmap18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap18);
  gtk_box_pack_start (GTK_BOX (vbox11), pixmap18, TRUE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixmap18), 16, 0);

  dialog_action_area5 = GTK_DIALOG (dialog5)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog5), "dialog_action_area5", dialog_action_area5);
  gtk_widget_show (dialog_action_area5);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area5), 10);

  button58 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button58);
  gtk_object_set_data_full (GTK_OBJECT (dialog5), "button58", button58,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button58);
  gtk_box_pack_start (GTK_BOX (dialog_action_area5), button58, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button58), "clicked",
                      GTK_SIGNAL_FUNC (on_button58_clicked),
                      dialog5);

  return dialog5;
}

GtkWidget*
create_dialog6 (void)
{
  GtkWidget *dialog6;
  GtkWidget *dialog_vbox6;
  GtkWidget *table5;
  GtkWidget *label100;
  GtkWidget *label102;
  GtkWidget *label103;
  GtkWidget *label104;
  GtkWidget *vbox12;
  GtkWidget *pixmap19;
  GtkWidget *dialog_action_area6;
  GtkWidget *button59;

  dialog6 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog6), "dialog6", dialog6);
  gtk_window_set_title (GTK_WINDOW (dialog6), _("ODBCConfig"));
  GTK_WINDOW (dialog6)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_modal (GTK_WINDOW (dialog6), TRUE);
  gtk_window_set_policy (GTK_WINDOW (dialog6), TRUE, TRUE, FALSE);

  dialog_vbox6 = GTK_DIALOG (dialog6)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog6), "dialog_vbox6", dialog_vbox6);
  gtk_widget_show (dialog_vbox6);

  table5 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table5);
  gtk_object_set_data_full (GTK_OBJECT (dialog6), "table5", table5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table5);
  gtk_box_pack_start (GTK_BOX (dialog_vbox6), table5, TRUE, TRUE, 0);

  label100 = gtk_label_new (_("This is the program you are using now. This program allows the user to easily configure ODBC."));
  gtk_widget_ref (label100);
  gtk_object_set_data_full (GTK_OBJECT (dialog6), "label100", label100,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label100);
  gtk_table_attach (GTK_TABLE (table5), label100, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label100), TRUE);

  label102 = gtk_label_new ("");
  gtk_widget_ref (label102);
  gtk_object_set_data_full (GTK_OBJECT (dialog6), "label102", label102,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label102);
  gtk_table_attach (GTK_TABLE (table5), label102, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label102, 16, -2);

  label103 = gtk_label_new ("");
  gtk_widget_ref (label103);
  gtk_object_set_data_full (GTK_OBJECT (dialog6), "label103", label103,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label103);
  gtk_table_attach (GTK_TABLE (table5), label103, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  label104 = gtk_label_new ("");
  gtk_widget_ref (label104);
  gtk_object_set_data_full (GTK_OBJECT (dialog6), "label104", label104,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label104);
  gtk_table_attach (GTK_TABLE (table5), label104, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  vbox12 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox12);
  gtk_object_set_data_full (GTK_OBJECT (dialog6), "vbox12", vbox12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox12);
  gtk_table_attach (GTK_TABLE (table5), vbox12, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  pixmap19 = create_pixmap (dialog6, "gODBCConfig/info.xpm", FALSE);
  gtk_widget_ref (pixmap19);
  gtk_object_set_data_full (GTK_OBJECT (dialog6), "pixmap19", pixmap19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap19);
  gtk_box_pack_start (GTK_BOX (vbox12), pixmap19, TRUE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixmap19), 16, 0);

  dialog_action_area6 = GTK_DIALOG (dialog6)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog6), "dialog_action_area6", dialog_action_area6);
  gtk_widget_show (dialog_action_area6);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area6), 10);

  button59 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button59);
  gtk_object_set_data_full (GTK_OBJECT (dialog6), "button59", button59,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button59);
  gtk_box_pack_start (GTK_BOX (dialog_action_area6), button59, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button59), "clicked",
                      GTK_SIGNAL_FUNC (on_button59_clicked),
                      dialog6);

  return dialog6;
}

GtkWidget*
create_dialog7 (void)
{
  GtkWidget *dialog7;
  GtkWidget *dialog_vbox7;
  GtkWidget *table6;
  GtkWidget *label105;
  GtkWidget *label107;
  GtkWidget *label108;
  GtkWidget *label109;
  GtkWidget *vbox13;
  GtkWidget *pixmap20;
  GtkWidget *dialog_action_area7;
  GtkWidget *button60;

  dialog7 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog7), "dialog7", dialog7);
  gtk_window_set_title (GTK_WINDOW (dialog7), _("ODBCConfig - odbcinst.ini"));
  GTK_WINDOW (dialog7)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_modal (GTK_WINDOW (dialog7), TRUE);
  gtk_window_set_policy (GTK_WINDOW (dialog7), TRUE, TRUE, FALSE);

  dialog_vbox7 = GTK_DIALOG (dialog7)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog7), "dialog_vbox7", dialog_vbox7);
  gtk_widget_show (dialog_vbox7);

  table6 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table6);
  gtk_object_set_data_full (GTK_OBJECT (dialog7), "table6", table6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table6);
  gtk_box_pack_start (GTK_BOX (dialog_vbox7), table6, TRUE, TRUE, 0);

  label105 = gtk_label_new (_("odbcinst.ini contains a list of all installed ODBC Drivers. Each entry also contains some information about the driver such as the file name(s) of the driver.\n\nAn entry should be made when an ODBC driver is installed and removed when the driver is uninstalled. This can be done using ODBCConfig or the odbcinst command tool."));
  gtk_widget_ref (label105);
  gtk_object_set_data_full (GTK_OBJECT (dialog7), "label105", label105,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label105);
  gtk_table_attach (GTK_TABLE (table6), label105, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label105), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label105), TRUE);

  label107 = gtk_label_new ("");
  gtk_widget_ref (label107);
  gtk_object_set_data_full (GTK_OBJECT (dialog7), "label107", label107,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label107);
  gtk_table_attach (GTK_TABLE (table6), label107, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label107, 16, -2);

  label108 = gtk_label_new ("");
  gtk_widget_ref (label108);
  gtk_object_set_data_full (GTK_OBJECT (dialog7), "label108", label108,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label108);
  gtk_table_attach (GTK_TABLE (table6), label108, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  label109 = gtk_label_new ("");
  gtk_widget_ref (label109);
  gtk_object_set_data_full (GTK_OBJECT (dialog7), "label109", label109,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label109);
  gtk_table_attach (GTK_TABLE (table6), label109, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  vbox13 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox13);
  gtk_object_set_data_full (GTK_OBJECT (dialog7), "vbox13", vbox13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox13);
  gtk_table_attach (GTK_TABLE (table6), vbox13, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  pixmap20 = create_pixmap (dialog7, "gODBCConfig/info.xpm", FALSE);
  gtk_widget_ref (pixmap20);
  gtk_object_set_data_full (GTK_OBJECT (dialog7), "pixmap20", pixmap20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap20);
  gtk_box_pack_start (GTK_BOX (vbox13), pixmap20, TRUE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixmap20), 15, 0);

  dialog_action_area7 = GTK_DIALOG (dialog7)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog7), "dialog_action_area7", dialog_action_area7);
  gtk_widget_show (dialog_action_area7);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area7), 10);

  button60 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button60);
  gtk_object_set_data_full (GTK_OBJECT (dialog7), "button60", button60,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button60);
  gtk_box_pack_start (GTK_BOX (dialog_action_area7), button60, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button60), "clicked",
                      GTK_SIGNAL_FUNC (on_button60_clicked),
                      dialog7);

  return dialog7;
}

GtkWidget*
create_dialog8 (void)
{
  GtkWidget *dialog8;
  GtkWidget *dialog_vbox8;
  GtkWidget *table7;
  GtkWidget *label111;
  GtkWidget *label112;
  GtkWidget *label114;
  GtkWidget *label115;
  GtkWidget *vbox14;
  GtkWidget *pixmap21;
  GtkWidget *dialog_action_area8;
  GtkWidget *button61;

  dialog8 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog8), "dialog8", dialog8);
  gtk_window_set_title (GTK_WINDOW (dialog8), _("ODBCConfig - Credits"));
  GTK_WINDOW (dialog8)->type = GTK_WINDOW_DIALOG;
  gtk_window_set_modal (GTK_WINDOW (dialog8), TRUE);
  gtk_window_set_policy (GTK_WINDOW (dialog8), TRUE, TRUE, FALSE);

  dialog_vbox8 = GTK_DIALOG (dialog8)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog8), "dialog_vbox8", dialog_vbox8);
  gtk_widget_show (dialog_vbox8);

  table7 = gtk_table_new (3, 3, FALSE);
  gtk_widget_ref (table7);
  gtk_object_set_data_full (GTK_OBJECT (dialog8), "table7", table7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table7);
  gtk_box_pack_start (GTK_BOX (dialog_vbox8), table7, TRUE, TRUE, 0);

  label111 = gtk_label_new ("");
  gtk_widget_ref (label111);
  gtk_object_set_data_full (GTK_OBJECT (dialog8), "label111", label111,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label111);
  gtk_table_attach (GTK_TABLE (table7), label111, 1, 2, 2, 3,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  label112 = gtk_label_new ("");
  gtk_widget_ref (label112);
  gtk_object_set_data_full (GTK_OBJECT (dialog8), "label112", label112,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label112);
  gtk_table_attach (GTK_TABLE (table7), label112, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

  label114 = gtk_label_new ("");
  gtk_widget_ref (label114);
  gtk_object_set_data_full (GTK_OBJECT (dialog8), "label114", label114,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label114);
  gtk_table_attach (GTK_TABLE (table7), label114, 2, 3, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (label114, 16, -2);

  label115 = gtk_label_new (_("unixODBC consists of the following components\n\n- Driver Manager\n- GUI Data Manager\n- GUI Config\n- Several Drivers and Driver Config libs\n- Driver Code Template\n- Driver Config Code Template\n- ODBCINST lib\n- odbcinst (command line tool for install scripts)\n- INI lib\n- LOG lib\n- LST lib\n- TRE lib\n- SQI lib\n- isql (command line tool for SQL)\n\nAll code is released under GPL and the LGPL license.\n"));
  gtk_widget_ref (label115);
  gtk_object_set_data_full (GTK_OBJECT (dialog8), "label115", label115,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label115);
  gtk_table_attach (GTK_TABLE (table7), label115, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label115), GTK_JUSTIFY_LEFT);

  vbox14 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox14);
  gtk_object_set_data_full (GTK_OBJECT (dialog8), "vbox14", vbox14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox14);
  gtk_table_attach (GTK_TABLE (table7), vbox14, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  pixmap21 = create_pixmap (dialog8, "gODBCConfig/info.xpm", FALSE);
  gtk_widget_ref (pixmap21);
  gtk_object_set_data_full (GTK_OBJECT (dialog8), "pixmap21", pixmap21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap21);
  gtk_box_pack_start (GTK_BOX (vbox14), pixmap21, TRUE, TRUE, 0);
  gtk_misc_set_padding (GTK_MISC (pixmap21), 16, 0);

  dialog_action_area8 = GTK_DIALOG (dialog8)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog8), "dialog_action_area8", dialog_action_area8);
  gtk_widget_show (dialog_action_area8);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area8), 10);

  button61 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button61);
  gtk_object_set_data_full (GTK_OBJECT (dialog8), "button61", button61,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button61);
  gtk_box_pack_start (GTK_BOX (dialog_action_area8), button61, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button61), "clicked",
                      GTK_SIGNAL_FUNC (on_button61_clicked),
                      dialog8);

  return dialog8;
}

GtkWidget*
create_window1 (void)
{
  GtkWidget *window1;
  GtkWidget *vbox2;
  GtkWidget *vbox3;
  GtkWidget *fixed31;
  GtkWidget *button68;
  GtkWidget *button69;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
  GtkWidget *entry5;
  GtkWidget *entry6;
  GtkWidget *button70;
  GtkWidget *button71;
  GtkWidget *label125;
  GtkWidget *label121;
  GtkWidget *label123;
  GtkWidget *label124;
  GtkWidget *label122;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (window1), "window1", window1);
  gtk_window_set_title (GTK_WINDOW (window1), _("gODBCConfig - New Driver"));
  gtk_window_set_modal (GTK_WINDOW (window1), TRUE);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox2);
  gtk_object_set_data_full (GTK_OBJECT (window1), "vbox2", vbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (window1), vbox2);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox3);
  gtk_object_set_data_full (GTK_OBJECT (window1), "vbox3", vbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox3);
  gtk_box_pack_start (GTK_BOX (vbox2), vbox3, TRUE, TRUE, 0);

  fixed31 = gtk_fixed_new ();
  gtk_widget_ref (fixed31);
  gtk_object_set_data_full (GTK_OBJECT (window1), "fixed31", fixed31,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed31);
  gtk_box_pack_start (GTK_BOX (vbox3), fixed31, TRUE, TRUE, 0);

  button68 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button68);
  gtk_object_set_data_full (GTK_OBJECT (window1), "button68", button68,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button68);
  gtk_fixed_put (GTK_FIXED (fixed31), button68, 192, 184);
  gtk_widget_set_uposition (button68, 192, 184);
  gtk_widget_set_usize (button68, 96, 32);

  button69 = gnome_stock_button (GNOME_STOCK_BUTTON_CANCEL);
  gtk_widget_ref (button69);
  gtk_object_set_data_full (GTK_OBJECT (window1), "button69", button69,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button69);
  gtk_fixed_put (GTK_FIXED (fixed31), button69, 312, 184);
  gtk_widget_set_uposition (button69, 312, 184);
  gtk_widget_set_usize (button69, 96, 32);

  entry2 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry2);
  gtk_object_set_data_full (GTK_OBJECT (window1), "entry2", entry2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry2);
  gtk_fixed_put (GTK_FIXED (fixed31), entry2, 88, 16);
  gtk_widget_set_uposition (entry2, 88, 16);
  gtk_widget_set_usize (entry2, 304, 24);

  entry3 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry3);
  gtk_object_set_data_full (GTK_OBJECT (window1), "entry3", entry3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry3);
  gtk_fixed_put (GTK_FIXED (fixed31), entry3, 88, 48);
  gtk_widget_set_uposition (entry3, 88, 48);
  gtk_widget_set_usize (entry3, 304, 24);

  entry4 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry4);
  gtk_object_set_data_full (GTK_OBJECT (window1), "entry4", entry4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry4);
  gtk_fixed_put (GTK_FIXED (fixed31), entry4, 88, 80);
  gtk_widget_set_uposition (entry4, 88, 80);
  gtk_widget_set_usize (entry4, 240, 24);

  entry5 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry5);
  gtk_object_set_data_full (GTK_OBJECT (window1), "entry5", entry5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry5);
  gtk_fixed_put (GTK_FIXED (fixed31), entry5, 88, 112);
  gtk_widget_set_uposition (entry5, 88, 112);
  gtk_widget_set_usize (entry5, 240, 24);

  entry6 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry6);
  gtk_object_set_data_full (GTK_OBJECT (window1), "entry6", entry6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry6);
  gtk_fixed_put (GTK_FIXED (fixed31), entry6, 88, 144);
  gtk_widget_set_uposition (entry6, 88, 144);
  gtk_widget_set_usize (entry6, 304, 24);

  button70 = gtk_button_new_with_label (_("Browse"));
  gtk_widget_ref (button70);
  gtk_object_set_data_full (GTK_OBJECT (window1), "button70", button70,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button70);
  gtk_fixed_put (GTK_FIXED (fixed31), button70, 336, 80);
  gtk_widget_set_uposition (button70, 336, 80);
  gtk_widget_set_usize (button70, 54, 24);

  button71 = gtk_button_new_with_label (_("Browse"));
  gtk_widget_ref (button71);
  gtk_object_set_data_full (GTK_OBJECT (window1), "button71", button71,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button71);
  gtk_fixed_put (GTK_FIXED (fixed31), button71, 336, 112);
  gtk_widget_set_uposition (button71, 336, 112);
  gtk_widget_set_usize (button71, 54, 24);

  label125 = gtk_label_new (_("FileUsage"));
  gtk_widget_ref (label125);
  gtk_object_set_data_full (GTK_OBJECT (window1), "label125", label125,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label125);
  gtk_fixed_put (GTK_FIXED (fixed31), label125, 8, 152);
  gtk_widget_set_uposition (label125, 8, 152);
  gtk_widget_set_usize (label125, 64, 16);
  gtk_label_set_justify (GTK_LABEL (label125), GTK_JUSTIFY_LEFT);

  label121 = gtk_label_new (_("Name"));
  gtk_widget_ref (label121);
  gtk_object_set_data_full (GTK_OBJECT (window1), "label121", label121,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label121);
  gtk_fixed_put (GTK_FIXED (fixed31), label121, 8, 24);
  gtk_widget_set_uposition (label121, 8, 24);
  gtk_widget_set_usize (label121, 40, 16);
  gtk_label_set_justify (GTK_LABEL (label121), GTK_JUSTIFY_LEFT);

  label123 = gtk_label_new (_("Driver"));
  gtk_widget_ref (label123);
  gtk_object_set_data_full (GTK_OBJECT (window1), "label123", label123,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label123);
  gtk_fixed_put (GTK_FIXED (fixed31), label123, 8, 88);
  gtk_widget_set_uposition (label123, 8, 88);
  gtk_widget_set_usize (label123, 40, 16);
  gtk_label_set_justify (GTK_LABEL (label123), GTK_JUSTIFY_LEFT);

  label124 = gtk_label_new (_("Setup"));
  gtk_widget_ref (label124);
  gtk_object_set_data_full (GTK_OBJECT (window1), "label124", label124,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label124);
  gtk_fixed_put (GTK_FIXED (fixed31), label124, 8, 120);
  gtk_widget_set_uposition (label124, 8, 120);
  gtk_widget_set_usize (label124, 40, 16);

  label122 = gtk_label_new (_("Description"));
  gtk_widget_ref (label122);
  gtk_object_set_data_full (GTK_OBJECT (window1), "label122", label122,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label122);
  gtk_fixed_put (GTK_FIXED (fixed31), label122, 8, 56);
  gtk_widget_set_uposition (label122, 8, 56);
  gtk_widget_set_usize (label122, 72, 16);
  gtk_label_set_justify (GTK_LABEL (label122), GTK_JUSTIFY_LEFT);

  gtk_signal_connect (GTK_OBJECT (button68), "clicked",
                      GTK_SIGNAL_FUNC (on_button68_clicked),
                      window1);
  gtk_signal_connect (GTK_OBJECT (button69), "clicked",
                      GTK_SIGNAL_FUNC (on_button69_clicked),
                      window1);
  gtk_signal_connect (GTK_OBJECT (button70), "clicked",
                      GTK_SIGNAL_FUNC (on_browse_clicked),
                      entry4);
  gtk_signal_connect (GTK_OBJECT (button71), "clicked",
                      GTK_SIGNAL_FUNC (on_browse_clicked),
                      entry5);
  gtk_signal_connect (GTK_OBJECT (entry2), "changed",
                      GTK_SIGNAL_FUNC (on_entry2_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry3), "changed",
                      GTK_SIGNAL_FUNC (on_entry3_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry4), "changed",
                      GTK_SIGNAL_FUNC (on_entry4_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry4), "realize",
                      GTK_SIGNAL_FUNC (on_entry4_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry5), "changed",
                      GTK_SIGNAL_FUNC (on_entry5_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry5), "realize",
                      GTK_SIGNAL_FUNC (on_entry5_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry6), "changed",
                      GTK_SIGNAL_FUNC (on_entry6_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry6), "realize",
                      GTK_SIGNAL_FUNC (on_entry6_realize),
                      NULL);

  return window1;
}

GtkWidget*
create_window2 (void)
{
  GtkWidget *window2;
  GtkWidget *vbox5;
  GtkWidget *vbox6;
  GtkWidget *fixed32;
  GtkWidget *button75;
  GtkWidget *button76;
  GtkWidget *entry11;
  GtkWidget *entry12;
  GtkWidget *entry13;
  GtkWidget *entry14;
  GtkWidget *entry15;
  GtkWidget *button77;
  GtkWidget *button78;
  GtkWidget *label130;
  GtkWidget *label131;
  GtkWidget *label132;
  GtkWidget *label133;
  GtkWidget *label134;

  window2 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (window2), "window2", window2);
  gtk_window_set_title (GTK_WINDOW (window2), _("gODBCConfig - Configure Driver"));
  gtk_window_set_modal (GTK_WINDOW (window2), TRUE);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox5);
  gtk_object_set_data_full (GTK_OBJECT (window2), "vbox5", vbox5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox5);
  gtk_container_add (GTK_CONTAINER (window2), vbox5);

  vbox6 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox6);
  gtk_object_set_data_full (GTK_OBJECT (window2), "vbox6", vbox6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox6);
  gtk_box_pack_start (GTK_BOX (vbox5), vbox6, TRUE, TRUE, 0);

  fixed32 = gtk_fixed_new ();
  gtk_widget_ref (fixed32);
  gtk_object_set_data_full (GTK_OBJECT (window2), "fixed32", fixed32,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed32);
  gtk_box_pack_start (GTK_BOX (vbox6), fixed32, TRUE, TRUE, 0);

  button75 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button75);
  gtk_object_set_data_full (GTK_OBJECT (window2), "button75", button75,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button75);
  gtk_fixed_put (GTK_FIXED (fixed32), button75, 192, 184);
  gtk_widget_set_uposition (button75, 192, 184);
  gtk_widget_set_usize (button75, 96, 32);

  button76 = gnome_stock_button (GNOME_STOCK_BUTTON_CANCEL);
  gtk_widget_ref (button76);
  gtk_object_set_data_full (GTK_OBJECT (window2), "button76", button76,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button76);
  gtk_fixed_put (GTK_FIXED (fixed32), button76, 312, 184);
  gtk_widget_set_uposition (button76, 312, 184);
  gtk_widget_set_usize (button76, 96, 32);

  entry11 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry11);
  gtk_object_set_data_full (GTK_OBJECT (window2), "entry11", entry11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry11);
  gtk_fixed_put (GTK_FIXED (fixed32), entry11, 88, 16);
  gtk_widget_set_uposition (entry11, 88, 16);
  gtk_widget_set_usize (entry11, 304, 24);

  entry12 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry12);
  gtk_object_set_data_full (GTK_OBJECT (window2), "entry12", entry12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry12);
  gtk_fixed_put (GTK_FIXED (fixed32), entry12, 88, 48);
  gtk_widget_set_uposition (entry12, 88, 48);
  gtk_widget_set_usize (entry12, 304, 24);

  entry13 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry13);
  gtk_object_set_data_full (GTK_OBJECT (window2), "entry13", entry13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry13);
  gtk_fixed_put (GTK_FIXED (fixed32), entry13, 88, 80);
  gtk_widget_set_uposition (entry13, 88, 80);
  gtk_widget_set_usize (entry13, 240, 24);

  entry14 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry14);
  gtk_object_set_data_full (GTK_OBJECT (window2), "entry14", entry14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry14);
  gtk_fixed_put (GTK_FIXED (fixed32), entry14, 88, 112);
  gtk_widget_set_uposition (entry14, 88, 112);
  gtk_widget_set_usize (entry14, 240, 24);

  entry15 = gtk_entry_new_with_max_length (256);
  gtk_widget_ref (entry15);
  gtk_object_set_data_full (GTK_OBJECT (window2), "entry15", entry15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry15);
  gtk_fixed_put (GTK_FIXED (fixed32), entry15, 88, 144);
  gtk_widget_set_uposition (entry15, 88, 144);
  gtk_widget_set_usize (entry15, 304, 24);

  button77 = gtk_button_new_with_label (_("Browse"));
  gtk_widget_ref (button77);
  gtk_object_set_data_full (GTK_OBJECT (window2), "button77", button77,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button77);
  gtk_fixed_put (GTK_FIXED (fixed32), button77, 336, 80);
  gtk_widget_set_uposition (button77, 336, 80);
  gtk_widget_set_usize (button77, 54, 24);

  button78 = gtk_button_new_with_label (_("Browse"));
  gtk_widget_ref (button78);
  gtk_object_set_data_full (GTK_OBJECT (window2), "button78", button78,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button78);
  gtk_fixed_put (GTK_FIXED (fixed32), button78, 336, 112);
  gtk_widget_set_uposition (button78, 336, 112);
  gtk_widget_set_usize (button78, 54, 24);

  label130 = gtk_label_new (_("FileUsage"));
  gtk_widget_ref (label130);
  gtk_object_set_data_full (GTK_OBJECT (window2), "label130", label130,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label130);
  gtk_fixed_put (GTK_FIXED (fixed32), label130, 8, 152);
  gtk_widget_set_uposition (label130, 8, 152);
  gtk_widget_set_usize (label130, 64, 16);
  gtk_label_set_justify (GTK_LABEL (label130), GTK_JUSTIFY_LEFT);

  label131 = gtk_label_new (_("Name"));
  gtk_widget_ref (label131);
  gtk_object_set_data_full (GTK_OBJECT (window2), "label131", label131,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label131);
  gtk_fixed_put (GTK_FIXED (fixed32), label131, 8, 24);
  gtk_widget_set_uposition (label131, 8, 24);
  gtk_widget_set_usize (label131, 40, 16);
  gtk_label_set_justify (GTK_LABEL (label131), GTK_JUSTIFY_LEFT);

  label132 = gtk_label_new (_("Driver"));
  gtk_widget_ref (label132);
  gtk_object_set_data_full (GTK_OBJECT (window2), "label132", label132,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label132);
  gtk_fixed_put (GTK_FIXED (fixed32), label132, 8, 88);
  gtk_widget_set_uposition (label132, 8, 88);
  gtk_widget_set_usize (label132, 40, 16);
  gtk_label_set_justify (GTK_LABEL (label132), GTK_JUSTIFY_LEFT);

  label133 = gtk_label_new (_("Setup"));
  gtk_widget_ref (label133);
  gtk_object_set_data_full (GTK_OBJECT (window2), "label133", label133,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label133);
  gtk_fixed_put (GTK_FIXED (fixed32), label133, 8, 120);
  gtk_widget_set_uposition (label133, 8, 120);
  gtk_widget_set_usize (label133, 40, 16);

  label134 = gtk_label_new (_("Description"));
  gtk_widget_ref (label134);
  gtk_object_set_data_full (GTK_OBJECT (window2), "label134", label134,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label134);
  gtk_fixed_put (GTK_FIXED (fixed32), label134, 8, 56);
  gtk_widget_set_uposition (label134, 8, 56);
  gtk_widget_set_usize (label134, 72, 16);
  gtk_label_set_justify (GTK_LABEL (label134), GTK_JUSTIFY_LEFT);

  gtk_signal_connect (GTK_OBJECT (button75), "clicked",
                      GTK_SIGNAL_FUNC (on_button68_clicked),
                      window2);
  gtk_signal_connect (GTK_OBJECT (button76), "clicked",
                      GTK_SIGNAL_FUNC (on_button69_clicked),
                      window2);
  gtk_signal_connect (GTK_OBJECT (entry11), "changed",
                      GTK_SIGNAL_FUNC (on_entry2_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry11), "realize",
                      GTK_SIGNAL_FUNC (on_entry2_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry12), "changed",
                      GTK_SIGNAL_FUNC (on_entry3_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry12), "realize",
                      GTK_SIGNAL_FUNC (on_entry3_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry13), "changed",
                      GTK_SIGNAL_FUNC (on_entry4_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry13), "realize",
                      GTK_SIGNAL_FUNC (on_entry4_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry14), "changed",
                      GTK_SIGNAL_FUNC (on_entry5_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry14), "realize",
                      GTK_SIGNAL_FUNC (on_entry5_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry15), "changed",
                      GTK_SIGNAL_FUNC (on_entry6_changed),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (entry15), "realize",
                      GTK_SIGNAL_FUNC (on_entry6_realize),
                      NULL);

  return window2;
}

GtkWidget*
create_window5 (void)
{
  GtkWidget *window5;
  GtkWidget *fixed33;
  GtkWidget *label135;
  GtkWidget *button79;
  GtkWidget *button82;
  GtkWidget *button83;
  GtkWidget *vseparator8;
  GtkWidget *frame10;
  GtkWidget *fixed34;
  GtkWidget *label140;
  GtkWidget *pixmap13;
  GtkWidget *scrolledwindow5;
  GtkWidget *clist7;
  GtkWidget *label136;
  GtkWidget *label137;
  GtkWidget *label138;
  GtkWidget *label139;

  window5 = gtk_window_new (GTK_WINDOW_DIALOG);
  gtk_object_set_data (GTK_OBJECT (window5), "window5", window5);
  gtk_window_set_title (GTK_WINDOW (window5), _("gODBCConfig - Add DSN"));
  gtk_window_set_modal (GTK_WINDOW (window5), TRUE);

  fixed33 = gtk_fixed_new ();
  gtk_widget_ref (fixed33);
  gtk_object_set_data_full (GTK_OBJECT (window5), "fixed33", fixed33,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed33);
  gtk_container_add (GTK_CONTAINER (window5), fixed33);

  label135 = gtk_label_new (_("Select the DRIVER to use or Add a new one"));
  gtk_widget_ref (label135);
  gtk_object_set_data_full (GTK_OBJECT (window5), "label135", label135,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label135);
  gtk_fixed_put (GTK_FIXED (fixed33), label135, 8, 8);
  gtk_widget_set_uposition (label135, 8, 8);
  gtk_widget_set_usize (label135, 256, 16);
  gtk_label_set_justify (GTK_LABEL (label135), GTK_JUSTIFY_LEFT);

  button79 = gtk_button_new_with_label (_("Add"));
  gtk_widget_ref (button79);
  gtk_object_set_data_full (GTK_OBJECT (window5), "button79", button79,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button79);
  gtk_fixed_put (GTK_FIXED (fixed33), button79, 312, 40);
  gtk_widget_set_uposition (button79, 312, 40);
  gtk_widget_set_usize (button79, 88, 32);

  button82 = gnome_stock_button (GNOME_STOCK_BUTTON_OK);
  gtk_widget_ref (button82);
  gtk_object_set_data_full (GTK_OBJECT (window5), "button82", button82,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button82);
  gtk_fixed_put (GTK_FIXED (fixed33), button82, 424, 208);
  gtk_widget_set_uposition (button82, 424, 208);
  gtk_widget_set_usize (button82, 88, 32);

  button83 = gnome_stock_button (GNOME_STOCK_BUTTON_CANCEL);
  gtk_widget_ref (button83);
  gtk_object_set_data_full (GTK_OBJECT (window5), "button83", button83,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button83);
  gtk_fixed_put (GTK_FIXED (fixed33), button83, 424, 248);
  gtk_widget_set_uposition (button83, 424, 248);
  gtk_widget_set_usize (button83, 88, 32);

  vseparator8 = gtk_vseparator_new ();
  gtk_widget_ref (vseparator8);
  gtk_object_set_data_full (GTK_OBJECT (window5), "vseparator8", vseparator8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vseparator8);
  gtk_fixed_put (GTK_FIXED (fixed33), vseparator8, 408, 8);
  gtk_widget_set_uposition (vseparator8, 408, 8);
  gtk_widget_set_usize (vseparator8, 16, 280);

  frame10 = gtk_frame_new (NULL);
  gtk_widget_ref (frame10);
  gtk_object_set_data_full (GTK_OBJECT (window5), "frame10", frame10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame10);
  gtk_fixed_put (GTK_FIXED (fixed33), frame10, 0, 216);
  gtk_widget_set_uposition (frame10, 0, 216);
  gtk_widget_set_usize (frame10, 408, 72);

  fixed34 = gtk_fixed_new ();
  gtk_widget_ref (fixed34);
  gtk_object_set_data_full (GTK_OBJECT (window5), "fixed34", fixed34,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed34);
  gtk_container_add (GTK_CONTAINER (frame10), fixed34);

  label140 = gtk_label_new (_("These drivers facilitate communication between the Driver Manager and the data server. Many ODBC drivers  for Linux can be downloaded from the Internet while others are obtained from your database vendor."));
  gtk_widget_ref (label140);
  gtk_object_set_data_full (GTK_OBJECT (window5), "label140", label140,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label140);
  gtk_fixed_put (GTK_FIXED (fixed34), label140, 72, 8);
  gtk_widget_set_uposition (label140, 72, 8);
  gtk_widget_set_usize (label140, 312, 56);
  gtk_label_set_line_wrap (GTK_LABEL (label140), TRUE);

  pixmap13 = create_pixmap (window5, "gODBCConfig/driver.xpm", FALSE);
  gtk_widget_ref (pixmap13);
  gtk_object_set_data_full (GTK_OBJECT (window5), "pixmap13", pixmap13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap13);
  gtk_fixed_put (GTK_FIXED (fixed34), pixmap13, 8, 8);
  gtk_widget_set_uposition (pixmap13, 8, 8);
  gtk_widget_set_usize (pixmap13, 48, 48);

  scrolledwindow5 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow5);
  gtk_object_set_data_full (GTK_OBJECT (window5), "scrolledwindow5", scrolledwindow5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow5);
  gtk_fixed_put (GTK_FIXED (fixed33), scrolledwindow5, 0, 32);
  gtk_widget_set_uposition (scrolledwindow5, 0, 32);
  gtk_widget_set_usize (scrolledwindow5, 304, 176);

  clist7 = gtk_clist_new (4);
  gtk_widget_ref (clist7);
  gtk_object_set_data_full (GTK_OBJECT (window5), "clist7", clist7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (clist7);
  gtk_container_add (GTK_CONTAINER (scrolledwindow5), clist7);
  gtk_widget_set_usize (clist7, 304, 176);
  gtk_clist_set_column_width (GTK_CLIST (clist7), 0, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist7), 1, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist7), 2, 80);
  gtk_clist_set_column_width (GTK_CLIST (clist7), 3, 80);
  gtk_clist_column_titles_show (GTK_CLIST (clist7));

  label136 = gtk_label_new (_("Name"));
  gtk_widget_ref (label136);
  gtk_object_set_data_full (GTK_OBJECT (window5), "label136", label136,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label136);
  gtk_clist_set_column_widget (GTK_CLIST (clist7), 0, label136);

  label137 = gtk_label_new (_("Description"));
  gtk_widget_ref (label137);
  gtk_object_set_data_full (GTK_OBJECT (window5), "label137", label137,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label137);
  gtk_clist_set_column_widget (GTK_CLIST (clist7), 1, label137);

  label138 = gtk_label_new (_("Driver Lib"));
  gtk_widget_ref (label138);
  gtk_object_set_data_full (GTK_OBJECT (window5), "label138", label138,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label138);
  gtk_clist_set_column_widget (GTK_CLIST (clist7), 2, label138);

  label139 = gtk_label_new (_("Setup Lib"));
  gtk_widget_ref (label139);
  gtk_object_set_data_full (GTK_OBJECT (window5), "label139", label139,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label139);
  gtk_clist_set_column_widget (GTK_CLIST (clist7), 3, label139);

  gtk_signal_connect (GTK_OBJECT (button79), "clicked",
                      GTK_SIGNAL_FUNC (on_button79_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (button82), "clicked",
                      GTK_SIGNAL_FUNC (on_button82_clicked),
                      window5);
  gtk_signal_connect (GTK_OBJECT (button83), "clicked",
                      GTK_SIGNAL_FUNC (on_button83_clicked),
                      window5);
  gtk_signal_connect (GTK_OBJECT (clist7), "realize",
                      GTK_SIGNAL_FUNC (on_clist7_realize),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (clist7), "select_row",
                      GTK_SIGNAL_FUNC (on_clist7_select_row),
                      NULL);

  return window5;
}

guint
gtk_odbcconfig_get_type()
{
	static guint odbc_type = 0;

	if (!odbc_type)
	{
		GtkTypeInfo odbc_info = 
		{
			"OdbcConfig",
			sizeof(GtkOdbcConfig),
			sizeof(GtkOdbcConfigClass),
			(GtkClassInitFunc) gtk_odbcconfig_class_init,
			(GtkObjectInitFunc) gtk_odbcconfig_init,
			(GtkArgSetFunc) NULL,
			(GtkArgGetFunc) NULL,
			(GtkClassInitFunc) NULL
					
		};

		odbc_type = gtk_type_unique( GTK_TYPE_VBOX, &odbc_info );
	}

	return odbc_type;
}

