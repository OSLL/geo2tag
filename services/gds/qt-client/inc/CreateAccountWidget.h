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

#ifndef CREATEACCOUNTWIDGET_H
#define CREATEACCOUNTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "LoginQuery.h"
#include "RegisterUserQuery.h"
#include "Settings.h"
#include "AddChannelQuery.h"
#include "SubscribeChannelQuery.h"

class CreateAccountWidget : public QWidget
{
  Q_OBJECT

    QString m_login;
  QString m_password;
  QString m_authToken;

  Settings m_settings;
  AddChannelQuery *m_addEventsChannelQuery;
  RegisterUserQuery *m_registerQuery;
  LoginQuery *m_loginQuery;
  AddChannelQuery *m_addChannelQuery;
  SubscribeChannelQuery *m_subscribeTracking;
  SubscribeChannelQuery *m_subscribeEvents;

  // GUI
  QLineEdit *m_loginEdit;
  QLineEdit *m_passwordEdit;
  QLineEdit *m_rePasswordEdit;
  QPushButton *m_registerButton;
  QPushButton *m_cancelButton;

  public:
    explicit CreateAccountWidget(QWidget *parent = 0);

    signals:
    void finished();

  public slots:
    void onRegisterClicked();
    void onCancelClicked();
    void onUserAdded();
    void onLoggedIn();
    void onChannelAdded();
    void onTrackingSubscribed();
    void onEventsSubscribed();

    void onError(QString error);

  private:
    void initGUI();

};
#endif                                  // CREATEACCOUNTWIDGET_H
