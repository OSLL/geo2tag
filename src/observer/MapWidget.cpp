#include "MapWidget.h"
#include "DataMarks.h"
MapWidget::MapWidget(QWidget *parent)
{
	m_scale=1;
  m_marks=makeHandle(new common::DataMarks);
}

void MapWidget::paintEvent(QPaintEvent *pe)
{
//I need get marks from Observer
//It will be signal
  QPainter painter(this);
  common::Picture picture = maps::MapLoaderFactory::getInstance(maps::MapLoader::GOOGLE)->getMapWithMarks(
                       60.0,30.0,m_scale,width(),height(),*m_marks);
  QByteArray array(&(picture.getData()[0]), picture.getData().size());
  painter.drawImage(rect(),QImage::fromData(array));
//	update(rect());
}

void MapWidget::updated(CHandlePtr<common::DataMarks>& marks)
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
