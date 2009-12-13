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
