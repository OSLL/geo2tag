/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

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
