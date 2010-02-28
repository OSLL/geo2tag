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
 * \file WorkerThread.h
 * \brief заголовок класса CWorkerThread
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CWorkerThread_H_37B38EE5_9F80_4951_9955_1D50346AB691_INCLUDED_
#define _CWorkerThread_H_37B38EE5_9F80_4951_9955_1D50346AB691_INCLUDED_

#include "Thread.h"
#include "Handle.h"
#include "Bind.h"

namespace Thread
{
  /*!
   * \brief рабочий поток выполняющий один функциональный объект
   */
  template<typename T>
  class CWorkerThread: public CThread
  {
    T m_task;

    virtual void thread()
    {
      m_task();
    }
    
  public:
    CWorkerThread(T task): m_task(task)
    {
    }
    
    virtual ~CWorkerThread()
    {
      join();
    }
  };
  
  /*!
   * \brief создать рабочий поток из функционального объекта
   */
  template<typename T>
  CHandlePtr<CThread> makeWorkerThread(T task)
  {
    return makeHandle(new CWorkerThread<T>(task));
  }

  /*!
   * \brief создать рабочий поток из объекта и указателя на функцию-член
   */
  template<typename T,typename Fn>
  CHandlePtr<CThread> makeWorkerThread(T obj,Fn task)
  {
    return makeWorkerThread(Bind::makeClosure(Bind::makeFn(obj,task),Bind::makeArgs()));
  }

  /*!
   * \brief создать рабочий поток из объекта, указателя на функцию-член и ее аргумента
   */
  template<typename T,typename Fn,typename A0>
  CHandlePtr<CThread> makeWorkerThread(T obj,Fn task,A0 a0)
  {
    return makeWorkerThread(Bind::makeClosure(Bind::makeFn(obj,task),Bind::makeArgs(a0)));
  }

  template<typename T,typename Fn,typename A0,typename A1>
  CHandlePtr<CThread> makeWorkerThread(T obj,Fn task,A0 a0,A1 a1)
  {
    return makeWorkerThread(Bind::makeClosure(Bind::makeFn(obj,task),Bind::makeArgs(a0,a1)));
  }

  template<typename T,typename Fn,typename A0,typename A1,typename A2>
  CHandlePtr<CThread> makeWorkerThread(T obj,Fn task,A0 a0,A1 a1,A2 a2)
  {
    return makeWorkerThread(Bind::makeClosure(Bind::makeFn(obj,task),Bind::makeArgs(a0,a1,a2)));
  }

}

#endif //_CWorkerThread_H_37B38EE5_9F80_4951_9955_1D50346AB691_INCLUDED_

/* ===[ End of file  ]=== */
