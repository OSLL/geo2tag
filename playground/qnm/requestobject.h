#ifndef REQUESTOBJECT_H
#define REQUESTOBJECT_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class RequestObject : public QNetworkAccessManager
{
    Q_OBJECT
    QString m_token;
public:
    explicit RequestObject(QObject *parent = 0);

    void requestSessionToken();
    void requestSessionToken(const QString& user, const QString& pass);
    
signals:
    
public slots:

    void parseRequest(QNetworkReply *nr);
};

#endif // REQUESTOBJECT_H
