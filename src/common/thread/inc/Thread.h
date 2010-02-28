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
 * \file Thread.h
 * \brief заголовок класса CThread
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CThread_H_6C18F639_4A40_4166_ADA9_60D8AE59743B_INCLUDED_
#define _CThread_H_6C18F639_4A40_4166_ADA9_60D8AE59743B_INCLUDED_

#include "Threading.h"
#include "Mutex.h"

namespace Thread
{
  /*!
   * \brief обертка для отслеживания жизненного цикла потоков выполнения
   */
  class CThread
  {
    CThread(const CThread &obj);
    CThread& operator=(const CThread& obj);
  
    CMutex m_lock;
    volatile bool m_isRunning;
    /*! идентификатор потока */
    pthread_t m_thread;
    
    /*!
     * \brief основная функция потока
     */
    static void* _startThread(void*);
    void startThread();
    
    /*!
     * \brief callback очистки признака исполнения потока
     */
    static void _cleanupRunning(void*);
    void cleanupRunning();
    
  protected:
    /*!
     * \brief функция наследника, выполняющая работу в параллельном потоке
     */
    virtual void thread()=0;
    
    /*!
     * \brief завершить работу потока
     */
    void exit();
  
  public:
    CThread(): m_isRunning(false),m_thread(0)
    {
    }
    
    virtual ~CThread();
  
    /*!
     * \brief получить идентификатор потока
     */
    static long getId();
    
    /*!
     * \brief запустить поток
     */
    void start();
    
    /*!
     * \brief дождаться завершения потока
     */
    void join();
    
    /*!
     * \brief проверить что поток выполняется
     */
    bool isRunning()
    {
      CMutex::CMutexLock l(m_lock);
      return m_isRunning;
    }  
    
  };//class CThread
  
}//namespace Thread

#endif //_CThread_H_6C18F639_4A40_4166_ADA9_60D8AE59743B_INCLUDED_

/* ===[ End of file  ]=== */
