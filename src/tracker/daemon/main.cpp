#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include "TrackerDaemon.h"

void myMessageHandler(QtMsgType type, const char *msg)
{
	QString txt;
	switch (type) {
		case QtDebugMsg:
			txt = QString("Debug: %1").arg(msg);
			break;
		case QtWarningMsg:
			txt = QString("Warning: %1").arg(msg);
			break;
		case QtCriticalMsg:
			txt = QString("Critical: %1").arg(msg);
			break;
		case QtFatalMsg:
			txt = QString("Fatal: %1").arg(msg);
			abort();
}
    QFile outFile("/tmp/wikigpsTracker.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << QTime::currentTime().toString() << ":" << txt << endl;
}

int main(int c, char **v)
{
  QCoreApplication app(c,v);
  qInstallMsgHandler(myMessageHandler);
  qDebug() << "Application is started";
  TrackerDaemon daemon;
//  daemon.start();
  daemon.run();
//  QObject::connect(&app,SIGNAL(aboutToQuit()),&daemon,SLOT(stop()));
  return app.exec();
}
