#include <QSettings>
#include "defines.h"

const QString getServerUrl()
{
        QSettings settings("osll","libs");
        if (settings.value("server_url").toString().isEmpty()){
                settings.setValue("server_url",DEFAULT_SERVER);
        }
        return settings.value("server_url").toString();
}

void setServerUrl(const QString& serverUrl){
        QSettings settings("osll","libs");
	if (!serverUrl.isEmpty()){
	        settings.setValue("server_url",serverUrl);
	}else{
		settings.setValue("server_url",DEFAULT_SERVER);
	}
}

int getServerPort()
{
        QSettings settings("osll","libs");
        if (settings.value("server_port").toInt()==0){
                settings.setValue("server_port",DEFAULT_PORT);
        }
        return settings.value("server_port").toInt();
}

void setServerPort(const int& port){
        QSettings settings("osll","libs");
	if (port!=0){
	        settings.setValue("server_port",port);
	}else{
		settings.setValue("server_port",DEFAULT_PORT);
	}
}
