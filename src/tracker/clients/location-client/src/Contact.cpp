#include "Contact.h"
Contact::Contact(QObject *parent)
{

}

Contact::Contact(QSharedPointer<Channel> channel, const QString &customName, QObject *parent): QObject(parent) {
    m_channel=channel;
    m_customName=customName;

}

void Contact::setChannel(QSharedPointer<Channel> channel)
{
    m_channel=channel;
}

void Contact::setCustomName(const QString & customName)
{
    m_customName = customName;
}

QString  Contact::getChannelName() const {
    return m_channel->getName();
}

QString Contact::getCustomName() const {
    return m_customName;
}
