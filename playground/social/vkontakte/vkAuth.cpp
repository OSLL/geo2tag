#include "vkAuth.h"
#include <QDebug>

vkAuth::vkAuth(QString appId, QWidget *parent):QWebView(parent)
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
  this->url = url.toString().replace("#","?");
  if (this->url.hasQueryItem("error"))
  {
    qDebug()<<"Error";

    return;
  }
  if (!this->url.hasQueryItem("access_token"))
  {
    return;
  }
  else
  {
    token = this->url.queryItemValue("access_token");
    expires = this->url.queryItemValue("expires_in").toInt();
    userId = this->url.queryItemValue("user_id");

    emit success(token,userId,expires);

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
