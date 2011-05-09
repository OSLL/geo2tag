#include <QSettings>
#include <QDebug>
#include <syslog.h>
#include "defines.h"

QString getServerUrl()
{
<<<<<<< HEAD
        QSettings settings(QSettings::SystemScope,"osll","libs");
        if (settings.value("server_url").toString().isEmpty())
	{
		return DEFAULT_SERVER;
	}
        return settings.value("server_url").toString();
=======
  QSettings settings(QSettings::SystemScope,"osll","libs");
  //      if (settings.value("server_url").toString().isEmpty()){
  //              settings.setValue("server_url",DEFAULT_SERVER);
  //    }
  //	qDebug() << "server url " << settings.value("server_url").toString();
  //	syslog(LOG_INFO,"getServerUrl() %s",settings.value("server_url").toString().toStdString().c_str());
  return settings.value("server_url").toString();
>>>>>>> 2174_huge_logs

}


void setServerUrl(QString serverUrl)
{
<<<<<<< HEAD
        QSettings settings(QSettings::SystemScope,"osll","libs");
        settings.setValue("server_url",serverUrl);
=======
  QSettings settings(QSettings::SystemScope,"osll","libs");
  //	if (!serverUrl.isEmpty()){
  settings.setValue("server_url",serverUrl);
  //		settings.sync();
  //	}else{
  //		settings.setValue("server_url",DEFAULT_SERVER);
  //	}
  //	syslog(LOG_INFO,"Setting server url by %s, result %s",serverUrl.toStdString().c_str(),settings.value("server_url").toString().toStdString().c_str());
  //	qDebug() << "Setting server url " << settings.value("server_url").toString();
>>>>>>> 2174_huge_logs
}


int getServerPort()
{
<<<<<<< HEAD
        QSettings settings(QSettings::SystemScope,"osll","libs");
        if (settings.value("server_port").toInt()==0)
	{
                return DEFAULT_PORT;
        }
        return settings.value("server_port").toInt();
=======
  QSettings settings(QSettings::SystemScope,"osll","libs");
  //      if (settings.value("server_port").toInt()==0){
  //          settings.setValue("server_port",DEFAULT_PORT);
  //    }
  //	qDebug() << "server port " << settings.value("server_port").toInt();
  //	syslog(LOG_INFO,"getServerPort() %i",settings.value("server_port").toInt());
  return settings.value("server_port").toInt();
>>>>>>> 2174_huge_logs
}


void setServerPort(int port)
{
<<<<<<< HEAD
        QSettings settings(QSettings::SystemScope,"osll","libs");
        settings.setValue("server_port",port);
=======
  QSettings settings(QSettings::SystemScope,"osll","libs");
  //	if (port!=0){
  settings.setValue("server_port",port);
  //		settings.sync();
  //	}else{
  //		settings.setValue("server_port",DEFAULT_PORT);
  //	}
  //	qDebug() << "Setting server port " << settings.value("server_port").toInt();
>>>>>>> 2174_huge_logs
}
