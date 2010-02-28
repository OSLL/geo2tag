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
#ifndef DBCONN_H
#define DBCONN_H

#include "Db.h"
namespace ODBC
{
/*!
 * \brief ODBC connection wrapper
 */
class CDbConn : public CDbHandle<SQL_HANDLE_DBC>
{
public:
  bool m_isConnected;
  const CDbEnv &m_env;

  /*!
   * \brief establishes ODBC connection
   * \param dsn: [in] data source name
   * \param user: [in] optional user name (overrides dsn-specified)
   * \param pwd: [in] optional password
   * \param schema: [in] schema name
   * \return whether the connection has been established
   */
  bool connect(const std::string& dsn,const char *user=NULL,const char *pwd=NULL,const char *schema=NULL);
  void connectX(const std::string& dsn,const char *user=NULL,const char *pwd=NULL,const char *schema=NULL);

  /*!
   * \brief tears ODBC connection down
   * \return whether disconnect succeeded
   */
  bool disconnect();

  /*!
   * \brief проверка наличия соединения с БД
   * \return установлено ли соединение
   */
  bool isConnected() const;

  explicit CDbConn(const CDbEnv &dbEnv) ;

  ~CDbConn();

  /*!
   * \brief connection-specific error text
   * \return last error textual description
   * text may be empty. see also ODBC::error
   */
  std::string error() const;

  bool setManualTransaction(bool isManual=true);

  //bool setTransactionIsolation()

  bool endTransaction(bool isCommit=true);

  bool commit();

  bool rollback();

};//class CDbConn
};
#endif // DBCONN_H
