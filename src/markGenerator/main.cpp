#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include "GeneratorDaemon.h"

int main(int c, char **v)
{
  QCoreApplication app(c,v);
  qDebug() << "Mark generator  started";
  if (c==2){ 
	  Generator Daemon daemon(argv[1]);
	  daemon.start();
  }else {
	  qDebug() <<  "now filename specified";
  }
  QObject::connect(&app,SIGNAL(aboutToQuit()),&daemon,SLOT(stop()));
  return app.exec();
}
