#include "inc/LocationManager.h"
#include <QDebug>
#include "defines.h"

LocationManager::LocationManager(QObject *parent) :
    QObject(parent)
{
    m_source = QGeoPositionInfoSource::createDefaultSource(this);
    if (m_source) {
        connect(m_source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
        m_source->setUpdateInterval(DEFAULT_LOCATION_TIMEOUT * 1000);
        m_source->startUpdates();
    } else {
        qDebug() << "Can't get geo source";
    }
}

QGeoPositionInfo LocationManager::getInfo()
{
    m_infoMutex.lock();
    QGeoPositionInfo info = m_info;
    m_infoMutex.unlock();
    return info;
}

void LocationManager::positionUpdated(const QtMobility::QGeoPositionInfo &info)
{
    m_infoMutex.lock();
    m_info = info;
    qDebug() << "Position updated:" << m_info;
    m_infoMutex.unlock();
}

