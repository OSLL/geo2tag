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
 * \file TypeInfo.h
 * \brief заголовок класса CTypeInfo
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CTypeInfo_H_BF565935_07E8_490B_BC35_6EC6A3B14A9F_INCLUDED_
#define _CTypeInfo_H_BF565935_07E8_490B_BC35_6EC6A3B14A9F_INCLUDED_

#include <typeinfo>
#include <string.h>
/*!
 * \brief valuetype-обертка вокруг std::type_info
 */
class CTypeInfo
{
  const std::type_info *m_info;
  
public:
  CTypeInfo(const std::type_info *info=NULL);

  CTypeInfo(const std::type_info& info);

  bool operator <(const CTypeInfo& obj) const;
  
  bool operator ==(const CTypeInfo& obj) const;
  
  bool before(const CTypeInfo& obj) const;
  
  const char *name() const;
  
  const std::type_info* getTypeInfo() const;
};//class CTypeInfo

#endif //_CTypeInfo_H_BF565935_07E8_490B_BC35_6EC6A3B14A9F_INCLUDED_

/* ===[ End of file  ]=== */
