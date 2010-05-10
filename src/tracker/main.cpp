#include <QApplication>
#include "mainwindow.h"

int main(int c, char **v)
{
  QApplication app(c,v);

  MainWindow mw;
  mw.show();

  return app.exec();
}
