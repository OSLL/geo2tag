#include <QWebView>
#include <QApplication>
#include "vkAuth.h"
#include "vkRequest.h"
#include <QObject>

#define APP_ID "2481170"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  vkAuth *auth = new vkAuth (APP_ID,NULL);
  vkRequest *request = new vkRequest(NULL);
  QObject::connect(auth, SIGNAL(success(QString,QString,int)),request,SLOT(getProfile(QString,QString)));
  auth->show();
  return a.exec();

}
