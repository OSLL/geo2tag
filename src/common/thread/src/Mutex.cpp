/*  */
/*!
 * \file Mutex.cpp
 * \brief реализация класса CMutex
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include "Mutex.h"

namespace Thread
{
#if WINVER

  #if _MT
  
  void CMutex::init()
  {
    m_mx=CreateMutex(NULL,FALSE,NULL);
  }
  
  #endif
  
#else

  #ifdef _REENTRANT
  
  void CMutex::init()
  {
    static const pthread_mutex_t mx=PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
    m_mx=mx;
  }
  
  #endif
  
#endif
}

/* ===[ End of file  ]=== */
