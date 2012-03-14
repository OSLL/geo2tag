#ifndef APPLYCHANNELQUERY_H
#define APPLYCHANNELQUERY_H
#include <QObject>
#include <QString>
#include "DefaultQuery.h"
#include "User.h"
#include "Channel.h"


class ApplyChannelQuery: public DefaultQuery
{
    Q_OBJECT

    QSharedPointer<Channel> m_channel;
    QSharedPointer<common::User> m_user;

    virtual QString getUrl() const;
    virtual QByteArray getRequestBody() const;
    virtual void processReply(QNetworkReply *reply);

    public:

    ApplyChannelQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<common::User> &user, QObject *parent = 0);

    ApplyChannelQuery(QObject *parent = 0);

    void setQuery(const QSharedPointer<Channel> &channel, const QSharedPointer<common::User> &user);

    const QSharedPointer<Channel>& getChannel() const;

    ~ApplyChannelQuery();

    Q_SIGNALS:

    void channelAdded(QSharedPointer<Channel> channel);


};
#endif // APPLYCHANNELQUERY_H
