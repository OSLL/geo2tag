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
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "QVariant"
#include "QVariantMap"

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

    ApplyMarkQuery::ApplyMarkQuery(QString auth_token, QString channel, QString title,
                                   QString link, QString description, qreal latitude,
                                   qreal longitude, QString time, QObject *parent)
                : QObject(parent)
    {
        manager = new QNetworkAccessManager(this);
        setQuery(auth_token, channel, title, link, description, latitude, longitude, time);
        qDebug() << "ApplyMarkQuery created:\n"
                 << httpQuery << jsonQuery;
    }

    void ApplyMarkQuery::setQuery(QString auth_token, QString channel, QString title,
                                  QString link, QString description, qreal latitude,
                                  qreal longitude, QString time)
    {
        QVariantMap request;
        request.insert("auth_token", auth_token);
        request.insert("channel", channel);
        request.insert("title", title);
        request.insert("link", link);
        request.insert("description", description);
        request.insert("latitude", QString::number(latitude).toDouble());
        request.insert("longitude", QString::number(longitude).toDouble());
        request.insert("time", time);
        QJson::Serializer serializer;
        jsonQuery = serializer.serialize(request);

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
	QUrl url(httpQuery);
	url.setPort(DEFAULT_PORT);
        request.setUrl(url);

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
            QJson::Parser parser;
            bool ok;
            QVariantMap result = parser.parse(QByteArray(jsonResponse.toAscii()), &ok).toMap();
            QString status("");
            QString status_desciption("");
            if (!ok)
            {
                qFatal("An error occured during parsing json with response to apply mark");
            }
            else
            {
                status = result["status"].toString();
                status_decription = result["status_description"].toString();
            }

            emit responseReceived(status,status_description);
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
