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
