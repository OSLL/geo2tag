#include "vkAuth.h"
#include <QDebug>

vkAuth::vkAuth(QString appId, QWebView *parent):QWebView(parent)
{



    connect(this, SIGNAL(urlChanged(QUrl)), this, SLOT(slotChanged(QUrl)));
    QUrl url("http://api.vkontakte.ru/oauth/authorize");
    url.addQueryItem("client_id", appId);
    url.addQueryItem("display", "popup");
    url.addQueryItem("scope", "friends,video,offline");
    url.addQueryItem("redirect_uri","http://api.vkontakte.ru/blank.html");
    url.addQueryItem("response_type", "token");
    this->load(url);




}

void vkAuth::slotChanged(const QUrl & url)
{
    qDebug()<<"SLOT CHANGED";
 this->url = url.toString().replace("#","?");
 if (this->url.hasQueryItem("error"))
 {
     qDebug()<<"Error";
 // обработка неудачной авторизации
 return;
 }
 if (!this->url.hasQueryItem("access_token"))
 {
     qDebug()<<"!url.hasQueryItem";
     return;
 }
 else
 {
     token = this->url.queryItemValue("access_token");
     expires = this->url.queryItemValue("expires_in").toInt();
     userId = this->url.queryItemValue("user_id");
     QUrl address("https://api.vkontakte.ru/method/");
     QString methodName = "getProfiles";
     QString methodParameters = "uid="+getUserId();
     QString requestString =methodName+"?"+methodParameters+"&access_token="+token;
     QNetworkRequest request(address);




     reply = manager.post(request, requestString.toUtf8().data());
     qDebug()<<"requestString="<<requestString;
     connect(reply, SIGNAL(finished()),this, SLOT(getReplyFinished()));

     qDebug() <<"token: "<< token;
     qDebug() <<"expires_in: "<< expires;
     qDebug()<<"ALL: "<<url.toString();
 }

}

QString & vkAuth::getToken()
{
    return token;
}

QString vkAuth::getUserId()
{
    return userId;
}

void vkAuth::getReplyFinished()
{
    QString answer = QString::fromUtf8(reply->readAll());
    reply->deleteLater();
    qDebug()<<answer;
}
