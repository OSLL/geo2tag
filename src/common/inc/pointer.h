/*!
 * \file pointer.h
 * \brief
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ------------------------------------------------------------------------ */

#ifndef _POINTER_H_INCLUDED_E3A9A686_4A07_4369_9180_836A66853FC0
#define _POINTER_H_INCLUDED_E3A9A686_4A07_4369_9180_836A66853FC0

#include <cstddef>
#include <cassert>
#include "AtomicCount.h"

/*!
 * \brief ленивая реализация счетчика ссылок как класса.
 *
 * может использоваться как базовый класс только для объектов,
 * создаваемых через new.
 */
class CCounted
{
  Thread::CAtomicCount m_ref;/*!< счетчик ссылок */

public:
  CCounted(): m_ref(0)
  {
  }
  CCounted(const CCounted&): m_ref(0)
  {
  }
  CCounted& operator =(const CCounted&)
  {
    return *this;
  }
  
  /*!
   * виртуальный деструктор, для правильной работы
   * delete this в void ref(long)
   */
  virtual ~CCounted()
  {
  }
  
  /*!
   * \brief изменение счетчика ссылок.
   * \param d: [in] на сколько изменить (+/-)
   * 
   * начальное значение счетчика -- 0.
   * на объект нужно сослаться хотя бы один раз.
   * когда значение счетчика становится <=0 он удаляется
   */
  void ref(int d=1)
  {
    Thread::CAtomicValue v=(m_ref+=d);
    assert(v>=0);
    if(v<=0)
      delete this;
  }
};

/*!
 * \brief базовый класс smart-pointer'ов
 */
template<class T>
class CPtr
{
  T *m_p;
  
protected:
  void setPtr(T* p)
  {
    m_p=p;
  }
  
public:
  typedef T Type;
  
  CPtr(T *p=NULL): m_p(p)
  {
  }
  
  operator T*() const
  {
    return m_p;
  }
  
  T* operator ->() const
  {
//    if(m_p==NULL)
//      throw null-pointer-exception
    return m_p;
  }
  
  template<typename NewType>
  NewType dynamicCast() const
  {
    return dynamic_cast<NewType>(m_p);
  }

  template<typename NewType>
  NewType staticCast() const
  {
    return static_cast<NewType>(m_p);
  }
};

/*!
 * \brief шаблонный класс указателя с подсчетом ссылок.
 *
 * каждый экземпляр CCntPtr держит одну ссылку на указуемый
 * объект. класс T должен иметь public-функцию ref(long)
 * для изменения счетчика ссылок. реализация не зависит от
 * способа, которым создан указуемый объект:
 *
 * T x;
 * CCntPtr<T> pT=&x;
 *
 * вполне корректно, если объект x знает как вести себя при
 * изменении счетчика ссылок.
 */
template<class T>
class CCntPtr: public CPtr<T>
{
  void init(T *p)
  {
    setPtr(p);
    if(p!=NULL)
      p->ref(1);
  }

public:
  CCntPtr()
  {
  }
  
  CCntPtr(T *p)
  {
    init(p);
  }
  
  CCntPtr(const CCntPtr& p): CPtr<T>()
  {
    init(p);
  }
  
  ~CCntPtr()
  {
    release();
  }
  
  template<typename Y>
  operator CCntPtr<Y>() const
  {
    return CCntPtr<Y>((Y*)(*this));
  }

  /*!
   * \brief оператор присваивания.
   *
   * освобождает предыдущий объект.
   */
  CCntPtr& operator=(CCntPtr a)
  {
    T* p=*this;
    setPtr(a);
    a.setPtr(p);
    return *this;
  }

  /*!
   * \brief освобождение объекта на который мы указываем
   */
  void release()
  {
    if(*this!=NULL)
      (*this)->ref(-1);
    this->setPtr(NULL);
  }
};

template<class T>
CCntPtr<T> makeCntPtr(T *p)
{
  return CCntPtr<T>(p);
}

template<typename NewType,typename T>
NewType staticCast(const CPtr<T>& p)
{
  return p.template staticCast<NewType>();
}

template<typename NewType,typename T>
NewType dynamicCast(const CPtr<T>& p)
{
  return p.template dynamicCast<NewType>();
}

#endif // Entry of _POINTER_H_INCLUDED_E3A9A686_4A07_4369_9180_836A66853FC0

