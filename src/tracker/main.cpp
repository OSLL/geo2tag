#include <QApplication>
#include "trackerdaemon.h"

#include "TrackerGUI.h"

int main(int c, char **v)
{
  QApplication app(c,v);
  //trackerDaemon daemon();
  TrackerGUI gui;
  gui.showMaximized();
  return app.exec();
}
