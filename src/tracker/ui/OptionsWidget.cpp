/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*!
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "OptionsWidget.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QToolBox>
#include <QLabel>
#include <QFile>
//#include <QtXml/QDomDocument>
//#include <QtXml/QDomElement>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#ifndef Q_WS_SYMBIAN
#include <DaemonManager.h>
#endif
#include <QtNetwork/QNetworkProxy>
#include <QDir>
#include <QFileDialog>

#include "tracker.h"
#include "inc/ByteSpinBox.h"
#include "defines.h"

OptionsWidget::OptionsWidget(QString productName,QWidget *parent) :
QScrollArea(parent), m_productName(productName), m_settings(QSettings::SystemScope,"osll",m_productName)
{
  m_widget = new QWidget(this);

  QVBoxLayout * layout = new QVBoxLayout(m_widget);

  layout->addWidget(new QLabel("Login", m_widget));
  layout->addWidget(m_nameEdit = new QLineEdit(m_widget));
  layout->addWidget(new QLabel("Password", m_widget));
  layout->addWidget(m_passwordEdit = new QLineEdit(m_widget));
  layout->addWidget(m_passwordCheckBox = new QCheckBox("Show password", m_widget));
  layout->addWidget(new QLabel("Channel name", m_widget));
  layout->addWidget(m_channelEdit = new QLineEdit(m_widget));
  // NOTE if we use m_widget windget in tracker addition field for username will displayed
  if (m_productName == "tracker")
  {
    layout->addWidget(new QLabel("Name that will be displayed\non Observer map:", m_widget));
    layout->addWidget(m_visibleNameEdit = new QLineEdit());
  }

  m_passwordEdit->setEchoMode(QLineEdit::Password);
  m_passwordCheckBox->setChecked(false);

  layout->addWidget(new QLabel("Server", m_widget));
  layout->addWidget(m_serverUrlEdit = new QLineEdit(m_widget));
  layout->addWidget(new QLabel("Port", m_widget));
  layout->addWidget(m_serverPortEdit = new QSpinBox(m_widget));

  m_serverPortEdit->setMinimum(0);
  m_serverPortEdit->setMaximum(65535);

  layout->addWidget(new QLabel("Proxy type", m_widget));
  layout->addWidget(m_proxyType = new QComboBox(m_widget));
  layout->addWidget(new QLabel("Proxy host", m_widget));
  layout->addWidget(m_proxyHostEdit = new QLineEdit(m_widget));
  layout->addWidget(new QLabel("Proxy port", m_widget));
  layout->addWidget(m_proxyPortEdit = new QSpinBox(m_widget));

  m_proxyType->addItem("DefaultProxy", QNetworkProxy::DefaultProxy);
  m_proxyType->addItem("Socks5Proxy", QNetworkProxy::Socks5Proxy);
  m_proxyType->addItem("NoProxy", QNetworkProxy::NoProxy);
  m_proxyType->addItem("HttpProxy", QNetworkProxy::HttpProxy);
  m_proxyType->addItem("HttpCachingProxy", QNetworkProxy::HttpCachingProxy);
  m_proxyType->addItem("FtpCachingProxy", QNetworkProxy::FtpCachingProxy);
  m_proxyPortEdit->setMinimum(0);
  m_proxyPortEdit->setMaximum(65535);

  layout->addWidget(new QLabel("Cache type", m_widget));
  layout->addWidget(m_cacheType = new QComboBox(m_widget));
  layout->addWidget(new QLabel("Cache path", m_widget));
  layout->addWidget(m_cachePath = new QLineEdit(m_widget));
  layout->addWidget(m_cachePathButton = new QPushButton("Choose dir...", m_widget));
  layout->addWidget(new QLabel("Max cache size", m_widget));
  layout->addWidget(m_cacheMaxSize = new QSpinBox(m_widget));

  m_cacheType->addItem("None", 0);
  m_cacheType->addItem("Network cache", 1);
  m_cacheType->addItem("Disk cache", 2);
  m_cacheType->setCurrentIndex(-1);

  layout->addWidget(m_doneButton = new QPushButton("Done", m_widget));
  layout->addWidget(m_defaultButton = new QPushButton("Default settings", m_widget));
  layout->addWidget(m_cancelButton = new QPushButton("Cancel", m_widget));

  m_widget->setLayout(layout);
  this->setWidget(m_widget);
  this->setWidgetResizable(true);

  connect(m_doneButton, SIGNAL(clicked()), this, SLOT(onDoneClicked()));
  connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
  connect(m_defaultButton,SIGNAL(clicked()), this, SLOT(setDefaultSettings()));
  connect(m_proxyType, SIGNAL(currentIndexChanged(int)), this, SLOT(onProxyTypeChanged(int)));
  connect(m_cacheType, SIGNAL(currentIndexChanged(int)), this, SLOT(onCacheTypeChanged(int)));
  connect(m_cachePathButton, SIGNAL(clicked()), this, SLOT(onCachePathButtonClick()));
  connect(m_passwordCheckBox, SIGNAL(clicked(bool)), this, SLOT(onShowPasswordChecked(bool)));

  onProxyTypeChanged(m_proxyType->currentIndex());

  if( m_settings.value("magic").toString() != APP_MAGIC )
  {
    setDefaultSettings();
    createSettings();
  }

  initSettings();
  applyProxySettings();
}


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
  QNetworkProxy::ProxyType proxy_type = (QNetworkProxy::ProxyType)m_proxyType->itemData(index).toInt();
  bool enabled_flag = (proxy_type != QNetworkProxy::DefaultProxy && proxy_type != QNetworkProxy::NoProxy);
  m_proxyHostEdit->setEnabled(enabled_flag);
  m_proxyPortEdit->setEnabled(enabled_flag);
}


void OptionsWidget::onCacheTypeChanged(int index)
{
  m_cachePath->setEnabled( m_cacheType->itemData(index).toInt() != 0 );
  m_cachePathButton->setEnabled( m_cachePath->isEnabled() );
}


void OptionsWidget::onShowPasswordChecked(bool checked)
{
  if(checked)
    m_passwordEdit->setEchoMode(QLineEdit::Normal);
  else
    m_passwordEdit->setEchoMode(QLineEdit::Password);
}


void OptionsWidget::onCachePathButtonClick()
{
  qDebug() <<"click";
  QFileDialog fd(this);
  fd.setFileMode(QFileDialog::DirectoryOnly);
  fd.setOptions(QFileDialog::ShowDirsOnly);
  connect(&fd, SIGNAL(fileSelected(QString)), this, SLOT(onCachePathSelected(QString)));
  fd.exec();
}


void OptionsWidget::onCachePathSelected(QString path)
{
  m_cachePath->setText(path + "/");
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
  if (m_productName == "tracker")
  {
    m_visibleNameEdit->setText(m_settings.value("visibleName").toString());
  }
  m_nameEdit->setText(m_settings.value("user").toString());
  m_passwordEdit->setText(m_settings.value("password").toString());
  m_channelEdit->setText(m_settings.value("channel").toString());

  m_proxyType->setCurrentIndex(m_proxyType->findData(m_settings.value("proxy_type").toInt()));
  m_proxyHostEdit->setText(m_settings.value("proxy_host").toString());
  m_proxyPortEdit->setValue(m_settings.value("proxy_port").toInt());

  m_serverUrlEdit->setText(getServerUrl());
  m_serverPortEdit->setValue(getServerPort());

  m_cacheType->setCurrentIndex(m_cacheType->findData(m_settings.value("cache_type", 0).toInt()));
  m_cachePath->setText(m_settings.value("cache_path", QDir::homePath() + "/.geo2tag/uploaded_maps/").toString());
}


void OptionsWidget::createSettings()
{
  m_settings.setValue("channel", m_channelEdit->text());
  m_settings.setValue("user", m_nameEdit->text());
  m_settings.setValue("password", m_passwordEdit->text());

  QNetworkProxy::ProxyType proxy_type = (QNetworkProxy::ProxyType)m_proxyType->itemData(m_proxyType->currentIndex()).toInt();
  m_settings.setValue("proxy_type", proxy_type);
  if(proxy_type != QNetworkProxy::DefaultProxy && proxy_type != QNetworkProxy::NoProxy)
  {
    m_settings.setValue("proxy_host", m_proxyHostEdit->text());
    m_settings.setValue("proxy_port", m_proxyPortEdit->value());
  }
  else
  {
    m_settings.remove("proxy_host");
    m_settings.remove("proxy_port");
  }

  setServerUrl(m_serverUrlEdit->text());
  setServerPort(m_serverPortEdit->value());

  m_settings.setValue("cache_type", m_cacheType->itemData(m_cacheType->currentIndex()).toInt());
  if(m_cacheType->itemData(m_cacheType->currentIndex()).toInt() > 0)
    m_settings.setValue("cache_path", m_cachePath->text());
  else
  if (m_productName == "tracker")
  {
    m_settings.setValue("visibleName", m_visibleNameEdit->text());
  }
  m_settings.remove("cache_path");
  m_settings.setValue("magic", APP_MAGIC);
}


void OptionsWidget::setDefaultSettings()
{
  m_nameEdit->setText(DEFAULT_USER_NAME);
  m_passwordEdit->setText(DEFAULT_USER_PASSWORD);
  m_channelEdit->setText(DEFAULT_CHANNEL);
  m_proxyType->setCurrentIndex(0);
  m_proxyHostEdit->setText("");
  m_proxyPortEdit->setValue(0);
  m_serverUrlEdit->setText("http://tracklife.ru/");
  m_serverPortEdit->setValue(80);
  m_cacheType->setCurrentIndex(m_cacheType->findData(0));
  m_cachePath->setText(QDir::homePath() + "/.geo2tag/uploaded_maps/");
  if (m_productName == "tracker")
  {
    m_settings.setValue("visibleName","FRUCT_participant");
  }

}
