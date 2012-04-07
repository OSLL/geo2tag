#include "Contact.h"

Contact::Contact(QObject *parent): QObject(parent) {

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
