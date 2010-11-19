#include <sstream>
#include <syslog.h>
#include <string.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QDebug>

#include "server.h"
#include "DbSession.h"

#define LISTENSOCK_FILENO 0
#define LISTENSOCK_FLAGS 0

Server::Server()
{
    int err = FCGX_Init(); // call before Accept in multithreaded apps

    if (err)
    {
        std::ostringstream s;
        s << "FCGX_Init failed: " << err;
        syslog(LOG_INFO, s.str().c_str());
    }

    err = FCGX_InitRequest(&m_cgi,LISTENSOCK_FILENO, LISTENSOCK_FLAGS);
    if (err)
    {
        std::ostringstream s;
        s << "FCGX_InitRequest failed: " << err;
        syslog(LOG_INFO, s.str().c_str());
    }
}

QMap<QString, QString> parseQuery(const QString& string)
{
    QMap<QString, QString> map;
    enum States {
        READ_NAME,
        READ_VALUE
    } state;

    size_t j = 0;
    std::string paramName(""), paramValue(""), s = string.toStdString();
    state = READ_NAME;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '=') {
            paramName = s.substr(j, i - j);
            j = i + 1;
            state=READ_VALUE;
        }
        if (s[i] == '&') {
            if (state == READ_NAME) {
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
    if (paramName.size()) {
        paramValue = s.substr(j, s.size() - j);
        map.insert(QString::fromStdString(paramName), QString::fromStdString(paramValue));
    }
    return map;
}

QByteArray Server::process(const QString& query, const QByteArray &data)
{
      QMap<QString, QString> queryParameters = parseQuery(query);

      return common::DbObjectsCollection::getInstance().process(queryParameters.value("query"), data);
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
        response = process(queryString, queryBody);
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
