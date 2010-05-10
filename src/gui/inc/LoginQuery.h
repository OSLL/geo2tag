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
 * \file LoginQuery.h
 * \brief Header of LoginQuery
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _LoginQuery_H_1ED2F4BC_FCC5_4CD7_85EB_9C83BEF4B96C_INCLUDED_
#define _LoginQuery_H_1ED2F4BC_FCC5_4CD7_85EB_9C83BEF4B96C_INCLUDED_

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace GUI
{
    /*!
     * LoginQuery class definition.
     *
     * The object of this class represents http query to server.
     * This query sends user name and password and gets auth_token.
     *
     */
    class LoginQuery: public QObject
    {
        Q_OBJECT

    private:
        QNetworkAccessManager *manager;

        /* Url of th request */
        QString httpQuery;

        /* Body of the request */
        QString jsonQuery;

    public:

        LoginQuery(QObject *parent = 0);

        LoginQuery(QString user, QString password, QObject *parent = 0);

        void setQuery(QString user, QString password);

        ~LoginQuery();

        const QString& getHttpQuery();
        const QString& getJsonQuery();

        void doRequest();

    signals:
        void responseReceived(QString status, QString auth_token);
        void errorReceived();

    private slots:
        void onManagerFinished(QNetworkReply *reply);
        void onReplyError(QNetworkReply::NetworkError);
        void onManagerSslErrors();

    private:
        /* \todo Do we need next constructor and overloaded operator? */
        LoginQuery(const LoginQuery& obj);
        LoginQuery& operator=(const LoginQuery& obj);

    }; // class LoginQuery

} // namespace GUI

#endif //_LoginQuery_H_1ED2F4BC_FCC5_4CD7_85EB_9C83BEF4B96C_INCLUDED_

/* ===[ End of file $HeadURL$ ]=== */
