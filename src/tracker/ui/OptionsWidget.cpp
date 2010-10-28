#include "OptionsWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <DaemonManager.h>

OptionsWidget::OptionsWidget(QWidget *parent) :
        QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Login:"));
    layout->addWidget(m_nameEdit = new QLineEdit());
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(m_passwordEdit = new QLineEdit());
    layout->addWidget(new QLabel("Channel name:"));
    layout->addWidget(m_channelEdit = new QLineEdit());
    layout->addStretch();
    layout->addWidget(m_doneButton = new QPushButton("Done"));
    this->setLayout(layout);

    connect(m_doneButton, SIGNAL(clicked()), this, SLOT(onDoneClicked()));

    initSettings();

}

QString OptionsWidget::name()
{
    return m_settings.user;
}

QString OptionsWidget::password()
{
    return m_settings.passw;
}

QString OptionsWidget::channel()
{
    return m_settings.channel;
}

void OptionsWidget::onDoneClicked()
{
    int changed = 0;

    if (m_nameEdit->text() == "")
    {
        QMessageBox::information(this, "Tracker", "User's name can be empy");
        return;
    }
    else if (m_passwordEdit->text() == "")
    {
        QMessageBox::information(this, "Tracker", "User's password can be empy");
        return;
    }
    else if (m_settings.channel == "")
    {
        QMessageBox::information(this, "Tracker", "Channel's name can be empy");
        return;
    }

    if (m_settings.user != m_nameEdit->text())
    {
        changed = 1;
        m_settings.user = m_nameEdit->text();
        qDebug() << "new name: " << m_settings.user;
    }
    if (m_settings.passw != m_passwordEdit->text())
    {
        changed = 1;
        m_settings.passw = m_passwordEdit->text();
        qDebug() << "new password: " << m_settings.passw;
    }
    if (m_settings.channel != m_channelEdit->text())
    {
        changed = 1;
        m_settings.channel = m_channelEdit->text();
        qDebug() << "new channel: " << m_settings.channel;
    }

    if (changed)
    {

        // 1. stop daemon
        DaemonManager::getInstance().start();

        // 2. update qsettings
        createSettings();

        // 3. start daemon
        DaemonManager::getInstance().stop();

        // 4. check status
    }

    emit this->done();
}

void OptionsWidget::initSettings()
{
    QSettings settings("osll","tracker");

    if( settings.value("magic").toString() == APP_MAGIC )
    {
        qDebug() << "magic = " << settings.value("magic").toString();
        emit readSettings();
    }
    else
    {
        emit createSettings();
    }
}

void OptionsWidget::readSettings()
{
    QSettings settings("osll","tracker");
    m_settings.channel = settings.value("channel").toString();
    m_settings.key = settings.value("key").toString();
    m_settings.user = settings.value("user").toString();
    m_settings.passw = settings.value("passwd").toString();
    m_settings.auth_token = settings.value("auth_token").toString();
    m_settings.initialized = true;

    m_nameEdit->setText(m_settings.user);
    m_passwordEdit->setText(m_settings.passw);
    m_channelEdit->setText(m_settings.channel);

    m_backupSettings = m_settings;
}

void OptionsWidget::createSettings()
{
    QSettings settings("osll","tracker");
    settings.setValue("channel",m_settings.channel);
    settings.setValue("key",m_settings.key);
    settings.setValue("user",m_settings.user);
    settings.setValue("passwd",m_settings.passw);
    settings.setValue("auth_token",m_settings.auth_token);
    settings.setValue("magic",APP_MAGIC);
    m_settings.initialized = true;

    m_backupSettings = m_settings;
}
