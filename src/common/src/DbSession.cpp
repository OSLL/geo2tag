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

#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <string.h>
#include "Handle.h"
#include "DbSession.h"
#include "DbQuery.h"
#include "Db.h"
#include "Sleep.h"
#include "DataMarkInternal.h"
#include "ChannelInternal.h"
#include "UserInternal.h"
#include "GpsInfo.h"

static std::map<long, CHandlePtr<loader::DataMark> > s_marks = std::map<long, CHandlePtr<loader::DataMark> >();
static std::map<long, CHandlePtr<loader::Channel> > s_channels = std::map<long, CHandlePtr<loader::Channel> >();
static std::map<long, CHandlePtr<loader::User> > s_users = std::map<long, CHandlePtr<loader::User> >();

namespace db
{

  struct User
  {
    char login[50];
    char password[50];
    unsigned long id;
  };

  struct Mark
  {
    unsigned long long id;
    double latitude;
    double longitude;
    char label[1024];
    char description[2048];
    char url[2048];
    SQL_TIMESTAMP_STRUCT time;
    SQLLEN timeLen;
    unsigned long user_id;
  };

  struct Channel
  {
    unsigned long long id;
    char name[300];
    char description[2048];
    char url[2048];
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
    LoadMarksQuery(const CDbConn& conn, unsigned long user): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
      COL_NAME(    1, "id",          SQL_C_LONG, id)
      COL_NAME_LEN(2, "time",        SQL_C_TIMESTAMP, time, timeLen)
      COL_NAME(    3, "latitude",    SQL_C_DOUBLE, latitude)
      COL_NAME(    4, "longitude",   SQL_C_DOUBLE, longitude)
      COL_NAME(    5, "label",       SQL_C_CHAR, label)
      COL_NAME(    6, "description", SQL_C_CHAR, description)
      COL_NAME(    7, "url",         SQL_C_CHAR, url)
    END_COLMAP()
    
    const char* sql() const
    {
      return "select id, time, latitude, longitude, label, description, url, user_id from tag order by time;";
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
      return "select nextval('tags_seq') as seq;";
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
      PAR(5, SQL_C_CHAR, SQL_CHAR, url)
      PAR(6, SQL_C_LONG, SQL_INTEGER, user_id)
      PAR(7, SQL_C_LONG, SQL_INTEGER, id)
    END_PARMAP()

    const char* sql() const
    {
      return "insert into tag (latitude, longitude, label, description, url, user_id, id) values(?,?,?,?,?,?,?);";
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
      PAR(5, SQL_C_CHAR, SQL_CHAR, url)
      PAR(6, SQL_C_LONG, SQL_INTEGER, id)
    END_PARMAP()

    const char* sql() const
    {
      return "update tag set latitude=?, longitude=?, label=?, description=?, url=? where id=?;";
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
      COL_NAME(3, "name", SQL_C_CHAR, name)
      COL_NAME(4, "url", SQL_C_CHAR, name)
    END_COLMAP()

    const char* sql() const
    {
      return "select id, description, name, url from channel order by id;";
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
      PAR(1, SQL_C_CHAR, SQL_CHAR, name)
      PAR(2, SQL_C_CHAR, SQL_CHAR, description)
      PAR(3, SQL_C_CHAR, SQL_CHAR, url)
      PAR(4, SQL_C_LONG, SQL_INTEGER, id)
    END_PARMAP()

    const char* sql() const
    {
      return "insert into channel (name, description, url, id) values(?,?,?,?);";
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
      PAR(2, SQL_C_CHAR, SQL_CHAR, url)
      PAR(3, SQL_C_LONG, SQL_INTEGER, id)
    END_PARMAP()

    const char* sql() const
    {
      return "update channel set description=?, url=? where id=?;";
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
      COL_NAME(1, "tag_id", SQL_C_LONG, mark)
      COL_NAME(2, "channel_id", SQL_C_LONG, channel)
    END_COLMAP()

    const char* sql() const
    {
      return "select tag_id, channel_id from tags order by channel_id;";
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
      return "insert into tags (tag_id,channel_id) values(?,?);";
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
      return "update tags set channel_id=? where tag_id=?;";
    }
  };

  //////////// Users
  
  class LoadUsersQuery: public User, public CDbQueryX
  {
  public:
    LoadUsersQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
      COL_NAME(1, "id", SQL_C_LONG, id)
      COL_NAME(2, "login", SQL_C_CHAR, login)
      COL_NAME(3, "password", SQL_C_CHAR, password)
    END_COLMAP()

    const char* sql() const
    {
      return "select id, login, password from users order by id;";
    }
  };
  
}

namespace common
{
  class UpdateThread: public Thread::CThread
  {
    bool m_needExit;

    void thread()
    {
      while(!m_needExit)
      {
        mSleep(10000);
        CHandlePtr<loader::User> u = DbSession::getInstance().getCurrentUser().staticCast<loader::User>();
        DbSession::getInstance().loadData(u->getLogin(), u->getPassword());
      }
    }
    public:
      UpdateThread():m_needExit(false)
      {
        start();
      }

      void shutdown()
      {
        m_needExit = true;
      }
  };
  DbSession::DbSession(): ODBC::CDbConn(*(ODBC::CDbEnv*)this), m_marks(makeHandle(new DataMarks)), m_channels(makeHandle(new Channels))
  {
    m_users = makeHandle(new std::vector<CHandlePtr<common::User> >());
#ifndef NO_DB_CONNECTION
    connect("geo2tag");
    std::cerr << "connected to database" << std::endl;
    m_updateThread = makeHandle(new UpdateThread());
#else
    CHandlePtr<loader::User> user = makeHandle(new loader::User("test0","test",0));
    m_users->push_back(user);
    s_users[0]=user;
    m_currtentUserId = 0;
    
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(1,"weather", "Tags with weather"));
      m_channels->push_back(channel);
      s_channels[1]=channel;
    }
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(2,"sales", "Sales"));
      m_channels->push_back(channel);
      s_channels[2]=channel;
    }
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(3,"petroleum costs", "Petroleum stations, prices"));
      m_channels->push_back(channel);
      s_channels[3]=channel;
    }
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(4,"announcements", "Art, cinema, show announcements"));
      m_channels->push_back(channel);
      s_channels[4]=channel;
    }
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(5,"personal", "User personal tags"));
      m_channels->push_back(channel);
      s_channels[5]=channel;
    }
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(6,"government","tags connected to government" ));
      m_channels->push_back(channel);
      s_channels[6]=channel;
    }
#endif
  }

  void DbSession::loadUsers(const std::string &login, const std::string &password)
  {
    db::LoadUsersQuery query(*this);
    query.prepare();
    ODBC::CExecuteClose x(query);
    while(query.fetch())
    {
      if(s_users.count(query.id)>0)
        continue;
      CHandlePtr<loader::User> u = makeHandle(new loader::User(query.login, query.password, query.id));
      m_users->push_back(u);
      s_users[query.id] = u;
      if(u->getLogin() == login)
        m_currtentUserId = query.id;
    }
  }
  
  void DbSession::loadMarks()
  {
    db::LoadMarksQuery query(*this, m_currtentUserId);
    std::cerr << "current user_id = " << m_currtentUserId << std::endl;
    query.prepare();
    ODBC::CExecuteClose x(query);
    while(query.fetch())
    {
      if(common::DataMark::getDistance(common::GpsInfo::getInstance().getLatitude(),
                                       common::GpsInfo::getInstance().getLongitude(),
                                       query.latitude, query.longitude)>=5)
      {
        std::cerr << "!!!!!!!!!!!!!!!!!!!!!" << std::endl << "The furthest mark was found" << std::endl << " !!!!!!!!!!!!! " << std::endl;
        continue;
      }
      if(s_marks.count(query.id)>0)
      {
        continue;
      }
      CHandlePtr<loader::DataMark> mark= makeHandle(new loader::DataMark(query.id,
                            query.latitude,
                            query.longitude,
                            query.label,
                            query.description,
                            query.url,
                            ODBC::convertTime(query.time,CTime::UTC),
                            s_users.find(query.user_id)->second
                            ));
      m_marks->push_back(mark);
      s_marks[query.id] = mark;
    }
  }
  
  void DbSession::storeMark(CHandlePtr<common::DataMark> m)
  {
    std::cerr << "Begin storing marks" << std::endl;
    CHandlePtr<loader::DataMark> mark = m.dynamicCast<loader::DataMark>();
    if(!mark)
    {
      // epic fail!!!
      return;
    }
    
    if(mark->getId()==0)
    {
      std::cerr << "New use mark" << std::endl;
      ODBC::CTransaction tr(*this);
      // Here is new object, has been created by user
      db::NewMarkKeyQuery query(*this);
      query.prepare();
      {
        ODBC::CExecuteClose keyExec(query);
        query.fetchNoEmpty();
      }
      std::cerr << "New id for user's mark = " << query.m_seq << std::endl;
      
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
      storeQuery.description[2047]='\0';
      strncpy(storeQuery.url,mark->getUrl().c_str(),2047);
      storeQuery.url[2047]='\0';
      storeQuery.user_id = m_currtentUserId;
      
      std::cerr << "ready for save" << std::endl;
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
      strncpy(updateQuery.url,mark->getUrl().c_str(),2047);
      updateQuery.url[2047]='\0';
      
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

#ifndef NO_DB_CONNECTION
    // store(update) mark to DB
    storeMark(mark);
    
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
#else
    if(mark->getId()==0)
    {
      s_marks[m_marks->size()+1]=mark;
      mark->setId(m_marks->size()+1);
      m_marks->push_back(mark);
    }
#endif
  }
  
  void DbSession::loadChannels()
  {
    db::LoadChannelQuery query(*this);
    query.prepare();
    ODBC::CExecuteClose x(query);
    while(query.fetch())
    {
      if(s_channels.count(query.id)>0)
        continue;

      CHandlePtr<loader::Channel> ch= makeHandle(new loader::Channel(query.id,
                            std::string(&(query.name[0])), std::string(&(query.description[0])) ));
      m_channels->push_back(ch);
      s_channels[query.id] = ch;
    }

  }

  void DbSession::storeChannel(CHandlePtr<common::Channel> c)
  {
      CHandlePtr<loader::Channel> ch = c.dynamicCast<loader::Channel>();
      if(!ch)
      {
        // big error!!!
        return;
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
        strncpy(storeQuery.name,ch->getName().c_str(),300);
        storeQuery.name[299]='\0';
        strncpy(storeQuery.url,ch->getUrl().c_str(),2047);
        storeQuery.url[2047]='\0';
        
        storeQuery.prepare();
        storeQuery.execute();
      }
      else
      {
        ODBC::CTransaction tr(*this);
        // this object need to be updated in data base
        
        db::UpdateChannelQuery updateQuery(*this);
        updateQuery.id = ch->getId();
        strncpy(updateQuery.description,ch->getDescription().c_str(),2047);
        updateQuery.description[2047]='\0';
        // we don't need save Channel's name because we don't update it in SQL query 
        strncpy(updateQuery.url,ch->getUrl().c_str(),2047);
        updateQuery.url[2047]='\0';

        updateQuery.prepare();
        updateQuery.execute();
      }
  }

  void DbSession::saveChannels()
  {
    for(int i=0; i<m_channels->size(); i++)
    {
      storeChannel((*m_channels)[i]);
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

  void DbSession::loadData(const std::string& login, const std::string& password)
  {
#ifndef NO_DB_CONNECTION
    loadUsers(login,password);std::cerr << "Users loaded" << std::endl;
    loadMarks(); std::cerr << "Marks loaded" << std::endl;
    loadChannels();std::cerr << "Channels loaded" << std::endl;
    loadRelations();std::cerr << "Relations loaded" << std::endl;
#endif
  }

  void DbSession::saveData()
  {
#ifndef NO_DB_CONNECTION
    std::cerr << "Saving channels" << std::endl;
    saveChannels();
    std::cerr << "Saving marks" << std::endl;
    saveMarks();
    std::cerr << "Saving relations" << std::endl;
    saveRelations();
#endif
}

  CHandlePtr<common::User> DbSession::getCurrentUser() const
  {
    return s_users.find(m_currtentUserId)->second;
  }

  DbSession& DbSession::getInstance()
  {
    static DbSession s;
    return s;
  }
  
  DbSession::~DbSession()
  {
#ifndef NO_DB_CONNECTION
    m_updateThread.staticCast<UpdateThread>()->shutdown();
    m_updateThread->join();
#endif
  }
} // namespace common

/* ===[ End of file ]=== */
