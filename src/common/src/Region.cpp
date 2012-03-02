/*
 * Copyright 2011  Mark Zaslavskiy  mark.zaslavskiy@gmail.com
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

/*! ---------------------------------------------------------------
 *
 * \file Region.cpp
 * \brief Region implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "Region.h"
#include <QPolygonF>
#include <QDebug>
namespace common
{
  Region::Region():m_points(new DataMarks)
  {
  }

  Region::Region(const QSharedPointer<DataMarks>& points):m_points(points)
  {
  }

  const QSharedPointer<DataMarks>&Region::getPoints() const
  {
    return m_points;
  }

  void Region::setPoints(const QSharedPointer<DataMarks>& points)
  {
    m_points = points;
  }

  void Region::addPoint(const QSharedPointer<DataMark>& point)
  {
    m_points->push_back(point);
  }

  bool Region::atRegion(const QSharedPointer<DataMark>& point)
  {
    if (m_points.isNull() || m_points->size() == 0) return false;

    QPolygonF polygon;
    for (int i=0;i<m_points->size();i++)
    {
      polygon << QPointF(m_points->at(i)->getLatitude(),m_points->at(i)->getLongitude());
    }
    polygon << QPointF(m_points->at(0)->getLatitude(),m_points->at(0)->getLongitude());
    return polygon.containsPoint(QPointF(point->getLatitude(),point->getLongitude()), Qt::OddEvenFill);

  }

  Region::~Region()
  {
  }

}
