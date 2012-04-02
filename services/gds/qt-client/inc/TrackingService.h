#ifndef TRACKINGSERVICE_H
#define TRACKINGSERVICE_H

#include <QPointF>
#include "WriteTagQuery.h"
#include "LocationManager.h"
#include "DataMarks.h"

class TrackingService : public QObject
{
    Q_OBJECT

    WriteTagQuery *m_writeTagQuery;
    LocationManager *m_locationManager;
    QSharedPointer<DataMark> m_dataMark;
    QSharedPointer<common::User> m_user;
    QSharedPointer<Channel> m_channel;

public:
    explicit TrackingService(LocationManager *locationManager, QObject *parent = 0);

public slots:
    void startTracking(QString name, QString password, QString authToken, QString serverUrl);
    void stopTracking();

signals:
    void markSent(QPointF coordinates);
    void errorOccured(QString error);

private slots:
    void sendMark();
    void onMarkSent();
    void onError(QString error);

};

#endif // TRACKINGSERVICE_H
