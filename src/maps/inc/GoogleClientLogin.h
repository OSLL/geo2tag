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
/*  */
/*!
 * \file GoogleClientLogin.h
 * \brief Header of GoogleClientLogin
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _GoogleClientLogin_H_E398ED1A_0E83_42CA_BC67_5C3668729B16_INCLUDED_
#define _GoogleClientLogin_H_E398ED1A_0E83_42CA_BC67_5C3668729B16_INCLUDED_

#include <string>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

namespace maps
{
  /*!
   * Google client auth
   */
  class GoogleClientLogin : public QObject
  {
    Q_OBJECT

      QNetworkAccessManager *manager;
    std::string m_email;
    std::string m_password;
    std::string m_authToken;

    public:
      GoogleClientLogin(std::string email, std::string password);

      void login();

      std::string getAuthToken() const;

      ~GoogleClientLogin();

    public slots:

      void onManagerFinished(QNetworkReply*);
      void onManagerSslErrors(/*QNetworkReply*, QList<QSslError>*/);
      void onReplyError(QNetworkReply::NetworkError);

    private:
      GoogleClientLogin(const GoogleClientLogin& obj);
      GoogleClientLogin& operator=(const GoogleClientLogin& obj);

      static size_t write(void *buffer, size_t size, size_t nmemb, void *stream);

      // class GoogleClientLogin
  };

  // namespace maps
}


//_GoogleClientLogin_H_E398ED1A_0E83_42CA_BC67_5C3668729B16_INCLUDED_
#endif

/* ===[ End of file  ]=== */
