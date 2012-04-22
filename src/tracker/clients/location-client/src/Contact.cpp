#include "Contact.h"
#include <QDateTime>
Contact::Contact(QObject *parent): QObject(parent)
{

}

Contact::Contact(const QString &channelName, const QString &customName, STATUS_TYPE status, QObject *parent): QObject(parent), m_status(status) {
    m_channelName=channelName;
    m_customName=customName;
    //m_status = LOST;
    //m_lastMark = QSharedPointer<DataMark>(new DataMark(0,60,30,"","","",QDateTime::currentDateTime()));



}



void Contact::setCustomName(const QString & customName)
{
    m_customName = customName;
}

QString  Contact::getChannelName() const {
    return m_channelName;
}

QString Contact::getCustomName() const {
    return m_customName;
}



QSharedPointer<DataMark> Contact::getLastMark() {
    return m_lastMark;
}


void Contact::setLastDataMark(QSharedPointer<DataMark> datamark)
{
    m_lastMark = datamark;

}

void Contact::setStatus(STATUS_TYPE status)
{
    m_status=status;
}

STATUS_TYPE Contact::getStatus() const {
    return m_status;
}
