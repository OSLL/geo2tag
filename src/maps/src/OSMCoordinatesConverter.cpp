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
