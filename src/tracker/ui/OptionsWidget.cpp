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
#include "tracker.h"

OptionsWidget::OptionsWidget(QString productName,QWidget *parent) :
        QWidget(parent), m_productName(productName), m_settings("osll",m_productName)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Login:"));
    layout->addWidget(m_nameEdit = new QLineEdit());
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(m_passwordEdit = new QLineEdit());
    layout->addWidget(new QLabel("Channel name:"));
    layout->addWidget(m_channelEdit = new QLineEdit());
    layout->addStretch();
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_doneButton = new QPushButton("Done"));
    buttonLayout->addWidget(m_cancelButton = new QPushButton("Cancel"));
    layout->addLayout(buttonLayout);
    this->setLayout(layout);

    connect(m_doneButton, SIGNAL(clicked()), this, SLOT(onDoneClicked()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

    initSettings();

}

QString OptionsWidget::name()
{
    return m_settings.value("user").toString();
}

QString OptionsWidget::password()
{
    return m_settings.value("password").toString();
}

QString OptionsWidget::channel()
{
    return m_settings.value("channel").toString();
}

void OptionsWidget::onDoneClicked()
{
    m_settings.setValue("user", m_nameEdit->text());
    m_settings.setValue("password", m_passwordEdit->text());
    m_settings.setValue("channel", m_channelEdit->text());
    emit done();
}

void OptionsWidget::onCancelClicked()
{
    initSettings();
    emit cancel();
}

void OptionsWidget::initSettings()
{
    if( m_settings.value("magic").toString() == APP_MAGIC )
    {
        qDebug() << "magic = " << m_settings.value("magic").toString();
        readSettings();
    }
    else
    {
        createSettings();
    }
}

void OptionsWidget::readSettings()
{
    QSettings settings("osll",m_productName);
    m_nameEdit->setText(m_settings.value("user").toString());
    m_passwordEdit->setText(m_settings.value("password").toString());
    m_channelEdit->setText(m_settings.value("channel").toString());
}

void OptionsWidget::createSettings()
{
    m_settings.setValue("channel",m_channelEdit->text());
    m_settings.setValue("user",m_nameEdit->text());
    m_settings.setValue("password",m_passwordEdit->text());
    m_settings.setValue("magic",APP_MAGIC);
}
