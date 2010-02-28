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
