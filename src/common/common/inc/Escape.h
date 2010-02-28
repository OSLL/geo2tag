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
 * \file Escape.h
 * \brief заголовок класса CEscape
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CEscape_H_C575F755_C148_4DC0_9A62_E0FBEDAF4CF5_INCLUDED_
#define _CEscape_H_C575F755_C148_4DC0_9A62_E0FBEDAF4CF5_INCLUDED_

#include "ExceptionSource.h"

/*!
 * \brief классы и методы представления строк в транспортабельном виде
 * 
 * Недопустимые в конечном контексте символы заменяются комбинациями допустимых символов
 */
namespace Escape
{
  /*!
   * \brief исключение при преобразовании строки
   */
  class CException: public CExceptionSource
  {
  public:
    CException(const CSource& src);
  };
  
  /*! 
   * \brief Заместить символы в строке.
   * \param str: [in] исходная строка
   * \param before: [in] что заместить.
   *        Первый символ -- префикс замещения (например, '\\').
   *        Остальные -- недопустимые в конечной строке символы.
   * \param after: [in] на что заместить.
   *        before[i] -> before[0],after[i]
   * \return строка с замещенными символами
   * \exception Escape::CException
   */ 
  std::string escape(const std::string& str,const std::string& before,const std::string& after);

  /*! 
   * \brief Востановить строку, полученную с помощью escape
   * \param str: [in] исходная строка
   * \param before: [in] что заместить
   * \param after: [in] на что заместить 
   * \return восстановленная строка 
   * \exception Escape::CException
   */ 
  std::string unEscape(const std::string& str,const std::string& before,const std::string& after);
};

#endif //_CEscape_H_C575F755_C148_4DC0_9A62_E0FBEDAF4CF5_INCLUDED_

/* ===[ End of file  ]=== */
