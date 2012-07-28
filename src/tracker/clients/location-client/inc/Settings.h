#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>
#include <QMap>

class Settings
{
  private:
    QSettings instance;
    Settings();
    QMap<QString,QString> m_channels;

  public:
    static Settings& getInstance();
    QString getLogin();
    QString getPassword();
    QString getCustomName(const QString &channelName);
    void setCustomName(const QString &channelName, const QString &customName);
    bool getPermission();
    int getTimeInterval();
    void setLogin(const QString &login);
    void setPassword(const QString &password);
    void setPermission(bool permission);
    void setTimeInterval(int interval);
    bool isHavingAuthData();

};
#endif                                  // SETTINGS_H
