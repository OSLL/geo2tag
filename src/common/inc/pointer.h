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
/*!
 * \file pointer.h
 * \brief
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 * ------------------------------------------------------------------------ */

#ifndef _POINTER_H_INCLUDED_E3A9A686_4A07_4369_9180_836A66853FC0
#define _POINTER_H_INCLUDED_E3A9A686_4A07_4369_9180_836A66853FC0

#include <cstddef>
#include <cassert>
#include "AtomicCount.h"

/*!
 * \brief ленивая реализация счетчика ссылок как класса.
 *
 * может использоваться как базовый класс только для объектов,
 * создаваемых через new.
 */
class CCounted
{
  /*!< счетчик ссылок */
  Thread::CAtomicCount m_ref;

  public:
    CCounted(): m_ref(0)
    {
    }
    CCounted(const CCounted&): m_ref(0)
    {
    }
    CCounted& operator =(const CCounted&)
    {
      return *this;
    }

    /*!
     * виртуальный деструктор, для правильной работы
     * delete this в void ref(long)
     */
    virtual ~CCounted()
    {
    }

    /*!
     * \brief изменение счетчика ссылок.
     * \param d: [in] на сколько изменить (+/-)
     *
     * начальное значение счетчика -- 0.
     * на объект нужно сослаться хотя бы один раз.
     * когда значение счетчика становится <=0 он удаляется
     */
    void ref(int d=1);
};
// Entry of _POINTER_H_INCLUDED_E3A9A686_4A07_4369_9180_836A66853FC0
#endif
