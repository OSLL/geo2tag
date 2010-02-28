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
 * \file Cache.h
 * \brief заголовок класса CCache
 * \todo комментировать
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag C++ Realtime Library (CLib)
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CCache_H_329F528B_DE62_44E9_A50F_D671FAC46782_INCLUDED_
#define _CCache_H_329F528B_DE62_44E9_A50F_D671FAC46782_INCLUDED_

#include <map>
#include "Handle.h"

/*!
 * \brief
 * \todo комментировать
 */
template<typename T=void>
class CCache
{
  CCache(const CCache &obj);
  CCache& operator=(const CCache &obj);
  
  mutable std::map<const void*,CHandlePtr<T> > m_cache;
  
  struct CCacheException
  {
  };
  
public:
  CCache()
  {
  }
  
  void clear()
  {
    m_cache.clear();
  }
  
  template<typename R>
  R& get(const void *key) const
  {
    typename std::map<const void*,CHandlePtr<T> >::const_iterator it=m_cache.find(key);
    
    if(it==m_cache.end())
      throw CCacheException();
    else
      return *staticCast<R*>(it->second);
  }
  
  template<typename R>
  R& getCached(const void *key) const
  {
    try
    {
      return get(key);
    }
    catch(const CCacheException&)
    {
    }
    
    return *staticCast<R*>(m_cache[key]=makeHandle(new R));
  }

  template<typename R,typename A1>
  R& getCached(const void *key,A1& a1) const
  {
    try
    {
      return get<R>(key);
    }
    catch(const CCacheException&)
    {
    }
    return *staticCast<R*>(m_cache[key]=makeHandle(new R(a1)));
  }

  template<typename Factory>
  typename Factory::result_type& getCached(const void *key,const Factory& factory) const
  {
    try
    {
      return get<typename Factory::result_type>(key);
    }
    catch(const CCacheException&)
    {
    }
    return *staticCast<typename Factory::result_type*>(m_cache[key]=factory.getInstance());
  }

};

#endif //_CCache_H_329F528B_DE62_44E9_A50F_D671FAC46782_INCLUDED_

/* ===[ End of file  ]=== */
