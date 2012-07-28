#include "Settings.h"

Settings::Settings(): instance("OSLL","ThereAndHere")
{

}


Settings& Settings::getInstance()
{
  static Settings instance;
  return instance;
}


QString Settings::getLogin()
{
  return instance.value("login", "unknown").toString();
}


QString Settings::getPassword()
{
  return instance.value("password", "unknown").toString();
}


bool Settings::getPermission()
{
  return instance.value("permission",true).toBool();
}


int Settings::getTimeInterval()
{
  return instance.value("timeinterval",60).toInt();
}


QString Settings::getCustomName(const QString &channelName)
{
  return instance.value(channelName,channelName).toString();
}


void Settings::setCustomName(const QString &channelName, const QString &customName)
{
  instance.setValue(channelName, customName);
}


void Settings::setLogin(const QString &login)
{
  instance.setValue("login",login);
}


void Settings::setPassword(const QString &password)
{
  instance.setValue("password", password);
}


void Settings::setPermission(bool permission)
{
  instance.setValue("permission", permission);
}


void Settings::setTimeInterval(int interval)
{
  instance.setValue("timeinterval", interval);
}


bool Settings::isHavingAuthData()
{
  return ((getLogin()!="unknown")&&(getPassword()!="unknown"));
}
