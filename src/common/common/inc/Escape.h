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

#include "Exception.h"

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
    CException(const CSource& src):
      CExceptionSource(0,src,0)
    {
    }
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
