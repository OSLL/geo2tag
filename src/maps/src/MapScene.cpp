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

#include "MapScene.h"
#include <QPixmap>
#include <QPaintEngine>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <math.h>
#include <QGraphicsView>
#include <QScrollBar>
#include <QPolygon>
#include <QSettings>

#include <QGraphicsProxyWidget>
#include <QPushButton>

#include <QList>
#include <QSharedPointer>

#include "MapsUploadThread.h"
#include "defines.h"

//Move distance for one arrow key press
#define KEY_MOVE_DIST 10

MapScene::MapScene(QObject *parent) :
QGraphicsScene(parent),
m_zoom(2),
m_latitude(0/*DEFAULT_LATITUDE*/),
m_longitude(DEFAULT_LONGITUDE)
{
  m_tiles = QHash<TilePoint, QGraphicsPixmapItem * >();
  m_preloader = new Preloading(3, this);
  m_uploader = new MapsUploader(this);
  connect(this, SIGNAL(uploadTiles(QVector<TilePoint> &)), m_uploader, SLOT(uploadTiles(QVector<TilePoint> &)));
  connect(m_uploader, SIGNAL(tileUploaded(QPixmap,TilePoint)), this, SLOT(tileUploaded(QPixmap,TilePoint)));
}


MapScene::~MapScene()
{
  if(m_uploader != 0)
    delete(m_uploader);
  if(m_preloader != 0)
    delete(m_preloader);
}


void MapScene::tileUploaded(const QPixmap &pixmap, const TilePoint & point)
{
  if(m_tiles.contains(point))
    return;

  QGraphicsPixmapItem * pm = addPixmap( pixmap );
  pm->setData(0, point.second);
  m_tiles.insert(point, pm);
  m_tiles.value(point)->setPos(point.first.x()*256.0, point.first.y()*256.0);
  m_tiles.value(point)->setVisible(point.second == m_zoom);
}


void MapScene::addMark(qreal latitude, qreal longitude, QVariant data)
{
  //NOTE: Need to be refactored for use with marks
  QPixmap pixmap(20,20);
  pixmap.fill(Qt::transparent);
  QPoint center(pixmap.width()/2, pixmap.height()/2);

  QPainter painter;
  painter.begin(&pixmap);
  painter.setBrush(Qt::blue);
  painter.drawEllipse(center, pixmap.width()/2, pixmap.height()/2);
  painter.setBrush(Qt::black);
  painter.drawEllipse(center, pixmap.width()/10, pixmap.height()/10);
  painter.end();

  QGraphicsItem * mark = this->addPixmap(pixmap);
  QPointF mark_point = OSMCoordinatesConverter::GeoToTile(latitude, longitude, this->m_zoom);

  mark_point.setX(mark_point.x()*256.0);
  mark_point.setY(mark_point.y()*256.0);
  mark->setX(mark_point.x());
  mark->setY(mark_point.y());
  mark->setData(0,data);

  this->views()[0]->centerOn(mark_point);
}


void MapScene::addMark(qreal latitude, qreal longitude, QVariant data, QWidget * widget)
{
  //NOTE: Need to be refactored for use with marks AS WIDGETS!
  QGraphicsProxyWidget * mark = this->addWidget(widget);
  widget->show();

  QPointF mark_point = OSMCoordinatesConverter::GeoToTile(latitude, longitude, this->m_zoom);
  mark_point.setX(mark_point.x()*256.0);
  mark_point.setY(mark_point.y()*256.0);

  mark->setX(mark_point.x());
  mark->setY(mark_point.y());
  mark->setData(0,data);

  this->views()[0]->centerOn(mark_point);
}


void MapScene::removeMark(QGraphicsItem * mark)
{
  //TODO: Implement removing marks by clicking on them
  mark->update();                       //Excludeing unused variable warning error
}


void MapScene::setMarks(DataChannels marks)
{
  double tdim=256.;
  QPointF pos;

  //Add here time and count filter
  QSettings settings(QSettings::SystemScope, "osll", "libs");
  //int maxAgeOfMark = settings.value("timeLimit").toInt();
  int marksCount = settings.value("marksCount").toInt();
  int markAge=0;
  int maxAgeOfMark=settings.value("timeLimit").toInt();
  //Getting list of all channels, wich marks are in request
  QList<QSharedPointer<DataMark> > marks_to_show;

  for(int i = 0; i < m_marks.size(); i++)
  {
    this->removeItem(m_marks.at(i));
  }
  m_marks.clear();

  QList<QSharedPointer<Channel> > channels = marks.uniqueKeys();
  for (int j = 0; j < channels.size(); j++)
  {
    marks_to_show = marks.values(channels.at(j));
    qSort(marks_to_show.begin(), marks_to_show.end(), qGreater<QSharedPointer<DataMark> >());
    for (int i = 0; i < qMin( marksCount, marks_to_show.size() ); i++)
    {
      //Check, that current mark isnt older that maxAgeOfMark minutes
      //qDebug() << "Mark time " << marks_to_show.at(i)->getTime().toString("dd.MM.yyyy hh:mm:ss");
      //qDebug() << "CurrTime-4min  " << QDateTime::currentDateTime().addSecs(-60 * maxAgeOfMark).toString("dd.MM.yyyy hh:mm:ss");
      markAge=marks_to_show.at(i)->getTime().toUTC().secsTo(QDateTime::currentDateTime())/60;
      qDebug() << "Mark "<< marks_to_show.at(i)->getLatitude()<<" "<< marks_to_show.at(i)->getLongitude()  <<" age in mins " << markAge
        << " from channel " << channels.at(j)->getName();
      if(markAge<maxAgeOfMark)          //marks_to_show.at(i)->getTime().toUTC()>QDateTime::currentDateTime().addSecs(-60 * maxAgeOfMark))
      {
        pos = OSMCoordinatesConverter::GeoToTile(
          marks_to_show.at(i)->getLatitude(),
          marks_to_show.at(i)->getLongitude(),
          m_zoom);
        pos = pos * qreal(tdim);
        this->add_mark(pos,marks_to_show.at(i),channels.at(j));
      }
    }
  }
}


void MapScene::add_mark(QPointF pos, QSharedPointer<DataMark> mark,QSharedPointer<Channel> channel)
{
  QPointF posForPicture = QPointF(pos.x()-12.0, pos.y()-12.0);
  //QPointF posForText = QPointF(pos.x()-24.0, pos.y()+24.0);
  QGraphicsPixmapItem * pi = 0;
  QString channel_name = channel->getName();
  if(channel_name == "Fuel prices")
  {
    pi = addPixmap(QPixmap(":/img/fuel.png"));
  }
  else if(channel_name == "Public announcements")
  {
    pi = addPixmap(QPixmap(":/img/public.png"));
  }
  else if(channel_name == "ObsTestChannel")
  {
    pi = addPixmap(QPixmap(":/img/test.png"));
    //painter.drawText(posForText, "Test text");
  }
  else if(channel_name.startsWith("bus_"))
  {
    pi = addPixmap(QPixmap(":/img/bus.png"));
    //painter.drawText(posForText, channel_name.split('_').at(1));
  }
  else if(channel_name.startsWith("tram_"))
  {
    pi = addPixmap(QPixmap(":/img/tram.png"));
    //painter.drawText(posForText, channel_name.split('_').at(1));
  }
  else if(channel_name.startsWith("troll_"))
  {
    pi = addPixmap(QPixmap(":/img/trolleybus.png"));
    //painter.drawText(posForText, channel_name.split('_').at(1));
  }
  else if(channel_name.startsWith("user_"))
  {
    pi = addPixmap(QPixmap(":/img/user.png"));
  }
  else
  {
    QPixmap pixmap(50,50);
    pixmap.fill(Qt::transparent);
    QPoint center(pixmap.width()/2, pixmap.height()/4);
    QPoint posForText = QPoint(0, pixmap.height()/2+8);
    QPainter painter;
    painter.begin(&pixmap);
    QFont font=painter.font();
    font.setPointSize(7);
    painter.setFont(font);
    painter.setBrush(Qt::blue);
    painter.drawEllipse(center, pixmap.width()/4, pixmap.height()/4);
    painter.setBrush(Qt::black);
    painter.drawEllipse(center, pixmap.width()/10, pixmap.height()/10);
    int mins_ago=(mark->getTime().toUTC().secsTo(QDateTime::currentDateTime()))/60;
    qDebug() << "Text for mark: " << mark->getLabel()+", "+ QString::number(mins_ago)+" min. ago" ;
    painter.drawText(posForText,QString::number(mins_ago)+" min ago");
    painter.end();

    pi = this->addPixmap(pixmap);
  }

  pi->setX(posForPicture.x());
  pi->setY(posForPicture.y());

  m_marks.push_back(pi);
}


int MapScene::maxThreads() const
{
  return m_preloader->maxThreads();
}


void MapScene::setMaxThreads(const int & max_threads)
{
  m_preloader->setMaxThreads(max_threads);
}


void MapScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
  if (event->delta() > 0 && m_zoom < 18)
    m_zoom++;
  else if (event->delta() < 0 && m_zoom > 0)
    m_zoom--;
  else
    return;

  this->set_zoom();
}


void MapScene::set_zoom()
{
  foreach(TilePoint tp, m_tiles.keys())
  {
    if(tp.second != m_zoom)
      m_tiles.value(tp)->hide();
    else
      m_tiles.value(tp)->show();
  }

  double max_point = double((pow(2.,double(m_zoom)) - 1)*256 + 256);

  QRectF zoom_rect = QRectF(
    QPointF(0.0, 0.0),
    QPointF(max_point, max_point)
    );
  this->setSceneRect(zoom_rect);
  if(!views().isEmpty())
    this->views()[0]->setSceneRect(zoom_rect);

  QPointF center_point =  OSMCoordinatesConverter::GeoToTile(m_latitude, m_longitude, m_zoom);
  center_point.setX(center_point.x()*256);
  center_point.setY(center_point.y()*256);
  if(!views().isEmpty())
    this->views()[0]->centerOn(center_point);

  //    qDebug() << center_point.x() << "\t" << center_point.y() << "\n";

  this->update_state();
}


void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if(event->buttons() != Qt::LeftButton)
    return;

  m_pressed_screen_position = event->screenPos();
}


void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (!event->buttons())
    return;

  QPoint screen_delta = event->screenPos() - m_pressed_screen_position;
  m_pressed_screen_position = event->screenPos();

  this->views()[0]->horizontalScrollBar()->setValue(
    this->views()[0]->horizontalScrollBar()->value()
    - screen_delta.x());
  this->views()[0]->verticalScrollBar()->setValue(
    this->views()[0]->verticalScrollBar()->value()
    - screen_delta.y());

  QPointF cur_pos = event->scenePos();
  cur_pos.setX(cur_pos.x()/256);
  cur_pos.setY(cur_pos.y()/256);

  GeoPoint geo_coord = OSMCoordinatesConverter::TileToGeo(qMakePair(cur_pos, m_zoom));
  m_longitude = geo_coord.first;
  m_latitude = geo_coord.second;

  this->update_state();
}


void MapScene::keyPressEvent(QKeyEvent *event)
{
  QPoint screen_delta(0,0);

  if(event->key() == Qt::Key_F)
  {
    this->preload();
    return;
  }
  if(event->key() == Qt::Key_Right)
    screen_delta.setX(screen_delta.x() - KEY_MOVE_DIST);
  if(event->key() == Qt::Key_Left)
    screen_delta.setX(screen_delta.x() + KEY_MOVE_DIST);
  if(event->key() == Qt::Key_Down)
    screen_delta.setY(screen_delta.y() - KEY_MOVE_DIST);
  if(event->key() == Qt::Key_Up)
    screen_delta.setY(screen_delta.y() + KEY_MOVE_DIST);

  switch(event->key())
  {
    case Qt::Key_Plus:
      m_zoom ++;
      break;

    case Qt::Key_Minus:
      m_zoom--;
      break;

    default:
      break;
  }
  set_zoom();
  emit update();

  qDebug() << "Pressed key: " << event->key() << "\n";

  this->views()[0]->horizontalScrollBar()->setValue(
    this->views()[0]->horizontalScrollBar()->value()
    - screen_delta.x());
  this->views()[0]->verticalScrollBar()->setValue(
    this->views()[0]->verticalScrollBar()->value()
    - screen_delta.y());

  if(event->key() == Qt::Key_Plus && m_zoom < 18)
  {
    m_zoom++;
    this->set_zoom();
  }
  if(event->key() == Qt::Key_Minus && m_zoom > 0)
  {
    m_zoom--;
    this->set_zoom();
  }
  else
    this->update_state();
}


QPair<QPoint, QPoint> MapScene::getBorders()
{
  if(this->views().isEmpty())
    return qMakePair(QPoint(-1,-1), QPoint(-1,-1));

  QPoint point_top_left = (this->views()[0]->mapToScene(this->views()[0]->frameRect().topLeft())).toPoint();
  QPoint point_bottom_right = (this->views()[0]->mapToScene(this->views()[0]->frameRect().bottomRight())).toPoint();

  point_top_left.setX(point_top_left.x()-256);
  point_top_left.setY(point_top_left.y()-256);

  if(point_top_left.x() < 0) point_top_left.setX(0);
  if(point_top_left.y() < 0) point_top_left.setY(0);

  point_bottom_right.setX(point_bottom_right.x()+256);
  point_bottom_right.setY(point_bottom_right.y()+256);

  int max_xy = this->sceneRect().width() - 1;
  if(point_bottom_right.x() > max_xy) point_bottom_right.setX(max_xy);
  if(point_bottom_right.y() > max_xy) point_bottom_right.setY(max_xy);

  point_top_left.setX(point_top_left.x()/256);
  point_top_left.setY(point_top_left.y()/256);

  point_bottom_right.setX(point_bottom_right.x()/256);
  point_bottom_right.setY(point_bottom_right.y()/256);

  return qMakePair(point_top_left, point_bottom_right);
}


void MapScene::update_state()
{
  QPair<QPoint, QPoint> borders = this->getBorders();
  QPoint point_top_left = borders.first;
  QPoint point_bottom_right = borders.second;

  if(point_top_left.x() < 0 || point_bottom_right.x() < 0)
    return;

  QVector<TilePoint> tiles_for_upload;

  int width = point_bottom_right.x() - point_top_left.x() + 1;
  int height = point_bottom_right.y() - point_top_left.y() + 1;

  while(width > 2 && height > 2)
  {
    point_top_left = point_top_left + QPoint(1,1);
    point_bottom_right = point_bottom_right - QPoint(1,1);
    width = point_bottom_right.x() - point_top_left.x() + 1;
    height = point_bottom_right.y() - point_top_left.y() + 1;
  }

  int x = 0;
  int y = 0;
  for(x = point_top_left.x() ; x <= point_bottom_right.x(); x++)
  {
    for(y = point_top_left.y() ; y <= point_bottom_right.y(); y++)
    {
      TilePoint tp = qMakePair(QPoint(x,y), m_zoom);
      if(!m_tiles.contains(tp))
        tiles_for_upload.push_back(tp);
    }
  }

  /*
    if(tiles_for_upload.size() != 0)
    {
      emit this->uploadTiles(tiles_for_upload);
    tiles_for_upload.clear();
    }
  */

  QPoint snake(x-1, y-1);
  QPoint move(0,1);
  while(snake != borders.second)
  {
    snake += move;

    TilePoint tp = qMakePair(snake, m_zoom);
    if(!m_tiles.contains(tp))
      tiles_for_upload.push_back(tp);

    if(snake.x() < point_top_left.x())
    {
      move.setX(0);
      move.setY(-1);
      point_top_left.setX(point_top_left.x() - 1);
    }
    else if(snake.x() > point_bottom_right.x())
    {
      move.setX(0);
      move.setY(1);
      point_bottom_right.setX(point_bottom_right.x() + 1);
    }

    if(snake.y() < point_top_left.y())
    {
      move.setY(0);
      move.setX(1);
      point_top_left.setY(point_top_left.y() - 1);
    }
    else if(snake.y() > point_bottom_right.y())
    {
      move.setY(0);
      move.setX(-1);
      point_bottom_right.setY(point_bottom_right.y() + 1);
    }
  }

  if(tiles_for_upload.size() != 0)
    emit this->uploadTiles(tiles_for_upload);
}


void MapScene::preload()
{
  QPair<QPoint, QPoint> borders = this->getBorders();

  if(borders.first.x() < 0 || borders.second.x() < 0)
    return;

  m_preloader->load(borders.first, borders.second, m_zoom);
}
