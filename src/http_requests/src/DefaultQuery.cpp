#include <QDebug>
#include <QNetworkConfiguration>
#include "DefaultQuery.h"
#include "defines.h"

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
    url.setPort(DEFAULT_PORT);
    request.setUrl(url);

    qDebug() << "doing post to" << url << " with body: " << getRequestBody();
    QNetworkReply *reply = m_manager->post(request, getRequestBody());
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError()));
}

void DefaultQuery::process(QNetworkReply *reply)
{
    qDebug() << "DefaultQuery::process";
    processReply(reply);
}

void DefaultQuery::handleError()
{
    emit errorOccured("network error occcured");
}
