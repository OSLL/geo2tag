#include "SubscribeChannelQuery.h"
#include "defines.h"
#include "SubscribeChannelJSON.h"
#include "SubscribeChannelResponseJSON.h"

SubscribeChannelQuery::SubscribeChannelQuery(
const QSharedPointer<common::User> &user,
const QSharedPointer<Channel> &channel,
QObject *parent) :
DefaultQuery(parent), m_user(user), m_channel(channel)
{
}


SubscribeChannelQuery::SubscribeChannelQuery(QObject *parent): DefaultQuery(parent)
{
}


void SubscribeChannelQuery::setQuery(const QSharedPointer<common::User> &user,
const QSharedPointer<Channel> &channel)
{
  m_user = user;
  m_channel = channel;
}


QString SubscribeChannelQuery::getUrl() const
{
  return SUBSCRIBE_HTTP_URL;
}


void SubscribeChannelQuery::setUrl(const QString &url)
{
  DefaultQuery::setUrl(url + SUBSCRIBE_HTTP_URL);
}


QByteArray SubscribeChannelQuery::getRequestBody() const
{
  SubscribeChannelRequestJSON request;
  request.addChannel(m_channel);
  request.addUser(m_user);
  return request.getJson();
}


void SubscribeChannelQuery::processReply(QNetworkReply *reply)
{
  SubscribeChannelResponseJSON response;
  response.parseJson(reply->readAll());
  if(response.getErrno() == SUCCESS)
  {
    Q_EMIT subscribed();
  }
  else
  {
    Q_EMIT errorOccured(getErrnoDescription(response.getErrno()));
  }
}


SubscribeChannelQuery::~SubscribeChannelQuery()
{
}
