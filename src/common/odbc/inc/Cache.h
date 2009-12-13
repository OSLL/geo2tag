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
