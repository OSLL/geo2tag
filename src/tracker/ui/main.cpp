#include <QApplication>
#include "TrackerGUI.h"

int main(int c, char **v)
{
  QApplication app(c,v);
  TrackerGUI gui;
  gui.showMaximized();
  return app.exec();
}
