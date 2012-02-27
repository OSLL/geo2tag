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

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include "LightMap.h"
#include <QSettings>
#include <QtAlgorithms>
// how long (milliseconds) the user need to hold (after a tap on the screen)
// before triggering the magnifying glass feature
// 701, a prime number, is the sum of 229, 233, 239
// (all three are also prime numbers, consecutive!)
#define HOLD_TIME 701

// maximum size of the magnifier
// Hint: see above to find why I picked this one :)
#define MAX_MAGNIFIER 229

LightMap::LightMap(QWidget *parent) :
QWidget(parent),
pressed(false),
snapped(false),
zoomed(false),
invert(false)
{
  m_normalMap = new SlippyMap(this);
  m_largeMap = new SlippyMap(this);
  connect(m_normalMap, SIGNAL(updated(QRect)), SLOT(updateMap(QRect)));
  connect(m_largeMap, SIGNAL(updated(QRect)), SLOT(update()));
}


void LightMap::setCenter(qreal lat, qreal lng)
{
  m_normalMap->latitude = lat;
  m_normalMap->longitude = lng;
  m_normalMap->invalidate();
  m_largeMap->invalidate();
}


void LightMap::toggleNightMode()
{
  invert = !invert;
  update();
}


void LightMap::updateMap(const QRect &r)
{
  update(r);
}


void LightMap::activateZoom()
{
  zoomed = true;
  tapTimer.stop();
  m_largeMap->zoom = m_normalMap->zoom + 1;
  m_largeMap->width = m_normalMap->width * 2;
  m_largeMap->height = m_normalMap->height * 2;
  m_largeMap->latitude = m_normalMap->latitude;
  m_largeMap->longitude = m_normalMap->longitude;
  m_largeMap->invalidate();
  update();
}


void LightMap::resizeEvent(QResizeEvent *)
{
  m_normalMap->width = width();
  m_normalMap->height = height();
  m_normalMap->invalidate();
  m_largeMap->width = m_normalMap->width * 2;
  m_largeMap->height = m_normalMap->height * 2;
  m_largeMap->invalidate();
}


void LightMap::paintEvent(QPaintEvent *event)
{
  QPainter p;
  p.begin(this);
  m_normalMap->render(&p, event->rect());
  p.setPen(Qt::black);
  #if defined(Q_OS_SYMBIAN)
  QFont font = p.font();
  font.setPixelSize(13);
  p.setFont(font);
  #endif
  p.drawText(rect(),  Qt::AlignBottom | Qt::TextWordWrap,
    "Map data CCBYSA 2009 OpenStreetMap.org contributors");
  drawMarks(p);
  p.end();

  /*    if (zoomed)
      {
          int dim = qMin(width(), height());
          int magnifierSize = qMin(MAX_MAGNIFIER, dim * 2 / 3);
          int radius = magnifierSize / 2;
          int ring = radius - 15;
          QSize box = QSize(magnifierSize, magnifierSize);

          // reupdate our mask
          if (maskPixmap.size() != box)
          {
              maskPixmap = QPixmap(box);
              maskPixmap.fill(Qt::transparent);

              QRadialGradient g;
              g.setCenter(radius, radius);
              g.setFocalPoint(radius, radius);
              g.setRadius(radius);
              g.setColorAt(1.0, QColor(255, 255, 255, 0));
              g.setColorAt(0.5, QColor(128, 128, 128, 255));

              QPainter mask(&maskPixmap);
              mask.setRenderHint(QPainter::Antialiasing);
              mask.setCompositionMode(QPainter::CompositionMode_Source);
              mask.setBrush(g);
              mask.setPen(Qt::NoPen);
              mask.drawRect(maskPixmap.rect());
              mask.setBrush(QColor(Qt::transparent));
              mask.drawEllipse(g.center(), ring, ring);
              mask.end();
          }

          QPoint center = dragPos - QPoint(0, radius);
          center = center + QPoint(0, radius / 2);
          QPoint corner = center - QPoint(radius, radius);

          QPoint xy = center * 2 - QPoint(radius, radius);

          // only set the dimension to the magnified portion
          if (zoomPixmap.size() != box)
          {
              zoomPixmap = QPixmap(box);
              zoomPixmap.fill(Qt::lightGray);
          }
          if (true)
          {
              QPainter p(&zoomPixmap);
              p.translate(-xy);
              m_largeMap->render(&p, QRect(xy, box));
              p.end();
          }

          QPainterPath clipPath;
          clipPath.addEllipse(center, ring, ring);

          QPainter p(this);
          p.setRenderHint(QPainter::Antialiasing);
          p.setClipPath(clipPath);
          p.drawPixmap(corner, zoomPixmap);
          p.setClipping(false);
          p.drawPixmap(corner, maskPixmap);
          p.setPen(Qt::gray);
          p.drawPath(clipPath);
      }*/
  if (invert)
  {
    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_Difference);
    p.fillRect(event->rect(), Qt::white);
    p.end();
  }
}


void LightMap::timerEvent(QTimerEvent *)
{
  if (!zoomed)
    activateZoom();
  update();
}


void LightMap::mousePressEvent(QMouseEvent *event)
{
  if (event->buttons() != Qt::LeftButton)
    return;
  pressed = snapped = true;
  pressPos = dragPos = event->pos();
  tapTimer.stop();
  tapTimer.start(HOLD_TIME, this);
}


void LightMap::mouseMoveEvent(QMouseEvent *event)
{
  if (!event->buttons())
    return;
  if (!zoomed)
  {
    if (!pressed || !snapped)
    {
      QPoint delta = event->pos() - pressPos;
      pressPos = event->pos();
      m_normalMap->pan(delta);
      return;
    }
    else
    {
      const int threshold = 10;
      QPoint delta = event->pos() - pressPos;
      if (snapped)
      {
        snapped &= delta.x() < threshold;
        snapped &= delta.y() < threshold;
        snapped &= delta.x() > -threshold;
        snapped &= delta.y() > -threshold;
      }
      if (!snapped)
        tapTimer.stop();
    }
  }
  else
  {
    dragPos = event->pos();
    update();
  }
}


void LightMap::mouseReleaseEvent(QMouseEvent *)
{
  zoomed = false;
  update();
}


void LightMap::setMarks(DataChannels marks)
{
  m_marks=marks;
  m_normalMap->invalidate();
}


void LightMap::wheelEvent(QWheelEvent *event)
{
  if (event->delta()>0 && m_normalMap->zoom<15)
  {
    m_normalMap->zoom++;
    m_normalMap->invalidate();          //update();
  }else if (event->delta()<0 && m_normalMap->zoom>1)
  {
    m_normalMap->zoom--;
    m_normalMap->invalidate();          //update();
  }
}


void LightMap::keyPressEvent(QKeyEvent *event)
{
  if (!zoomed)
  {
    if (event->key() == Qt::Key_Left)
      m_normalMap->pan(QPoint(20, 0));
    if (event->key() == Qt::Key_Right)
      m_normalMap->pan(QPoint(-20, 0));
    if (event->key() == Qt::Key_Up)
      m_normalMap->pan(QPoint(0, 20));
    if (event->key() == Qt::Key_Down)
      m_normalMap->pan(QPoint(0, -20));
    if (event->key() == Qt::Key_Z || event->key() == Qt::Key_Select)
    {
      dragPos = QPoint(width() / 2, height() / 2);
      activateZoom();
    }
  }
  else
  {
    if (event->key() == Qt::Key_Z || event->key() == Qt::Key_Select)
    {
      zoomed = false;
      update();
    }
    QPoint delta(0, 0);
    if (event->key() == Qt::Key_Left)
      delta = QPoint(-15, 0);
    if (event->key() == Qt::Key_Right)
      delta = QPoint(15, 0);
    if (event->key() == Qt::Key_Up)
      delta = QPoint(0, -15);
    if (event->key() == Qt::Key_Down)
      delta = QPoint(0, 15);
    if (delta != QPoint(0, 0))
    {
      dragPos += delta;
      update();
    }
  }
}


void LightMap::drawMarks(QPainter& painter)
{
  //    QList<QSharedPointer<DataMark> > marks = m_marks.values();
  double tdim=256.;
  QPointF center=tileForCoordinate(m_normalMap->latitude,m_normalMap->longitude,m_normalMap->zoom);
  QPointF pos,posOnMap,winCenter(m_normalMap->width/2,m_normalMap->height/2);
  //Add here time and count filter
  QSettings settings(QSettings::SystemScope,"osll","libs");
  int markAge=0;
  int maxAgeOfMark=settings.value("timeLimit").toInt();
  int marksCount=settings.value("marksCount").toInt();
  //Getting list of all channels, wich marks are in request
  QList<QSharedPointer<DataMark> > marks;
  QList<QSharedPointer<Channel> > channels=m_marks.uniqueKeys();
  for (int j=0;j<channels.size();j++)
  {
    marks=m_marks.values(channels.at(j));
    qSort(marks.begin(), marks.end(), qGreater<QSharedPointer<DataMark> >());
    for (int i=0; i < qMin(marksCount, marks.size()) ; i++)
    {
      markAge=marks.at(i)->getTime().toUTC().secsTo(QDateTime::currentDateTime())/60;
      qDebug() << "current mark age "<< markAge;
      if( markAge<maxAgeOfMark)
      {
        pos=center-tileForCoordinate(marks.at(i)->getLatitude(),marks.at(i)->getLongitude(),m_normalMap->zoom);
        posOnMap=winCenter-pos*qreal(tdim);
        this->drawMarkIco(painter, posOnMap, marks.at(i),channels.at(j));
      }

    }
  }
}


void LightMap::drawMarkIco(QPainter& painter, QPointF& posOnMap,QSharedPointer<DataMark> mark,QSharedPointer<Channel> channel)
{
  QPointF posForPicture = QPointF(posOnMap.x()-12.0, posOnMap.y()-12.0);
  QPointF posForText = QPointF(posOnMap.x()-24.0, posOnMap.y()+24.0);
  QString channel_name = channel->getName();
  if(channel_name == "Fuel prices")
  {
    painter.drawImage(posForPicture, QImage(":/img/fuel.png"));
  }
  else if(channel_name == "Public announcements")
  {
    painter.drawImage(posForPicture, QImage(":/img/public.png"));
  }
  else if(channel_name == "ObsTestChannel")
  {
    painter.drawImage(posForPicture, QImage(":/img/test.png"));
    painter.drawText(posForText, "Test text");
  }
  else if(channel_name.startsWith("bus_"))
  {
    painter.drawImage(posForPicture, QImage(":/img/bus.png"));
    painter.drawText(posForText, channel_name.split('_').at(1));
  }
  else if(channel_name.startsWith("tram_"))
  {
    painter.drawImage(posForPicture, QImage(":/img/tram.png"));
    painter.drawText(posForText, channel_name.split('_').at(1));
  }
  else if(channel_name.startsWith("troll_"))
  {
    painter.drawImage(posForPicture, QImage(":/img/trolleybus.png"));
    painter.drawText(posForText, channel_name.split('_').at(1));
  }
  else if(channel_name.startsWith("user_"))
  {
    painter.drawImage(posForPicture, QImage(":/img/user.png"));
  }
  else
  {
    qDebug() << "drawing blue mark";
    painter.setBrush(Qt::blue);
    painter.drawEllipse(posOnMap,10,10);
    painter.setBrush(Qt::black);
    painter.drawEllipse(posOnMap,3,3);
    if (mark->getLabel()!="tracker's tag")
    {
      int mins_ago=(mark->getTime().toUTC().secsTo(QDateTime::currentDateTime()))/60;
      painter.drawText(posForText,mark->getLabel()+", "+ QString::number(mins_ago)+" min. ago");
    }
  }
}
