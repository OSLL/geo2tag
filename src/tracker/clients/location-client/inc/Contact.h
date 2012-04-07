#ifndef CONTACT_H
#define CONTACT_H
#include <QSharedPointer>
#include "Channel.h"

class Contact: public QObject
{
private:
    QSharedPointer<Channel> m_channel;
    QString m_customName;
public:
    Contact(QObject* parent);
    QString getChannelName() const;
    QString getCustomName() const;
    void setChannel(QSharedPointer<Channel> channel);
    void setCustomName(const QString &customName);

};

#endif // CONTACT_H
