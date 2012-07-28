#include <QSettings>
#include <QDebug>
#include "common/inc/defines.h"
#include "common/inc/SettingsStorage.h"

QString getServerUrl()
{
  SettingsStorage storage(SETTINGS_STORAGE_FILENAME);

  QString serverUrl = storage.getValue("General_Settings/server_url", QVariant(DEFAULT_SERVER)).toString();

  return serverUrl;
}


void setServerUrl(QString serverUrl)
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  settings.setValue("server_url",serverUrl);
}


int getServerPort()
{
  SettingsStorage storage(SETTINGS_STORAGE_FILENAME);

  int serverPort = storage.getValue("General_Settings/server_port", QVariant(DEFAULT_PORT)).toInt();

  return serverPort;
}


void setServerPort(int port)
{
  QSettings settings(QSettings::SystemScope,"osll","libs");
  settings.setValue("server_port",port);
}
