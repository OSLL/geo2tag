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
#include "DataMarkInternal.h"

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

  struct Channel
  {
    unsigned long long id;
    char description[2048];
  }

  struct Marks
  {
    unsigned long long mark;
    unsigned long long channel;
  }

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
      return "select id, time, latitude, longitude, label, description from mark order by time;";
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
      return "insert into mark (latitude, longitude, label, description,id) values(?,?,?,?,?);";
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
      return "update mark set latitude=?, longitude=?, label=?, description=? where id=?;";
    }
  };

  /////// Channels 
  
  class LoadChannelQuery: public Channel, public CDbQueryX
  {
  public:
    LoadChannelQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
      COL_NAME(1, "id", SQL_C_LONG, id)
      COL_NAME(2, "description", SQL_C_CHAR, description)
    END_COLMAP()

    const char* sql() const
    {
      return "select id, description from channel order by id;";
    }
  };

  class NewChannelKeyQuery: public CDbQueryX
  {
  public:
    long m_key;
    
    NewChannelKeyQuery(const CDbConn &dbConn) : CDbQueryX(dbConn)
    {
    }

    BEGIN_COLMAP()
      COL_NAME(1,"key",SQL_C_LONG,m_key)
    END_COLMAP()
    
    const char *sql() const
    {
      return "select nextval('channels_seq') as key;";
    }
  };

  class StoreChannelQuery: public Mark, public CDbQueryX
  {
  public:
    StoreChannelQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
    END_COLMAP()
    
    BEGIN_PARMAP()
      PAR(1, SQL_C_CHAR, SQL_CHAR, description)
      PAR(2, SQL_C_LONG, SQL_INTEGER, id)
    END_PARMAP()

    const char* sql() const
    {
      return "insert into channel (description,id) values(?,?);";
    }
  };
  
  class UpdateChannelQuery: public Mark, public CDbQueryX
  {
  public:
    UpdateChannelQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
    END_COLMAP()
    
    BEGIN_PARMAP()
      PAR(4, SQL_C_CHAR, SQL_CHAR, description)
      PAR(5, SQL_C_LONG, SQL_INTEGER, id)
    END_PARMAP()

    const char* sql() const
    {
      return "update channel set description=? where id=?;";
    }
  };
  
}

namespace common
{
  DbSession::DbSession(): ODBC::CDbConn(*(ODBC::CDbEnv*)this)
  {
    connect("geo2tag");
  }
  
  void DbSession::loadMarks()
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
      m_marks->push_back(mark);
    }
  }

  void DbSession::saveMarks()
  { 
    for(int i=0; i<m_marks->size(); i++)
    {
      CHandlePtr<loader::DataMark> mark = (*m_marks)[i].dynamicCast<loader::DataMark>();
      if(!mark)
      {
        // big error!!!
        continue;
      }
      
      if(mark->getId()==0)
      {
        ODBC::CTransaction tr(*this);
        // Here is new object, has been created by user
        db::NewMarkKeyQuery query(*this);
        query.prepare();
        {
          ODBC::CExecuteClose keyExec(query);
          query.fetchNoEmpty();
        }
        
        mark->setId(query.m_seq);
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
        updateQuery.description[2047]='\0';
        
        updateQuery.prepare();
        updateQuery.execute();
      }
    }
  }
 
  void DbSession::loadChannels()
  {
    db::LoadChannelsQuery query(*this);
    query.prepare();
    ODBC::CExecuteClose x(query);
    while(query.fetch())
    {
      CHandlePtr<common::Channel> ch= makeHandle(new loader::channel(query.id,
                            query.description,
                            ));
      m_channels->push_back(ch);
    }

  }

  void DbSession::saveChannels()
  {
    for(int i=0; i<m_channels->size(); i++)
    {
      CHandlePtr<loader::Channel> ch = (*m_channels)[i].dynamicCast<loader::Channel>();
      if(!ch)
      {
        // big error!!!
        continue;
      }
      
      if(ch->getId()==0)
      {
        ODBC::CTransaction tr(*this);
        // Here is new object, has been created by user
        db::NewChannelKeyQuery query(*this);
        query.prepare();
        {
          ODBC::CExecuteClose keyExec(query);
          query.fetchNoEmpty();
        }
        
        ch->setId(query.m_key);
        db::StoreChannelQuery storeQuery(*this);
        storeQuery.id = ch->getId();
        strncpy(storeQuery.description,ch->getDescription().c_str(),2047);
        storeQuery.description[2047]='\0';
        
        storeQuery.prepare();
        storeQuery.execute();
      }
      else
      {
        ODBC::CTransaction tr(*this);
        // this object need to be updated in data base
        
        db::UpdateMarkQuery updateQuery(*this);
        updateQuery.id = ch->getId();
        strncpy(updateQuery.description,ch->getDescription().c_str(),2047);
        updateQuery.description[2047]='\0';
        
        updateQuery.prepare();
        updateQuery.execute();
      }
    }
  }
  
  void DbSession::loadRelations()
  {
  }

  void DbSession::saveRelations()
  {
  }

CHandlePtr<DataMarks> DbSession::getMarks() const
  {
    return m_marks;
  }

  CHandlePtr<Channels> DbSession::getChannels() const
  {
    return m_channels;
  }

  void DbSession::loadData()
  {
    loadMarks();
    loadChannels();
    loadRelations();
  }

  void DbSession::saveData()
  {
    saveChannels();
    saveMarks();
    saveRelations();
  }

  static DbSession& DbSession::getInstance()
  {
    static DbSession s;
    return s;
  }
  
  DbSession::~DbSession()
  {

  }
} // namespace common

/* ===[ End of file ]=== */
