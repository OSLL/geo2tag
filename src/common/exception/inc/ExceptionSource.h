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
#ifndef EXCEPTIONSOURCE_H
#define EXCEPTIONSOURCE_H

#include "Exception.h"



//=======================[ class CExceptionSource ]======================================


/*!
  \class CExceptionSource
  \brief Класс исключений с информацией о месте исключения и ссылками на исходный текст
*/
class CExceptionSource : public CException
{
  CSource m_src;  /*!< ссылка на источник исключения */
  CStackTrace m_stack;

public:

  /*!
    Конструктор инициализации
    \param errorcode код ошибки, определенный в файле errcodes.h
    \param source описатель точки возникновения исключения (см. описание \ref SRC, \ref CSource)
    \param package_id идентификатор пакета
  */
  CExceptionSource(unsigned short errorcode, const CSource& src,unsigned char package_id);

  /*! получить строку с именем файла
    \return строка с именем файла, в котором сгенерировано исключение
  */
  std::string getSource() const;

  /*! получить имя функции, сгенерировашей исключение
  */
  std::string getSourceFunction() const ;

  /*! получить номер строки в которой сгенерировано исключение
    \return номер строки-источника исключения
  */
  unsigned int getLine() const ;

  /*!
   * вывод объекта в поток
   * \param stm: [in] поток для вывода
   * \return поток stm после вывода
   */
  virtual std::ostream& outToStream(std::ostream& stm) const;
}; // class CExceptionSource


//=======================[ class CNotImplementedException ]======================================

/*!
  \class CNotImplementedException
  \brief Исключение отсутствия реализации

  Данный тип исключений предназначен для описания ошибок связанных с
  вызовом методов с неполной или отсутствующей реализацией
*/
class CNotImplementedException: public CExceptionSource
{
  std::string m_description;

public:
  /*!
   * \brief конструктор инициализации
   * \param source: [in] описатель точки возникновения исключения (см. описание \ref SRC, \ref CSource)
   * \param description: [in] описание
   */
  CNotImplementedException(const CSource& source,const std::string& description):
    CExceptionSource(0,source,0),m_description(description)
  {
  }

  virtual std::string getDescription() const
  {
    return m_description;
  }
};


#ifndef NOT_IMPLEMENTED
/*!
 * \brief генерация исключения о нереализованной функциональности
 * \param description: [safe] описание
 */
#define NOT_IMPLEMENTED(description) throw CNotImplementedException(SRC(),(description))
#endif




#endif // EXCEPTIONSOURCE_H
