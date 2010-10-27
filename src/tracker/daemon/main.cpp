#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include "TrackerDaemon.h"


int main(int c, char **v)
{
  QCoreApplication app(c,v);
  qDebug() << "Application is started";
  TrackerDaemon daemon;
  QObject::connect(&app,SIGNAL(aboutToQuit()),&daemon,SLOT(stop()));
  return app.exec();
}
