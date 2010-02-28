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
 * \file Lock.h
 * \brief заголовок класса CLock
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CLock_H_959A7C4C_E612_45E0_AC97_F3480BE01AA5_INCLUDED_
#define _CLock_H_959A7C4C_E612_45E0_AC97_F3480BE01AA5_INCLUDED_

namespace Thread
{
  /*!
   * \brief автоматический объект блокировки
   *
   * снимает блокировку при разрушении
   *
   * \param T: [typename] класс объекта синхронизации
   * \param Lock: функция-член класса T устанавливающая блокировку
   * \param Unlock: функция-член класса T снимающая блокировку
   */
  template<typename T,void (T::*Lock)() const=&T::lock,void (T::*Unlock)() const=&T::unlock>
  class CLock
  {
    /*! заблокированный объект */
    const T* m_lock;
    
  public:
    /*!
     * \brief заблокировать объект
     */
    explicit CLock(const T& o): m_lock(&o)
    {
      (m_lock->*Lock)();
    }

    /*!
     * \brief снять блокировку, если она еще не снята
     */
    void unlock()
    {
      if(m_lock)
      {
        (m_lock->*Unlock)();
        m_lock=NULL;
      }
    }
    
    ~CLock()
    {
      unlock();
    }
  }; // class CLock

}//namespace Thread

#endif //_CLock_H_959A7C4C_E612_45E0_AC97_F3480BE01AA5_INCLUDED_

/* ===[ End of file  ]=== */
