#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "tracker.h"
#include "ui_formCreateChannel.h"


class SettingsDialog : public QDialog
{
  Q_OBJECT;

  Ui::SettingsForm m_ui;
  bool  m_isCreateNewChannel; // if true, then we shoul create new channel

  Settings m_settings;

public:
    SettingsDialog(int type, QWidget *parent);
    Settings getData();

protected:
    bool createUser();
    bool loginUser();
    bool createChannel();
    bool subscribeChannel();
    void reflectSettings();

public slots:
    void accept();
    void reject();

    void onAddUserResponse(QString status, QString auth_token);
    void onLoginResponse(QString status, QString auth_token);
    void onApplyChannelResponse(QString status);
    void onSubscribeChannelResponse(QString status);
};

#endif // SETTINGSDIALOG_H
