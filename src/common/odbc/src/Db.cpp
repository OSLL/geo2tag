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
 * \file Db.cpp
 * \brief вспомогательные средства из ODBC::
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag Real Time Project
 *
 * 
 * ------------------------------------------------------------------------ */

#include "Db.h"
#include <string.h>
#include <math.h>

namespace ODBC
{
  std::string error(SQLHANDLE hEnv,SQLHANDLE hConn,SQLHANDLE hStmt)
  {
    SQLCHAR state[10];
    SQLINTEGER nativeError;
    char text[1000];
    SQLSMALLINT sz;

    text[0]=0;
    SQLError(hEnv,hConn,hStmt,state,&nativeError,(SQLCHAR*)text,sizeof(text),&sz);
    return std::string(text);
  }

  CTime convertTime(const SQL_TIMESTAMP_STRUCT& t,CTime::CType type)
  {
    return CTime(t.year,t.month,t.day,t.hour,t.minute,t.second+t.fraction/1000000000.,type);
  }

  void convertTime(SQL_DATE_STRUCT& dbDate,const CTime& timeStamp,CTime::CType type)
  {
    const tm& stm=timeStamp.getTime(type);

    memset(&dbDate,0,sizeof(dbDate));
    dbDate.year=stm.tm_year+1900;
    dbDate.month=stm.tm_mon+1;
    dbDate.day=stm.tm_mday;
  }

  void convertTime(SQL_TIMESTAMP_STRUCT& dbDate,const CTime& timeStamp,CTime::CType type)
  {
    const tm& stm=timeStamp.getTime(type);

    memset(&dbDate,0,sizeof(dbDate));
    dbDate.year=stm.tm_year+1900;
    dbDate.month=stm.tm_mon+1;
    dbDate.day=stm.tm_mday;
    dbDate.hour=stm.tm_hour;
    dbDate.minute=stm.tm_min;
    dbDate.second=stm.tm_sec;

    double dummy;
    dbDate.fraction=(SQLUINTEGER)(1000000000.*modf(timeStamp.getSec(),&dummy));
  }


};

/* ===[ End of file  ]=== */
