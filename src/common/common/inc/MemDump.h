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
 * \file MemDump.h
 * \brief заголовок класса CMemDump
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CMemDump_H_216747D4_A640_44BD_91BE_88E6DF55A12B_INCLUDED_
#define _CMemDump_H_216747D4_A640_44BD_91BE_88E6DF55A12B_INCLUDED_

#include <iostream>
#include <string>

namespace BitTools
{
  /*!
   * \brief вывести дамп области памяти в поток
   * \param stm: [in,out] поток для вывода
   * \param data: [in] область памяти для вывода
   * \param sz: [in] количество байт для вывода
   * \param pfx: [in] строка, добавляемая в начало каждой строки дампа
   */
  std::ostream& dump(std::ostream& stm,const void *data,size_t sz,const std::string& pfx="");
  
  /*!
   * \brief вывести дамп объекта в поток
   * \param T: [typename] тип объекта
   */
  template<typename T>
  std::ostream& dump(std::ostream& stm,const T& data,const std::string& pfx="")
  {
    return dump(stm,&data,sizeof(data),pfx);
  }
} // namespace BitTools

#endif //_CMemDump_H_216747D4_A640_44BD_91BE_88E6DF55A12B_INCLUDED_

/* ===[ End of file  ]=== */
