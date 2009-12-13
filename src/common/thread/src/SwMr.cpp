/*  */
/*!
 * \file SwMr.cpp
 * \brief реализация класса CSwMr
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#ifdef _REENTRANT

#include "SwMr.h"
#include "Event.h"
#include <map>

namespace Thread
{
  /*!
   * \brief рекурсивная реализация SwMr
   */
  class CSwMrImpl: public CCounted
  {
    /*! количество писателей */
    volatile long m_writers;
  
    /*! идентификатор потока-писателя */
    volatile pthread_t m_tid;
  
    /*!
     * признак качественного изменения состояния блокировки;
     * защита внутреннего состояния
     */
    CEvent m_cs;
  
    /*! колическтво блокировок на чтение от каждого потока */
    std::map<pthread_t,long> m_readCnt;
  
    /*!
     * \brief захват объекта на чтение.
     * m_cs уже захвачен.
     */
    void erLocked();
  
    /*!
     * \brief захват объекта на запись.
     * m_cs уже захвачен.
     */
    void ewLocked();
  
  public:  
    CSwMrImpl();
    virtual ~CSwMrImpl();
  
    /*!
     * \brief захват объекта на чтение.
     */
    void lockRead();
    /*!
     * \brief захват объекта на запись.
     */
    void lockWrite();
    /*!
     * \brief освобождение объекта.
     */
    void unlock();
  };
  
  CSwMrImpl::CSwMrImpl(): m_writers(0)
  {
  }
  
  CSwMrImpl::~CSwMrImpl()
  {
    CEvent::CMutexLock l(m_cs);
    assert(m_writers==0 && m_readCnt.empty());
  }
  
  void CSwMrImpl::lockRead()
  {
    CEvent::CMutexLock l(m_cs);
    erLocked();
  }
  
  void CSwMrImpl::erLocked()
  {
    pthread_t tid=pthread_self();
  
    for(;;)
    {
      if(m_writers)
      {
        if(m_tid==tid)
        {
          ewLocked();
          break;
        }
        else
        {
          m_cs.wait();
        }
      }
      else
      {
        ++m_readCnt[tid];
        break;
      }
    }
  }
  
  void CSwMrImpl::lockWrite()
  {
    CEvent::CMutexLock l(m_cs);
    ewLocked();
  }
  
  void CSwMrImpl::ewLocked()
  {
    pthread_t tid=pthread_self();
  
    for(;;)
    {
      if(m_writers)
      {
        if(m_tid==tid)
        {
          ++m_writers;
          break;
        }
      }
  
      if(!m_writers && m_readCnt.size()==m_readCnt.count(tid))
      {
        m_tid=tid;
        m_writers=1;
        break;
      }
  
      m_cs.wait();
    }
  }
  
  void CSwMrImpl::unlock()
  {
    pthread_t tid=pthread_self();
    
    CEvent::CMutexLock l(m_cs);

    if(m_writers && m_tid==tid)
    {
      if(!--m_writers)
        m_cs.broadcast();
    }
    else
    {
      if(!--m_readCnt[tid])
        m_readCnt.erase(tid);

      if(m_readCnt.size()<=1)
        m_cs.broadcast();
    }
  }
  
  CSwMr::CSwMr(): m_p(new CSwMrImpl)
  {
  }
  
  CSwMr::CSwMr(const CSwMr &obj): m_p(new CSwMrImpl)
  {
  }
  
  void CSwMr::lockRead() const
  {
    staticCast<CSwMrImpl*>(m_p)->lockRead();
  }
  
  void CSwMr::lockWrite() const
  {
    staticCast<CSwMrImpl*>(m_p)->lockWrite();
  }
  
  void CSwMr::unlock() const
  {
    staticCast<CSwMrImpl*>(m_p)->unlock();
  }
  
}//namespace Thread

#endif // _REENTRANT

/* ===[ End of file  ]=== */
