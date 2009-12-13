/*  */
/*!
 * \file Event.h
 * \brief заголовок класса CEvent
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CEvent_H_08369A70_6310_450F_8310_3A4FE7F82D07_INCLUDED_
#define _CEvent_H_08369A70_6310_450F_8310_3A4FE7F82D07_INCLUDED_

#include "Threading.h"
#include "Mutex.h"
#include "AtomicCount.h"

namespace Thread
{

#ifdef _REENTRANT

  /*!
   * \brief POSIX-событие
   */
  class CEvent: public CMutex
  {
    /*! событие */
    mutable pthread_cond_t m_cond;
    
    void init()
    {
      static const pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
      m_cond=cond;
    }
    
  public:
    /*!
     * \brief присваивание без побочных эффектов
     */
    CEvent& operator=(const CEvent &obj)
    {
      return *this;
    }
    
    CEvent(const CEvent&)
    {
      init();
    }
    
    CEvent()
    {
      init();
    }
    
    ~CEvent()
    {
      pthread_cond_destroy(&m_cond);
    }
    
    /*!
     * \brief ожидать события.
     * \note вызывать для заблокированного объекта
     *
     * POSIX-событие не имеет постоянного состояния "signaled"/"not signaled",
     * поэтому wait всегда ожидает _следующего_ signal или broadcast.
     *
     * \bug pthread_cond_wait -- cancellation point 
     */
    void wait() const
    {
      pthread_cond_wait(&m_cond,&m_mx);
    }

    /*!
     * \brief событие для одного ожидающего потока
     */
    void signal() const
    {
      CMutexLock l(*this);
      pthread_cond_signal(&m_cond);
    }
    
    /*!
     * \brief событие для всех ожидающих потоков
     */
    void broadcast() const
    {
      CMutexLock l(*this);
      pthread_cond_broadcast(&m_cond);
    }
    
  };//class CEvent


  /*!
   * \brief классическое событие с состоянием
   */
  class CClassicEvent: public CEvent
  {
    CAtomicCount m_cnt;
    
  public:
    /*!
     * \brief установить событие
     */
    void setEvent()
    {
      if(1==++m_cnt)
        broadcast();
    }

    /*!
     * \brief сбросить событие
     */
    void resetEvent()
    {
      m_cnt=0;
    }

    /*!
     * \brief ожидать установки события, если оно не установлено
     */
    void classicWait() const
    {
      if(!m_cnt)
      {
        CMutexLock l(*this);
        if(!m_cnt)
          wait();
      }
    }
  };
  
#else // _REENTRANT

  class CEvent: public CMutex
  {
  public:
    
    /*!
     * \brief ожидать события
     * \note вызывать для заблокированного объекта
     * \bug pthread_cond_wait -- cancellation point 
     */
    void wait() const
    {
    }

    /*!
     * \brief событие для одного ожидающего потока
     */
    void signal() const
    {
    }
    
    /*!
     * \brief событие для всех ожидающих потоков
     */
    void broadcast() const
    {
    }    
  };//class CEvent

#endif // _REENTRANT
  
}//namespace Thread

#endif //_CEvent_H_08369A70_6310_450F_8310_3A4FE7F82D07_INCLUDED_

/* ===[ End of file  ]=== */
