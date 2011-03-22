#include <QSettings>
#include <QDebug>
#include "defines.h"

QString getServerUrl()
{
        QSettings settings("osll","libs");
        if (settings.value("server_url").toString().isEmpty()){
                settings.setValue("server_url",DEFAULT_SERVER);
        }
	qDebug() << "server url " << settings.value("server_url").toString();
        return settings.value("server_url").toString();
}

void setServerUrl(const QString& serverUrl)
{
        QSettings settings("osll","libs");
	if (!serverUrl.isEmpty()){
	        settings.setValue("server_url",serverUrl);
	}else{
		settings.setValue("server_url",DEFAULT_SERVER);
	}
	qDebug() << "Setting server url " << settings.value("server_url").toString();
}

int getServerPort()
{
        QSettings settings("osll","libs");
        if (settings.value("server_port").toInt()==0){
                settings.setValue("server_port",DEFAULT_PORT);
        }
	qDebug() << "server port " << settings.value("server_port").toInt();
        return settings.value("server_port").toInt();
}

void setServerPort(int port)
{
        QSettings settings("osll","libs");
	if (port!=0){
	        settings.setValue("server_port",port);
	}else{
		settings.setValue("server_port",DEFAULT_PORT);
	}
	qDebug() << "Setting server port " << settings.value("server_port").toInt();
}
