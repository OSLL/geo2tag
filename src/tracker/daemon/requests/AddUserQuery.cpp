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
 * \file AddUserQuery.cpp
 * \brief AddUserQuery implementation
 *
 * File description
 *
 * PROJ: OSLL/fedexday
 * ---------------------------------------------------------------- */

#include "AddUserQuery.h"
#include "defines.h"
#include <QDebug>
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "QVariant"
#include "QVariantMap"

//Task #1113
#ifndef ADD_USER_HTTP_URL
#define ADD_USER_HTTP_URL "http://zps.spb.su/service?query=addUser"
#endif

AddUserQuery::AddUserQuery(QObject *parent)
: QObject(parent)
{
  jsonQuery = "";
  httpQuery = "";
  manager = new QNetworkAccessManager(this);

  qDebug() << "Free AddUserQuery created";
}


AddUserQuery::AddUserQuery(QString login, QString password, QObject *parent)
: QObject(parent)
{
  manager = new QNetworkAccessManager(this);
  setQuery(login, password);
  qDebug() << "AddUserQuery created:\n"
    << httpQuery << jsonQuery;
}


void AddUserQuery::setQuery(QString login, QString password)
{
  QVariantMap request;
  request.insert("login", login);
  request.insert("password", password);

  QJson::Serializer serializer;
  QString json(serializer.serialize(request));

  jsonQuery = json;
  httpQuery = ADD_USER_HTTP_URL;
}


AddUserQuery::~AddUserQuery()
{

}


const QString& AddUserQuery::getHttpQuery()
{
  return httpQuery;
}


const QString& AddUserQuery::getJsonQuery()
{
  return jsonQuery;
}


void AddUserQuery::doRequest()
{
  if (httpQuery == "" || jsonQuery == "")
  {
    qDebug() << "AddUserQuery: can't do request because query isn't set";
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

  qDebug() << "AddUserQuery did request:\n"
    << httpQuery << jsonQuery;
}


void AddUserQuery::onManagerFinished(QNetworkReply *reply)
{
  QByteArray jsonResponseByteArray = reply->readAll();

  if (jsonResponseByteArray.size() > 0)
  {
    QString jsonResponse(jsonResponseByteArray);
    qDebug() << "Gotten response (json): " << jsonResponse;
    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(QByteArray(jsonResponse.toAscii()), &ok).toMap();
    QString status("Error");
    QString auth_token("");
    if (!ok)
    {
      qFatal("An error occured during parsing json with user name and password");
    }
    else
    {
      status = result["status"].toString();
      auth_token = result["auth_token"].toString();
    }

    emit responseReceived(status, auth_token);
  }
}


void AddUserQuery::onReplyError(QNetworkReply::NetworkError error)
{
  qDebug("Network error: %d \n", error);
}


void AddUserQuery::onManagerSslErrors()
{
  qDebug("ssl error \n");
}


/* ===[ End of file $HeadURL$ ]=== */
