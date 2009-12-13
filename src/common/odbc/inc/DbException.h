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

#include "Exception.h"
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
    CException(const CSource &src,const std::string &comment, const std::string &errorText):
      CExceptionSource(0,src,0),m_comment(comment),m_errorText(errorText)
    {
    }
    
    virtual std::ostream& outDescription(std::ostream& stm) const
    {
      return stm << m_comment << ": " << m_errorText;
    }
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
