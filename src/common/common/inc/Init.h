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
 * \file Init.h
 * \brief заголовок класса CInit
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CInit_H_AF3658DD_0B5A_4E54_B063_C6F6568E3493_INCLUDED_
#define _CInit_H_AF3658DD_0B5A_4E54_B063_C6F6568E3493_INCLUDED_

#include "ExceptionSource.h"

/*!
 * исключение использования неинициализированного объекта
 */
class CExceptionInit: public CExceptionSource
{
public:
  CExceptionInit(const CSource& src);
  
  std::ostream& outDescription(std::ostream& stm) const;
};

/*!
 * \brief класс инициализируемых объектов
 */
class CInit
{
  /*! признак того, что объект инициализирован */
  bool m_isInit;

protected:
  /*! \brief установить признак инициализации объекта */
  void setInit(bool isInit=true);
  
public:
  CInit(bool isInit=false);
  
  /*! \brief получить признак инициализации объекта */
  bool isInit() const;
  
  /*!
   * \brief проверить, что объект инициализирован
   * \exception CInitException в случае, если объект не инициализирован
   */
  void assertInit() const;
};

#endif //_CInit_H_AF3658DD_0B5A_4E54_B063_C6F6568E3493_INCLUDED_

/* ===[ End of file  ]=== */
