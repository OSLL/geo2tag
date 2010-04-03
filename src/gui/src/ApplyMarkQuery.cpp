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

/*! ---------------------------------------------------------------
 * $Id$ 
 *
 * \file ApplyMarkQuery.cpp
 * \brief ApplyMarkQuery implementation
 *
 * File description
 *
 * PROJ: OSLL/fedexday
 * ---------------------------------------------------------------- */

#include "ApplyMarkQuery.h"
#include "defines.h"
#include <QDebug>

namespace GUI
{
    ApplyMarkQuery::ApplyMarkQuery(QObject *parent)
        : QObject(parent)
    {
        jsonQuery = "";
        httpQuery = "";
        manager = new QNetworkAccessManager(this);

        qDebug() << "Free ApplyMarkQuery created";
    }

    ApplyMarkQuery::ApplyMarkQuery(QString user, QString channel, QString title,
                                   QString link, QString description, qreal latitude,
                                   qreal longitude, QString time, QObject *parent)
                : QObject(parent)
    {
        manager = new QNetworkAccessManager(this);
        setQuery(user, channel, title, link, description, latitude, longitude, time);
        qDebug() << "ApplyMarkQuery created:\n"
                 << httpQuery << jsonQuery;
    }

    void ApplyMarkQuery::setQuery(QString user, QString channel, QString title,
                                  QString link, QString description, qreal latitude,
                                  qreal longitude, QString time)
    {
        jsonQuery = "{\"user\":\"" + user +
                    "\", \"channel\":\"" + channel +
                    "\", \"title\":\"" + title +
                    "\", \"link\":\"" + link +
                    "\", \"description\":\"" + description +
                    "\", \"latitude\":" + QString::number(latitude) +
                    ", \"longitude\":" + QString::number(longitude) +
                    ", \"time\":\"" + time +
                    "\"}";
        httpQuery = APPLY_HTTP_URL;
    }

    ApplyMarkQuery::~ApplyMarkQuery()
    {

    }

    const QString& ApplyMarkQuery::getHttpQuery()
    {
        return httpQuery;
    }

    const QString& ApplyMarkQuery::getJsonQuery()
    {
        return jsonQuery;
    }

    void ApplyMarkQuery::doRequest()
    {
        if (httpQuery == "" || jsonQuery == "")
        {
            qDebug() << "ApplyMarkQuery: can't do request because query isn't set";
            return;
        }

        QNetworkRequest request;
        request.setUrl(QUrl(httpQuery));

        QByteArray data(jsonQuery.toAscii(), jsonQuery.size());

        QNetworkReply *reply = manager->post(request, data);

        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(onManagerFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(onManagerSslErrors()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(onReplyError(QNetworkReply::NetworkError)));

        qDebug() << "ApplyMarkQuery did request:\n"
                 << httpQuery << jsonQuery;
    }

    void ApplyMarkQuery::onManagerFinished(QNetworkReply *reply)
    {
        QByteArray jsonResponseByteArray = reply->readAll();

        if (jsonResponseByteArray.size() > 0)
        {
            QString jsonResponse(jsonResponseByteArray);
            qDebug() << "Gotten response (json): " << jsonResponse;
            if (jsonResponse.contains("\"status\":\"ok\"", Qt::CaseInsensitive))
            {
                emit responseReceived(1);
            }
            else
            {
                emit responseReceived(0);
            }
        }
    }

    void ApplyMarkQuery::onReplyError(QNetworkReply::NetworkError error)
    {
        qDebug("Network error: %d \n", error);
    }

    void ApplyMarkQuery::onManagerSslErrors()
    {
        qDebug("ssl error \n");
    }
} // namespace GUI

/* ===[ End of file $HeadURL$ ]=== */
