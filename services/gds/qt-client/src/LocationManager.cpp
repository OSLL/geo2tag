#include "inc/LocationManager.h"
#include <QDebug>
#include <QStringList>
#include "defines.h"

LocationManager::LocationManager(QObject *parent) :
QObject(parent)
{
  m_satelliteSource = QGeoPositionInfoSource::createDefaultSource(this);
  qDebug() << "satellite source: " << m_satelliteSource;
  if (m_satelliteSource)
  {
    connect(m_satelliteSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
      this, SLOT(satellitePositionUpdated(QGeoPositionInfo)));
    m_satelliteSource->setPreferredPositioningMethods(
      QGeoPositionInfoSource::SatellitePositioningMethods);
    m_satelliteSource->startUpdates();
  }
  else
  {
    qDebug() << "Can't get satellite source";
  }

  m_nonSatelliteSource = QGeoPositionInfoSource::createDefaultSource(this);
  qDebug() << "non satellite source: " << m_nonSatelliteSource;
  if (m_nonSatelliteSource)
  {
    connect(m_nonSatelliteSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
      this, SLOT(nonSatellitePositionUpdated(QGeoPositionInfo)));
    m_nonSatelliteSource->setPreferredPositioningMethods(
      QGeoPositionInfoSource::NonSatellitePositioningMethods);
    m_nonSatelliteSource->startUpdates();
  }
  else
  {
    qDebug() << "Can't get non satellite source";
  }

}


QGeoPositionInfo LocationManager::getInfo()
{
  m_infoMutex.lock();
  QGeoPositionInfo info;
  if (m_satelliteInfo.isValid() && !m_nonSatelliteInfo.isValid())
  {
    info = m_satelliteInfo;
  }
  else if (!m_satelliteInfo.isValid() && m_nonSatelliteInfo.isValid())
  {
    info = m_nonSatelliteInfo;
  }
  else if (m_satelliteInfo.isValid() && m_nonSatelliteInfo.isValid())
  {
    if (m_satelliteInfo.timestamp() >= m_nonSatelliteInfo.timestamp().addSecs(-60))
    {
      info = m_satelliteInfo;
    }
    else
    {
      info = m_nonSatelliteInfo;
    }
  }
  m_infoMutex.unlock();
  return info;
}


void LocationManager::satellitePositionUpdated(const QtMobility::QGeoPositionInfo &info)
{
  m_infoMutex.lock();
  m_satelliteInfo = info;
  //qDebug() << "Satellite position updated:" << m_satelliteInfo;
  m_infoMutex.unlock();

  emit positionUpdated();
}


void LocationManager::nonSatellitePositionUpdated(const QtMobility::QGeoPositionInfo &info)
{
  m_infoMutex.lock();
  m_nonSatelliteInfo = info;
  //qDebug() << "Non satellite position updated:" << m_nonSatelliteInfo;
  m_infoMutex.unlock();

  emit positionUpdated();
}
