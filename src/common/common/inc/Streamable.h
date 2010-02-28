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
 * \file Streamable.h
 * \brief заголовок класса IStreamable
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CStreamable_H_1BBACD52_4FAC_4F9B_9C15_4AFE84F1D35F_INCLUDED_
#define _CStreamable_H_1BBACD52_4FAC_4F9B_9C15_4AFE84F1D35F_INCLUDED_

#include <iostream>

/*!
 * \brief интерфейс выводимого в поток объекта
 */
class IStreamable
{
public:
  virtual ~IStreamable()
  {
  }
  
  /*!
   * \brief вывести объект в поток
   * \param stm: [in] поток для вывода
   * \return stm после вывода
   */
  virtual std::ostream& outToStream(std::ostream& stm) const=0;
}; // class IStreamable

/*!
 * \brief оператор вывода объекта в поток
 */
inline std::ostream& operator << (std::ostream& stm,const IStreamable& obj)
{
  return obj.outToStream(stm);
}

#endif //_CStreamable_H_1BBACD52_4FAC_4F9B_9C15_4AFE84F1D35F_INCLUDED_

/* ===[ End of file  ]=== */
