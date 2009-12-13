/*  */
/*!
 * \file DbLock.h
 * \brief заголовок класса CDbLock
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CDbLock_H_1E6B475C_1BBE_4472_9BBC_481F3E061A38_INCLUDED_
#define _CDbLock_H_1E6B475C_1BBE_4472_9BBC_481F3E061A38_INCLUDED_

#include "Mutex.h"

namespace ODBC
{
  /*!
   * \brief интерфейс блокировок для ODBC
   */
  class CDbLock
  {
  public:
    enum LockType
    {
      LOCK_HANDLE,
      LOCK_MAX,
    };

    typedef Thread::CFastMutex CMutex;

    static CMutex& getLock(LockType);
  }; // class CDbLock

} // namespace ODBC

#endif //_CDbLock_H_1E6B475C_1BBE_4472_9BBC_481F3E061A38_INCLUDED_

/* ===[ End of file  ]=== */
