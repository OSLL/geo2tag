#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QBasicTimer>
#include <QHash>
#include <QVector>
#include "MapsUploader.h"
#include "DataChannel.h"
#include "MapsUploadThread.h"


class MapScene : public QGraphicsScene
{
    Q_OBJECT
	
private:
    QHash<TilePoint, QGraphicsPixmapItem *> m_maps;
	QVector<QGraphicsPixmapItem *> m_marks;
    MapsUploader * m_uploader;
	MapsUploadThread * m_map_uploader;

    int m_zoom;
    qreal m_latitude;
    qreal m_longitude;

    QPoint pressed_screen_position;

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

signals:
    void uploadTiles(QVector<TilePoint> & tiles_to_upload);

public slots:
    void tileUploaded(const QPixmap & pixmap, const QPoint & point, int zoom);
	void preFetch();

};

#endif // MAPSCENE_H
