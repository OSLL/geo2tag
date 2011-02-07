#include <QApplication>
#include "LightMap.h"
#include "DataThread.h"

int main(int c, char **v)
{
  QApplication app(c,v);
  LightMap map;
  map.setCenter( 60 , 30);
  map.show();
  DataThread thr(&map);
  thr.start();

  return app.exec();
}
