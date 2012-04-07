#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

#define SETTINGS_STATUS "settings_status"
#define LOGIN "login"
#define PASSWORD "password"
#define REMEMBER "remember"
#define SERVER_URL "server_url"
#define AUTH_TOKEN "auth_token"
#define DESCRIPTION "description"

class Settings
{

    QSettings m_settings;

public:
    Settings();

    bool isSettingsEmpty();
    void setDefaultSettings();

    QString getLogin();
    void setLogin(const QString& login);
    QString getPassword();
    void setPassword(const QString& password);
    bool isRememberMe();
    void setRememberMe(bool status);
    QString getAuthToken();
    void setAuthToken(const QString& password);

    QString getDescription();
    void setDescription(const QString& description);
    QString getServerUrl();
    void setServerUrl(const QString& serverUrl);

};

#endif // SETTINGS_H
