/*  */
/*!
 * \file Mutex.h
 * \brief заголовок класса CMutex
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CMutex_H_972A8705_EE88_42C2_A3E5_F0310BBB54DB_INCLUDED_
#define _CMutex_H_972A8705_EE88_42C2_A3E5_F0310BBB54DB_INCLUDED_

#if WINVER
  #include <winbase.h>
#else
  #include "Threading.h"
#endif

#include "Lock.h"
#include "DummySync.h"
//#include "SpinLock.h"

namespace Thread
{
#if WINVER

  #if _MT

  /*!
   * \brief рекурсивный mutex
   */
  class CMutex
  {
    void init();
    
  protected:  
    /*! объект-мьютекс */
    HANDLE m_mx;
    
  public:
    /*!
     * \brief присваивание без побочных эффектов
     */
    CMutex& operator=(const CMutex &obj)
    {
      return *this;
    }
  
    CMutex()
    {
      init();
    }

    CMutex(const CMutex &obj)
    {
      init();
    }
    
    ~CMutex()
    {
      CloseHandle(m_mx);
    }
    
    /*!
     * \brief заблокировать мьютекс
     */
    void lock() const
    {
      WaitForSingleObject(m_mx,INFINITE);
    }
  
    /*!
     * \brief освободить мьютекс
     */
    void unlock() const
    {
      ReleaseMutex(m_mx);
    }

    /*!
     * \brief класс автоматической блокировки
     */
    typedef CLock<CMutex> CLock;
    typedef CLock CReadLock;
    typedef CLock CWriteLock;
  };//class CMutex

  typedef CMutex CFastMutex;

  #else
  
  typedef CDummySync CMutex;
  typedef CMutex CFastMutex;
  
  #endif

#else

  #ifdef _REENTRANT
  /*!
   * \brief рекурсивный mutex
   */
  class CMutex
  {
    void init();
    
  protected:  
    /*! объект-мьютекс */
    mutable pthread_mutex_t m_mx;
    
  public:
    /*!
     * \brief присваивание без побочных эффектов
     */
    CMutex& operator=(const CMutex &)
    {
      return *this;
    }
  
    CMutex()
    {
      init();
    }

    CMutex(const CMutex &)
    {
      init();
    }
    
    ~CMutex()
    {
      pthread_mutex_destroy(&m_mx);
    }
    
    /*!
     * \brief заблокировать мьютекс
     */
    void lock() const
    {
      pthread_mutex_lock(&m_mx);
    }
  
    /*!
     * \brief освободить мьютекс
     */
    void unlock() const
    {
      pthread_mutex_unlock(&m_mx);
    }

    /*!
     * \brief класс автоматической блокировки
     */
    typedef CLock<CMutex> CMutexLock;
    typedef CMutexLock CReadLock;
    typedef CMutexLock CWriteLock;
  };//class CMutex

//  typedef CSpinLock CFastMutex;
  typedef CMutex CFastMutex;

  #else
  
  typedef CDummySync CMutex;
  typedef CMutex CFastMutex;
  
  #endif

#endif
  
}//namespace Thread

#endif //_CMutex_H_972A8705_EE88_42C2_A3E5_F0310BBB54DB_INCLUDED_

/* ===[ End of file  ]=== */
