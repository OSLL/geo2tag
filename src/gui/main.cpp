/*  */
/*  */



#include <QApplication>
#include "MapForm.h" 
#include <curl/curl.h>
#include "defs.h"

int main(int c, char **v)
{
	QApplication 	app(c,v);

//  getMarks().load();

	curl_global_init(CURL_GLOBAL_ALL);
	
	QDialog * dialog = new GUI::MapForm (NULL);

	//dialog->show();
	dialog->showMaximized();

	app.exec();
	curl_global_cleanup();

//  getMarks().save();

	return 0;

}

