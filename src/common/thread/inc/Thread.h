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
