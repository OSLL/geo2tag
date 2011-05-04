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
 * \file AddUserQuery.h
 * \brief Header of AddUserQuery
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/fedexday
 * ---------------------------------------------------------------- */

#ifndef _AddUserQuery_H_B7CB9D45_F02D_4617_8C62_B56B0A2AF6A7_INCLUDED_
#define _AddUserQuery_H_B7CB9D45_F02D_4617_8C62_B56B0A2AF6A7_INCLUDED_

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

/*!
 * AddUserQuery class definition.
 *
 * The object of this class represents http query to server.
 * This query sends user name and password and gets auth_token.
 *
 */
class AddUserQuery: public QObject
{
  Q_OBJECT

    private:
    QNetworkAccessManager *manager;

    /* Url of th request */
    QString httpQuery;

    /* Body of the request */
    QString jsonQuery;

  public:

    AddUserQuery(QObject *parent = 0);

    AddUserQuery(QString login, QString password, QObject *parent = 0);

    void setQuery(QString login, QString password);

    ~AddUserQuery();

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
    AddUserQuery(const AddUserQuery& obj);
    AddUserQuery& operator=(const AddUserQuery& obj);

    // class AddUserQuery
};
//_AddUserQuery_H_B7CB9D45_F02D_4617_8C62_B56B0A2AF6A7_INCLUDED_
#endif

/* ===[ End of file $HeadURL$ ]=== */
