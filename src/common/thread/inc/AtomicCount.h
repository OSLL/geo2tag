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
 * \file AtomicCount.h
 * \brief заголовок класса CAtomicCount
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CAtomicCount_H_38BC48B1_9930_41E7_A7E3_8DB911E16463_INCLUDED_
#define _CAtomicCount_H_38BC48B1_9930_41E7_A7E3_8DB911E16463_INCLUDED_

#include "Mutex.h"

namespace Thread
{
#ifdef _REENTRANT

  typedef int CAtomicValue;

  /*!
   * \brief атомарный (mt/SMP) счетчик.
   */
  class CAtomicCount
  {
    typedef Thread::CFastMutex CMutex;
    
    CMutex m_mx;
    volatile CAtomicValue m_cnt;
    
  public:
    CAtomicCount(CAtomicValue cnt=0): m_cnt(cnt)
    {
    }
  
    CAtomicValue operator ++()
    {
      return (*this)+=1;
    }
    
    CAtomicValue operator --()
    {
      return (*this)-=1;
    }

    CAtomicValue operator +=(CAtomicValue a)
    {
      CMutex::CMutexLock l(m_mx);
      return m_cnt+=a;
    }

    CAtomicValue operator -=(CAtomicValue a)
    {
      CMutex::CMutexLock l(m_mx);
      return m_cnt-=a;
    }

    CAtomicValue operator =(CAtomicValue a)
    {
      CMutex::CMutexLock l(m_mx);
      return m_cnt=a;
    }

    operator CAtomicValue() const
    {
      CMutex::CMutexLock l(m_mx);
      return m_cnt;
    }
    
  };//class CAtomicCount
  
#else

  typedef int CAtomicCount;
  typedef int CAtomicValue;
  
#endif

} // namespace Thread

#endif //_CAtomicCount_H_38BC48B1_9930_41E7_A7E3_8DB911E16463_INCLUDED_

/* ===[ End of file  ]=== */
