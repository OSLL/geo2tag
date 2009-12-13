/*  */
/*!
 * \file DbLock.cpp
 * \brief реализация класса CDbLock
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include <cassert>
#include "DbLock.h"

namespace ODBC
{
  /*
   * мне кажется, что инициализированные таким образом,
   * элементы массива готовы к использованию потоками,
   * запущеными после окончания инициализации статических переменных.
   *
   * если этот массив внести в функцию, что-то ломается :7
   */
  static Thread::CFastMutex s_obj[CDbLock::LOCK_MAX];

  Thread::CFastMutex& CDbLock::getLock(LockType l)
  {
    assert(0<=l && l<LOCK_MAX);
    return s_obj[l];
  }

} // namespace ODBC

/* ===[ End of file  ]=== */
