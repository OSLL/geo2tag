#ifndef ADDCHANNELQUERY_H
#define ADDCHANNELQUERY_H

#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "Channel.h"
#include "User.h"

class AddChannelQuery: public DefaultQuery
{
  Q_OBJECT

    QSharedPointer<common::User> m_user;
  QSharedPointer<Channel> m_channel;

  virtual QString getUrl() const;
  virtual QByteArray getRequestBody() const;
  virtual void processReply(QNetworkReply *reply);

  public:
    AddChannelQuery(const QSharedPointer<common::User> &user,
      const QSharedPointer<Channel> &channel,
      QObject *parent = 0);
    AddChannelQuery(QObject *parent = 0);

    ~AddChannelQuery();
    void setQuery(const QSharedPointer<common::User> &user,
      const QSharedPointer<Channel> &channel);
    void setUrl(const QString &url);

    QSharedPointer<common::User> getUser() const;
    QSharedPointer<Channel> getChannel() const;

    Q_SIGNALS:

    void channelAdded();
};
// ADDCHANNELQUERY_H
#endif
