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
 * \file RSSFeedQuery.cpp
 * \brief RSSFeedQuery implementation
 *
 * File description
 *
 * PROJ: geo2tag
 * ---------------------------------------------------------------- */

#include "RSSFeedQuery.h"
#include "RSSFeedJSON.h"
#include "defines.h"
#include <QDebug>

namespace GUI
{
    RSSFeedQuery::RSSFeedQuery(QObject *parent)
        : QObject(parent)
    {
        jsonQuery = "";
        httpQuery = "";
        manager = new QNetworkAccessManager(this);

        qDebug() << "Free RSSFeedQuery created";
    }

    RSSFeedQuery::RSSFeedQuery
            (QString user, qreal latitude, qreal longitude, qreal radius, bool isLastOne, QObject *parent)
                : QObject(parent)
    {
        manager = new QNetworkAccessManager(this);
        setQuery(user,latitude,longitude,radius,isLastOne);

        qDebug() << "RSSFeedQuery created:\n"
                 << httpQuery << jsonQuery;
    }

    void RSSFeedQuery::setQuery(QString auth_token, qreal latitude,
                                              qreal longitude, qreal radius,bool isLastOne)
    {
        jsonQuery = "{\"auth_token\":\"" + auth_token +
                    "\", \"latitude\":" + QString::number(latitude) +
                    ", \"longitude\":" + QString::number(longitude) +
                    ", \"radius\":" + QString::number(radius);
	if (isLastOne){ 
		 jsonQuery+=", \"type\":\"last_one\"";
		 qDebug() << "isLastOne=1";
	}
	qDebug() << auth_token;
	jsonQuery += "}";
//        qDebug() << "++++Builded request ++ "<< jsonQuery;
        httpQuery = FEED_HTTP_URL;
    }

    RSSFeedQuery::~RSSFeedQuery()
    {

    }

    const QString& RSSFeedQuery::getHttpQuery()
    {
        return httpQuery;
    }

    const QString& RSSFeedQuery::getJsonQuery()
    {
        return jsonQuery;
    }

    void RSSFeedQuery::doRequest()
    {
        if (httpQuery == "" || jsonQuery == "")
        {
            qDebug() << "RSSFeedQuery: can't do request cause query isn't set";
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

        qDebug() << "RSSFeedQuery did request:\n"
                 << httpQuery << jsonQuery;
    }

    void RSSFeedQuery::onManagerFinished(QNetworkReply *reply)
    {
        QByteArray jsonResponseByteArray = reply->readAll();

        if (jsonResponseByteArray.size() > 0)
        {
            QString jsonResponse(jsonResponseByteArray);
            qDebug() << "Gotten response (json): " << jsonResponse;
            std::stringstream jsonStream(jsonResponse.toStdString());
            RSSFeedResponseJSON rssFeed(jsonStream);
            CHandlePtr<common::DataMarks> marks = rssFeed.getMarks();
            emit responseReceived(marks);
            /* check response and emit signal */
        }
    }

    void RSSFeedQuery::onReplyError(QNetworkReply::NetworkError error)
    {
        qDebug("Network error: %d \n", error);
    }

    void RSSFeedQuery::onManagerSslErrors()
    {
        qDebug("ssl error \n");
    }
} // namespace GUI

/* ===[ End of file $HeadURL$ ]=== */
