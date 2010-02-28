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
 * \file Interval.h
 * \brief Класс упорядоченной пары значений
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CInterval_H_79EAE609_63B2_4534_BD69_EEAB0BEA2958_INCLUDED_
#define _CInterval_H_79EAE609_63B2_4534_BD69_EEAB0BEA2958_INCLUDED_

#include <algorithm>
#include <functional>

/*!
 * \brief Упорядоченный набор двух значений
 */
template<typename Type>
class CInterval
{
  Type m_min; //!< Минимальное значение
  Type m_max; //!< Максимальное значение
  
public:

  CInterval()
  {
  }

  /*!
   * \brief конструктор инициализации
   * \param a первое значение
   * \param b второе значение
   * \note \i a, \i b не обязательно должны быть упорядочены
   */
  CInterval(const Type &a, const Type &b)
  {
    set(a,b);
  }
  
  /*!
   * \brief получить минимальное значение
   * \return значение
   */
  const Type& getMin() const
  {
    return m_min;
  }
  
  /*!
   * \brief получить максимальное значение
   * \return значение
   */
  const Type& getMax() const
  {
    return m_max;
  }

  /*!
   * \brief установить значения
   * \param a первое значение
   * \param b второе значение
   * \note \i a, \i b не обязательно должны быть упорядочены
   */
  void set(const Type &a, const Type &b)
  {
    m_min=std::min(a,b);
    m_max=std::max(a,b);
  }
  
  /*!
   * \brief определить, содержит ли интервал данное значение
   * \param v: [in] значение, проверяемое на вхождение в интервал
   * \return входит ли v в интервал (min <= v <= max)
   */
  bool contains(const Type& v) const
  {
    return contains(v,std::less_equal<Type>());
  }

  /*!
   * \brief определить, содержит ли интервал данное значение
   * \param comparator: [in] предикат для проверки границ интервала
   * \param v: [in] значение, проверяемое на вхождение в интервал
   * \return входит ли v в интервал (comparator(min,v) && comparator(v,max))
   */
  template<typename C>
  bool contains(const Type& v,const C& comparator) const
  {
    return contains(v,comparator,comparator);
  }

  /*!
   * \brief определить, содержит ли интервал данное значение
   * \param leftComparator: [in] предикат для проверки левой границы интервала
   * \param rightComparator: [in] предикат для проверки правой границы интервала
   * \param v: [in] значение, проверяемое на вхождение в интервал
   * \return входит ли v в интервал (leftComparator(min,v) && rightComparator(v,max))
   */
  template<typename L,typename R>
  bool contains(const Type& v,
    const L& leftComparator,const R& rightComparator) const
  {
    return leftComparator(getMin(),v) && rightComparator(v,getMax());
  }

}; // template<typename Type> class CInterval

template<typename Type>
CInterval<Type> makeInterval(const Type& a,const Type& b)
{
  return CInterval<Type>(a,b);
}

#endif //_CInterval_H_79EAE609_63B2_4534_BD69_EEAB0BEA2958_INCLUDED_

/* ===[ End of file  ]=== */
