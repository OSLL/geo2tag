/*  */
/*!
 * \file SpinLock.h
 * \brief заголовок класса CSpinLock
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#ifndef _CSpinLock_H_681293E9_A835_4AEB_A68C_35000B7698FB_INCLUDED_
#define _CSpinLock_H_681293E9_A835_4AEB_A68C_35000B7698FB_INCLUDED_

namespace Thread
{

#ifdef _REENTRANT

  /*!
   * \brief спин-лок
   *
   * организует быструю синхронизацию потоков на SMP.
   * конкурирующий поток находится в состоянии активного ожидания.
   */
  class CSpinLock
  {
    /*! объект-спин-лок */
    mutable pthread_spinlock_t m_lock;

    void init()
    {
      pthread_spin_init(&m_lock,0);
    }
        
  public:
    /*!
     * \brief присваивание без побочных эффектов
     */
    CSpinLock& operator=(const CSpinLock &obj)
    {
      return *this;
    }
  
    CSpinLock()
    {
      init();
    }

    CSpinLock(const CSpinLock &obj)
    {
      init();
    }
    
    ~CSpinLock()
    {
      pthread_spin_destroy(&m_lock);
    }
    
    /*!
     * \brief заблокировать
     */
    void lock() const
    {
      pthread_spin_lock(&m_lock);
    }
  
    /*!
     * \brief освободить
     */
    void unlock() const
    {
      pthread_spin_unlock(&m_lock);
    }

    /*!
     * \brief класс автоматической блокировки
     */
    typedef CLock<CSpinLock> CLock;
    typedef CLock CReadLock;
    typedef CLock CWriteLock;
  };//class CSpinLock
  
#endif
}

#endif //_CSpinLock_H_681293E9_A835_4AEB_A68C_35000B7698FB_INCLUDED_

/* ===[ End of file  ]=== */
