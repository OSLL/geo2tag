/*
 * Copyright 2010 - 2012  Kirill Krinkin  kirill.krinkin@gmail.com
 *
 * Geo2tag LBS Platform (geo2tag.org)
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
 * \file Session.cpp
 * \brief Session implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "../inc/session.h"
#include "../inc/log.h"
#include "queryobject.h"

#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QWaitCondition>
#include <QTimer>
#include <QTest>

namespace
{
  QMutex        g_sessionMutex;
  Geo::Session   *g_sessionObject=NULL;

  bool init_map(std::map<std::string,std::string>& map, const char *str)
  {
      QString s(str);
      QStringList sl = s.split(";");
      QString kv;
      foreach(kv,sl)
      {
          QStringList keyValue = kv.split("=");
          if(keyValue.size()!=2)
              return false;
          QString key   = keyValue[0].trimmed();
          QString value = keyValue[1].trimmed();
          map[key.toStdString()]=value.toStdString();
      }
      return true;
  }
}

namespace Geo
{

bool Session::init(const char *initializationString)
{
    if(g_sessionObject != NULL) //already initialized
        return false;
    g_sessionObject = new Session();

    g_sessionObject->m_isValid = init_map((g_sessionObject->m_params),initializationString);


    return true;

}

Session& Session::instance()
{
    //LOCK_SESSION;

    Q_ASSERT(g_sessionObject != NULL);

    return *g_sessionObject;
}

Session::Session(): m_isValid(false)
{
}

const std::string &Session::param(const std::string &key) const
{
    return m_params[key];
}

void Session::checkValid()
{
    Q_ASSERT(false);//not implemented
}

bool Session::login()
{
    QString req_str =
        "{ \"login\":\""
        +QString(m_params["user"].c_str())
        +"\", \"password\":\""
        +QString(m_params["password"].c_str())+"\"}";

    qDebug() << "request=" << req_str;

    QNetworkAccessManager nam;
    QByteArray requestBody(req_str.toStdString().data());
    QNetworkRequest req(QUrl(m_params["url"].c_str()));

    QNetworkReply *rep = nam.post(req,requestBody);

    while(!rep->isFinished())
    {
       QTest::qSleep(1000);
                qDebug() << "1";
                QByteArray data=rep->readAll();


                qDebug() << QString(data);

    }

    QByteArray data=rep->readAll();


    qDebug() << QString(data);

    return false;
}

} // namespace Geo2tag
