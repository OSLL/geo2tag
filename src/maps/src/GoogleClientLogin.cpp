/*
 * Copyright 2010  OSLL osll@osll.spb.ru
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
/*! ---------------------------------------------------------------
 *
 *
 * \file GoogleClientLogin.cpp
 * \brief GoogleClientLogin implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <iostream>
#include <sstream>
#include "GoogleClientLogin.h"
//#include <QSslError>
#include <QDebug>

namespace maps
{
  GoogleClientLogin::GoogleClientLogin(std::string email, std::string password):m_email(email), m_password(password), m_authToken("")
  {
    manager = new QNetworkAccessManager(this);
  }

  void GoogleClientLogin::login()
  {
    std::ostringstream ss;
    ss << "Email="<< m_email << "&Passwd=" << m_password << "&service=local&source=TestLogin";
    std::string s = ss.str();
    QByteArray sByteArray(s.c_str(), s.size());

    QNetworkRequest request;
    request.setUrl(QUrl("https://www.google.com/accounts/ClientLogin"));

    QNetworkReply *reply = manager->post(request, sByteArray);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
      this, SLOT(onManagerFinished(QNetworkReply*)));
    connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
      this, SLOT(onManagerSslErrors(/*QNetworkReply*,QList<QSslError>*/)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
      this, SLOT(onReplyError(QNetworkReply::NetworkError)));
  }

  void GoogleClientLogin::onManagerFinished(QNetworkReply* reply)
  {

    QByteArray bufByteArray(reply->readAll());
    std::string buf(bufByteArray.data());

    // Parsing Buffer for auth substring
    int authToken = buf.find("Auth",0);

    m_authToken = buf.substr(authToken,buf.size()-authToken);
    qDebug() << "Auth:" << QString(m_authToken.c_str());

  }

  std::string GoogleClientLogin::getAuthToken() const
  {
    return m_authToken;
  }

  size_t GoogleClientLogin::write(void *buffer, size_t size, size_t nmemb, void *stream)
  {
    std::string *data = reinterpret_cast<std::string *>(stream);
    if(data==NULL)
    {
      // Epic FAIL
      return -1;
    }
    for(int i=0; i<size*nmemb; ++i)
    {
      (*data)+=(((char*)buffer)[i]);
      std::cout <<(((char*)buffer)[i]);
    }
    return size*nmemb;
  }

  void GoogleClientLogin::onManagerSslErrors(/*QNetworkReply* reply, QList<QSslError> errors*/)
  {
    qDebug("ssl error \n");
  }

  void GoogleClientLogin::onReplyError(QNetworkReply::NetworkError error)
  {
    qDebug("Network error: %d \n", error);
  }

  GoogleClientLogin::~GoogleClientLogin()
  {

  }
}                                       // namespace maps


/* ===[ End of file  ]=== */
