#ifndef SLIPPYMAP_H
#define SLIPPYMAP_H

#include <QObject>
#include <QPointF>
#include <QRect>
#include <QHash>
#include <QPoint>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QUrl>

/*
 * Calculate tile's center for geographic coordinates
 */
QPointF tileForCoordinate(qreal lat, qreal lng, int zoom);

/*
 * Convert tile's coordinate to Longitude
 */
qreal longitudeFromTile(qreal tx, int zoom);

/*
 * Convert tile's coordinate to Latitude
 */
qreal latitudeFromTile(qreal ty, int zoom);

/*
 * Map
 */
class SlippyMap: public QObject
{
  Q_OBJECT

    QPoint m_offset;
  QRect m_tilesRect;
  QPixmap m_emptyTile;
  QHash<QPoint, QPixmap> m_tilePixmaps;
  QNetworkAccessManager m_manager;
  QUrl m_url;

  public:
    int width;
    int height;
    int zoom;
    qreal latitude;
    qreal longitude;

    SlippyMap(QObject *parent = 0);

    void invalidate();

    void render(QPainter *p, const QRect &rect);

    void pan(const QPoint &delta);

  private slots:

    void handleNetworkData(QNetworkReply *reply);

    void download();

    signals:
    void updated(const QRect &rect);

  protected:
    QRect tileRect(const QPoint &tp);

    };
#endif                                                      // SLIPPYMAP_H
