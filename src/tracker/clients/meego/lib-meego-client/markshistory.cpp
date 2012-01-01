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
/*!
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "markshistory.h"
#include <QDebug>

#define DEFAULT_CAPACITY 50

MarksHistory::MarksHistory(QObject *parent) :
QObject(parent),m_historyLimit(DEFAULT_CAPACITY)
{
}


void MarksHistory::setHistoryLimit(int limit)
{
  if (limit<m_historyLimit)
  {
    //Erase all elements from limit to m_historyLimit
    if (limit<m_marks.size()) m_marks.remove(limit,m_marks.size()-limit);

  }
  m_historyLimit = limit;

}


int MarksHistory::getHistoryLimit()
{
  return m_historyLimit;
}


bool MarksHistory::isEmpty()
{
  if (m_marks.size()==0) return true;
  else return false;
}


void MarksHistory::pushMark(QSharedPointer<DataMark> mark)
{
  // if vector size < m_historyLimit, than add new mark
  // if it is equal than emit isFull
  qDebug() << "Pushing mark into vector. In vector " <<m_marks.size()+1 <<", limit "<<m_historyLimit ;
  m_marks.push_front(mark);
  if (m_marks.size()==m_historyLimit+1)
  {
    emit isFull();
    m_marks.pop_back();
  }
  QString tmp="Mark history:";
  for (int i=0;i<m_marks.size();i++)
  {
    tmp+=QString("%1;%2 ").arg(QString::number(m_marks.at(i)->getLatitude())).arg(QString::number(m_marks.at(i)->getLongitude()));
  }
  qDebug() << tmp;
}


QSharedPointer<DataMark> MarksHistory::popMark()
{
  //return and remove the last element from vector
  qDebug() << "Accessing to "<< m_marks.size();
  QSharedPointer<DataMark> mark(m_marks.at(m_marks.size()-1));
  m_marks.remove(m_marks.size()-1);
  return mark;
}
