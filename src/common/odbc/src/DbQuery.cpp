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
 * \file DbQuery.cpp
 * \brief реализация параметризованных запросов
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag Real Time Project
 *
 * 
 * ------------------------------------------------------------------------ */

#include "DbQueryX.h"
#include <iostream>
#include <sstream>
#include <string.h>
namespace ODBC
{

  std::ostream& operator << (std::ostream &stm,const CDbQuery::CParDef &a)
  {
    stm << "#" << a.m_id << ":";
    stm << "sqlType:" << a.m_sqlType << ":";
    stm << "sz:" << a.m_sz << ":";
    stm << "val:";
    
    switch(a.m_cType)
    {
    case SQL_C_LONG:
      stm << *(long*)a.m_p;
      break;
  
    case SQL_C_DOUBLE:
      stm << *(double*)a.m_p;
      break;
  
    case SQL_C_CHAR:
      {
        std::streamsize sz=stm.precision(a.m_sz);
        stm << "\"" << (char*)a.m_p << "\"";
        stm.precision(sz);
      }
      break;
      
  
    /*! \todo временная затычка -- отформатировать по полной программе */
    case SQL_C_BINARY:
      stm << "\"" << (char*)a.m_p << "\"";
      break;
      
    case SQL_C_TIMESTAMP:
      stm << "[";
      stm << ((SQL_TIMESTAMP_STRUCT*)a.m_p)->year << ".";
      stm << ((SQL_TIMESTAMP_STRUCT*)a.m_p)->month << ".";
      stm << ((SQL_TIMESTAMP_STRUCT*)a.m_p)->day << "][";
      stm << ((SQL_TIMESTAMP_STRUCT*)a.m_p)->hour << ":";
      stm << ((SQL_TIMESTAMP_STRUCT*)a.m_p)->minute << ":";
      stm << ((SQL_TIMESTAMP_STRUCT*)a.m_p)->second << ".";
      stm << ((SQL_TIMESTAMP_STRUCT*)a.m_p)->fraction << "]";
      break;
  
    default:
      stm << "UNKNOWN cType(" << a.m_cType << ")";
      break;
    }
  
    return stm;
  }


/////////////////////////////////////////////////////////////////////////////

  
  std::ostream& Exception::CSql::outDescription(std::ostream& stm) const
  {
    CException::outDescription(stm) << std::endl;
    
    stm << "SQL: " << m_pSql << std::endl;
    
    if(!m_param.empty())
    {
      stm << "Параметры:" << std::endl;
      for(CDbQueryX::CParList::const_iterator p=m_param.begin();p!=m_param.end();++p)
        stm << "\t" << *p << std::endl;
    }
    return stm;
  }
  

/////////////////////////////////////////////////////////////////////////////


  bool CDbQuery::bind(int colSet)
  {
    m_fields.clear();
    getColset(m_fields,colSet);

    for(size_t i=0;i<m_fields.size();i++)
    {
      CFieldDef& p=m_fields[i];
  
      if(p.m_pName!=NULL)
      {
        char name[100];
        SQLSMALLINT nameSz;
  
        if(!colAttribute(p.m_id,SQL_DESC_NAME,name,sizeof(name),&nameSz,NULL))
          return false;
  
        if(strcmp(name,p.m_pName)!=0)
        {        
          //LOGO(Log::WARN) << "column " << p.m_pName << " not found at index " << p.m_id << " [" << name << "]";
          return false;
        }
      }
  
      if(!p.m_pDataSink)
        if(!CDbStmt::bind(p.m_id,p.m_cType,p.m_p,p.m_sz,p.m_pLen))
          return false;

    }
    return true;
  }
  
  bool CDbQuery::bindParam()
  {
    CParList v;
  
    getParset(v);
  
    for(CParList::iterator i=v.begin();i!=v.end();i++)
    {
      CParDef &p=*i;
  
      if(!CDbStmt::bindParameter(p.m_id,SQL_PARAM_INPUT,p.m_cType,
        p.m_sqlType,0,0,p.m_p,p.m_sz,p.m_pLen))
        return false;
    }
    return true;
  }

  bool CDbQuery::fetch()
  {
    if(!CDbStmt::fetch())
      return false;

    for(size_t i=0;i<m_fields.size();i++)
    {
      CFieldDef& field=m_fields[i];

      if(field.m_pDataSink)
      {
        field.m_pDataSink->init();

        for(;;)
        {
          SQLLEN len,*pLen=field.m_pLen?field.m_pLen:&len;

          if(getData(field.m_id,field.m_cType,field.m_p,field.m_sz,pLen))
          {
            if(!field.m_pDataSink->putData(field.m_p,field.m_sz,*pLen))
              break;
          }
          else
            break;
        }
      }
    }

    return true;
  }

  void CDbQuery::CDefaultStringDataSink::init()
  {
    m_str=std::string();
    m_isNull=true;
  }

  bool CDbQuery::CDefaultStringDataSink::putData(const void *data,size_t sz,SQLLEN lenOrInd)
  {
    if(lenOrInd!=SQL_NULL_DATA)
    {
      m_isNull=false;
      m_str+=(const char*)data;
    }
    return true;
  }


/////////////////////////////////////////////////////////////////////////////  
  
  

};

/* ===[ End of file  ]=== */
