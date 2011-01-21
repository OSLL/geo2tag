#include "MapWidget.h"
#include "DataMarks.h"
#include <QDebug>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QPainter>
#include "Picture.h"
#include "GoogleMapLoader.h"
#include "MapLoaderFactory.h"
#include "MapLoader.h" 

//static double s_scales[]={83333.33,41666.67,21052.63,10526.32 ,5263.16,2500.00,1250.00,625.0,307.69,160.00,80.00,40.00,20.00,10.00,5.00,2.50,1.25,0.63,0.31,0.16};

MapWidget::MapWidget(QWidget *parent)
{
	m_scale=1;
  m_B=0.;
  m_L=0.;
  m_moving=false;
  m_marks=makeHandle(new DataMarks);
}

void MapWidget::paintEvent(QPaintEvent *pe)
{
//I need get marks from Observer
//It will be signal
  QPainter painter(this);
  common::Picture picture = maps::MapLoaderFactory::getInstance(maps::MapLoader::GOOGLE)->getMapWithMarks(
                       m_B,m_L,m_scale,width(),height(),*m_marks);
  QByteArray array(&(picture.getData()[0]), picture.getData().size());
  painter.drawImage(rect(),QImage::fromData(array));
//	update(rect());
}

void MapWidget::updated(QSharedPointer<DataMarks>& marks)
{
 //connect this to RSSFeed slot with the same parametres
  m_marks=marks;
  update(rect());

}

void MapWidget::scaleChanged(int newScale )
{
  qDebug() << "scaleChanged" << newScale;
  m_scale=newScale;
  update(rect());
}

void MapWidget::setB(double b){
  m_B=b;
}

void MapWidget::setL(double l){
  m_L=l;
}

void MapWidget::mousePressEvent ( QMouseEvent * event ){
  qDebug() << "Pres";
  m_moving = true;
  m_mousePosition = event->posF();
}

void MapWidget::mouseReleaseEvent ( QMouseEvent * event ){
  m_moving = false;
  QPointF currentPos = event->posF();
  double dl = (currentPos.x() - m_mousePosition.x())/(100.0*m_scale);
  double db = (currentPos.y() - m_mousePosition.y())/(100.0*m_scale);
//  double dl = (currentPos.x() - m_mousePosition.x())/(s_scales[m_scale+1]);
//  double db = (currentPos.y() - m_mousePosition.y())/(s_scales[m_scale+1]);
  m_B += db;//*180./(3.1415*6400.);
  m_L -= dl;//*180/(3.1415*6400.*cos(3.1415*m_B/180.)) ;
  update(rect());

}

