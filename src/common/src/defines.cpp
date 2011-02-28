#include <QSettings>
#include "defines.h"

const QString getServerUrl()
{
        QSettings settings("osll","libs");
        if (settings.value("server_url").isNull()){
                settings.setValue("server_url",DEFAULT_SERVER);
        }
        return settings.value("server_url").toString();
}

void setServerUrl(const QString& serverUrl){
        QSettings settings("osll","libs");
        settings.setValue("server_url",serverUrl);
}

const int getServerPort()
{
        QSettings settings("osll","libs");
        if (settings.value("server_port").isNull()){
                settings.setValue("server_port",DEFAULT_PORT);
        }
        return settings.value("server_port").toInt();
}

void setServerPort(const int& port){
        QSettings settings("osll","libs");
        settings.setValue("server_port",port);
}
