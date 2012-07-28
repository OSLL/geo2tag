#ifndef SUBSCRIBECHANNELQUERY_H
#define SUBSCRIBECHANNELQUERY_H

#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "Channel.h"
#include "User.h"

class SubscribeChannelQuery: public DefaultQuery
{
  Q_OBJECT

    QSharedPointer<common::User> m_user;
  QSharedPointer<Channel> m_channel;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;
  virtual void processReply(QNetworkReply *reply);

  public:
    SubscribeChannelQuery(const QSharedPointer<common::User> &user,
      const QSharedPointer<Channel> &channel,
      QObject *parent = 0);
    SubscribeChannelQuery(QObject *parent = 0);

    ~SubscribeChannelQuery();
    void setQuery(const QSharedPointer<common::User> &user,
      const QSharedPointer<Channel> &channel);
    void setUrl(const QString &url);

    Q_SIGNALS:

    void subscribed();
};
// SUBSCRIBECHANNELQUERY_H
#endif
