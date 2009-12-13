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
