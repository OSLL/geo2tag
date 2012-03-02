#include "mainwindow.h"

#include <QtGui/QApplication>
#include <QDebug>
#include <QSettings>

int main(int argc, char *argv[])
{
#ifdef Q_OS_SYMBIAN
     // Use QDesktopServices:storageLocation as QApplication is not yet created
     QSettings::setPath(
         QSettings::NativeFormat, QSettings::UserScope,
         QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/settings");
 #endif

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
    mainWindow.showExpanded();

    return app.exec();
}
