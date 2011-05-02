#include <QApplication>
#include <QMenu>
#include <QVBoxLayout>
#include "LightMap.h"
#include "DataThread.h"
#include "MainWindow.h"
int main(int c, char **v)
{
  QApplication app(c,v);
  MainWindow window;
  window.show();
  return app.exec();
}
