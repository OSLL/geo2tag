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
    bool disconnect()
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

    /*!
     * \brief проверка наличия соединения с БД
     * \return установлено ли соединение
     */
    bool isConnected() const
    {
      return m_isConnected;
    }

    explicit CDbConn(const CDbEnv &dbEnv) : CDbHandle<SQL_HANDLE_DBC>(dbEnv.m_h),m_env(dbEnv)
    {
      m_isConnected=false;
    }

    ~CDbConn()
    {
      if(m_isConnected)
        disconnect();
    }

    /*!
     * \brief connection-specific error text
     * \return last error textual description
     * text may be empty. see also ODBC::error
     */
    std::string error() const
    {
      return ODBC::error(m_env.m_h,m_h);
    }

    bool setManualTransaction(bool isManual=true)
    {
      m_rc=SQLSetConnectAttr(
        m_h,SQL_ATTR_AUTOCOMMIT,(void*)(isManual?SQL_AUTOCOMMIT_OFF:SQL_AUTOCOMMIT_ON),0);

      return SUCCEEDED(m_rc);
    }

    //bool setTransactionIsolation()

    bool endTransaction(bool isCommit=true)
    {
      m_rc=SQLEndTran(SQL_HANDLE_DBC,m_h,isCommit?SQL_COMMIT:SQL_ROLLBACK);
      return SUCCEEDED(m_rc);
    }

    bool commit()
    {
      return endTransaction(true);
    }

    bool rollback()
    {
      return endTransaction(false);
    }
  };//class CDbConn

  /*!
   * \brief автоматически завершаемая транзакция
   */
  class CTransaction
  {
    CTransaction(const CTransaction&);
    CTransaction& operator =(const CTransaction&);

    /*! соединение с БД */
    CDbConn& m_conn;

    /*! статус завершения транзакции */
    bool m_isCommit;

  public:
    /*!
     * \brief
     * \param conn: [in] соединение с БД в котором начинается транзакция
     * \param isCommit: [in] подтверждать ли транзакцию по завершению
     */
    explicit CTransaction(CDbConn& conn,bool isCommit=true):
      m_conn(conn),m_isCommit(isCommit)
    {
      STRACE();

      if(!m_conn.setManualTransaction())
        throw CException(SRC(),"",m_conn.error());
    }
    ~CTransaction()
    {
      m_conn.endTransaction(m_isCommit);
      m_conn.setManualTransaction(false);
    }

    /*!
     * \brief установить статус завершения
     * \param isCommit: [in] подтверждать ли транзакцию по завершению
     */
    void setCommit(bool isCommit=true)
    {
      m_isCommit=isCommit;
    }
  };//class CTransaction


  /*!
   * \brief ODBC statement wrapper
   */
  class CDbStmt : public CDbHandle<SQL_HANDLE_STMT>
  {
  public:
    const CDbConn &m_conn;

    explicit CDbStmt(const CDbConn &dbConn) : CDbHandle<SQL_HANDLE_STMT>(dbConn.m_h),m_conn(dbConn)
    {
      STRACE();
      if(m_h==NULL)
        throw DBXCO("allocating STMT handle",*this);
    }

    ~CDbStmt()
    {
      if(m_h!=NULL)
        SQLCloseCursor(m_h);
    }

    #if 0
    void nativeSql(const char *pSql)
    {
      char text[1000];
      SQLINTEGER sz;

      SQLNativeSql(m_conn.m_h,(SQLCHAR*)pSql,SQL_NTS,(SQLCHAR*)text,sizeof(text),&sz);
      TRACE("I: nativeSql:\n\t%s\n\t%s\n",pSql,text);
    }
    #endif

    /*!
     * \brief prepares SQL statement
     * \param sql: [in] SQL statement text
     * \return whether prepare succeeded
     */
    bool prepare(const char *sql)
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
    bool execute()
    {
      m_rc=SQLExecute(m_h);

      return SUCCEEDED(m_rc);
    }

    /*!
     * \brief executes SQL statement directly
     * \param sql: [in] SQL statement text
     * \return whether execution succeeded
     */
    bool execDirect(const char *sql)
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
    bool bind(int colNumber, int targetType, void *target, SQLLEN targetLen,SQLLEN *lenOrInd)
    {
      m_rc=SQLBindCol(m_h,colNumber,targetType,target,targetLen,lenOrInd);

      return SUCCEEDED(m_rc);
    }

    bool unbind()
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
    bool fetch()
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
    bool getData(int colNumber, int targetType, void *target, SQLLEN targetLen,SQLLEN *lenOrInd)
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
    bool isFetchFailed() const
    {
      return FAILED(m_rc) && m_rc!=SQL_NO_DATA;
    }

    /*!
     * \brief closes cursor
     * \return whether close succeeded
     */
    bool close()
    {
      m_rc=SQLCloseCursor(m_h);

      return SUCCEEDED(m_rc);
    }

    bool bindParameter(int parNumber,int ioType,int valType,int parType,
      int colSize,int decDigits,void *target,SQLLEN bufferLen,SQLLEN *lenOrInd)
    {
      m_rc=SQLBindParameter(m_h,parNumber,ioType,valType,parType,
        colSize,decDigits,target,bufferLen,lenOrInd);

      return SUCCEEDED(m_rc);
    }

    bool resetParams()
    {
      m_rc=SQLFreeStmt(m_h,SQL_RESET_PARAMS);

      return SUCCEEDED(m_rc);
    }

    bool colAttribute(int colNumber,int attribute,char *pString,
      int stringSz,SQLSMALLINT *pStringLen, SQLINTEGER *pNumeric)
    {
      m_rc=SQLColAttribute(m_h,colNumber,attribute,
        pString,stringSz,pStringLen,pNumeric);

      return SUCCEEDED(m_rc);
    }

    std::string error() const
    {
      return ODBC::error(m_conn.m_env.m_h,m_conn.m_h,m_h);
    }
  };//class CDbStmt

  /*!
   * \brief
   */
  class CExecuteClose
  {
    /*! оператор SQL */
    CDbStmt& m_stmt;

  public:
    explicit CExecuteClose(CDbStmt& stmt): m_stmt(stmt)
    {
      m_stmt.execute();
    }

    ~CExecuteClose()
    {
      m_stmt.close();
    }
  };
}

#endif
