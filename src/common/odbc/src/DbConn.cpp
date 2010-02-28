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
#include "DbStmt.h"

namespace ODBC
{

    bool CDbConn::connect(const std::string& dsn,const char *user,const char *pwd,const char *schema)
    {
      m_rc=SQLConnect(m_h,(SQLCHAR*)dsn.c_str(),SQL_NTS,(SQLCHAR*)user,SQL_NTS,(SQLCHAR*)pwd,SQL_NTS);

      if(SUCCEEDED(m_rc))
      {
        m_isConnected=true;
        if (schema != NULL)
        {
          std::string sql = "SET search_path TO ";
          sql.append(schema);
          sql.append(";");

          CDbStmt db(*this);
          if (db.prepare(sql.c_str()))
            db.execute();
        }
      }
      return SUCCEEDED(m_rc);
    }

    void CDbConn::connectX(const std::string& dsn,const char *user,const char *pwd,const char *schema)
    {
      if(!connect(dsn,user,pwd))
        throw ODBC::CException(SRC(),"при присоединении к БД",error());
    }

    bool CDbConn::disconnect()
    {
      m_rc=SQL_ERROR;

      if(m_isConnected)
      {
        m_rc=SQLDisconnect(m_h);

        if(SUCCEEDED(m_rc))
          m_isConnected=false;
      }

      return SUCCEEDED(m_rc);
    }

    bool CDbConn::isConnected() const
    {
      return m_isConnected;
    }

    CDbConn::CDbConn(const CDbEnv &dbEnv) : CDbHandle<SQL_HANDLE_DBC>(dbEnv.m_h),m_env(dbEnv)
    {
      m_isConnected=false;
    }

    CDbConn::~CDbConn()
    {
      if(m_isConnected)
        disconnect();
    }

    /*!
     * \brief connection-specific error text
     * \return last error textual description
     * text may be empty. see also ODBC::error
     */
    std::string CDbConn::error() const
    {
      return ODBC::error(m_env.m_h,m_h);
    }

    bool CDbConn::setManualTransaction(bool isManual)
    {
      m_rc=SQLSetConnectAttr(
        m_h,SQL_ATTR_AUTOCOMMIT,(void*)(isManual?SQL_AUTOCOMMIT_OFF:SQL_AUTOCOMMIT_ON),0);

      return SUCCEEDED(m_rc);
    }

    //bool setTransactionIsolation()

    bool CDbConn::endTransaction(bool isCommit)
    {
      m_rc=SQLEndTran(SQL_HANDLE_DBC,m_h,isCommit?SQL_COMMIT:SQL_ROLLBACK);
      return SUCCEEDED(m_rc);
    }

    bool CDbConn::commit()
    {
      return endTransaction(true);
    }

    bool CDbConn::rollback()
    {
      return endTransaction(false);
    }
};
