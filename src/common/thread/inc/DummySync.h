/*  */
/*!
 * \file DummySync.h
 * \brief заголовок класса CDummySync
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CDummySync_H_477EE850_90EC_43A8_82B7_C16A8B5C0EB5_INCLUDED_
#define _CDummySync_H_477EE850_90EC_43A8_82B7_C16A8B5C0EB5_INCLUDED_

#include "Lock.h"

namespace Thread
{
  /*!
   * \brief пустой синхронизирующий объект
   */
  class CDummySync
  {
  public:
    /*!
     * \brief заблокировать
     */
    void lock() const
    {
    }

    /*!
     * \brief блокировка на запись
     */
    void lockWrite() const
    {
    }
  
    /*!
     * \brief блокировка на чтение
     */
    void lockRead() const
    {
    }
  
    /*!
     * \brief освободить
     */
    void unlock() const
    {
    }

    /*!
     * \brief класс автоматической блокировки
     */
    typedef CLock<CDummySync,&CDummySync::lockRead> CReadLock;
    typedef CLock<CDummySync,&CDummySync::lockWrite> CWriteLock;
    typedef CLock<CDummySync> CDummyLock;
  };//class CDummySync
  
}

#endif //_CDummySync_H_477EE850_90EC_43A8_82B7_C16A8B5C0EB5_INCLUDED_

/* ===[ End of file  ]=== */
