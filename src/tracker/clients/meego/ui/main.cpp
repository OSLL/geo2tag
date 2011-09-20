#include <QApplication>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc,argv);
	MainWindow wind;
	wind.show();
        qDebug() << "Started";
	return app.exec();
}
