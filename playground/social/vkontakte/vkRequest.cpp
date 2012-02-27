#include "vkRequest.h"
#include <QDebug>

vkRequest::vkRequest(QObject *parent) :
QObject(parent)
{
}


void vkRequest::getProfile(const QString &token, const QString userId)
{
  QString methodName = "getProfiles";
  QString methodParameters = "uids="+userId;
  QString requestString="https://api.vkontakte.ru/method/"+methodName+"?"+methodParameters+"&access_token="+token +"&fields=education,contacts,country,city";
  QUrl address(requestString);
  QNetworkRequest request(address);
  replyProfile = manager.post(request,"");
  connect(replyProfile, SIGNAL(finished()),this, SLOT(getReplyFinished()));
}


void vkRequest::getReplyFinished()
{
  QString answer = QString::fromUtf8(replyProfile->readAll());
  replyProfile->deleteLater();
  qDebug()<<answer;
}
