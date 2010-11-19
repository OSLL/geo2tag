#ifndef DEFAULTQUERY_H
#define DEFAULTQUERY_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DefaultQuery : public QObject
{
    Q_OBJECT
protected:
    QNetworkAccessManager *m_manager;

    virtual QString getUrl() const = 0;
    virtual QByteArray getRequestBody() const = 0;

protected slots:

    virtual void processReply(QNetworkReply *reply) = 0;

public:
    DefaultQuery(QObject *parent = 0);

    void doRequest();

signals:

    void responseReceived();

    void errorOccured();
};

#endif // DEFAULTQUERY_H
