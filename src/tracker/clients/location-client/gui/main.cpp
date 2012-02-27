#include <QApplication>
#include <MainWindow.h>

int main(int argc, char *argv[])
{
  QApplication application(argc, argv);
  MainWindow win;

  win.showMaximized();

  QObject::connect(&win, SIGNAL(destroyed()), qApp, SLOT(quit()));

  return application.exec();
}
