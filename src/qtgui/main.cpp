#include <QtGui/QApplication>
#include "mainwindowimpl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindowImpl w;
    w.show();
    return a.exec();
}
