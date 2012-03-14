#include "SubscribeChannelQuery.h"
#include "defines.h"
#include "SubscribeChannelJSON.h"
#include "SubscribeChannelResponseJSON.h"

SubscribeChannelQuery::SubscribeChannelQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<common::User> &user, QObject *parent): DefaultQuery(parent), m_channel(channel), m_user(user)
{

}


SubscribeChannelQuery::SubscribeChannelQuery(QObject *parent): DefaultQuery(parent)
{
}


void SubscribeChannelQuery::setQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<common::User> &user)
{
  m_channel = channel;
  m_user = user;
}


QString SubscribeChannelQuery::getUrl() const
{
  return SUBSCRIBE_HTTP_URL;
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
  m_errno = response.getErrno();
  if(response.getErrno() == SUCCESS)
  {
    Q_EMIT channelSubscribed(m_channel);
  }
  else
  {
    Q_EMIT errorOccured(response.getErrno());
  }
}


SubscribeChannelQuery::~SubscribeChannelQuery()
{

}


