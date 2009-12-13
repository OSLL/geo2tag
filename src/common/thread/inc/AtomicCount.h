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
