#ifndef CONTACT_H
#define CONTACT_H
#include <QSharedPointer>
#include "Channel.h"

class Contact: public QObject
{
    Q_OBJECT
private:
    QSharedPointer<Channel> m_channel;
    QString m_customName;
public:
    Contact(QObject* parent=0);
    Contact(QSharedPointer<Channel>,const QString &customName, QObject *parent=0);
    QString getChannelName() const;
    QString getCustomName() const;
    void setChannel(QSharedPointer<Channel> channel);
    void setCustomName(const QString &customName);

};

#endif // CONTACT_H
