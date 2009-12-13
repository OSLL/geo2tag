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
