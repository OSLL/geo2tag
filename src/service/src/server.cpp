/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
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
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <QDebug>
#include <sstream>
#include <syslog.h>
#include <string.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "server.h"
#include "DbSession.h"

#define LISTENSOCK_FILENO 0
#define LISTENSOCK_FLAGS 0

Server::Server()
{
  int err = FCGX_Init();                // call before Accept in multithreaded apps

  if (err)
  {
    syslog(LOG_INFO, "FCGX_Init failed, errcode=%d",err);
  }

  err = FCGX_InitRequest(&m_cgi,LISTENSOCK_FILENO, LISTENSOCK_FLAGS);

  if (err)
  {
    syslog(LOG_INFO, "FCGX_InitRequest failed, errcode=%d",err);
  }
}


QMap<QString, QString> parseQuery(const QString& string)
{
  QMap<QString, QString> map;
  enum States
  {
    READ_NAME,
    READ_VALUE
  } state;

  size_t j = 0;
  std::string paramName(""), paramValue(""), s = string.toStdString();
  state = READ_NAME;
  for (size_t i = 0; i < s.size(); ++i)
  {
    if (s[i] == '=')
    {
      paramName = s.substr(j, i - j);
      j = i + 1;
      state=READ_VALUE;
    }
    if (s[i] == '&')
    {
      if (state == READ_NAME)
      {
        // variable without value
        map.insert(QString::fromStdString(s.substr(j, i - j)), "");
        j = i + 1;
        continue;
      }
      paramValue = s.substr(j, i - j );
      j = i + 1;
      map.insert(QString::fromStdString(paramName), QString::fromStdString(paramValue));
      state=READ_NAME;
    }
  }
  if (paramName.size())
  {
    paramValue = s.substr(j, s.size() - j);
    map.insert(QString::fromStdString(paramName), QString::fromStdString(paramValue));
  }
  return map;
}


QString Server::extractRESTQuery()
{
  QString query_string(FCGX_GetParam("PATH_INFO", m_cgi.envp));
  // remove first simbol - slash
  return query_string.right(query_string.size()-1);
}


QByteArray Server::process( const QByteArray &data)
{
  //  QMap<QString, QString> queryParameters = parseQuery(query);
  common::DbObjectsCollection &dboc = common::DbObjectsCollection::getInstance();
  //  QByteArray result = dboc.process(queryParameters.value("query"), data);
  QByteArray result = dboc.process(extractRESTQuery(), data);
  return result;
}


void Server::extractIncomingData(const FCGX_Request& request, QString& queryString, QByteArray& queryBody)
{
  queryString.clear();
  queryBody.clear();
  queryString = FCGX_GetParam("QUERY_STRING", m_cgi.envp);
  for(;;)
  {
    char ch = FCGX_GetChar(request.in);
    if(ch != -1)
      queryBody.append(ch);
    else
      break;
  }
}


void Server::run()
{
  syslog(LOG_INFO,"Starting server thread...");
  for(;;)
  {
    int err = FCGX_Accept_r(&m_cgi);
    if (err)
    {
      syslog(LOG_INFO, "FCGX_Accept_r stopped: %d", err);
      break;
    }
    QString queryString;
    QByteArray queryBody, response;
    extractIncomingData(m_cgi,queryString,queryBody);
    syslog(LOG_INFO, "query: %s", queryString.toStdString().c_str());
    response = process( queryBody);
    int written = FCGX_PutStr(response.data(), response.size(), m_cgi.out);
    if(written != response.size())
    {
      qDebug() << "some data was loast during writing to the pipe";
    }
    FCGX_Finish_r(&m_cgi);
  }
}


void Server::serve()
{
  start();
}


Server::~Server()
{
  wait();
  FCGX_ShutdownPending();
}
