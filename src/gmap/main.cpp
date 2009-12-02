#include "GetGoogleMarks.h"
//#include "SetGoogleMarks.h"

#include <QtCore/QCoreApplication>
#include <QEventLoop>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GetGoogleMarks h(QString("http://maps.google.com/maps/ms?ie=UTF8&hl=en&msa=0&msid=117415784199486045584.000475bce25163bc75220&z=19"),Position("30.348080,59.986595"),6700.0,QStringList("55555")); 
    return a.exec();
}
