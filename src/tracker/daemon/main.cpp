#include <QCoreApplication>
#include "trackerdaemon.h"


int main(int c, char **v)
{
  QCoreApplication app(c,v);
  trackerDaemon daemon();
  return app.exec();
}
