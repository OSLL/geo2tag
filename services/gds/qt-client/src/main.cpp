#include "mainwindow.h"

#include <QtGui/QApplication>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QDesktopServices>

int main(int argc, char *argv[])
{
  #ifdef Q_OS_SYMBIAN
  // Use QDesktopServices:storageLocation as QApplication is not yet created
  QSettings::setPath(
    QSettings::NativeFormat, QSettings::UserScope,
    QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/settings");
  #endif

  QApplication app(argc, argv);
  app.setApplicationName("GeoDoctorSearch");

  QFile file(":/data/SliderStyleSheet.qss");
  if (!file.open(QFile::ReadOnly))
  {
    qDebug() << "can't find style file";
    return 0;
  }
  QString StyleSheet = QLatin1String(file.readAll());

  qApp->setStyleSheet(StyleSheet);

  MainWindow mainWindow;
  mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
  mainWindow.showExpanded();

  return app.exec();
}
