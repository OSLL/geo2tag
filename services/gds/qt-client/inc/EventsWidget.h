/*
 * Copyright 2012  Ivan Bezyazychnyy  ivan.bezyazychnyy@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

#ifndef EVENTSWIDGET_H
#define EVENTSWIDGET_H

#include <QWidget>
#include <QGeoMappingManager>
#include <QGeoServiceProvider>
#include <QGeoMapPixmapObject>
#include <QGraphicsView>
#include <QSlider>
#include <QThread>
#include <QDateTime>

#include "LocationManager.h"
#include "EventsService.h"
#include "MapWidget.h"
#include "Settings.h"

class EventsWidget : public QWidget
{
  Q_OBJECT

    QGeoServiceProvider *m_serviceProvider;
  QGeoMappingManager *m_mapManager;

  MapWidget *m_mapWidget;
  QGraphicsView *m_graphicsView;
  QSlider *m_slider;

  QDateTime m_lastCentered;

  LocationManager *m_locationManager;
  EventsService m_eventsService;
  QThread m_eventsThread;
  Settings m_settings;

  QPixmap m_positionIcon;
  QPoint m_positionOffset;
  QGeoMapPixmapObject *m_positionObject;

  QPixmap m_eventIcon;
  QPoint m_eventOffset;
  QList<QGeoMapPixmapObject*> m_eventObjects;
  Events m_events;

  public:
    explicit EventsWidget(LocationManager *locationManager, QWidget *parent = 0);
    ~EventsWidget();

    signals:

  public slots:
    void startEventsService();
    void stopEventsService();
    void sliderValueChanged(int zoomLevel);
    void mapZoomLevelChanged(qreal zoomLevel);
    void onMapWidgetClicked(QPointF pos);
    void onEventsReceived(Events events);
    void onEventsThreadStarted();
    void onEventsServiceError(QString error);
    void onPositionUpdated();
    QGeoCoordinate getCenter();
    void setCenter(QGeoCoordinate coordinate);
    void onSettingsUpdated();

  private:
    void initGUI();

};
#endif                                  // EVENTSWIDGET_H
