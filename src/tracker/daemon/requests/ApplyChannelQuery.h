/*
 * Copyright 2010  Open Source & Linux Lab (OSLL)  osll@osll.spb.ru
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

/* $Id$ */
/*!
 * \file ApplyChannelQuery.h
 * \brief Header of ApplyChannelQuery
 * \todo add comment here
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#ifndef _ApplyChannelQuery_H_A2EBB6AB_07D4_4326_AB6A_39441A3DD564_INCLUDED_
#define _ApplyChannelQuery_H_A2EBB6AB_07D4_4326_AB6A_39441A3DD564_INCLUDED_

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

/*!
 * Class description. May use HTML formatting
 *
 */
namespace GUI
{
  class ApplyChannelQuery : public QObject
  {
    Q_OBJECT

      private:
      QNetworkAccessManager *manager;

      /* Url of th request */
      QString m_httpQuery;

      /* Body of the request */
      QString m_jsonQuery;

    public:
      ApplyChannelQuery(QObject *parent = 0);

      ApplyChannelQuery(QString auth_token, QString name, QString description,
        QString url, qreal activeRadius, QObject *parent = 0);

      void setQuery(QString auth_token, QString name, QString description,
        QString url, qreal activeRadius);

      ~ApplyChannelQuery();

      const QString& getHttpQuery();
      const QString& getJsonQuery();

      void doRequest();

      signals:
      void responseReceived(QString status,QString status_description);

    private slots:
      void onManagerFinished(QNetworkReply *reply);
      void onReplyError(QNetworkReply::NetworkError);
      void onManagerSslErrors();

    private:
      /* \todo Do we need next constructor and overloaded operator? */
      ApplyChannelQuery(const ApplyChannelQuery& obj);
      ApplyChannelQuery& operator=(const ApplyChannelQuery& obj);

      // class ApplyMarkQuery
  };
}


//_ApplyChannelQuery_H_A2EBB6AB_07D4_4326_AB6A_39441A3DD564_INCLUDED_
#endif

/* ===[ End of file $HeadURL$ ]=== */
