#include "OSMCoordinatesConverter.h"
#include <QHash>


uint qHash(const QPoint & p)
{
    return qHash(qMakePair(p.x(),p.y()));
}


QPointF OSMCoordinatesConverter::GeoToTile(qreal latitude, qreal longitude, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal tx = (longitude + circle_half) / circle_full;
    qreal ty = (1.0 - log(tan(latitude * M_PI / circle_half) +
                          1.0 / cos(latitude * M_PI / circle_half)) / M_PI) / 2.0;
    return QPointF(tx * zn, ty * zn);
}

GeoPoint OSMCoordinatesConverter::TileToGeo(const TilePointF & point)
{
    qreal longitude = point.first.x() / pow(2.0, point.second) * circle_full - circle_half;
    qreal latitude = M_PI - 2.0 * M_PI * point.first.y() / pow(2.0, point.second);
    latitude = circle_half / M_PI * atan(0.5 * (exp(latitude) - exp(-latitude)));

    return qMakePair(longitude, latitude);
}
