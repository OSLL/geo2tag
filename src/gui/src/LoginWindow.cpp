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
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <QDebug>
#include "LoginWindow.h"
#include <QMessageBox>
namespace GUI
{

  LoginWindow::LoginWindow(QWidget * parent, Qt::WindowFlags f):QMainWindow(parent,f)
  {
    QWidget *widget = new QWidget(this);
    m_form.setupUi(widget);
    setCentralWidget(widget);
    connect(m_form.m_login, SIGNAL(clicked()), SLOT(okPressed()));
    connect(&m_query,SIGNAL(responseReceived(QString, QString,QString)),this,SLOT(responseReceived(QString,QString,QString)));
    connect(&m_query,SLOT(onManagerSslErrors()),&m_query,SIGNAL(errorReceived()));
    connect(&m_query,SLOT(onReplyError(QNetworkReply::NetworkError)),&m_query,SIGNAL(errorReceived()));
    connect(&m_query,SIGNAL(errorReceived()),this,SLOT(errorReceived()));
  }

  void LoginWindow::okPressed()
  {
    qDebug() << "ok Pressed, query";
    m_query.setQuery(m_form.m_userName->text(),m_form.m_password->text());
    m_query.doRequest();
  }

  void LoginWindow::responseReceived(QString status, QString auth_token,QString status_description)
  {
    qDebug() << "received login query response";
    if(status=="Ok")
    {
      emit onSucsess(auth_token);
    }
    else
    {
      QMessageBox::information( this, "wikigps","Wrong user name or password.");
      //error
    }
  }

  void LoginWindow::errorReceived()
  {
    QMessageBox::information(this, "wikigps", "Network error");
  }

}
