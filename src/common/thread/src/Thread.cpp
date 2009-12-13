/*  */
/*!
 * \file Thread.cpp
 * \brief реализация класса CThread
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include <cassert>
#include <typeinfo>
#include "Thread.h"
#include "toolDefs.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

namespace Thread
{
  CThread::~CThread()
  {
    try
    {
      join();
    }
    catch(...)
    {
    }
  }

  void* CThread::_startThread(void *p)
  {
    CThread *_p=reinterpret_cast<CThread*>(p);
    _p->startThread();
    return NULL;
  }

  void CThread::startThread()
  {
    pthread_cleanup_push(_cleanupRunning,this);
    
    try
    {
      thread();
    }
    catch(...)
    {
    }
    pthread_cleanup_pop(1);
  }
  
  void CThread::_cleanupRunning(void *p)
  {
    CThread *_p=reinterpret_cast<CThread*>(p);
    _p->cleanupRunning();
  }
  
  void CThread::cleanupRunning()
  {
    CMutex::CMutexLock l(m_lock);
    m_isRunning=false;
  }
  
  void CThread::start()
  {
    CMutex::CMutexLock l(m_lock);
    if(!m_thread)
      m_isRunning=(0==pthread_create(&m_thread,NULL,_startThread,reinterpret_cast<void*>(this)));
  }
  
  void CThread::exit()
  {
    if(isRunning())
    {
      pthread_exit(NULL);
    }
  }
  
  void CThread::join()
  {
    CMutex::CMutexLock l(m_lock);
    pthread_t thread=m_thread;
    m_thread=0;
    l.unlock();
    
    if(thread)
      pthread_join(thread,NULL);
  }

  long CThread::getId()
  {
    C_ASSERT(sizeof(long)>=sizeof(pthread_t));
    return (long)pthread_self();
  }
  
}//namespace Thread

/* ===[ End of file  ]=== */
