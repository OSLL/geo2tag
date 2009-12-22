/*  */
/*  */



#include <QApplication>
#include "MainWindow.h" 
#include <curl/curl.h>
#include "DbSession.h"

int main(int c, char **v)
{
  common::DbSession::getInstance().loadData();
	QApplication 	app(c,v);

	curl_global_init(CURL_GLOBAL_ALL);
	
  GUI::MainWindow window;

#ifdef DESKTOP_STYLE
	window.show();
#else  
	window.showMaximized();
#endif  

	app.exec();
	curl_global_cleanup();
  common::DbSession::getInstance().saveData();

	return 0;

}

