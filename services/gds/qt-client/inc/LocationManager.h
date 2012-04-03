#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H

#include <QObject>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QMutex>

QTM_USE_NAMESPACE

class LocationManager : public QObject
{
    Q_OBJECT

    QGeoPositionInfoSource *m_source;
    QGeoPositionInfo m_info;
    QMutex m_infoMutex;

public:
    explicit LocationManager(QObject *parent = 0);
    QGeoPositionInfo getInfo();
    bool isInfoValid();


signals:

public slots:
    void positionUpdated(const QGeoPositionInfo &info);

};

#endif // LOCATIONMANAGER_H
