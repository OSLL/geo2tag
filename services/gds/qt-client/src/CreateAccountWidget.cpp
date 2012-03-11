#include "inc/CreateAccountWidget.h"
#include <QDebug>
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

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

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

    connect(m_registerButton, SIGNAL(clicked()),
            this, SLOT(onRegisterClicked()));
    connect(m_cancelButton, SIGNAL(clicked()),
            this, SLOT(onCancelClicked()));
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
    this->setLayout(mainLayout);
}

void CreateAccountWidget::onRegisterClicked()
{
    qDebug() << "register clicked";

    m_login = m_loginEdit->text();
    m_password = m_passwordEdit->text();
    QString rePassword = m_rePasswordEdit->text();

    if (m_login.length() == 0) {
        QMessageBox::information(this, "Geo Doctor Search","Login can't be empty");
        return;
    }
    if (m_password.length() == 0) {
        QMessageBox::information(this, "Geo Doctor Search","Password can't be empty");
        return;
    }
    if (rePassword != m_password) {
        QMessageBox::information(this, "Geo Doctor Search","Passwords should match");
        return;
    }

    // do add user request

    emit finished();
}

void CreateAccountWidget::onCancelClicked()
{
    qDebug() << "register clicked";
    emit finished();
}

void CreateAccountWidget::onLogicError(int errno)
{
    qDebug() << "login error, errno = " << errno;
    QMessageBox::information(this, "Geo Doctor Search","Internal error: code " + errno);
}

void CreateAccountWidget::onNetworkError(QString error)
{
    qDebug() << "Network error: " << error;
    QMessageBox::information(this, "Geo Doctor Search","Network error");
}
