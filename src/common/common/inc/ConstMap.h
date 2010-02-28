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
 * \file ConstMap.h
 * \brief заголовок класса CConstMap
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ------------------------------------------------------------------------ */


#ifndef _CConstMap_H_B4C15E68_6CFD_4F2E_A0A0_92C493E6DA61_INCLUDED_
#define _CConstMap_H_B4C15E68_6CFD_4F2E_A0A0_92C493E6DA61_INCLUDED_

#include "toolDefs.h"
#include <map>

/*!
 * \brief наследник и полный аналог std::map
 *
 * добавлена операциея const T& [](const Key&) const
 * возвращает константную ссылку объект, значение которого задается при создании
 * мэпа и может изменяться setDefault
 */
template<typename _Key,
  typename _Value,
  typename _Compare=std::less<_Key>,
#if __GNUC_PREREQ(3,2)
  typename _Allocator=std::allocator<std::pair<const _Key,_Value> > >
#else
  typename _Allocator=std::allocator<_Value> >
#endif
class CConstMap: public std::map<_Key,_Value,_Compare,_Allocator>
{
  typedef std::map<_Key,_Value,_Compare,_Allocator> Super;
  _Value m_default;
  
public:
  typedef _Key Key;
  typedef _Value Value;
  
  typedef typename Super::key_type key_type;
  typedef typename Super::mapped_type mapped_type;
  typedef typename Super::value_type value_type;
  typedef typename Super::key_compare key_compare;
  
  typedef typename Super::allocator_type allocator_type;
  
  typedef typename Super::reference reference;
  typedef typename Super::const_reference const_reference;
  
  typedef typename Super::iterator iterator;
  typedef typename Super::const_iterator const_iterator;
  
  typedef typename Super::size_type size_type;
  typedef typename Super::difference_type difference_type;
  
  typedef typename Super::pointer pointer;
  typedef typename Super::const_pointer const_pointer;

  typedef typename Super::reverse_iterator reverse_iterator;
  typedef typename Super::const_reverse_iterator const_reverse_iterator;

  typedef typename Super::value_compare value_compare;

  /*!
   * \brief конструктор по умолчанию
   */
  CConstMap(): m_default()
  {
  }

  /*!
   * \brief конструктор со значением по-умолчанию
   * \param def: [in] значение по-умолчанию
   */
  explicit
  CConstMap(const Value& def,const _Compare& cmp=_Compare(),const _Allocator& a=_Allocator()):
    Super(cmp,a),m_default(def)
  {
  }

  /*!
   * \brief конструктор по паре итераторов
   */
  template<typename InputIterator>
  CConstMap(InputIterator first,InputIterator last): Super(first,last),m_default()
  {
  }

  /*!
   * \brief конструктор по паре итераторов
   * \param def: [in] значение по-умолчанию
   */
  template<typename InputIterator,typename V>
  CConstMap(InputIterator first,InputIterator last,const V& def,
    const _Compare& cmp=_Compare(),const _Allocator& a=_Allocator()):
      Super(first,last,cmp,a),m_default(def)
  {
  }

  /*!
   * \brief конструктор по массиву
   * \param r: [in] массив со значениями для инициализации
   * \param def: [in] значение по-умолчанию
   */
  template<typename T,int SZ>
  explicit
  CConstMap(const T (&r)[SZ],const Value& def=Value(),
    const _Compare& cmp=_Compare(),const _Allocator& a=_Allocator()):
      Super(r,r+SZ,cmp,a),m_default(def)
  {
  }

  /*!
   * \brief установить значение по-умолчанию
   */
  void setDefault(const Value& def)
  {
    m_default=def;
  }

  Value& operator [](const Key& key)
  {
    return static_cast<Super&>(*this)[key];
  }
  
  /*!
   * \brief получить константную ссылку на существующий элемент мэпа
   * или на значение по-умолчанию, если запрошенного ключа нет
   */
  const Value& operator [](const Key& key) const
  {
    const_iterator it=find(key);
    if(it==this->end())
    {
      return m_default;
    }
    else
      return it->second;
  }
  
  /*!
   * \brief аналог []
   */
  Value& get(const Key& key)
  {
    return (*this)[key];
  }

  /*!
   * \brief получить константную ссылку на существующий элемент мэпа
   * или на значение по-умолчанию, если запрошенного ключа нет
   */
  const Value& get(const Key& key) const
  {
    return (*this)[key];
  }

  const Value& get(const Key& key,const Value& def) const
  {
    const_iterator it=find(key);
    if(it==this->end())
    {
      return def;
    }
    else
      return it->second;
  }

  /*!
   * \brief обменять содержимым с другим CConstMap
   * \note значения по-умолчанию тоже обмениваются
   */
  void swap(CConstMap& obj)
  {
    static_cast<Super&>(*this).swap(static_cast<Super&>(obj));
    std::swap(m_default,obj.m_default);
  }
};//class CConstMap

template<typename Key,
  typename Value,
  typename Compare,
  typename Allocator>
void swap(CConstMap<Key,Value,Compare,Allocator>& a,
  CConstMap<Key,Value,Compare,Allocator>& b)
{
  a.swap(b);
}

#endif //_CConstMap_H_B4C15E68_6CFD_4F2E_A0A0_92C493E6DA61_INCLUDED_

