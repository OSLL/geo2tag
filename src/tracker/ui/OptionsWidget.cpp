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
#include <QtNetwork/QNetworkProxy>
#include "tracker.h"

void OptionsWidget::applyProxySettings()
{
    QNetworkProxy proxy;
    QNetworkProxy::ProxyType proxy_type;
    proxy_type = (QNetworkProxy::ProxyType) m_proxyType->itemData(m_proxyType->currentIndex()).value<int>();
    proxy.setType(proxy_type);
    proxy.setHostName(m_proxyHostEdit->text());
    proxy.setPort(m_proxyPortEdit->value());
    QNetworkProxy::setApplicationProxy(proxy);
}

OptionsWidget::OptionsWidget(QString productName,QWidget *parent) :
        QScrollArea(parent), m_productName(productName), m_settings("osll",m_productName)
{   
    m_widg=new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_widg);
    layout->addWidget(new QLabel("Login:"));
    layout->addWidget(m_nameEdit = new QLineEdit());
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(m_passwordEdit = new QLineEdit());
    layout->addWidget(m_passwordCheckBox = new QCheckBox("Show password"));
    layout->addWidget(new QLabel("Channel name:"));
    layout->addWidget(m_channelEdit = new QLineEdit());

    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordCheckBox->setChecked(false);

    layout->addWidget(new QLabel("Proxy type"));
    layout->addWidget(m_proxyType = new QComboBox());
    layout->addWidget(new QLabel("Host:"));
    layout->addWidget(m_proxyHostEdit = new QLineEdit());
    layout->addWidget(new QLabel("port:"));
    layout->addWidget(m_proxyPortEdit = new QSpinBox());
    layout->addWidget(m_serverUrlEdit = new QLineEdit());
    layout->addWidget(m_serverPortEdit = new QSpinBox());

    m_proxyType->addItem("DefaultProxy", QNetworkProxy::DefaultProxy);
    m_proxyType->addItem("Socks5Proxy", QNetworkProxy::Socks5Proxy);
    m_proxyType->addItem("NoProxy", QNetworkProxy::NoProxy);
    m_proxyType->addItem("HttpProxy", QNetworkProxy::HttpProxy);
    m_proxyType->addItem("HttpCachingProxy", QNetworkProxy::HttpCachingProxy);
    m_proxyType->addItem("FtpCachingProxy", QNetworkProxy::FtpCachingProxy);
    m_proxyPortEdit->setMinimum(0);
    m_proxyPortEdit->setMaximum(65535);
    m_serverPortEdit->setMinimum(0);
    m_serverPortEdit->setMaximum(65535);

    layout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_doneButton = new QPushButton("Done"));
    buttonLayout->addWidget(m_cancelButton = new QPushButton("Cancel"));
    layout->addLayout(buttonLayout);

    m_widg->setLayout(layout);
    this->setWidget(m_widg);
    this->setWidgetResizable(true);
    connect(m_doneButton, SIGNAL(clicked()), this, SLOT(onDoneClicked()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
    connect(m_proxyType, SIGNAL(currentIndexChanged(int)), this, SLOT(onProxyTypeChanged(int)));
    connect(m_passwordCheckBox, SIGNAL(clicked(bool)), this, SLOT(onShowPasswordChecked(bool)));

    onProxyTypeChanged(m_proxyType->currentIndex());

    initSettings();
    applyProxySettings();
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
    createSettings();

    applyProxySettings();

    emit done();
}

void OptionsWidget::onCancelClicked()
{
    initSettings();
    emit cancel();
}

void OptionsWidget::onProxyTypeChanged(int index)
{
    bool enabled_flag = index != 0 && index != 2;
    m_proxyHostEdit->setEnabled(enabled_flag);
    m_proxyPortEdit->setEnabled(enabled_flag);
}

void OptionsWidget::onShowPasswordChecked(bool checked)
{
    if(checked)
        m_passwordEdit->setEchoMode(QLineEdit::Normal);
    else
        m_passwordEdit->setEchoMode(QLineEdit::Password);
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
    m_proxyType->setCurrentIndex(m_proxyType->findData(m_settings.value("proxy_type").toInt()));
    m_proxyHostEdit->setText(m_settings.value("proxy_host").toString());
    m_proxyPortEdit->setValue(m_settings.value("proxy_port").toInt());
    m_serverUrlEdit->setText(getServerUrl());
    m_serverPortEdit->setValue(getServerPort());
}

void OptionsWidget::createSettings()
{
    m_settings.setValue("channel", m_channelEdit->text());
    m_settings.setValue("user", m_nameEdit->text());
    m_settings.setValue("password", m_passwordEdit->text());
    m_settings.setValue("proxy_type", m_proxyType->itemData(m_proxyType->currentIndex()).value<int>());
    m_settings.setValue("proxy_host", m_proxyHostEdit->text());
    setServerUrl(m_serverUrlEdit->text());
    setServerPort(m_serverPortEdit->value());
    m_settings.setValue("proxy_port", m_proxyPortEdit->value());
    m_settings.setValue("magic", APP_MAGIC);
}
