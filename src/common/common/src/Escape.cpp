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
 *//*  */
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
    CException::CException(const CSource& src):
      CExceptionSource(0,src,0)
    {
    }
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
