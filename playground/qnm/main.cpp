#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QDebug>
#include <QThread>
#include <QTest>
#include "requestobject.h"

int main(int c, char **v)
{
    QApplication app(c,v);

    RequestObject ro;
    ro.requestSessionToken("mark","test");

    return app.exec();
}
