#include "inc/MainWidget.h"
#include <QVBoxLayout>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
QTabWidget(parent),
m_shouldStartTracking(false)
{
  m_locationManager = new LocationManager(this);
  m_sosWidget = new SosWidget(m_locationManager, this);
  m_trackingWidget = new TrackingWidget(m_locationManager, this);
  m_eventsWidget = new EventsWidget(m_locationManager, this);

  initGUI();
}


MainWidget::~MainWidget()
{
  qDebug() << "Stoping other threads";
  m_eventsWidget->stopEventsService();
  m_trackingWidget->stopTracking();
  qDebug() << "stoped";
}


void MainWidget::initGUI()
{
  this->addTab(m_sosWidget, "SOS");
  this->addTab(m_eventsWidget, "Events");
  this->addTab(m_trackingWidget, "Tracking");
}


void MainWidget::signIn(const QString & /*authToken*/)
{
  m_eventsWidget->startEventsService();
  if (m_shouldStartTracking)
  {
    m_trackingWidget->startTracking();
  }
}


void MainWidget::signOut()
{
  m_eventsWidget->stopEventsService();
  if (m_trackingWidget->isTracking())
  {
    m_shouldStartTracking = true;
  }
  else
  {
    m_shouldStartTracking = false;
  }
  m_trackingWidget->stopTracking();
}


void MainWidget::onSettingsUpdated()
{
  m_trackingWidget->onSettingsUpdated();
  m_eventsWidget->onSettingsUpdated();
}
