#ifndef EVENTSWIDGET_H
#define EVENTSWIDGET_H

#include <QWidget>
#include <QGeoMappingManager>
#include <QGeoServiceProvider>
#include <QGeoMapPixmapObject>
#include <QGraphicsView>
#include <QSlider>
#include <QThread>

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

    LocationManager *m_locationManager;
    EventsService m_eventsService;
    QThread m_eventsThread;
    Settings m_settings;


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

private:
    void initGUI();

};

#endif // EVENTSWIDGET_H
