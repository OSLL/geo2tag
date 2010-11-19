#include "DefaultQuery.h"
#include "defines.h"

DefaultQuery::DefaultQuery(QObject *parent) :QObject(parent),
        m_manager(new QNetworkAccessManager(this))
{
}


void DefaultQuery::doRequest()
{
    QNetworkRequest request;

    QUrl url(getUrl());
    url.setPort(DEFAULT_PORT);
    request.setUrl(url);

    QNetworkReply *reply = m_manager->post(request, getRequestBody());

    connect(m_manager, SIGNAL(finished(QNetworkReply*)),SLOT(processReply(QNetworkReply*)));
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SIGNAL(errorOccured()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SIGNAL(errorOccured()));
}
