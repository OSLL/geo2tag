#include <QSettings>
#include <QDebug>
#include <syslog.h>
#include "defines.h"

QString getServerUrl()
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  //      if (settings.value("server_url").toString().isEmpty()){
  //              settings.setValue("server_url",DEFAULT_SERVER);
  //    }
  //	qDebug() << "server url " << settings.value("server_url").toString();
  //	syslog(LOG_INFO,"getServerUrl() %s",settings.value("server_url").toString().toStdString().c_str());
  return settings.value("server_url").toString();

}


void setServerUrl(QString serverUrl)
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  //	if (!serverUrl.isEmpty()){
  settings.setValue("server_url",serverUrl);
  //		settings.sync();
  //	}else{
  //		settings.setValue("server_url",DEFAULT_SERVER);
  //	}
  //	syslog(LOG_INFO,"Setting server url by %s, result %s",serverUrl.toStdString().c_str(),settings.value("server_url").toString().toStdString().c_str());
  //	qDebug() << "Setting server url " << settings.value("server_url").toString();
}


int getServerPort()
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  //      if (settings.value("server_port").toInt()==0){
  //          settings.setValue("server_port",DEFAULT_PORT);
  //    }
  //	qDebug() << "server port " << settings.value("server_port").toInt();
  //	syslog(LOG_INFO,"getServerPort() %i",settings.value("server_port").toInt());
  return settings.value("server_port").toInt();
}


void setServerPort(int port)
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  //	if (port!=0){
  settings.setValue("server_port",port);
  //		settings.sync();
  //	}else{
  //		settings.setValue("server_port",DEFAULT_PORT);
  //	}
  //	qDebug() << "Setting server port " << settings.value("server_port").toInt();
}
