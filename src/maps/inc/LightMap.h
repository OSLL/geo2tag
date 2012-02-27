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

#ifndef LIGHTMAP_H
#define LIGHTMAP_H

#include <QWidget>
#include <QPixmap>
#include <QBasicTimer>
#include "DataChannel.h"
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

  void drawMarks(QPainter& painter);
  void drawMarkIco(QPainter& painter, QPointF& position, QSharedPointer<DataMark> mark, QSharedPointer<Channel> channel);

  DataChannels m_marks;
  private slots:
    void updateMap(const QRect &r);

  protected:

    void activateZoom();

    void resizeEvent(QResizeEvent *);

    void paintEvent(QPaintEvent *event);

    void timerEvent(QTimerEvent *);

    void wheelEvent(QWheelEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *);

    void keyPressEvent(QKeyEvent *event);

  public:
    LightMap(QWidget *parent = 0);

    void setMarks(DataChannels marks);

    void setCenter(qreal lat, qreal lng);

  public slots:
    void toggleNightMode();

};
// LIGHTMAP_H
#endif
