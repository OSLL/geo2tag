/*
 * Copyright 2012  Ivan Bezyazychnyy  ivan.bezyazychnyy@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

#include "inc/CreateAccountWidget.h"
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "defines.h"
#include "JsonUser.h"

CreateAccountWidget::CreateAccountWidget(QWidget *parent) :
QWidget(parent)
{
  m_loginEdit = new QLineEdit(this);
  m_passwordEdit = new QLineEdit(this);
  m_passwordEdit->setEchoMode(QLineEdit::Password);
  m_rePasswordEdit = new QLineEdit(this);
  m_rePasswordEdit->setEchoMode(QLineEdit::Password);
  m_registerButton = new QPushButton("Register", this);
  m_cancelButton = new QPushButton("Cancel", this);

  initGUI();

  m_addEventsChannelQuery = new AddChannelQuery(this);
  m_registerQuery = new RegisterUserQuery(this);
  m_loginQuery = new LoginQuery(DEFAULT_USER_NAME, DEFAULT_USER_PASSWORD, this);
  m_addChannelQuery = new AddChannelQuery(this);
  m_subscribeTracking = new SubscribeChannelQuery(this);
  m_subscribeEvents = new SubscribeChannelQuery(this);

  connect(m_registerButton, SIGNAL(clicked()),
    this, SLOT(onRegisterClicked()));
  connect(m_cancelButton, SIGNAL(clicked()),
    this, SLOT(onCancelClicked()));
  connect(m_registerQuery, SIGNAL(connected()),
    this, SLOT(onUserAdded()));
  connect(m_registerQuery, SIGNAL(errorOccured(QString)),
    this, SLOT(onError(QString)));
  connect(m_registerQuery, SIGNAL(networkErrorOccured(QString)),
    this, SLOT(onError(QString)), Qt::QueuedConnection);
  connect(m_loginQuery, SIGNAL(connected()),
    this, SLOT(onLoggedIn()));
  connect(m_loginQuery, SIGNAL(errorOccured(QString)),
    this, SLOT(onError(QString)));
  connect(m_loginQuery, SIGNAL(networkErrorOccured(QString)),
    this, SLOT(onError(QString)), Qt::QueuedConnection);
  connect(m_addChannelQuery, SIGNAL(channelAdded()),
    this, SLOT(onChannelAdded()));
  connect(m_addChannelQuery, SIGNAL(errorOccured(QString)),
    this, SLOT(onError(QString)));
  connect(m_addChannelQuery, SIGNAL(networkErrorOccured(QString)),
    this, SLOT(onError(QString)), Qt::QueuedConnection);
  connect(m_subscribeTracking, SIGNAL(subscribed()),
    this, SLOT(onTrackingSubscribed()));
  connect(m_subscribeTracking, SIGNAL(errorOccured(QString)),
    this, SLOT(onError(QString)));
  connect(m_subscribeTracking, SIGNAL(networkErrorOccured(QString)),
    this, SLOT(onError(QString)), Qt::QueuedConnection);
  connect(m_subscribeEvents, SIGNAL(subscribed()),
    this, SLOT(onEventsSubscribed()));
  connect(m_subscribeEvents, SIGNAL(errorOccured(QString)),
    this, SLOT(onError(QString)));
  connect(m_subscribeEvents, SIGNAL(networkErrorOccured(QString)),
    this, SLOT(onError(QString)), Qt::QueuedConnection);
}


void CreateAccountWidget::initGUI()
{
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(new QLabel("Login", this));
  mainLayout->addWidget(m_loginEdit);
  mainLayout->addWidget(new QLabel("Password", this));
  mainLayout->addWidget(m_passwordEdit);
  mainLayout->addWidget(new QLabel("Reenter password:", this));
  mainLayout->addWidget(m_rePasswordEdit);
  QHBoxLayout *btnsLayout = new QHBoxLayout();
  btnsLayout->addWidget(m_registerButton);
  btnsLayout->addWidget(m_cancelButton);
  mainLayout->addLayout(btnsLayout);
  mainLayout->addStretch();
  this->setLayout(mainLayout);
}


void CreateAccountWidget::onRegisterClicked()
{
  qDebug() << "register clicked";

  m_login = m_loginEdit->text();
  m_password = m_passwordEdit->text();
  QString rePassword = m_rePasswordEdit->text();

  if (m_login.length() == 0)
  {
    QMessageBox::information(this, "Geo Doctor Search","Login can't be empty");
    return;
  }
  if (m_password.length() == 0)
  {
    QMessageBox::information(this, "Geo Doctor Search","Password can't be empty");
    return;
  }
  if (rePassword != m_password)
  {
    QMessageBox::information(this, "Geo Doctor Search","Passwords should match");
    return;
  }

  m_registerQuery->setQuery("no email", m_login, m_password);
  m_registerQuery->setUrl(m_settings.getServerUrl() + ADD_USER_HTTP_URL);
  m_registerQuery->doRequest();

}


void CreateAccountWidget::onCancelClicked()
{
  qDebug() << "cancel clicked";
  emit finished();
}


void CreateAccountWidget::onError(QString error)
{
  qDebug() << "create account error: " << error;
  QMessageBox::information(this, "Error", error);
}


void CreateAccountWidget::onUserAdded()
{
  qDebug() << "User added";
  m_loginQuery->setQuery(m_login, m_password);
  m_loginQuery->setUrl(m_settings.getServerUrl());
  m_loginQuery->doRequest();
}


void CreateAccountWidget::onLoggedIn()
{
  qDebug() << "User logged in, token: " << m_loginQuery->getUser()->getToken();

  QSharedPointer<Channel> eventsChannel =
    QSharedPointer<Channel>(new Channel(EVENTS_CHANNEL, "Channel with events", ""));
  m_addEventsChannelQuery->setQuery(m_loginQuery->getUser(), eventsChannel);
  m_addEventsChannelQuery->setUrl(m_settings.getServerUrl());
  m_addEventsChannelQuery->doRequest();

  QSharedPointer<Channel> channel =
    QSharedPointer<Channel>(new Channel(m_login, m_login + "'s channel", ""));
  m_addChannelQuery->setQuery(m_loginQuery->getUser(), channel);
  m_addChannelQuery->setUrl(m_settings.getServerUrl());
  m_addChannelQuery->doRequest();
}


void CreateAccountWidget::onChannelAdded()
{
  qDebug() << "Channel added";
  m_subscribeTracking->setQuery(m_loginQuery->getUser(),
    m_addChannelQuery->getChannel());
  m_subscribeTracking->setUrl(m_settings.getServerUrl());
  m_subscribeTracking->doRequest();
}


void CreateAccountWidget::onTrackingSubscribed()
{
  qDebug() << "Tracking subscribed";
  QSharedPointer<Channel> eventsChannel =
    QSharedPointer<Channel>(new Channel(EVENTS_CHANNEL, ""));
  m_subscribeEvents->setQuery(m_loginQuery->getUser(),
    eventsChannel);
  m_subscribeEvents->setUrl(m_settings.getServerUrl());
  m_subscribeEvents->doRequest();
}


void CreateAccountWidget::onEventsSubscribed()
{
  qDebug() << "Events subscribed";
  emit finished();
}
