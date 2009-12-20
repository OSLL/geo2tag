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

#include <iostream>
#include <vector>
#include <map>
#include <string.h>
#include "Handle.h"
#include "DbSession.h"
#include "DbQuery.h"
#include "Db.h"
#include "DataMarkInternal.h"
#include "ChannelInternal.h"

static std::map<long, CHandlePtr<loader::DataMark> > s_marks = std::map<long, CHandlePtr<loader::DataMark> >();
static std::map<long, CHandlePtr<loader::Channel> > s_channels = std::map<long, CHandlePtr<loader::Channel> >();

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
  };

  struct MarkRelation
  {
    unsigned long long mark;
    unsigned long long channel;
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

  class StoreChannelQuery: public Channel, public CDbQueryX
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
  
  class UpdateChannelQuery: public Channel, public CDbQueryX
  {
  public:
    UpdateChannelQuery(const CDbConn& conn): CDbQueryX(conn)
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
      return "update channel set description=? where id=?;";
    }
  };
  
  /////// Marks: Mark <---> Channel
  
  class LoadMarkRelationsQuery: public MarkRelation, public CDbQueryX
  {
  public:
    LoadMarkRelationsQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
      COL_NAME(1, "mark_id", SQL_C_LONG, mark)
      COL_NAME(2, "channel_id", SQL_C_LONG, channel)
    END_COLMAP()

    const char* sql() const
    {
      return "select mark_id, channel_id from marks order by channel_id;";
    }
  };

  class StoreMarkRelationQuery: public MarkRelation, public CDbQueryX
  {
  public:
    StoreMarkRelationQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
    END_COLMAP()
    
    BEGIN_PARMAP()
      PAR(1, SQL_C_LONG, SQL_INTEGER, mark)
      PAR(2, SQL_C_LONG, SQL_INTEGER, channel)
    END_PARMAP()

    const char* sql() const
    {
      return "insert into marks (mark_id,channel_id) values(?,?);";
    }
  };
  
  class UpdateMarkRelationQuery: public MarkRelation, public CDbQueryX
  {
  public:
    UpdateMarkRelationQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
    END_COLMAP()
    
    BEGIN_PARMAP()
      PAR(1, SQL_C_LONG, SQL_INTEGER, channel)
      PAR(2, SQL_C_LONG, SQL_INTEGER, mark)
    END_PARMAP()

    const char* sql() const
    {
      return "update marks set channel_id=? where mark_id=?;";
    }
  };
}

namespace common
{
  DbSession::DbSession(): ODBC::CDbConn(*(ODBC::CDbEnv*)this), m_marks(makeHandle(new DataMarks)), m_channels(makeHandle(new Channels))
  {
    connect("geo2tag");
    std::cerr << "connected to database" << std::endl;
  }
  
  void DbSession::loadMarks()
  {
    db::LoadMarksQuery query(*this);
    query.prepare();
    ODBC::CExecuteClose x(query);
    while(query.fetch())
    {
      CHandlePtr<loader::DataMark> mark= makeHandle(new loader::DataMark(query.id,
                            query.latitude,
                            query.longitude,
                            query.label,
                            query.description,
                            ODBC::convertTime(query.time,CTime::UTC)
                            ));
      m_marks->push_back(mark);
      s_marks[query.id] = mark;
    }
  }
  
  void DbSession::storeMark(CHandlePtr<common::DataMark> m)
  {
    CHandlePtr<loader::DataMark> mark = m.dynamicCast<loader::DataMark>();
    if(!mark)
    {
      // epic fail!!!
      return;
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
      s_marks[mark->getId()] = mark;
      m_marks->push_back(mark); // update global marks list
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

  void DbSession::saveMarks()
  { 
    for(int i=0; i<m_marks->size(); i++)
    {
      storeMark((*m_marks)[i]);
    }
  }
 
  void DbSession::updateChannel(unsigned long long channel_id,  CHandlePtr<common::DataMark> m) //! this routine will be in private area
  {

    CHandlePtr<loader::DataMark> mark = m.dynamicCast<loader::DataMark>();
    if(mark->getId()!=0)
      return;
    
    // store(update) mark to DB
    common::DbSession::getInstance().storeMark(mark);

    db::StoreMarkRelationQuery query(*this);
    query.mark = mark->getId();
    query.channel = channel_id;
    try
    {
      ODBC::CTransaction tr(*this);
      query.prepare();
      query.execute();
    }
    catch (...)
    {
      // it is hack, but we need it
    }
  }
  
  void DbSession::loadChannels()
  {
    db::LoadChannelQuery query(*this);
    query.prepare();
    ODBC::CExecuteClose x(query);
    while(query.fetch())
    {
      CHandlePtr<loader::Channel> ch= makeHandle(new loader::Channel(query.id,
                            std::string(&(query.description[0])) ));
      m_channels->push_back(ch);
      s_channels[query.id] = ch;
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
        s_channels[ch->getId()]=ch;
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
    db::LoadMarkRelationsQuery query(*this);
    query.prepare();
    ODBC::CExecuteClose x(query);
    while(query.fetch())
    {
      s_channels.find(query.channel)->second->addData(s_marks.find(query.mark)->second);
    }
  }

  void DbSession::saveRelations()
  {
    // nothing
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
    loadMarks(); std::cerr << "Marks loaded" << std::endl;
    loadChannels();std::cerr << "Channels loaded" << std::endl;
    loadRelations();std::cerr << "Relations loaded" << std::endl;
  }

  void DbSession::saveData()
  {
    saveChannels();
    saveMarks();
    saveRelations();
  }

  DbSession& DbSession::getInstance()
  {
    static DbSession s;
    return s;
  }
  
  DbSession::~DbSession()
  {

  }
} // namespace common

/* ===[ End of file ]=== */
