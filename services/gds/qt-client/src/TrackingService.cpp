#include "inc/TrackingService.h"
#include "JsonUser.h"

#include <QTimer>
#include <QDebug>

TrackingService::TrackingService(LocationManager *locationManager, QObject *parent) :
    QObject(parent),
    m_locationManager(locationManager)
{
    m_dataMark = QSharedPointer<DataMark>(
                new DataMark(0, 0, 0, "Tracking", "Tracking", "", QDateTime::currentDateTime()));   
}

void TrackingService::startTracking(QString name, QString password, QString authToken, QString serverUrl)
{
    qDebug() << "startTracking url: " << serverUrl;

    if (m_writeTagQuery != 0)
        m_writeTagQuery->deleteLater();

    m_writeTagQuery = new WriteTagQuery(this);
    m_writeTagQuery->setTag(m_dataMark);
    connect(m_writeTagQuery, SIGNAL(tagAdded()), this, SLOT(onMarkSent()));
    connect(m_writeTagQuery, SIGNAL(errorOccured(int)), this, SLOT(onErrorOccured(int)));

    m_user = QSharedPointer<JsonUser>(new JsonUser(name, password, authToken));
    m_channel = QSharedPointer<Channel>(new Channel(name, name + "'s channel", ""));
    m_dataMark->setUser(m_user);
    m_dataMark->setChannel(m_channel);
    m_writeTagQuery->setUrl(serverUrl);

    sendMark();
}

void TrackingService::stopTracking()
{
}

void TrackingService::sendMark()
{
    qDebug() << "sendMark";
    QGeoPositionInfo info = m_locationManager->getInfo();
    if (info.isValid()) {
        m_dataMark->setLatitude(info.coordinate().latitude());
        m_dataMark->setLongitude(info.coordinate().longitude());
        m_dataMark->setTime();
        m_writeTagQuery->doRequest();
    } else {
        qDebug() << "invalid geo info, waitin and trying again";
        QTimer::singleShot(5 * 1000, this, SLOT(sendMark()));
    }
}

void TrackingService::onMarkSent()
{
    qDebug() << "TrackingService::onMarkSent";
    emit markSent(QPointF(m_dataMark->getLatitude(), m_dataMark->getLongitude()));
    QTimer::singleShot(5 * 1000, this, SLOT(sendMark()));
}

void TrackingService::onError(QString error)
{
    qDebug() << "TrackingService::onErrorOccured error: " << error;
    emit errorOccured(error);
    QTimer::singleShot(5 * 1000, this, SLOT(sendMark()));
}
