#ifndef CONTACT_H
#define CONTACT_H
#include <QSharedPointer>
#include "Channel.h"
#include "DataMarks.h"
enum STATUS_TYPE
{
  MY_CHANNEL,
  AVAILABLE,
  LOST
};
class Contact: public QObject
{
  Q_OBJECT
    private:
    QString m_channelName;
    QString m_customName;
    QSharedPointer<DataMark> m_lastMark;
    STATUS_TYPE m_status;

  public:
    Contact(QObject* parent=0);
    Contact(const QString &channelName,const QString &customName, STATUS_TYPE status = LOST, QObject *parent=0);
    QString getChannelName() const;
    QString getCustomName() const;
    void setChannel(QSharedPointer<Channel> channel);
    void setCustomName(const QString &customName);
    QSharedPointer<DataMark> getLastMark();
    void setLastDataMark(QSharedPointer<DataMark> datamark);
    void setStatus(STATUS_TYPE status);
    STATUS_TYPE getStatus() const;
    signals:
    void contactChanged();

};
#endif                                  // CONTACT_H
