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
    virtual void processReply(QNetworkReply *reply) = 0;

protected Q_SLOTS:

    void process(QNetworkReply *reply);

    void handleError();

public:
    DefaultQuery(QObject *parent = 0);

    void doRequest();

Q_SIGNALS:

    void responseReceived();

    void errorOccured(QString);
};

#endif // DEFAULTQUERY_H
