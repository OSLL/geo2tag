#include <QApplication>
#include "observer.h"
#include <curl/curl.h>
#include "MapWidget.h"

int main(int c, char **v)
{
    QApplication app(c,v);
    curl_global_init(CURL_GLOBAL_ALL);
    Observer obs;
    obs.show();

    return app.exec();
}
