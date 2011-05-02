#ifndef CONCURRENTVECTOR_H
#define CONCURRENTVECTOR_H

#include <QVector>
#include <QSharedPointer>
#include <QMutex>
#include <QMutexLocker>
#include <QMap>

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
        m_container.remove(i);
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
#endif                                                      // CONCURRENTVECTOR_H
