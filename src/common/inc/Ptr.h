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
#ifndef PTR_H
#define PTR_H

#include "pointer.h"

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


// PTR_H
#endif
