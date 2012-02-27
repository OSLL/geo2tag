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

#include "inc/LoginWidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include "defines.h"


LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent)
{
    m_loginEdit = new QLineEdit(this);
    m_passwordEdit = new QLineEdit(this);
    m_rememberCheck = new QCheckBox("Remember me", this);
    m_signInButton = new QPushButton("Sign in", this);
    m_createAccountButton = new QPushButton("Create account", this);

    initGUI();

    m_loginQuery = new LoginQuery(DEFAULT_USER_NAME, DEFAULT_USER_PASSWORD, this);
    // TODO read settings!!!
    m_loginQuery->setUrl(LOGIN_HTTP_URL);

    connect(m_signInButton, SIGNAL(clicked()),
             this, SLOT(onSignInClicked()));
    connect(m_createAccountButton, SIGNAL(clicked()),
            this, SLOT(onCreateAccountClicked()));
    connect(m_loginQuery, SIGNAL(connected()),
            this, SLOT(onLoginConnected()));
    connect(m_loginQuery, SIGNAL(errorOccured(int)),
            this, SLOT(onLoginError(int)));
    connect(m_loginQuery, SIGNAL(errorOccured(QString)),
            this, SLOT(onLoginNetworkError(QString)));
}

void LoginWidget::initGUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(new QLabel("Login", this));
    mainLayout->addWidget(m_loginEdit);
    mainLayout->addWidget(new QLabel("Password"));
    mainLayout->addWidget(m_passwordEdit);
    mainLayout->addWidget(m_rememberCheck);
    mainLayout->addWidget(m_signInButton);
    mainLayout->addWidget(m_createAccountButton);
    mainLayout->addStretch();
    this->setLayout(mainLayout);

    m_passwordEdit->setEchoMode(QLineEdit::Password);
}

void LoginWidget::onSignInClicked()
{
    qDebug() << "signing in";
    QString login = m_loginEdit->text();
    QString password = m_passwordEdit->text();
    m_loginQuery->setQuery(login, password);
    m_loginQuery->doRequest();
}

void LoginWidget::onCreateAccountClicked()
{
    qDebug() << "creating account";
}

void LoginWidget::onLoginConnected()
{
    qDebug() << "logged in";
    QString auth_token = m_loginQuery->getUser()->getToken();
    qDebug() << "token: " << auth_token;
    emit signedIn(auth_token);
}

void LoginWidget::onLoginError(int errno)
{
    qDebug() << "login error, errno = " << errno;
}

void LoginWidget::onLoginNetworkError(QString error)
{
    qDebug() << "Network error: " << error;
}


