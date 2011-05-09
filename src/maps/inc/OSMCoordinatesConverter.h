#ifndef OSMCOORDINATESCONBERTER_H
#define OSMCOORDINATESCONBERTER_H

#include <math.h>
#include <QPoint>
#include <QPointF>
#include <QPair>
#include <QDebug>

// x,y,zoom - OSM tile coordinate
typedef QPair<QPoint, int> TilePoint;
// x,y,zoom - OSM point on tile coordinate
typedef QPair<QPointF, int> TilePointF;
// longitude, latitude - geographics coordinates
typedef QPair<qreal, qreal> GeoPoint;

/*
 * For use QPoint in QHash
 * Make unique hash for each point
 */
uint qHash(const QPoint & p);

class OSMCoordinatesConverter
{
  private:
    /*Full circle degrees*/
    static const qreal circle_full = 360.0;

    /*Half circle degrees*/
    static const qreal circle_half = 180.0;
  public:

    /* Convert latitude, longitude to OSM tile for zoom */
    static QPointF GeoToTile(qreal latitude, qreal longitude, int zoom);

    /* Convert TilePointF to geographics coordinates*/
    static GeoPoint TileToGeo(const TilePointF & point);
};
// OSMCOORDINATESCONBERTER_H
#endif
