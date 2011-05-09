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
/*! ---------------------------------------------------------------
 *
 *
 * \file MapWidget.cpp
 * \brief MapWidget implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "MapWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QTimer>
#include "MapLoaderFactory.h"
#include "MapLoader.h"
#include "Picture.h"
//#include "TabWidget.h"
#include "GpsInfo.h"
#include <QDebug>
#include "OnLineInformation.h"
#include "math.h"

#ifndef DEFAULT_SOURCE_TYPE
#define DEFAULT_SOURCE_TYPE GOOGLE
#endif

namespace GUI
{

  MapWidget::MapWidget(QWidget *parent, double b, double l, double scale) : QWidget(parent)
  {
    m_B = b;
    m_L = l;
    m_scale = scale;
    m_moving = false;
    sourceType = maps::MapLoader::DEFAULT_SOURCE_TYPE;
    m_markerDx = 0;
    m_markerDy = 0;

    connect(&m_updateGpsDataTimer, SIGNAL(timeout()),this, SLOT(updateGpsData()));
    connect(&m_delay, SIGNAL(timeout()), this, SLOT(onDelayTimeout()));

    using namespace maps;
    connect(MapLoaderFactory::getInstance(maps::MapLoader::GOOGLE), SIGNAL(mapUpdated(QByteArray &)),
      this, SLOT(onMapUpdated(QByteArray&)));
    connect(MapLoaderFactory::getInstance(maps::MapLoader::OPEN_STREET), SIGNAL(mapUpdated(QByteArray &)),
      this, SLOT(onMapUpdated(QByteArray&)));

    m_updateGpsDataTimer.setInterval(8000);
    m_updateGpsDataTimer.start();
    m_delay.setInterval(300);
    m_delay.start();

    //updateMap();
  }

  void MapWidget::paintEvent(QPaintEvent *pe)
  {
    QPainter painter( this );

    using namespace common;
    using namespace maps;

    qDebug() << "loading map for b=" << m_L << ", " << m_B;

    QByteArray picture = MapLoaderFactory::getInstance(sourceType)->getMapByteArray();
    painter.drawImage(pe->rect(),QImage::fromData(picture));

    qDebug() << "| paint event done |";
  }

  void MapWidget::resizeEvent(QResizeEvent *re)
  {
    updateMap();
  }

  void MapWidget::mousePressEvent ( QMouseEvent * event )
  {
    qDebug() << "Pres";
    m_moving = true;
    m_mousePosition = event->posF();
  }

  void MapWidget::updateGpsData()
  {
    m_B=common::GpsInfo::getInstance().getLatitude();
    m_L=common::GpsInfo::getInstance().getLongitude();
    qDebug() << "timer..." << m_B << " " << m_L;
    m_markerDx = 0;
    m_markerDy = 0;

    updateMap();
  }

  void MapWidget::mouseReleaseEvent ( QMouseEvent * event )
  {
    m_moving = false;
    QPointF currentPos = event->posF();

    double dl = (currentPos.x() - m_mousePosition.x())/(100.0*m_scale);
    double db = (currentPos.y() - m_mousePosition.y())/(100.0*m_scale);

    m_B += db;
    m_L -= dl;

    updateMap();

  }

  void MapWidget::mouseMoveEvent    ( QMouseEvent * event )
  {
    int dl = event->x();
    int db = event->y();
    qDebug() << "move db=" << db << ", dl" << dl;
  }

  void MapWidget::setB(double b)
  {
    m_B = b;
    qDebug() << "new B=" << m_B;
  }

  void MapWidget::setL(double l)
  {
    m_L = l;
    qDebug() << "new L=" << m_L;
  }

  void MapWidget::setScale(int scale)
  {
    m_scale = scale ;
    qDebug() << "new cale" << m_scale;
  }

  void MapWidget::updateMap()
  {
    qDebug() << "MapWidget::updateMap";

    if (OnLineInformation::getInstance().getMarks() != NULL)
    {
      qDebug() <<rect().width() << " " << rect().height();
      maps::MapLoaderFactory::getInstance(sourceType)->getMapWithMarks(
        m_B,m_L,m_scale,rect().width(),rect().height(),
        *OnLineInformation::getInstance().getMarks());
    }
    else
    {
      qDebug() << "OnLineInformation::getInstance().getMarks() == NULL!!!!!!!!!!!!!!";
      maps::MapLoaderFactory::getInstance(sourceType)->getMap(m_B,m_L,m_scale,
        rect().width(),rect().height());
    };
  }

  void MapWidget::onMapUpdated(QByteArray &picture)
  {
    qDebug("onMapUpdated");
    emit repaint();
  }

  void MapWidget::setSourceType(maps::MapLoader::SourceType sourceType)
  {
    if (this->sourceType != sourceType)
    {
      this->sourceType = sourceType;
      if (sourceType == maps::MapLoader::GOOGLE)
      {
        qDebug() << "Google maps source type set";
      }
      if (sourceType == maps::MapLoader::OPEN_STREET)
      {
        qDebug() << "Open Street maps source type set";
      }
      updateMap();
    }
  }

  void MapWidget::onDelayTimeout()
  {
    updateMap();
    m_delay.stop();
  }

}                                       // namespace GUI


/* ===[ End of file  ]=== */
