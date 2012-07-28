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

#ifndef CONCURRENTVECTOR_H
#define CONCURRENTVECTOR_H

#include <QVector>
#include <QSharedPointer>
#include <QMutex>
#include <QMutexLocker>
#include <QMap>
#include <syslog.h>

template<typename T>
class ConcurrentVector
{
  mutable QMutex m_lock;

  QVector<QSharedPointer<T> > m_container;
  QMap<qlonglong, QSharedPointer<T> > m_map;

  public:

    typedef typename QVector<QSharedPointer<T> >::iterator iterator;
    typedef typename QVector<QSharedPointer<T> >::const_iterator const_iterator;

    ConcurrentVector(const QVector<QSharedPointer<T> > &container=QVector<QSharedPointer<T> >()):
    m_container(container)
    {
    }

    ConcurrentVector(const ConcurrentVector<T>& obj):m_container(obj.m_container)
    {
    }

    void push_back(const QSharedPointer<T>& obj)
    {
      QMutexLocker locker(&m_lock);
      if(!m_map.contains(obj->getId()))
      {
        m_container.push_back(obj);
        m_map.insert(obj->getId(), obj);
      }
    }

    const QSharedPointer<T>& at(int i) const
    {
      QMutexLocker locker(&m_lock);
      return m_container.at(i);
    }

    const QSharedPointer<T>& operator [](int i) const
    {
      QMutexLocker locker(&m_lock);
      return m_container[i];
    }

    QSharedPointer<T>& operator [](int i)
    {
      QMutexLocker locker(&m_lock);
      return m_container[i];
    }

    int size() const
    {
      QMutexLocker locker(&m_lock);
      return m_container.size();
    }

    bool exist(qlonglong objectId) const
    {
      QMutexLocker locker(&m_lock);
      return m_map.contains(objectId);
    }

    const QSharedPointer<T>& item(qlonglong id) const
    {
      QMutexLocker locker(&m_lock);
      return m_map.value(id);
    }

    QSharedPointer<T>& item(qlonglong id)
    {
      QMutexLocker locker(&m_lock);
      return m_map[id];
    }

    void merge(const ConcurrentVector<T>& vector)
    {
      for(int i=0; i<vector.size(); i++)
      {
        if(exist(vector.at(i)->getId()))
          continue;
        push_back(vector.at(i));
      }
    }

    void erase(const QSharedPointer<T> &val)
    {
      QMutexLocker locker(&m_lock);
      int i = m_container.indexOf(val);
      if(i != -1)
      {
        m_container.remove(i);
        m_map.remove(m_map.key(val));
      }
    }

    QVector<QSharedPointer<T> > vector() const
    {
      QMutexLocker locker(&m_lock);
      return m_container;
    }

    void clear()
    {
      QMutexLocker locker(&m_lock);
      m_container.clear();
      m_map.clear();
    }

};
// CONCURRENTVECTOR_H
#endif
