#include <QSettings>
#include "defines.h"

const QString& getServerUrl()
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
