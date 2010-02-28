/*
 * Copyright 2010  OSLL osll@osll.spb.ru
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
