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
/*!
 * \file Handle.h
 * \brief заголовок класса CHandle
 *
 * File description
 *
 *
 *  PROJ: OSLL/geo2tag 
 * ------------------------------------------------------------------------ */


#ifndef _CHandle_H_87C0433B_EE7A_4FE1_9E5F_E8997B96094E_INCLUDED_
#define _CHandle_H_87C0433B_EE7A_4FE1_9E5F_E8997B96094E_INCLUDED_

#include <cassert>
#include <typeinfo>
#include <map>
#include <functional>
#include "pointer.h"
#include "SwMr.h"

/*
 * модель представления объектов:
 * Указатель -> Описатель -> Объект
 * Указатель -- smart-pointer (далее -- QSharedPointer)
 * Описатель -- прокси для объекта, предоставляющий поддержку smart-pointer'у
 * Зачем?
 * 1) описатель создает уровень абстракции от способа размещения в памяти
 * 2) описатель избавляет от необходимости порождать Объекты от общего предка,
 *    специфичного для Указателя или как-либо иначе модифицировать Объекты,
 *    исключительно для предоставления сервиса Указателю
 */

namespace Handle
{
  template<typename T>
  class CHandle;
}//namespace Handle

/*!
 * Указатель на объект с описателем
 * \param T: [typename] тип Объекта
 */
template<typename T>
class QSharedPointer
{
  template<typename Y> friend class QSharedPointer;
  
  CPtr<T> m_rp;
  CCntPtr<CCounted> m_p;

protected:
  /*!
   * \brief конструктор создания hp из Объекта и Описателя.
   * \note используется Описателем через потомка этого класса
   */
  QSharedPointer(T* rp,const CCntPtr<CCounted>& p): m_rp(rp),m_p(p)
  {
  }
  
public:
  typedef T Type;
  
  QSharedPointer()
  {
  }

  explicit QSharedPointer(Handle::CHandle<T> *p): m_rp(*p),m_p(p)
  {
  }
  
  template<typename Y>
  QSharedPointer(QSharedPointer<Y> p): m_rp(p),m_p(p.m_p)
  {
  }

  operator T*() const
  {
    return m_rp;
  }

  T* operator ->() const
  {
    return m_rp;
  }

  /*!
   * \brief динамическое приведение к HandlePtr другого типа
   *
   * QSharedPointer<A> pa; QSharedPointer<B> pb=pa.dynamicCast<B>();
   */
  template<typename NewType>
  QSharedPointer<NewType> dynamicCast() const
  {
    return QSharedPointer<NewType>(m_rp.dynamicCast<NewType*>(),m_p);
  }

  /*!
   * \brief статическое приведение к HandlePtr другого типа
   *
   * QSharedPointer<A> pa; QSharedPointer<B> pb=pa.staticCast<B>();
   */
  template<typename NewType>
  QSharedPointer<NewType> staticCast() const
  {
    return QSharedPointer<NewType>(m_rp.staticCast<NewType*>(),m_p);
  }
  
  /*!
   * \brief порождение HandlePtr, ссылающегося на этот же описатель,
   * но имеющего другой тип и указывающего в другое место.
   *
   * \note зачем?
   * например я хочу создавать объект, ассоциированный с HP на интерфейс,
   * который у меня запрашивают, ассоциировать его с экземпляром интерфейса
   * и разрушать при освобождении интерфейса. возможное решение -- proxy.
   * однако, оно неэффективно и не всегда реализуемо. 
   * нереализуемо, если интерфейс невиртуальный.
   * неэффективно в двух отношениях: там где был один вызов появляется два,
   * а реализация прокси -- тупая, но ее невозможно автоматизировать базовыми
   * средствами языка.
   */
  template<typename NewType>
  QSharedPointer<NewType> repointCast(NewType *ptr) const
  {
    return QSharedPointer<NewType>(ptr,m_p);
  }

  template<typename NewType>
  QSharedPointer<NewType> repointCast(QSharedPointer<NewType> ptr) const
  {
    return QSharedPointer<NewType>(ptr.m_rp,m_p);
  }
  
  /*!
   * \brief освободить указуемый объект
   */
  void release()
  {
    m_rp=NULL;
    m_p.release();
  }
};

/*!
 * \brief статическое приведение HandlePtr к указателю
 *
 * QSharedPointer<A> pa; B* pb=staticCast<B*>(pa);
 */
template<typename NewType,typename T>
NewType staticCast(const QSharedPointer<T>& p)
{
  return static_cast<NewType>((T*)p);
}

/*!
 * \brief динамическое приведение HandlePtr к указателю
 *
 * QSharedPointer<A> pa; B* pb=dynamicCast<B*>(pa);
 */
template<typename NewType,typename T>
NewType dynamicCast(const QSharedPointer<T>& p)
{
  return dynamic_cast<NewType>((T*)p);
}

namespace Handle
{
  /*!
   * \brief свойства описателя для объектов класса T
   * \note точка настройки, вариант 1: специализировать CHandleTraits для типа,
   *  которому требуется makeHandle(this) в пространстве имен Handle
   */
  template<typename T>
  class CHandleTraits
  {
  public:
    /*!
     * хранить ли обратную связь Объект -> Описатель
     */
    static const bool IS_SEARCHABLE=false;
  };
  
  /*!
   * \brief определить, есть ли обратная связь Объект->Описатель
   * для данного объекта
   * \note точка настройки, вариант 2: специализировать isSearchable для типа,
   *  которому требуется makeHandle(this) в пространстве имен этого типа
   */
  template<typename T>
  bool isSearchable(const T&)
  {
    return CHandleTraits<T>::IS_SEARCHABLE;
  }
  
  /*!
   * \brief средство регистрации/синхронизации/поиска обратных связей Объект -> Описатель
   */
  class CHandleMap: public CCounted
  {
    typedef Thread::CFastSwMr CLock;
    /*!
     * \brief синхронизированный мэп Объект (const void*) -> Описатель (его подобъект CHandleMap)
     */
    class CSyncMap: public std::map<const void*,CHandleMap*>, public CLock
    {
    };
  
    /*!
     * \brief получить экземпляр мэпа (синглетон)
     */
    static CSyncMap& getMap()
    {
      static CSyncMap obj;
      return obj;
    }
  
    /*! размер полного Объекта, связанного с этим Описателем */
    const size_t m_sz;
    
  public:
    CHandleMap(size_t sz): m_sz(sz)
    {
    }
    
    /*!
     * \brief связать Объект с Описателем
     * \param p: [in] указатель на Объект
     * \param h: [in] указатель на Описатель
     */
    void set(const void* p,CHandleMap* h)
    {
      CLock::CWriteLock l(getMap());
      getMap()[p]=h;
    }
    
    /*!
     * \brief удалить связь Объекта с Описателем
     * \param p: [in] указатель на Объект
     */
    void erase(const void* p)
    {
      CLock::CWriteLock l(getMap());
      getMap().erase(p);
    }
    
    /*!
     * \brief получить Указатель на Описатель полного Объекта по данному Объекту
     * \param p: [in] указатель на Объект
     * \return Указатель на Описатель полного Объекта, или пустой, если Объект не зарегистрирован
     */
    template<typename T>
    static QSharedPointer<T> get(T* p);
  };
  
  /*!
   * \brief Описатель объекта
   * \param T: [typename] тип Объекта
   */
  template<typename T>
  class CHandle: public CPtr<T>, public CHandleMap
  {
    CHandle(const CHandle&);
    CHandle& operator =(const CHandle&);

    typedef CPtr<T> Super;
    
  protected:
    CHandle(T *p=NULL): Super(p),CHandleMap(sizeof(T))
    {
      if(isSearchable(*p))
      {
        assert(typeid(T)==typeid(*p));
        set(Super::template staticCast<const void*>(),this);
      }
    }
    
    ~CHandle()
    {
      if(isSearchable(*(T*)(*this)))
      {
        erase(Super::template staticCast<const void*>());
      }
    }

  public:
    static QSharedPointer<T> getHandle(T* pObj)
    {
      QSharedPointer<T> p(CHandleMap::get(pObj));
      assert(p);
      return p;
    }

  };
  
  template<typename T>
  class QSharedPointerFactory: public QSharedPointer<T>
  {
    QSharedPointerFactory(T* rp,const CCntPtr<CCounted>& p):
      QSharedPointer<T>(rp,p)
    {
    }
      
  public:
    static QSharedPointer<T> createHandle(T* rp,const CCntPtr<CCounted>& p)
    {
      return QSharedPointerFactory(rp,p);
    }
  };
    
  /*!
   * \note см. 114882:1998 п. 9.2:12
   */
  template<typename T>
  QSharedPointer<T> CHandleMap::get(T* p)
  {
    CLock::CReadLock l(getMap());
    std::map<const void*,CHandleMap*>::const_iterator it=getMap().upper_bound(p);
    if(it==getMap().begin())
      return QSharedPointer<T>();
      
    --it;
    if(static_cast<const char*>(it->first)+it->second->m_sz<static_cast<const void*>(p+1))
      return QSharedPointer<T>();
      
    return QSharedPointerFactory<T>::createHandle(p,it->second);
  }

  /*!
   * \brief Ссылочный (не-владеющий) описатель объекта
   * \param T: [typename] тип Объекта
   */
  template<typename T>
  class CRefHandle: public CHandle<T>
  {
    CRefHandle(T* pObj): CHandle<T>(pObj)
    {
    }

  public:

    static QSharedPointer<T> makeHandle(T* pObj)
    {
      QSharedPointer<T> p(CHandleMap::get<T>(pObj));
      if(p)
        return p;
      return QSharedPointer<T>(new CRefHandle(pObj));
    }
  };
  
  /*!
   * \brief механизм разрушения объекта класса T по-умолчанию
   * \note точка настройки
   */
  template<typename T>
  class CDefaultDisposer
  {
  public:
    void operator ()(T *p) const
    {
      delete p;
    }
  };
  
  /*!
   * \brief Владеющий описатель объекта
   * \param T: [typename] тип Объекта
   */
  template<typename T,typename Disposer>
  class COwnHandle: public CHandle<T>
  {
    Disposer m_d;
    
    COwnHandle(T* pObj,Disposer d): CHandle<T>(pObj), m_d(d)
    {
    }
    
    ~COwnHandle()
    {
      m_d(*this);
    }
    
  public:

    static QSharedPointer<T> makeHandle(T* pObj,Disposer d)
    {
      QSharedPointer<T> p(CHandleMap::get<T>(pObj));
      if(p)
        return p;
      return QSharedPointer<T>(new COwnHandle(pObj,d));
    }
  };  
}//namespace Handle

/*!
 * \brief создать ссылочный (не-владеющий) HandlePtr
 * \param pObj: [in] ссылка на объект
 */
template<typename T>
QSharedPointer<T> makeHandle(T& obj)
{
  return Handle::CRefHandle<T>::makeHandle(&obj);
}

/*!
 * \brief создать владеющий HandlePtr с явным механизмом разрушения
 * \param pObj: [in] указатель на объект
 * \param d: [in] механизм разрушения объекта -- функциональный объект или указатель на функцию
 *
 * при освобождении хэндла вызывается d(pObj)
 */
template<typename T,typename Disposer>
QSharedPointer<T> makeHandle(T* pObj,Disposer d)
{
  return Handle::COwnHandle<T,Disposer>::makeHandle(pObj,d);
}

/*!
 * \brief создать владеющий HandlePtr с явным механизмом разрушения функцией-членом
 * \param pObj: [in] указатель на объект
 * \param d: [in] функция-член для разрушения объекта
 */
template<typename T,typename DisposerClass>
QSharedPointer<T> makeHandle(T* pObj,void (DisposerClass::*d)())
{
  return makeHandle(pObj,std::mem_fun(d));
}

/*!
 * \brief создать ссылочный (не-владеющий) либо владеющий HandlePtr
 * \param pObj: [in] указатель на объект
 * \param isOwner: [in] создавать владеющий (true, по умолчанию) или ссылочный HandlePtr
 */
template<typename T>
QSharedPointer<T> makeHandle(T* pObj,bool isOwner=true)
{
  if(isOwner)
    return makeHandle(pObj,Handle::CDefaultDisposer<T>());
  else
    return Handle::CRefHandle<T>::makeHandle(pObj);
}

/*!
 * \brief создать ссылочный (не-владеющий) HandlePtr на константный объект
 * \param pObj: [in] указатель на объект
 */
template<typename T>
QSharedPointer<const T> makeHandle(const T* pObj)
{
  return Handle::CRefHandle<const T>::makeHandle(pObj);
}

/*!
 * \brief получить существующий HandlePtr
 * \param pObj: [in] указатель на объект
 */
template<typename T>
QSharedPointer<T> getHandle(T* pObj)
{
  return Handle::CHandle<T>::getHandle(pObj);
}

/*!
 * \brief получить существующий HandlePtr
 * \param pObj: [in] ссылка на объект
 */
template<typename T>
QSharedPointer<T> getHandle(T& obj)
{
  return Handle::CHandle<T>::getHandle(&obj);
}


/*!
 * \brief создать void NULL HandlePtr
 */
inline QSharedPointer<void> makeHandle()
{
  return QSharedPointer<void>();
}

#endif //_CHandle_H_87C0433B_EE7A_4FE1_9E5F_E8997B96094E_INCLUDED_

