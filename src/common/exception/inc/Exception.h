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
 * \file Exception.h
 * \brief заголовок класса CException
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CException_H_71DA1F16_33DC_432E_8F07_BBEBB4297269_INCLUDED_
#define _CException_H_71DA1F16_33DC_432E_8F07_BBEBB4297269_INCLUDED_

#include <string>
#include <iostream>
#include "Streamable.h"
#include "Trace.h"


/*!
  \class CException
  \brief Базовый класс для всех исключений в библиотеке CLib
  \todo добавить описание правил создания исключений нижнего уровня
*/
class CException: public IStreamable
{
  unsigned short m_errcode;  /*!< уникальный идентификатор ошибки */
  unsigned char  m_package;  /*!< идентификатор пакета - источника исключения */

public:

  /*! конструктор исключения
    \param  errorcode идентификатор ошибки
    \param  package_id идентификатор пакета-источника исключения
  */
  CException(unsigned short errorcode, unsigned char package_id);

  /*! виртуальный деструктор */
  virtual ~CException()
  {
  }

  /*! получить код ошибки
    \return код ошибки, определенный в \ref errcodes.h
  */
  const unsigned short& getErrorCode() const;

  /*! получить идентификатор пакета- источника ошибки
    \return идентификатор пакета-источника исключения
  */
  const unsigned char& getPackage() const;

  /*! сформировать уникальный дескриптор ошибки

    Уникальный дескриптор ошибки представляет собой беззнаковое целое содержащее код
    ошибки и идентификатор пакета-источника. Может использоваться в качестве ключа в протоколах
    работы или при записи в таблицы.
    \code
    формат

    31         23         15          7         0
    |==3 байт==|==2 байт==|==1 байт==|==0 байт==|
    |----------+----------+----------+----------|
    | package  | reserved |    error code       |
    |----------+----------+----------+----------|
    \endcode

    \return уникальный дескриптор ошибки
  */
  unsigned long getErrorHandle() const;

  virtual std::ostream& outToStream(std::ostream& stm) const;

  /*!
   * \brief вывести в поток текстовое описание исключения.
   *
   * описание не содержит никакой служебной информации генерируемой автоматически.
   * следует использовать для передачи строки, описывающей, что именно произошло.
   *
   * \param stm: поток для вывода описания
   * \return поток после вывода
   */
  virtual std::ostream& outDescription(std::ostream& stm) const;

  /*!
   * \brief получить текстовое описание исключения.
   *
   * описание не содержит никакой служебной информации генерируемой автоматически.
   * следует использовать для передачи строки, описывающей, что именно произошло.
   *
   */
  virtual std::string getDescription() const;

}; // class CException

/* ===[ End of file  ]=== */
#endif //_CException_H_71DA1F16_33DC_432E_8F07_BBEBB4297269_INCLUDED_
