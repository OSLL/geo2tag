#include "inc/EventsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QDebug>
#include "EventsService.h"
#include "defines.h"

EventsWidget::EventsWidget(LocationManager *locationManager, QWidget *parent) :
QWidget(parent),
m_locationManager(locationManager),
m_eventsService(locationManager),
m_positionIcon(":/data/position32.png"),
m_positionOffset(-16, - 16),
m_positionObject(0),
m_eventIcon(":/data/event64.png"),
m_eventOffset(-32, -32)
{
  m_serviceProvider = new QGeoServiceProvider("nokia");
  if (m_serviceProvider->error() != QGeoServiceProvider::NoError)
  {
    QMessageBox::information(this, tr("GeoDoctorSearch"), tr(
      "Unable to find the %1 geoservices plugin.").arg("nokia"));
    return;
  }

  m_mapManager = m_serviceProvider->mappingManager();
  m_mapWidget = new MapWidget(m_mapManager);

  QGraphicsScene* scene = new QGraphicsScene(this);
  m_graphicsView = new QGraphicsView(scene, this);
  //m_graphicsView = new QGraphicsView(scene);
  m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_graphicsView->setVisible(true);
  m_graphicsView->setInteractive(true);
  m_graphicsView->scene()->addItem(m_mapWidget);

  m_slider = new QSlider(Qt::Vertical, this);
  m_slider->setTickInterval(1);
  m_slider->setTickPosition(QSlider::TicksBothSides);
  m_slider->setMaximum(m_mapManager->maximumZoomLevel());
  m_slider->setMinimum(m_mapManager->minimumZoomLevel());
  m_slider->setSliderPosition(m_mapWidget->zoomLevel());
  m_slider->setMaximumWidth(15);

  m_mapWidget->setCenter(QGeoCoordinate(60, 30));
  m_mapWidget->setZoomLevel(5);

  initGUI();

  connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
  connect(m_mapWidget, SIGNAL(zoomLevelChanged(qreal)), this, SLOT(mapZoomLevelChanged(qreal)));
  connect(m_mapWidget, SIGNAL(clicked(QPointF)), this, SLOT(onMapWidgetClicked(QPointF)));
  connect(m_locationManager, SIGNAL(positionUpdated()),
    this, SLOT(onPositionUpdated()));

  /* Initialize events thread */
  connect(&m_eventsThread, SIGNAL(started()),
    this, SLOT(onEventsThreadStarted()));
  m_eventsService.moveToThread(&m_eventsThread);
  qRegisterMetaType<Events>("Events");
  connect(&m_eventsService, SIGNAL(eventsReceived(Events)),
    this, SLOT(onEventsReceived(Events)));
}


EventsWidget::~EventsWidget()
{
  stopEventsService();
  delete m_serviceProvider;
}


void EventsWidget::initGUI()
{
  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(m_graphicsView);
  mainLayout->addWidget(m_slider);
  this->setLayout(mainLayout);
}


void EventsWidget::startEventsService()
{
  m_eventsThread.start();
  QMetaObject::invokeMethod(&m_eventsService, "startService", Qt::QueuedConnection,
    Q_ARG(QString, m_settings.getLogin()),
    Q_ARG(QString, m_settings.getPassword()),
    Q_ARG(QString, m_settings.getAuthToken()),
    Q_ARG(QString, m_settings.getServerUrl()));
}


void EventsWidget::stopEventsService()
{
  m_eventsThread.exit();
  m_eventsThread.wait();
}


void EventsWidget::sliderValueChanged(int zoomLevel)
{
  QGeoCoordinate center = getCenter();
  m_mapWidget->setZoomLevel(zoomLevel);
  setCenter(center);
}


void EventsWidget::mapZoomLevelChanged(qreal zoomLevel)
{
  m_slider->setSliderPosition(qRound(zoomLevel));
}


void EventsWidget::onMapWidgetClicked(QPointF pos)
{
  qDebug() << "Clicked, x=" << pos.x() << ", y=" << pos.y();
  QGeoCoordinate click_coord = m_mapWidget->screenPositionToCoordinate(pos);
  qDebug() << "Clicked, lat=" << click_coord.latitude() << ", lon=" << click_coord.longitude();

  for (int i = 0; i < m_eventObjects.size(); ++i)
  {
    QPointF center = m_mapWidget->coordinateToScreenPosition(
      QGeoCoordinate(
      m_events[i]->getLatitude(),
      m_events[i]->getLongitude()));
    int delta = 32 - 2;
    if ((pos.x() > (center.x() - delta)) &&
      (pos.x() < (center.x() + delta)) &&
      (pos.y() > (center.y() - delta)) &&
      (pos.y() < (center.y() + delta)))
    {
      QString message = QString("%1 (%2):\n%3")
        .arg(m_events[i]->getUser()->getLogin(),
        m_events[i]->getTime().toLocalTime().toString(),
        m_events[i]->getDescription());
      QMessageBox::information(this, "SOS message", message);
      break;
    }
  }
}


void EventsWidget::onEventsReceived(Events events)
{
  qDebug() << "EventsWidget::onEventsReceived";

  // Clear current events
  // m_mapWidget->clearMapObjects();
  while (m_eventObjects.size() > 0)
  {
    QGeoMapPixmapObject *marker = m_eventObjects.takeFirst();
    m_mapWidget->removeMapObject(marker);
    marker->deleteLater();
  }

  for (int i = 0; i < events.size(); i++)
  {
    QGeoMapPixmapObject *marker = new QGeoMapPixmapObject(
      QGeoCoordinate(events[i]->getLatitude(), events[i]->getLongitude()),
      m_eventOffset, m_eventIcon);
    m_mapWidget->addMapObject(marker);
    m_eventObjects.append(marker);
    qDebug() << "Received event: lat=" << events[i]->getLatitude()
      << ", lon=" << events[i]->getLongitude()
      << ", time=" << events[i]->getTime();
  }

  m_events = events;
}


void EventsWidget::onEventsThreadStarted()
{
  qDebug() << "thread started";
}


void EventsWidget::onEventsServiceError(QString error)
{
  QMessageBox::information(this, "Error", error);
}


void EventsWidget::onPositionUpdated()
{
  //qDebug() << "EventsWidget::onPositionUpdated";
  if (m_positionObject != 0)
  {
    m_mapWidget->removeMapObject(m_positionObject);
    m_positionObject->deleteLater();
    m_positionObject = 0;
  }

  QGeoPositionInfo info = m_locationManager->getInfo();
  if (info.isValid())
  {
    m_positionObject = new QGeoMapPixmapObject(
      info.coordinate(), m_positionOffset, m_positionIcon);
    m_mapWidget->addMapObject(m_positionObject);
    if (QDateTime::currentDateTime().addSecs( - MAP_CENTERED_PERIOD) > m_lastCentered)
    {
      setCenter(info.coordinate());
      //qDebug() << "EventsWidget width: " << this->geometry().width();
      //qDebug() << "MapWidget width: " << m_mapWidget->geometry().width();
      //qDebug() << "Graphics view width: " << m_graphicsView->geometry().width();
      m_lastCentered = QDateTime::currentDateTime();
    }
  }
}


QGeoCoordinate EventsWidget::getCenter()
{
  QGeoCoordinate center = m_mapWidget->center();
  QPointF position = m_mapWidget->coordinateToScreenPosition(center);
  QPointF delta = QPointF((m_mapWidget->geometry().width()
    - m_graphicsView->geometry().width()) / 2,
    (m_mapWidget->geometry().height()
    - m_graphicsView->geometry().height()) / 2);
  QPointF new_position = position - delta;
  return m_mapWidget->screenPositionToCoordinate(new_position);
}


void EventsWidget::setCenter(QGeoCoordinate bad_coordinate)
{
  QPointF position = m_mapWidget->coordinateToScreenPosition(bad_coordinate);
  QPointF delta = QPointF((m_mapWidget->geometry().width()
    - m_graphicsView->geometry().width()) / 2,
    (m_mapWidget->geometry().height()
    - m_graphicsView->geometry().height()) / 2);
  QPointF new_position = position;
  if (m_graphicsView->geometry().width() > 10)
  {
    new_position = position + delta;
  }

  QGeoCoordinate coordinate =
    m_mapWidget->screenPositionToCoordinate(new_position);
  m_mapWidget->setCenter(coordinate);
}


void EventsWidget::onSettingsUpdated()
{
  if (m_eventsThread.isRunning())
  {
    QMetaObject::invokeMethod(&m_eventsService, "updateSettings", Qt::QueuedConnection);
  }
}
