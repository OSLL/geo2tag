#include <QApplication>
#include "mainwindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc,argv);
	MainWindow wind;
	wind.show();
	return app.exec();
}
