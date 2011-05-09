/*
 * Copyright 2010  OSLL osll@osll.spb.ru
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
/*  */
/*!
 * \file MapWidget.h
 * \brief Header of MapWidget
 * \todo add comment here
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _MapWidget_H_065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_
#define _MapWidget_H_065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QDebug>
#include <QtGui/QImage>
#include <QtGui/QSlider>
#include <QtCore/QTimer>
#include "MapLoader.h"
#include "OnLineInformation.h"
#include "SlippyMap.h"

namespace GUI
{
  const double DEFAULT_POSITION_B = 59.939145;
  const double DEFAULT_POSITION_L = 30.311642;
  const int    SCALE_DEFAULT      = 10;
  const int    SCALE_MIN          = 2;
  const int    SCALE_MAX          = 20;
  const int    SCALE_RATIO        = 10;

  /*!
   * Class description. May use HTML formatting
   *
   */
  class MapWidget : public QWidget
  {
    Q_OBJECT

      double m_B;
    double m_L;
    double m_scale;
    QPointF m_mousePosition;
    bool m_moving;
    QTimer m_updateGpsDataTimer;
    QTimer m_delay;
    maps::MapLoader::SourceType sourceType;
    qreal m_markerDx;
    qreal m_markerDy;

    public:

      MapWidget(QWidget *parent, double b = DEFAULT_POSITION_B , double l = DEFAULT_POSITION_L, double scale = SCALE_DEFAULT);

      virtual ~MapWidget()
      {
      }

      void setB(double b);
      void setL(double l);
      void setScale(int scale);

    protected:
      void paintEvent(QPaintEvent *pe);
      void resizeEvent(QResizeEvent *re);
      void mouseMoveEvent ( QMouseEvent * event );
      void mousePressEvent( QMouseEvent * event );
      void mouseReleaseEvent(QMouseEvent * event );

    public slots:
      void updateMap();
      void updateGpsData();
      void onMapUpdated(QByteArray & picture);
      void setSourceType(maps::MapLoader::SourceType sourceType);
      void onDelayTimeout();

    private:
      MapWidget(const MapWidget& obj);
      MapWidget& operator=(const MapWidget& obj);
      // class MapWidget
  };

  // namespace GUI
}


//_MapWidget_H_065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_
#endif

/* ===[ End of file  ]=== */
