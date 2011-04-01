#include "MapsUploader.h"
#include <math.h>
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QVector>
#include <QVectorIterator>
#include <QDebug>


QPointF tileForCoordinate(qreal lat, qreal lng, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal tx = (lng + 180.0) / 360.0;
    qreal ty = (1.0 - log(tan(lat * M_PI / 180.0) +
                          1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0;
    return QPointF(tx * zn, ty * zn);
}

qreal longitudeFromTile(qreal tx, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal lat = tx / zn * 360.0 - 180.0;
    return lat;
}

qreal latitudeFromTile(qreal ty, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal n = M_PI - 2 * M_PI * ty / zn;
    qreal lng = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
    return lng;
}

MapsUploader::MapsUploader(QObject *parent) :
    QObject(parent)
{
    QNetworkDiskCache *cache = new QNetworkDiskCache;
    cache->setCacheDirectory(QDesktopServices::storageLocation
                             (QDesktopServices::CacheLocation));

    m_manager.setCache(cache);
    connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleNetworkData(QNetworkReply*)));
}


void MapsUploader::handleNetworkData(QNetworkReply *reply)
{
    QImage img;
    QList<QVariant> data = reply->request().attribute(QNetworkRequest::User).toList();
    QPoint tp = data.front().toPoint();
    int zoom = data.back().toInt();
    QUrl url = reply->url();
    if (!reply->error())
        if (!img.load(reply, 0))
            img = QImage();
    reply->deleteLater();

    if(!img.isNull())
    {
        QPixmap pixmap = QPixmap::fromImage(img);
        emit this->tileUploaded(pixmap, tp, zoom);
    }
}

void MapsUploader::downloadTile(int lat, int lg, int zoom)
{
    QPoint grab(lg, lat);

    QString path = "http://tile.openstreetmap.org/%1/%2/%3.png";
    m_url = QUrl(path.arg(zoom).arg(lg).arg(lat));
    //qDebug() << m_url << "\n";
    QNetworkRequest request;
    request.setUrl(m_url);
    request.setRawHeader("User-Agent", "OSLL Observer");
    QList<QVariant> data;
    data.push_back(QVariant(grab));
    data.push_back(QVariant(zoom));
    request.setAttribute(QNetworkRequest::User, QVariant(data));
    m_manager.get(request);
}


void MapsUploader::uploadTiles(QVector<TilePoint> tiles_to_upload)
{
    for(int i = 0; i < tiles_to_upload.size(); i++)
    {
        TilePoint tp = tiles_to_upload.at(i);
        if(m_loaded.contains(tp))
            continue;

        QPoint p = tp.first;
        int zoom = tp.second;
        this->downloadTile(p.x(), p.y(), zoom);

        m_loaded.push_back(tp);
    }
}
