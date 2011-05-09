#include <QApplication>
#include <QDebug>
#include <QColor>
#include "testwidget.h"

int main(int c, char **v)
{
  QApplication app(c,v);
  TestWidget tw(NULL);

  tw.show();

  return app.exec();
}
