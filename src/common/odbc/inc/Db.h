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
 * \file Db.h
 * \brief заголовок базовых классов доступа к бд через ODBC
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag Real Time Project
 *
 * 
 * ------------------------------------------------------------------------ */

#ifndef _DB_H_INCLUDED_f8a9d8bd_7e7d_4382_9a74_0165df80ffc0
#define _DB_H_INCLUDED_f8a9d8bd_7e7d_4382_9a74_0165df80ffc0

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#include "toolDefs.h"
#include "DbException.h"
#include "Time.h"


/*!
 * \brief доступ к данным через  ODBC
 *
 * доступ к БД включает в себя следующие классы:
 * - CDbEnv: обертка окружения ODBC;
 * - CDbConn: обертка соединения с БД;
 * - CDbStmt: обертка SQL-запроса;
 * - CDbQuery и CDbXQuery: запросы с параметрами и возвращаемыми значениями.
 */
namespace ODBC
{
  inline bool SUCCEEDED(SQLRETURN rc)
  {
    return SQL_SUCCEEDED(rc);
  }

  inline bool FAILED(SQLRETURN rc)
  {
    return !SUCCEEDED(rc);
  }

  CTime convertTime(const SQL_TIMESTAMP_STRUCT& t,CTime::CType type);

  void convertTime(SQL_DATE_STRUCT& dbDate,const CTime& timeStamp,CTime::CType type);
  void convertTime(SQL_TIMESTAMP_STRUCT& t,const CTime& timeStamp,CTime::CType type);

  /*!
   * \brief textual error description
   * \param hEnv: [in] environment handle where error occured
   * \param hConn: [in] connection handle where error occured
   * \param hStmt: [in] statement handle where error occured
   * \return textual error description
   */
  std::string error(SQLHANDLE hEnv=SQL_NULL_HANDLE,
    SQLHANDLE hConn=SQL_NULL_HANDLE,SQLHANDLE hStmt=SQL_NULL_HANDLE);

  /*!
   * \brief ODBC handle wrapper
   *
   * features type-safe auto cleanup and most recent call return code
   */
  template<SQLSMALLINT type>
  class CDbHandle
  {
  public:
    /*! ODBC handle */
    SQLHANDLE m_h;
    /*! most recent call return code */
    SQLRETURN m_rc;

    /*!
     * \brief constructor
     * \param ih: [in] parent handle for the handle being allocated (omit for environment handle)
     */
    CDbHandle(SQLHANDLE ih=SQL_NULL_HANDLE): m_h(NULL)
    {
      STRACE();

      m_rc=SQLAllocHandle(type,ih,&m_h);

      if(FAILED(m_rc))
        throw DBXCE("SQLAllocHandle","");
    }
    ~CDbHandle()
    {
      m_rc=SQLFreeHandle(type,m_h);
    }
  };

  /*!
   * \brief ODBC environment wrapper
   */
  class CDbEnv : public CDbHandle<SQL_HANDLE_ENV>
  {
  public:
    CDbEnv() : CDbHandle<SQL_HANDLE_ENV>()
    {
      STRACE();
      m_rc=SQLSetEnvAttr(m_h, SQL_ATTR_ODBC_VERSION,(void*)SQL_OV_ODBC3,0);

      if(FAILED(m_rc))
        throw DBXCE("SQLSetEnvAttr, SQL_ATTR_ODBC_VERSION",error(m_h));
    }
    ~CDbEnv()
    {
    }
  };

}

#endif
