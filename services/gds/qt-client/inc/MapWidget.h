#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <qgraphicsgeomap.h>
#include <qgeomapcircleobject.h>
#include <qgeomappingmanager.h>
#include <qgeocoordinate.h>

#include <QGraphicsSceneMouseEvent>
#include <QTime>

QTM_USE_NAMESPACE

class MapWidget : public QGraphicsGeoMap
{
  Q_OBJECT

    public:
    MapWidget(QGeoMappingManager *manager);
    ~MapWidget();

  public slots:
    void setMouseClickCoordQuery(bool state);

    signals:
    void coordQueryResult(const QGeoCoordinate &coord);
    void clicked(QPointF pos);

  private slots:
    void kineticTimerEvent();

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent* event);
    void wheelEvent(QGraphicsSceneWheelEvent* event);

  private:
    bool coordQueryState;
    bool panActive;
    bool panDecellerate;

    // Fractional pan, used by panFloatWrapper
    QPointF remainingPan;

    // current kinetic panning speed, in pixel/msec
    QPointF kineticPanSpeed;
    QPoint panDir;
    QTimer *kineticTimer;
    QTime lastMoveTime;

    // An entry in the mouse history. first=speed, second=time
    typedef QPair<QPointF, QTime> MouseHistoryEntry;
    // A history of the last (currently 5) mouse move events is stored in order to smooth out movement detection for kinetic panning
    QList<MouseHistoryEntry> mouseHistory;

    void panFloatWrapper(const QPointF& delta);
    void applyPan(const Qt::KeyboardModifiers& modifiers);

  public:
    QGeoMapCircleObject *lastCircle;
};
#endif                                  /* MAPWIDGET_H */
