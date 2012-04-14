#include "UnsubscribeChannelQuery.h"
#include "UnsubscribeChannelRequestJSON.h"
#include "UnsubscribeChannelResponseJSON.h"
#include "defines.h"

UnsubscribeChannelQuery::UnsubscribeChannelQuery( const QSharedPointer<common::User> &user,const QSharedPointer<Channel> &channel, QObject *parent): DefaultQuery(parent), m_user(user), m_channel(channel)
{

}


UnsubscribeChannelQuery::UnsubscribeChannelQuery(QObject *parent): DefaultQuery(parent)
{
}


void UnsubscribeChannelQuery::setQuery( const QSharedPointer<common::User> &user,const QSharedPointer<Channel> &channel)
{
  m_channel = channel;
  m_user = user;
}


QString UnsubscribeChannelQuery::getUrl() const
{
  return UNSUBSCRIBE_HTTP_URL;
}


QByteArray UnsubscribeChannelQuery::getRequestBody() const
{
  UnsubscribeChannelRequestJSON request;
  request.addChannel(m_channel);
  request.addUser(m_user);
  return request.getJson();
}


void UnsubscribeChannelQuery::processReply(QNetworkReply *reply)
{
  UnsubscribeChannelResponseJSON response;
  response.parseJson(reply->readAll());
  m_errno = response.getErrno();
  if(response.getErrno() == SUCCESS)
  {
      Q_EMIT channelUnsubscribed();
  }
  else
  {
    Q_EMIT errorOccured(response.getErrno());
  }
}
QSharedPointer<Channel> UnsubscribeChannelQuery::getChannel()
{
    return m_channel;
}

UnsubscribeChannelQuery::~UnsubscribeChannelQuery()
{

}
