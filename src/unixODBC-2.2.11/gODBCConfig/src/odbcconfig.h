/* odbcconfig */

#ifndef __ODBCCONFIG_H__
#define __ODBCCONFIG_H__

#include <gdk/gdk.h>
#include <gtk/gtkvbox.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GTK_TYPE_ODBCCONFIG			 	(gtk_odbcconfig_get_type ())
#define GTK_ODBCCONFIG(obj)				(GTK_CHECK_CAST ((obj), \
									 	GTK_TYPE_ODBCCONFIG, GtkOdbcConfig ))
#define GTK_ODBCCONFIG_CLASS(klass) 	(GTK_CHECK_CLASS_CAST ((klass), \
										GTK_TYPE_ODBCCONFIG, GtkOdbcConfigClass))
#define GTK_IS_ODBCCONFIG(obj)			(GTK_CHECK_TYPE ((obj), \
										GDK_TYPE_ODBCCONFIG))
#define GTK_IS_ODBCCONFIG_CLASS(klass)	(GTK_CHECK_CLASS_TYPE ((klass), \
										GDK_TYPE_ODBCCONFIG))


typedef struct _GtkOdbcConfig		GtkOdbcConfig;
typedef struct _GtkOdbcConfigClass	GtkOdbcConfigClass;

struct _GtkOdbcConfig
{
  GtkVBox	vbox; 

  GtkWidget *window;
  GtkWidget *gODBCConfig;
  GtkWidget *notebook1;
  GtkWidget *fixed2;
  GtkWidget *button2;
  GtkWidget *button3;
  GtkWidget *button4;
  GtkWidget *frame1;
  GtkWidget *fixed5;
  GtkWidget *label24;
  GtkWidget *pixmap3;
  GtkWidget *scrolledwindow2;
  GtkWidget *clist1;
  GtkWidget *Name;
  GtkWidget *Desc;
  GtkWidget *Driver;
  GtkWidget *User;
  GtkWidget *fixed13;
  GtkWidget *button16;
  GtkWidget *button17;
  GtkWidget *button18;
  GtkWidget *frame4;
  GtkWidget *fixed14;
  GtkWidget *label37;
  GtkWidget *pixmap7;
  GtkWidget *scrolledwindow3;
  GtkWidget *clist5;
  GtkWidget *label34;
  GtkWidget *label35;
  GtkWidget *label36;
  GtkWidget *System;
  GtkWidget *fixed4;
  GtkWidget *fixed8;
  GtkWidget *fixed15;
  GtkWidget *fixed20;
  GtkWidget *button25;
  GtkWidget *button26;
  GtkWidget *button27;
  GtkWidget *frame7;
  GtkWidget *fixed21;
  GtkWidget *label52;
  GtkWidget *pixmap10;
  GtkWidget *scrolledwindow4;
  GtkWidget *clist6;
  GtkWidget *label117;
  GtkWidget *label118;
  GtkWidget *label119;
  GtkWidget *label120;
  GtkWidget *Drivers;
  GtkWidget *fixed29;
  GtkWidget *button49;
  GtkWidget *frame9;
  GtkWidget *fixed30;
  GtkWidget *label73;
  GtkWidget *pixmap12;
  GtkWidget *entry1;
  GtkWidget *button51;
  GtkWidget *label74;
  GtkWidget *tracing_on;
  GtkWidget *Tracing;
  GtkWidget *fixed24;
  GtkWidget *frame8;
  GtkWidget *fixed25;
  GtkWidget *label61;
  GtkWidget *label62;
  GtkWidget *button52;
  GtkWidget *button53;
  GtkWidget *pixmap11;
  GtkWidget *button38;
  GtkWidget *button39;
  GtkWidget *button40;
  GtkWidget *button41;
  GtkWidget *button42;
  GtkWidget *button43;
  GtkWidget *button44;
  GtkWidget *vseparator4;
  GtkWidget *vseparator5;
  GtkWidget *vseparator6;
  GtkWidget *vseparator7;
  GtkWidget *hseparator2;
  GtkWidget *hseparator3;
  GtkWidget *About;
};

struct _GtkOdbcConfigClass
{
	GtkVBoxClass parent_class;
};

GtkType		gtk_odbcconfig_get_type		(void);
GtkWidget*	gtk_odbcconfig_new			(void);

#ifdef __cplusplus
}
#endif

#endif
