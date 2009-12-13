/*  */
/*!
 * \file Escape.cpp
 * \brief реализация класса CEscape
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include <map>
#include "Escape.h"

namespace Escape
{
  std::string escape(const std::string& str,const std::string& before,const std::string& after)
  {
    if (before.length() != after.length())
      throw CException(SRC());
      
    std::string res = str;
  
    std::map <char, char> cache;
    for (size_t i = 0; i < before.length(); i++)
      cache[before[i]]=after[i];
  
    size_t pos = 0;
    for (;;)
    {
      pos = res.find_first_of(before, pos);
      if (pos == res.npos)
        return res;
      res[pos] = cache[res[pos]];
      res.insert(pos, 1, before[0]); 
      pos+=2;
    }
  }
  
  std::string unEscape(const std::string& str,const std::string& before,const std::string& after)
  {
    if (before.length() != after.length())
      throw CException(SRC());
      
    std::string res = str;
  
    std::map <char, char> cache;
    for (size_t i = 0; i < before.length(); i++)
      cache[after[i]]=before[i];
  
    size_t pos = 0;
    for (;;)
    {
      pos = res.find(before[0], pos);
      if (pos == res.npos)
        return res;
      if ((pos+1<res.length()) && (cache.count(res[pos+1])))
      {   
        res.erase(pos,1);
        res[pos]=cache[res[pos]];
      }
      pos++;
    }
  }

}

/* ===[ End of file  ]=== */
