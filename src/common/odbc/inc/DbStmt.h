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
#ifndef DBSTMT_H
#define DBSTMT_H

#include "DbConn.h"

namespace ODBC
{
    /*!
     * \brief ODBC statement wrapper
     */
    class CDbStmt : public CDbHandle<SQL_HANDLE_STMT>
    {
    public:
      const CDbConn &m_conn;

      explicit CDbStmt(const CDbConn &dbConn);
      ~CDbStmt();


      /*!
       * \brief prepares SQL statement
       * \param sql: [in] SQL statement text
       * \return whether prepare succeeded
       */
      bool prepare(const char *sql);

      /*!
       * \brief executes prepared statement
       * \return whether execution succeeded
       *
       * shall be preceeded by prepare() call
       */
      bool execute();

      /*!
       * \brief executes SQL statement directly
       * \param sql: [in] SQL statement text
       * \return whether execution succeeded
       */
      bool execDirect(const char *sql);

      /*!
       * \brief binds column field storage
       * \param colNumber: [in] column index (1..)
       * \param targetType: [in] C variable type we want to receive (SQL_C_...)
       * \param target: [deferred out] pointer to the variable
       * \param targetLen: [in] size of buffer pointed by target
       * \param lenOrInd: [deferred out] pointer to the length of returned data
       * \return whether binding succeeded
       */
      bool bind(int colNumber, int targetType, void *target, SQLLEN targetLen,SQLLEN *lenOrInd);

      bool unbind();

      /*!
       * \brief fetches row of data
       * \return whether fetch succeeded
       *
       * fetch may fail due to lack of data (normal way :) or seriously (e.g. convesrion error)
       * examine m_rc to distinguish the cases
       */
      bool fetch();

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
      bool getData(int colNumber, int targetType, void *target, SQLLEN targetLen,SQLLEN *lenOrInd);

      /*!
       * \brief test whether fetch failed hardly. call right after fetch()
       * \return whether fetch() really failed
       *
       * formally fetch() fails when there's no (more) data.
       * as this failure is not real, here's the way
       */
      bool isFetchFailed() const;

      /*!
       * \brief closes cursor
       * \return whether close succeeded
       */
      bool close();

      bool bindParameter(int parNumber,int ioType,int valType,int parType,
        int colSize,int decDigits,void *target,SQLLEN bufferLen,SQLLEN *lenOrInd);

      bool resetParams();

      bool colAttribute(int colNumber,int attribute,char *pString,
        int stringSz,SQLSMALLINT *pStringLen, SQLINTEGER *pNumeric);

      std::string error() const;

  };//class CDbStmt
};

#endif // DBSTMT_H
