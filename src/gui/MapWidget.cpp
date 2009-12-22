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
#include "Picture.h"
//#include "TabWidget.h"
#include "GpsInfo.h"
#include "DbSession.h"

common::DataMarks getMarks()
{
  using namespace common;
  return *(DbSession::getInstance().getMarks());
}

namespace GUI
{



	MapWidget::MapWidget(QWidget *parent, double b, double l, double scale) : QWidget(parent)
	{
		m_B = b;
		m_L = l;
		m_scale = scale; 
    m_moving = false;
    
    connect(&m_updateGpsDataTimer, SIGNAL(timeout()),this, SLOT(updateGpsData()));
    m_updateGpsDataTimer.setInterval(15000);
	}

	void MapWidget::paintEvent(QPaintEvent *pe)
	{
  	QPainter painter( this );

		using namespace common;
		using namespace maps;

		qDebug() << "loading map for b=" << m_L << ", " << m_B;

		//TODO, kkv constants for scale and size
		Picture picture = MapLoaderFactory::getInstance(MapLoader::GOOGLE)->getMapWithMarks(
			m_B,m_L,m_scale,rect().width(),rect().height(),getMarks());


		QByteArray array(&(picture.getData()[0]), picture.getData().size());
		qDebug() << "Loaded" << array.size();

		painter.drawImage(pe->rect(),QImage::fromData(array));
		qDebug() << "| paint event done |";
	}

  void MapWidget::mousePressEvent ( QMouseEvent * event )
  {
    qDebug() << "Pres"; 
    m_moving = true;
    m_mousePosition = event->posF();
  }

  void MapWidget::updateGpsData()
  {
    qDebug() << "timer...";
    m_B=common::GpsInfo::getInstance().getLatitude();
    m_L=common::GpsInfo::getInstance().getLongitude();

    updateMap();
  }
  
  void MapWidget::mouseReleaseEvent ( QMouseEvent * event )
  {
    m_moving = false;
    QPointF currentPos = event->posF();

    double dl = (currentPos.x() - m_mousePosition.x())/(100.0*m_scale);
    double db = (currentPos.y() - m_mousePosition.y())/(100.0*m_scale);

    qDebug() << "release: db=" << db << ", dl " << dl; 

    m_B += db;
    m_L -= dl;

    updateMap();

  }
  
  void MapWidget::mouseMoveEvent    ( QMouseEvent * event )
  {
    int db = event->x();
    int dl = event->y();
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
		m_scale = scale	;
		qDebug() << "new cale" << m_scale;
	}

	void MapWidget::updateMap()
	{
		qDebug() << "MapWidget::updateMap";
		emit repaint();
	}

} // namespace GUI

/* ===[ End of file  ]=== */
