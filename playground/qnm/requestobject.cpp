#include "requestobject.h"
#include <QDebug>
#include <QStringList>

RequestObject::RequestObject(QObject *parent) :
    QNetworkAccessManager(parent)
{
    connect(this,SIGNAL(finished(QNetworkReply*)),this,SLOT(parseRequest(QNetworkReply*)));
}

void RequestObject::requestSessionToken()
{

}

void RequestObject::requestSessionToken(const QString &user, const QString &pass)
{
    QUrl url("http://demo.geo2tag.org/service/login");
    QByteArray ba;

    QString requestdata(" {\"login\" : \""+user+"\" , \"password\" : \""+pass+"\"} ");
    qDebug() << "sending " << requestdata;
    ba = requestdata.toStdString().c_str();
    QNetworkRequest nr(url);
    post(nr,ba);
}

void RequestObject::parseRequest(QNetworkReply *nr)
{
    QByteArray ba = nr->readAll();
    QStringList sl = QString(ba).split(" ");

    m_token = sl[3];
    m_token.remove(',');
    m_token.remove('\"');
    qDebug() << m_token;
}
