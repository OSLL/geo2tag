#include <QApplication>
#include "MainWindow.h"

int main(int c, char **v)
{
  QApplication app(c,v);
  MainWindow main;
  main.show();
  return app.exec();
}
