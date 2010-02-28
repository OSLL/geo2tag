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
