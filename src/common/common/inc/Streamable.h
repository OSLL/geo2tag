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
