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

#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QPainter>
#include <QNetworkReply>

#include "SlippyMap.h"
#include "defines.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// tile size in pixels
static const int tdim = 256;

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


SlippyMap::SlippyMap(QObject *parent) :
QObject(parent),
width(400),
height(300),
zoom(15),
latitude(59.9138204),
longitude(10.7387413)
{
  m_emptyTile = QPixmap(tdim, tdim);
  m_emptyTile.fill(Qt::lightGray);

  QNetworkDiskCache *cache = new QNetworkDiskCache;
  cache->setCacheDirectory(QDesktopServices::storageLocation
    (QDesktopServices::CacheLocation));
  m_manager.setCache(cache);
  connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
    this, SLOT(handleNetworkData(QNetworkReply*)));
}


void SlippyMap::invalidate()
{
  if (width <= 0 || height <= 0)
    return;

  QPointF ct = tileForCoordinate(latitude, longitude, zoom);
  qreal tx = ct.x();
  qreal ty = ct.y();

  // top-left corner of the center tile
  int xp = width / 2 - (tx - floor(tx)) * tdim;
  int yp = height / 2 - (ty - floor(ty)) * tdim;

  // first tile vertical and horizontal
  int xa = (xp + tdim - 1) / tdim;
  int ya = (yp + tdim - 1) / tdim;
  int xs = static_cast<int>(tx) - xa;
  int ys = static_cast<int>(ty) - ya;

  // offset for top-left tile
  m_offset = QPoint(xp - xa * tdim, yp - ya * tdim);

  // last tile vertical and horizontal
  int xe = static_cast<int>(tx) + (width - xp - 1) / tdim;
  int ye = static_cast<int>(ty) + (height - yp - 1) / tdim;

  // build a rect
  m_tilesRect = QRect(xs, ys, xe - xs + 1, ye - ys + 1);

  if (m_url.isEmpty())
    download();

  emit updated(QRect(0, 0, width, height));
}


void SlippyMap::render(QPainter *p, const QRect &rect)
{
  for (int x = 0; x <= m_tilesRect.width(); ++x)
    for (int y = 0; y <= m_tilesRect.height(); ++y)
  {
    QPoint tp(x + m_tilesRect.left(), y + m_tilesRect.top());
    QRect box = tileRect(tp);
    if (rect.intersects(box))
    {
      if (m_tilePixmaps.contains(tp))
        p->drawPixmap(box, m_tilePixmaps.value(tp));
      else
        p->drawPixmap(box, m_emptyTile);
    }
  }
}


void SlippyMap::pan(const QPoint &delta)
{
  QPointF dx = QPointF(delta) / qreal(tdim);
  QPointF center = tileForCoordinate(latitude, longitude, zoom) - dx;
  latitude = latitudeFromTile(center.y(), zoom);
  longitude = longitudeFromTile(center.x(), zoom);
  invalidate();
}


void SlippyMap::handleNetworkData(QNetworkReply *reply)
{
  QImage img;
  QPoint tp = reply->request().attribute(QNetworkRequest::User).toPoint();
  QUrl url = reply->url();
  if (!reply->error())
    if (!img.load(reply, 0))
      img = QImage();
  reply->deleteLater();
  m_tilePixmaps[tp] = QPixmap::fromImage(img);
  if (img.isNull())
    m_tilePixmaps[tp] = m_emptyTile;
  emit updated(tileRect(tp));

  // purge unused spaces
  QRect bound = m_tilesRect.adjusted(-2, -2, 2, 2);
  foreach(QPoint tp, m_tilePixmaps.keys())
    if (!bound.contains(tp))
    m_tilePixmaps.remove(tp);

  download();
}


void SlippyMap::download()
{
  QPoint grab(0, 0);
  for (int x = 0; x <= m_tilesRect.width(); ++x)
    for (int y = 0; y <= m_tilesRect.height(); ++y)
  {
    QPoint tp = m_tilesRect.topLeft() + QPoint(x, y);
    if (!m_tilePixmaps.contains(tp))
    {
      grab = tp;
      break;
    }
  }
  if (grab == QPoint(0, 0))
  {
    m_url = QUrl();
    return;
  }

  QString path = "http://tile.openstreetmap.org/%1/%2/%3.png";
  m_url = QUrl(path.arg(zoom).arg(grab.x()).arg(grab.y()));
  QNetworkRequest request;
  request.setUrl(m_url);
  request.setRawHeader("User-Agent", "OSLL Observer");
  request.setAttribute(QNetworkRequest::User, QVariant(grab));
  m_manager.get(request);
}


QRect SlippyMap::tileRect(const QPoint &tp)
{
  QPoint t = tp - m_tilesRect.topLeft();
  int x = t.x() * tdim + m_offset.x();
  int y = t.y() * tdim + m_offset.y();
  return QRect(x, y, tdim, tdim);
}
