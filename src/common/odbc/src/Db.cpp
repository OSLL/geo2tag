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

};

/* ===[ End of file  ]=== */
