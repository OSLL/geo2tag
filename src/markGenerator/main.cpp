#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include "GeneratorDaemon.h"

int main(int c, char **v)
{
  QCoreApplication app(c,v);
  qDebug() << "Mark generator  started";
  if (c==2){ 
	  GeneratorDaemon * daemon=new GeneratorDaemon(v[1]);
	  daemon->start();
  }else {
	  qDebug() <<  "no filename specified";
	  return 0;
  }
//  QObject::connect(&app,SIGNAL(aboutToQuit()),&daemon,SLOT(stop()));
  return app.exec();
}
