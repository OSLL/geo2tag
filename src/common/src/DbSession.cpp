/*! ---------------------------------------------------------------
 *  
 *
 * \file DbSession.cpp
 * \brief DbSession implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include <string.h>
#include "Handle.h"
#include "DbSession.h"
#include "DbQuery.h"
#include "Db.h"

namespace db
{

  struct Mark
  {
    unsigned long long id;
    double latitude;
    double longitude;
    char label[1024];
    char description[2048];
    SQL_TIMESTAMP_STRUCT time;
    SQLLEN timeLen;
  };

  using namespace ODBC;

  class LoadMarksQuery: public Mark, public CDbQueryX
  {
  public:
    LoadMarksQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
      COL_NAME(1, "id", SQL_C_LONG, id)
      COL_NAME_LEN(2, "time", SQL_C_TIMESTAMP, time, timeLen)
      COL_NAME(3, "latitude", SQL_C_DOUBLE, latitude)
      COL_NAME(4, "longitude", SQL_C_DOUBLE, longitude)
      COL_NAME(5, "label", SQL_C_CHAR, label)
      COL_NAME(6, "description", SQL_C_CHAR, description)
    END_COLMAP()

    const char* sql() const
    {
      return "select id, time, latitude, longitude, label, description from marks order by time;";
    }
  };

  class NewMarkKeyQuery: public CDbQueryX
  {
  public:
    long m_seq;
    
    NewMarkKeyQuery(const CDbConn &dbConn) : CDbQueryX(dbConn)
    {
    }

    BEGIN_COLMAP()
      COL_NAME(1,"seq",SQL_C_LONG,m_seq)
    END_COLMAP()
    
    const char *sql() const
    {
      return "select nextval('marks_seq') as seq;";
    }
  };

  class StoreMarkQuery: public Mark, public CDbQueryX
  {
  public:
    StoreMarkQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
    END_COLMAP()
    
    BEGIN_PARMAP()
      PAR(1, SQL_C_DOUBLE, SQL_DOUBLE, latitude)
      PAR(2, SQL_C_DOUBLE, SQL_DOUBLE, longitude)
      PAR(3, SQL_C_CHAR, SQL_CHAR, label)
      PAR(4, SQL_C_CHAR, SQL_CHAR, description)
      PAR(5, SQL_C_LONG, SQL_INTEGER, id)
    END_PARMAP()

    const char* sql() const
    {
      return "insert into marks (latitude, longitude, label, description,id) values(?,?,?,?,?);";
    }
  };
  
  class UpdateMarkQuery: public Mark, public CDbQueryX
  {
  public:
    UpdateMarkQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
    END_COLMAP()
    
    BEGIN_PARMAP()
      PAR(1, SQL_C_DOUBLE, SQL_DOUBLE, latitude)
      PAR(2, SQL_C_DOUBLE, SQL_DOUBLE, longitude)
      PAR(3, SQL_C_CHAR, SQL_CHAR, label)
      PAR(4, SQL_C_CHAR, SQL_CHAR, description)
      PAR(5, SQL_C_LONG, SQL_INTEGER, id)
    END_PARMAP()

    const char* sql() const
    {
      return "update marks set latitude=?, longitude=?, label=?, description=? where id=?;";
    }
  };
}

namespace loader
{
  class DataMark: public common::DataMark
  {
    unsigned long long m_id;

  public:
    DataMark(unsigned long long id, double latitude, double longitude, 
             std::string label, std::string description, const CTime& time):
              common::DataMark(latitude, longitude, label, description, time), m_id(id)
    {
    }
    
    unsigned long long getId() const
    {
      return m_id;
    }

    virtual ~DataMark()
    {
    }
  };
}

namespace common
{
  DbSession::DbSession(): ODBC::CDbConn(*(ODBC::CDbEnv*)this)
  {
    connect("geo2tag");
  }
  
  void DbSession::loadMarks(DataMarks &marks)
  {
    db::LoadMarksQuery query(*this);
    query.prepare();
    ODBC::CExecuteClose x(query);
    while(query.fetch())
    {
      CHandlePtr<common::DataMark> mark= makeHandle(new loader::DataMark(query.id,
                            query.latitude,
                            query.longitude,
                            query.label,
                            query.description,
                            ODBC::convertTime(query.time,CTime::UTC)
                            ));
      marks.push_back(mark);
    }
  }

  void DbSession::storeMarks(DataMarks &marks)
  {
    for(int i=0; i<marks.size(); i++)
    {
      CHandlePtr<common::DataMark> mp = marks[i];
      CHandlePtr<loader::DataMark> mark = mp.dynamicCast<loader::DataMark>();
      if(!mark)
      {
        ODBC::CTransaction tr(*this);
        // Here is new objectd, has been created by user
        db::NewMarkKeyQuery query(*this);
        query.prepare();
        {
          ODBC::CExecuteClose keyExec(query);
          query.fetchNoEmpty();
        }
        CHandlePtr<common::DataMark> userMark=marks[i];
        mark = makeHandle(new loader::DataMark(query.m_seq,
                            userMark->getLatitude(),
                            userMark->getLongitude(),
                            userMark->getLabel(),
                            userMark->getDescription(),
                            userMark->getTime()));
        marks[i]=mark;
        db::StoreMarkQuery storeQuery(*this);
        storeQuery.id = mark->getId();
        storeQuery.latitude = mark->getLatitude();
        storeQuery.longitude = mark->getLongitude();
        strncpy(storeQuery.label,mark->getLabel().c_str(),1023);
        storeQuery.label[1023]='\0';
        strncpy(storeQuery.description,mark->getDescription().c_str(),2047);
        storeQuery.label[2047]='\0';
        
        storeQuery.prepare();
        storeQuery.execute();
      } 
      else
      {
        ODBC::CTransaction tr(*this);
        // this object need to be updated in data base
        
        db::UpdateMarkQuery updateQuery(*this);
        updateQuery.id = mark->getId();
        updateQuery.latitude = mark->getLatitude();
        updateQuery.longitude = mark->getLongitude();
        strncpy(updateQuery.label,mark->getLabel().c_str(),1023);
        updateQuery.label[1023]='\0';
        strncpy(updateQuery.description,mark->getDescription().c_str(),2047);
        updateQuery.label[2047]='\0';
        
        updateQuery.prepare();
        updateQuery.execute();
      }
    }
  }
  
  DbSession::~DbSession()
  {

  }
} // namespace common

/* ===[ End of file ]=== */
