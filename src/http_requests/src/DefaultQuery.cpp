#include <QDebug>
#include <QNetworkConfiguration>
#include "DefaultQuery.h"
#include "defines.h"

#ifndef Q_OS_SYMBIAN
#include <syslog.h>
#else
#include "symbian.h"
#endif

DefaultQuery::DefaultQuery(QObject *parent): QObject(parent),
m_manager(new QNetworkAccessManager(parent))
{
  connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(process(QNetworkReply*)));
  connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(handleError()));
}


void DefaultQuery::doRequest()
{
  QNetworkRequest request;

  QUrl url(getUrl());
  url.setPort(getServerPort());
  request.setUrl(url);

  qDebug() << "doing post to" << url << " with body: " << getRequestBody();
  syslog(LOG_INFO,"posting http request to %s with body %s",url.toString().toStdString().c_str(),QString(getRequestBody()).toStdString().c_str());
  QNetworkReply *reply = m_manager->post(request, getRequestBody());
  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError()));
}


void DefaultQuery::process(QNetworkReply *reply)
{
  processReply(reply);
}


void DefaultQuery::handleError()
{
  syslog(LOG_INFO,"Network error occured while sending request");
  Q_EMIT errorOccured("network error occcured");
}
