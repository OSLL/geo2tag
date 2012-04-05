#include "inc/Settings.h"
#include "defines.h"

Settings::Settings() :
    m_settings("osll", "gds_test")
{
}

bool Settings::isSettingsEmpty()
{
    return m_settings.value(SETTINGS_STATUS, true).toBool();
}

void Settings::setDefaultSettings()
{
    setLogin(DEFAULT_USER_NAME);
    setPassword(DEFAULT_USER_PASSWORD);
    setRememberMe(DEFAULT_REMEMBER);
    setServerUrl(DEFAULT_URL);
    setAuthToken(DEFAULT_TOKEN);
    m_settings.setValue(SETTINGS_STATUS, false);
}

QString Settings::getLogin()
{
    return m_settings.value(LOGIN, "").toString();
}

void Settings::setLogin(const QString &login)
{
    m_settings.setValue(LOGIN, login);
}

QString Settings::getPassword()
{
    return m_settings.value(PASSWORD, "").toString();
}

void Settings::setPassword(const QString &password)
{
    m_settings.setValue(PASSWORD, password);
}

bool Settings::isRememberMe()
{
    return m_settings.value(REMEMBER, false).toBool();
}

void Settings::setRememberMe(bool status)
{
    m_settings.setValue(REMEMBER, status);
}

QString Settings::getAuthToken()
{
    return m_settings.value(AUTH_TOKEN, "").toString();
}

void Settings::setAuthToken(const QString &authToken)
{
    m_settings.setValue(AUTH_TOKEN, authToken);
}

QString Settings::getServerUrl()
{
    return m_settings.value(SERVER_URL, "").toString();
}

void Settings::setServerUrl(const QString &serverUrl)
{
    m_settings.setValue(SERVER_URL, serverUrl);
}
