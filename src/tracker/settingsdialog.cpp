#include "settingsdialog.h"

#include <QMessageBox>
#include <QDebug>

#include "AddUserQuery.h"
#include "ApplyChannelQuery.h"
#include "SubscribeChannelQuery.h"
#include "LoginQuery.h"

SettingsDialog::SettingsDialog(int type, QWidget *parent) : QDialog(parent), m_isCreateNewChannel(type == 0)
{
  m_ui.setupUi(this);
  

  setWindowTitle("Setup configuration");
  //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  this->showMaximized();
  
  if( m_isCreateNewChannel) // create new channel
  {
    m_ui.channelKey->setEnabled(false);
  }

  connect(m_ui.okButton, SIGNAL(pressed()), this, SLOT(accept()));
  connect(m_ui.cancelButton, SIGNAL(pressed()), this, SLOT(reject()));
}

Settings SettingsDialog::getData()
{
  reflectSettings();
  return m_settings;
}

void SettingsDialog::reflectSettings()
{
  m_settings.channel= m_ui.channelName->text();
  m_settings.key = m_ui.channelKey->text();
  m_settings.user = m_ui.userName->text();
  m_settings.passw = m_ui.password->text();
  m_settings.initialized = false;
}

void SettingsDialog::reject()
{
  qDebug() << "reject";
  QDialog::reject();
}

void SettingsDialog::accept()
{
  reflectSettings();

  if( m_ui.newUser->isChecked())
      createUser();
  else
      loginUser();
}



bool SettingsDialog::createUser()
{
    AddUserQuery *addUserQuery = new AddUserQuery(m_settings.user, m_settings.passw, this);
    connect(addUserQuery, SIGNAL(responseReceived(QString,QString)), this, SLOT(onAddUserResponse(QString,QString)));
    addUserQuery->doRequest();
    return true;
}

void SettingsDialog::onAddUserResponse(QString status, QString auth_token)
{
    if (status == QString("Ok"))
    {
        m_settings.auth_token = auth_token;
        if(m_isCreateNewChannel)
        {
            createChannel();
        }
        else
        {
            subscribeChannel();
        }
    }
    else
    {
        QMessageBox::critical(this,"Error", QString("Cannot create user '%1', passwd '%2'")
                              .arg(m_settings.user)
                              .arg(m_settings.passw));
    }
}

bool SettingsDialog::loginUser()
{
    GUI::LoginQuery *loginQuery = new GUI::LoginQuery(m_settings.user,
                                                      m_settings.passw,
                                                      this);
    connect(loginQuery, SIGNAL(responseReceived(QString,QString)), this, SLOT(onLoginResponse(QString,QString)));
    loginQuery->doRequest();

    return true;
}

void SettingsDialog::onLoginResponse(QString status, QString auth_token)
{
    if (status == QString("Ok"))
    {
        m_settings.auth_token = auth_token;
        if(m_isCreateNewChannel)
        {
            createChannel();
        }
        else
        {
            subscribeChannel();
        }
    }
    else
    {
        QMessageBox::critical(this,"Error", QString("Cannot login user '%1', passwd '%2'")
                              .arg(m_settings.user)
                              .arg(m_settings.passw));
    }
}

bool SettingsDialog::createChannel()
{
    ApplyChannelQuery *applyChannelQuery = new ApplyChannelQuery(m_settings.auth_token, m_settings.channel,
                                                                 QString(""), QString(""), 100, this);
    connect(applyChannelQuery, SIGNAL(responseReceived(QString)), this, SLOT(onApplyChannelResponse(QString)));
    applyChannelQuery->doRequest();

    return true;
}

void SettingsDialog::onApplyChannelResponse(QString status)
{
    if (status == QString("Ok"))
    {
        subscribeChannel();
    }
    else
    {
        QMessageBox::critical(this,"Error", QString("Cannot create channel '%1'")
                              .arg(m_settings.channel));
    }
}

bool SettingsDialog::subscribeChannel()
{
    GUI::SubscribeChannelQuery *subscribeChannelQuery = new GUI::SubscribeChannelQuery(m_settings.auth_token,
                                                                                       m_settings.channel,
                                                                                       this);
    connect(subscribeChannelQuery, SIGNAL(responseReceived(QString)), this, SLOT(onSubscribeChannelResponse(QString)));
    subscribeChannelQuery->doRequest();

    return true;
}

void SettingsDialog::onSubscribeChannelResponse(QString status)
{
    if (status == QString("Ok"))
    {
        subscribeChannel();
        QDialog::accept();
    }
    else
    {
        QMessageBox::critical(this,"Error", QString("Cannot subscribe to channel '%1' with key '%2'")
                              .arg(m_settings.channel)
                              .arg(m_settings.key));
    }
}
