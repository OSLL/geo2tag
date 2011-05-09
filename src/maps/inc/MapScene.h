#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QHash>
#include <QVector>

#include "MapsUploader.h"
#include "MapsUploadThread.h"
#include "Preloading.h"

#include "DataChannel.h"

class MapScene : public QGraphicsScene
{
  Q_OBJECT

    private:
    QHash<TilePoint, QGraphicsPixmapItem *> m_tiles;
    QVector<QGraphicsPixmapItem *> m_marks;
    MapsUploader * m_uploader;
    Preloading * m_preloader;

    int m_zoom;
    qreal m_latitude;
    qreal m_longitude;

    QPoint m_pressed_screen_position;

  public:
    explicit MapScene(QObject *parent = 0);
    ~MapScene();

  public:
    void addMark(qreal x, qreal y, QVariant data);
    void addMark(qreal x, qreal y, QVariant data, QWidget * widget);
    void removeMark(QGraphicsItem * mark);
    void setMarks(DataChannels marks);

    int maxThreads() const;
    void setMaxThreads(const int & max_threads);

  public:
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    void set_zoom();
    void update_state();

  private:
    void add_mark(QPointF pos,QSharedPointer<DataMark> mark,QSharedPointer<Channel> channel);

    signals:
    void uploadTiles(QVector<TilePoint> & tiles_to_upload);

  public slots:
    void tileUploaded(const QPixmap & pixmap, const TilePoint & point);
    void preload();

};
// MAPSCENE_H
#endif
