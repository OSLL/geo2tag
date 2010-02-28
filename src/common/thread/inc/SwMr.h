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
 * \file SwMr.h
 * \brief заголовок класса CSwMr
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CSwMr_H_38EC3CB9_C1DF_4ABB_A606_B4DBB7B13FA0_INCLUDED_
#define _CSwMr_H_38EC3CB9_C1DF_4ABB_A606_B4DBB7B13FA0_INCLUDED_

#include "Threading.h"
#include "Ptr.h"
#include "Lock.h"
#include "DummySync.h"

namespace Thread
{

#ifdef _REENTRANT

  /*!
   * \brief объект синхронизации типа "один писатель/множество читателей"
   */
  class CSwMr
  {
    /*!
     * реализация конкретного алгоритма блокировки
     */
    CCntPtr<CCounted> m_p;
      
  public:
    CSwMr();

    CSwMr(const CSwMr &obj);
    
    ~CSwMr()
    {
    }
  
    /*!
     * \brief присваивание без побочных эффектов
     */
    CSwMr& operator=(const CSwMr &)
    {
      return *this;
    }  
  
    /*!
     * \brief блокировка на запись
     */
    void lockWrite() const;
  
    /*!
     * \brief блокировка на чтение
     */
    void lockRead() const;
  
    /*!
     * \brief освободить блокировку
     */
    void unlock() const;
  
    /*!
     * \brief класс автоматической блокировки на чтение
     */
    typedef CLock<CSwMr,&CSwMr::lockRead> CReadLock;
    
    /*!
     * \brief класс автоматической блокировки на запись
     */
    typedef CLock<CSwMr,&CSwMr::lockWrite> CWriteLock;
  
    typedef CWriteLock CSwMrLock;  

  };//class CSwMr

  /*!
   * \brief объект синхронизации типа "один писатель/множество читателей"
   * быстрый но нерекурсивный
   */
  class CFastSwMr
  {
    mutable pthread_rwlock_t m_lock;
    
    void init()
    {
      static const pthread_rwlock_t i=PTHREAD_RWLOCK_INITIALIZER;
      m_lock=i;
    }
    
  public:
    CFastSwMr()
    {
      init();
    }
    
    CFastSwMr(const CFastSwMr&)
    {
      init();
    }
    
    CFastSwMr& operator =(const CFastSwMr&)
    {
      return *this;
    }
    
    ~CFastSwMr()
    {
      pthread_rwlock_destroy(&m_lock);
    }
  
    /*!
     * \brief захват объекта на чтение.
     */
    void lockRead() const
    {
      pthread_rwlock_rdlock(&m_lock);
    }
  
    /*!
     * \brief захват объекта на запись.
     */
    void lockWrite() const
    {
      pthread_rwlock_wrlock(&m_lock);
    }
  
    /*!
     * \brief освобождение объекта объекта.
     */
    void unlock() const
    {
      pthread_rwlock_unlock(&m_lock);
    }

    /*!
     * \brief класс автоматической блокировки на чтение
     */
    typedef CLock<CFastSwMr,&CFastSwMr::lockRead> CReadLock;
    
    /*!
     * \brief класс автоматической блокировки на запись
     */
    typedef CLock<CFastSwMr,&CFastSwMr::lockWrite> CWriteLock;
  
    typedef CWriteLock CFastSwMrLock;  
  };

#else // _REENTRANT

  typedef CDummySync CSwMr;
  typedef CSwMr CFastSwMr;

#endif // _REENTRANT
    
}//namespace Thread

#endif //_CSwMr_H_38EC3CB9_C1DF_4ABB_A606_B4DBB7B13FA0_INCLUDED_

/* ===[ End of file  ]=== */
