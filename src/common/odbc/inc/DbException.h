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
 * \file DbException.h
 * \brief заголовок классов исключений доступа к данным
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag Real Time Project
 *
 * 
 * ------------------------------------------------------------------------ */

#ifndef _DBEXCEPTION_H_INCLUDED_73356eda_e72b_4154_91b3_177165e8be2a
#define _DBEXCEPTION_H_INCLUDED_73356eda_e72b_4154_91b3_177165e8be2a

#include "ExceptionSource.h"
#include <string>

namespace ODBC
{
  /*!
   * \brief корневое исключение ODBC
   *
   * исключения этого класса несут информацию о действии вызвавшем исключение,
   * об ошибке, с которой закончилось выполнение и трассу стека от места возникновения
   * ошибки до места поимки. исключения этого типа возбуждаются при нарушении
   * ограничений на данные, в случае, если дальнейшее уточнение невозможно.
   */
  class CException: public CExceptionSource
  {
    std::string m_comment;
    std::string m_errorText;
    
  public:
    CException(const CSource &src,const std::string &comment, const std::string &errorText);
    
    virtual std::ostream& outDescription(std::ostream& stm) const;
  };

/*!
 * \brief создание объекта CDBException по текстовым описаниям
 * \param comment: [in, safe] действие вызвавшее исключение
 * \param error: [in, safe] описание ошибки
 */
#define DBXCE(comment,error) CException(SRC(),(comment),(error))

/*!
 * \brief создание объекта CDBException по объекту поддерживающему .error()
 * \param comment: [in, safe] действие вызвавшее исключение
 * \param obj: [in, safe] объект, имеющий информацию об ошибке, доступную по .error()
 */
#define DBXCO(comment,obj) CException(SRC(),(comment),(obj).error())

};

#endif // Entry of _DBEXCEPTION_H_INCLUDED_73356eda_e72b_4154_91b3_177165e8be2a

/* ===[ End of file  ]=== */
