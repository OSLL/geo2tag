#include <QWebView>
#include <QApplication>
#include "vkAuth.h"

#define APP_ID "2481170"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  /*  QWebView *view = new QWebView(NULL);
    view->load(QUrl("http://qt.nokia.com/"));
    view->show();*/
  vkAuth *auth = new vkAuth (APP_ID,NULL);
  auth->show();
  return a.exec();
}
