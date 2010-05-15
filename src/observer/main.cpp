#include <QApplication>
#include <QTextCodec>
#include "observer.h"
#include <curl/curl.h>
#include "MapWidget.h"

int main(int c, char **v)
{
    QApplication app(c,v);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    curl_global_init(CURL_GLOBAL_ALL);
    Observer obs;
    obs.show();

    return app.exec();
}
