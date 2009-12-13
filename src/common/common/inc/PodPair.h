/*  */
/*!
 * \file PodPair.h
 * \brief заголовок класса CPodPair
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CPodPair_H_63F6D912_FEDC_4FCE_B9E6_6325F672CE81_INCLUDED_
#define _CPodPair_H_63F6D912_FEDC_4FCE_B9E6_6325F672CE81_INCLUDED_

#include <utility>

/*!
 * \brief пара объектов POD для статической инициализации
 */
template<typename T1,typename T2>
struct CPodPair
{
  T1 first;
  T2 second;
  
  typedef T1 first_type;
  typedef T2 second_type;
  
  template<typename A,typename B>
  operator std::pair<A,B>() const
  {
    return std::pair<A,B>(first,second);
  }
};

#endif //_CPodPair_H_63F6D912_FEDC_4FCE_B9E6_6325F672CE81_INCLUDED_

/* ===[ End of file  ]=== */
