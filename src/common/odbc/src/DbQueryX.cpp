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
#include "DbQueryX.h"
#include <string.h>
#include <sstream>
namespace ODBC
{
    void CDbQueryX::bind(int colSet)
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
          {
            std::stringstream c;

            c << "colAttribute(" << p.m_id << ")";
            throw SQLXC(c.str(),*this);
          }

          if(strcmp(name,p.m_pName)!=0)
          {
            std::stringstream err;

            err << "expected column #" << p.m_id << " \"" << p.m_pName << "\"";
            err << ", found \"" << name << "\"";

            throw SQLXCE("bind",err.str(),*this);
          }
        }

        if(!p.m_pDataSink)
          if(!CDbStmt::bind(p.m_id,p.m_cType,p.m_p,p.m_sz,p.m_pLen))
          {
            std::stringstream c;

            c << "bind(" << p.m_id << ")";
            throw SQLXC(c.str(),*this);
          }
      }
    }

    void CDbQueryX::bindParam()
    {
      CParList v;

      getParset(v);

      for(CParList::iterator i=v.begin();i!=v.end();i++)
      {
        CParDef &p=*i;

        if(!CDbStmt::bindParameter(p.m_id,SQL_PARAM_INPUT,p.m_cType,
          p.m_sqlType,0,0,p.m_p,p.m_sz,p.m_pLen))
        {
          std::stringstream c;

          c << "bindParameter(" << p.m_id << ")";
          throw SQLXC(c.str(),*this);
        }
      }
    }

    CDbQueryX::CDbQueryX(const CDbConn &dbConn) : CDbQuery(dbConn)
    {
    }

    bool CDbQueryX::fetch()
    {
      bool r=CDbQuery::fetch();
      if(!r && isFetchFailed())
        throw SQLXC("fetch",*this);

      return r;
    }

    void CDbQueryX::fetchNoEmpty()
    {
      STRACE();
      if(!fetch())
        throw Exception::CEmptyRowset(SRC(),"fetchNoEmpty","empty rowset",*this);
    }

    bool CDbQueryX::fetch(int colSet)
    {
      STRACE();
      if(!unbind())
        throw SQLXC("unbind",*this);

      bind(colSet);

      return fetch();
    }

    void CDbQueryX::fetchNoEmpty(int colSet)
    {
      STRACE();
      if(!fetch(colSet))
        throw Exception::CEmptyRowset(SRC(),"fetchNoEmpty","empty rowset",*this);
    }

    void CDbQueryX::prepare(const char *pSql)
    {
      STRACE();
      if(pSql==NULL)
        pSql=sql();

      m_pSql=pSql;

      if(pSql==NULL)
        throw DBXCE("prepare","NULL SQL statement");

      if(!CDbStmt::prepare(pSql))
        throw SQLXC("prepare",*this);

      bind();
      bindParam();
    }

    void CDbQueryX::execute()
    {
      if(!CDbStmt::execute())
        throw SQLXC("execute",*this);
    }

    void CDbQueryX::execDirect(const char *sql)
    {
      if(!CDbStmt::execDirect(sql))
        throw SQLXC("execDirect",*this);
    }

    void CDbQueryX::close()
    {
      if(!CDbStmt::close())
        throw SQLXC("close",*this);
    }

};
