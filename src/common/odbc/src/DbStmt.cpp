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
    CDbStmt::CDbStmt(const CDbConn &dbConn) : CDbHandle<SQL_HANDLE_STMT>(dbConn.m_h),m_conn(dbConn)
    {
      STRACE();
      if(m_h==NULL)
        throw DBXCO("allocating STMT handle",*this);
    }

    CDbStmt::~CDbStmt()
    {
      if(m_h!=NULL)
        SQLCloseCursor(m_h);
    }


    /*!
     * \brief prepares SQL statement
     * \param sql: [in] SQL statement text
     * \return whether prepare succeeded
     */
    bool CDbStmt::prepare(const char *sql)
    {
      m_rc=SQLPrepare(m_h,(SQLCHAR*)sql,SQL_NTS);

      return SUCCEEDED(m_rc);
    }

    /*!
     * \brief executes prepared statement
     * \return whether execution succeeded
     *
     * shall be preceeded by prepare() call
     */
    bool CDbStmt::execute()
    {
      m_rc=SQLExecute(m_h);

      return SUCCEEDED(m_rc);
    }

    /*!
     * \brief executes SQL statement directly
     * \param sql: [in] SQL statement text
     * \return whether execution succeeded
     */
    bool CDbStmt::execDirect(const char *sql)
    {
      m_rc=SQLExecDirect(m_h,(SQLCHAR*)sql,SQL_NTS);

      return SUCCEEDED(m_rc);
    }

    /*!
     * \brief binds column field storage
     * \param colNumber: [in] column index (1..)
     * \param targetType: [in] C variable type we want to receive (SQL_C_...)
     * \param target: [deferred out] pointer to the variable
     * \param targetLen: [in] size of buffer pointed by target
     * \param lenOrInd: [deferred out] pointer to the length of returned data
     * \return whether binding succeeded
     */
    bool CDbStmt::bind(int colNumber, int targetType, void *target, SQLLEN targetLen,SQLLEN *lenOrInd)
    {
      m_rc=SQLBindCol(m_h,colNumber,targetType,target,targetLen,lenOrInd);

      return SUCCEEDED(m_rc);
    }

    bool CDbStmt::unbind()
    {
      m_rc=SQLFreeStmt(m_h,SQL_UNBIND);

      return SUCCEEDED(m_rc);
    }

    /*!
     * \brief fetches row of data
     * \return whether fetch succeeded
     *
     * fetch may fail due to lack of data (normal way :) or seriously (e.g. convesrion error)
     * examine m_rc to distinguish the cases
     */
    bool CDbStmt::fetch()
    {
      m_rc=SQLFetch(m_h);

      return SUCCEEDED(m_rc);
    }

    /*!
     * \brief получить очередную часть данных неопределенной длины
     * \param colNumber: [in] column index (1..)
     * \param targetType: [in] C variable type we want to receive (SQL_C_...)
     * \param target: [out] pointer to the variable
     * \param targetLen: [in] size of buffer pointed by target
     * \param lenOrInd: [out] pointer to the length of returned data;
     * помимо длины может получить значения SQL_NO_TOTAL и SQL_NULL_DATA
     * \return получено ли
     */
    bool CDbStmt::getData(int colNumber, int targetType, void *target, SQLLEN targetLen,SQLLEN *lenOrInd)
    {
      m_rc=SQLGetData(m_h,colNumber,targetType,target,targetLen,lenOrInd);

      return SUCCEEDED(m_rc);
    }

    /*!
     * \brief test whether fetch failed hardly. call right after fetch()
     * \return whether fetch() really failed
     *
     * formally fetch() fails when there's no (more) data.
     * as this failure is not real, here's the way
     */
    bool CDbStmt::isFetchFailed() const
    {
      return FAILED(m_rc) && m_rc!=SQL_NO_DATA;
    }

    /*!
     * \brief closes cursor
     * \return whether close succeeded
     */
    bool CDbStmt::close()
    {
      m_rc=SQLCloseCursor(m_h);

      return SUCCEEDED(m_rc);
    }

    bool CDbStmt::bindParameter(int parNumber,int ioType,int valType,int parType,
      int colSize,int decDigits,void *target,SQLLEN bufferLen,SQLLEN *lenOrInd)
    {
      m_rc=SQLBindParameter(m_h,parNumber,ioType,valType,parType,
        colSize,decDigits,target,bufferLen,lenOrInd);

      return SUCCEEDED(m_rc);
    }

    bool CDbStmt::resetParams()
    {
      m_rc=SQLFreeStmt(m_h,SQL_RESET_PARAMS);

      return SUCCEEDED(m_rc);
    }

    bool CDbStmt::colAttribute(int colNumber,int attribute,char *pString,
      int stringSz,SQLSMALLINT *pStringLen, SQLINTEGER *pNumeric)
    {
      m_rc=SQLColAttribute(m_h,colNumber,attribute,
        pString,stringSz,pStringLen,pNumeric);

      return SUCCEEDED(m_rc);
    }

    std::string CDbStmt::error() const
    {
      return ODBC::error(m_conn.m_env.m_h,m_conn.m_h,m_h);
    }
};
