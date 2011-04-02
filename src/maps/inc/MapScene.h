#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QBasicTimer>
#include <QHash>
#include <QPoint>
#include "MapsUploader.h"
#include "DataChannel.h"


class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapScene(QObject *parent = 0);

public:
    void addMark(qreal x, qreal y, QVariant data);
    void addMark(qreal x, qreal y, QVariant data, QWidget * widget);
    void removeMark(QGraphicsItem * mark);
	void setMarks(DataChannels marks);

public:
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    void set_zoom();

private:
    void update_state();
	void add_mark(QPointF pos, QString channel_name);

    QPointF convertCoordinates(qreal lat, qreal lng, int zoom);

    int long2tilex(qreal lon, int z);
    int lat2tiley(qreal lat, int z);
    qreal tilex2long(int x, int z);
    qreal tiley2lat(int y, int z);
    qreal tilex2long(qreal x, int z);
    qreal tiley2lat(qreal y, int z);

private:

    QHash<TilePoint, QGraphicsPixmapItem *> m_maps;
	QHash<TilePoint, QGraphicsPixmapItem *> m_marks;
    MapsUploader * m_uploader;

    int m_zoom;
    qreal m_latitude;
    qreal m_longitude;

    QPoint pressed_screen_position;

signals:
    void uploadTiles(QVector<TilePoint> tiles_to_upload);

public slots:
    void tileUploaded(const QPixmap pixmap, const QPoint point, const int zoom);

};

#endif // MAPSCENE_H
