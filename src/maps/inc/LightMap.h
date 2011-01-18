#ifndef LIGHTMAP_H
#define LIGHTMAP_H

#include <QWidget>
#include <QPixmap>
#include <QBasicTimer>

#include "SlippyMap.h"
class LightMap: public QWidget
{
    Q_OBJECT

    SlippyMap *m_normalMap;
    SlippyMap *m_largeMap;

    bool pressed;
    bool snapped;
    QPoint pressPos;
    QPoint dragPos;
    QBasicTimer tapTimer;
    bool zoomed;
    QPixmap zoomPixmap;
    QPixmap maskPixmap;
    bool invert;

private slots:
    void updateMap(const QRect &r);

protected:

    void activateZoom();

    void resizeEvent(QResizeEvent *);

    void paintEvent(QPaintEvent *event);

    void timerEvent(QTimerEvent *);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *);

    void keyPressEvent(QKeyEvent *event);


public:
    LightMap(QWidget *parent = 0);

    void setCenter(qreal lat, qreal lng);

public slots:
    void toggleNightMode();

};

#endif // LIGHTMAP_H