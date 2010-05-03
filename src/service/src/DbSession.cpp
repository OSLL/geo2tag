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
#include <syslog.h>
#include "Handle.h"
#include "DbSession.h"
#include "DbQueryX.h"
#include "DbConn.h"
#include "Transaction.h"
#include "ExecuteClose.h"
#include "DataMarkInternal.h"
#include "ChannelInternal.h"
#include "UserInternal.h"
#include "DynamicCastFailure.h"

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
    char token[65];

    User():id(0)
    {
      memset(login, 0, 50);
      memset(password, 0, 50);
      memset(token, 0, 50);
    }
  };

  struct Mark
  {
    unsigned long id;
    double latitude;
    double longitude;
    char label[1024];
    char description[2048];
    char url[2048];
    SQL_TIMESTAMP_STRUCT time;
    SQLLEN timeLen;
    unsigned long user_id;

    Mark():id(0), latitude(0.), longitude(0.), user_id(0)
    {
      memset(label, 0, 1024);
      memset(description, 0, 2048);
      memset(url, 0, 2048);
      memset(&time, 0, sizeof(time));
      memset(&timeLen, 0, sizeof(timeLen));
    }
  };

  struct Channel
  {
    unsigned long id;
    char name[300];
    char description[2048];
    char url[2048];

    Channel():id(0)
    {
      memset(name, 0, 1024);
      memset(description, 0, 2048);
      memset(url, 0, 2048);
    }
  };

  struct MarkRelation
  {
    unsigned long mark;
    unsigned long channel;

    MarkRelation():mark(0), channel(0)
    {
    }
  };
  
  struct SubscribeRelation
  {
    unsigned long user;
    unsigned long channel;

    SubscribeRelation():user(0), channel(0)
    {
    }
  };

  using namespace ODBC;

  class LoadMarksQuery: public Mark, public CDbQueryX
  {
  public:
    LoadMarksQuery(const CDbConn& conn): CDbQueryX(conn)
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
      COL_NAME(    8, "user_id",     SQL_C_LONG, user_id)
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
      COL_NAME(4, "url", SQL_C_CHAR, url)
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
  
  class RemoveChannelQuery: public Channel, public CDbQueryX
  {
  public:
    RemoveChannelQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
    END_COLMAP()
    
    BEGIN_PARMAP()
      PAR(1, SQL_C_LONG, SQL_INTEGER, id)
    END_PARMAP()

    const char* sql() const
    {
      return "delete from channel where id=?;";
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
      return "select tag_id, channel_id from tags;";
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
      COL_NAME(4, "token", SQL_C_CHAR, token)
    END_COLMAP()

    const char* sql() const
    {
      return "select id, login, password, token from users order by id;";
    }
  };
  
  /////// Users: User <--(subscribed)--> Channel
  
  class LoadSubscribedQuery: public SubscribeRelation, public CDbQueryX
  {
  public:
    LoadSubscribedQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
      COL_NAME(1, "user_id", SQL_C_LONG, user)
      COL_NAME(2, "channel_id", SQL_C_LONG, channel)
    END_COLMAP()

    const char* sql() const
    {
      return "select user_id, channel_id from subscribe;";
    }
  };

  class StoreSubscribedQuery: public SubscribeRelation, public CDbQueryX
  {
  public:
    StoreSubscribedQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
    END_COLMAP()
    
    BEGIN_PARMAP()
      PAR(1, SQL_C_LONG, SQL_INTEGER, user)
      PAR(2, SQL_C_LONG, SQL_INTEGER, channel)
    END_PARMAP()

    const char* sql() const
    {
      return "insert into subscribe (user_id,channel_id) values(?,?);";
    }
  };
  
  class RemoveSubscribedQuery: public SubscribeRelation, public CDbQueryX
  {
  public:
    RemoveSubscribedQuery(const CDbConn& conn): CDbQueryX(conn)
    {
    }

    BEGIN_COLMAP()
    END_COLMAP()
    
    BEGIN_PARMAP()
      PAR(1, SQL_C_LONG, SQL_INTEGER, user)
      PAR(2, SQL_C_LONG, SQL_INTEGER, channel)
    END_PARMAP()

    const char* sql() const
    {
      return "delete from subscribe where user_id=? and channel_id=?;";
    }
  };
}

namespace common
{
  DbSession::DbSession(): ODBC::CDbConn(*(ODBC::CDbEnv*)this), m_marks(makeHandle(new DataMarks)), m_channels(makeHandle(new Channels))
  {
    m_users = makeHandle(new std::vector<CHandlePtr<common::User> >());
    syslog(LOG_INFO, "trying to connect to database..., file: %s, line: %ld", __FILE__, __LINE__);
    try
    {
      connect("geo2tag");
      syslog(LOG_INFO, "connected to database");
      m_updateThread = makeHandle(new UpdateThread<DbSession>(this));
    }
    catch(...)
    {
      syslog(LOG_INFO, "UUuuups...");
    }
  }

  void DbSession::loadUsers()
  {
    db::LoadUsersQuery query(*this);
    query.prepare();
    ODBC::CExecuteClose x(query);
    while(query.fetch())
    {
      if(s_users.count(query.id)>0) // This user already exists we shoudl skip it.
        continue;
      CHandlePtr<loader::User> u = makeHandle(new loader::User(query.login, query.password, query.id, query.token));
      m_users->push_back(u);
      s_users[query.id] = u;
      m_tokensMap[query.token] = u; // store alias between hash and User. 
    }
  }
 

  const std::map<std::string,CHandlePtr<common::User> >& DbSession::getTokensMap() const
  {
    return m_tokensMap;
  }
 
  void DbSession::loadMarks()
  {
    int i = 0;
    db::LoadMarksQuery query(*this);
    query.prepare();
    query.execute();
    while(query.fetch())
    {
      if(s_marks.count(query.id)>0)
      {
        continue;
      }
      else
      {
        syslog(LOG_INFO, "loading mark with id=[%lu]", query.id);
      }
      i++;
      CHandlePtr<loader::DataMark> mark= makeHandle(new loader::DataMark(query.id,
                            query.latitude,
                            query.longitude,
                            query.label,
                            query.description,
                            query.url,
                            ODBC::convertTime(query.time,CTime::UTC),
                            s_users.find(query.user_id)->second,
                            CHandlePtr<loader::Channel>()));

      s_marks[query.id] = mark;
      m_marks->push_back(mark);
    syslog(LOG_INFO, "loaded new %i mark[%f,%f]", i, query.latitude, query.longitude);
    }
    syslog(LOG_INFO, "loaded new %i marks", i);
  }
  
  void DbSession::storeMark(CHandlePtr<common::DataMark> m)
  {
    CHandlePtr<loader::DataMark> mark = m.dynamicCast<loader::DataMark>();
    if(!mark)
    {
      // epic fail!!!
      // std::ostringstream s("Can't cast from common::DataMark to loader::, DbSession::"); s << __func__ << __LINE__;
      syslog(LOG_INFO,"Failure at storeMark, dynamicCast<loader::DataMark>(), at storeMark ");
//      throw CDynamicCastFailure(1,SRC(),1);
      return;
    }
    
    if(mark->getId()==0)
    {
      CHandlePtr<loader::User> user = mark->getUser().dynamicCast<loader::User>();
      if(!user){
        syslog(LOG_INFO,"Failure at storeMark, dynamicCast<loader::User>(), at storeMark");
//        throw CDynamicCastFailure(1,SRC(),1);
       return; // epic fail!!!
      }
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
      storeQuery.description[2047]='\0';
      strncpy(storeQuery.url,mark->getUrl().c_str(),2047);
      storeQuery.url[2047]='\0';
      storeQuery.user_id = user->getId();
      
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


  void DbSession::unsubscribe(CHandlePtr<common::User> user, CHandlePtr<common::Channel> channel)
  {
    CHandlePtr<loader::User> u = user.dynamicCast<loader::User>();
    CHandlePtr<loader::Channel> c = channel.dynamicCast<loader::Channel>();
    if (!u || !c)
    {
       // we need error checking & processing here
       return;
    }
    unsigned long long user_id=u->getId(), channel_id=c->getId();
     
    db::RemoveSubscribedQuery query(*this);
    query.user = user_id;
    query.channel = channel_id;
    try
    {
      ODBC::CTransaction tr(*this);
      query.prepare();
      query.execute();
      u->unsubscribe(c);
    }
    catch (...)
    {
      // we need error checking & processing here
    }
    
  }
  
  void DbSession::subscribe(const std::string& userName, const std::string &hannelName)
  {
    syslog(LOG_INFO, "entered %s %s", __FILE__, __func__);
    unsigned long long user_id, channel_id;
    CHandlePtr<loader::Channel> u;
    CHandlePtr<loader::User> du; 
    syslog(LOG_INFO, "subscribe: users->size=%ld", m_users->size());
    for(size_t i=0; i<m_users->size(); ++i)
    {
	      syslog(LOG_INFO, "subscribe: i=%ld", i);
        du = ((*m_users)[i]).dynamicCast<loader::User>();
        if(du && du->getLogin() == userName)
        {
             user_id = du->getId();
             break;
        }
    }
    for(size_t i=0; i< m_channels->size(); ++i)
    {
        u  = ((*m_channels)[i]).dynamicCast<loader::Channel>();
        if(u && u->getName() == hannelName)
        {
             channel_id = u->getId();
             break;
        }
    }
    du->subscribe(u);
    db::StoreSubscribedQuery query(*this);
    query.user = user_id;
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
  void DbSession::updateChannel(unsigned long long channel_id,  CHandlePtr<common::DataMark> m) //! this routine will be in private area
  {
    CHandlePtr<loader::DataMark> mark = m.dynamicCast<loader::DataMark>();
    if(mark->getId()!=0)
    {
        // syslog(LOG_INFO,"Failure at updateChannel, dynamicCast<loader::DataMark>(), in Dbsession::updateChannel");
        // It's not a failure. because this mark already in channel!
	return;
    }
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

  void DbSession::removeChannel(CHandlePtr<common::Channel> c)
  {
      CHandlePtr<loader::Channel> ch = c.dynamicCast<loader::Channel>();
      if(!ch)
      {
        syslog(LOG_INFO,"Failure at removeChannel, dynamicCast<loader::Channel>() at removeChannel");
	      return; 
      }
      
      if(ch->getId()==0)
      {
        return;
      }
      else
      {
        ODBC::CTransaction tr(*this);
        db::RemoveChannelQuery removeQuery(*this);
        removeQuery.id = ch->getId();
        removeQuery.prepare();
        removeQuery.execute();
      }
  }
  void DbSession::storeChannel(CHandlePtr<common::Channel> c)
  {
      CHandlePtr<loader::Channel> ch = c.dynamicCast<loader::Channel>();
      if(!ch)
      {
        syslog(LOG_INFO,"Failure at storeChannel, dynamicCast<loader::Channel>() at storeChannel");
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
    { // LoadMarkRelationsQuery block
      syslog(LOG_INFO, "loading MarkRelations: mark <=> tag");
      db::LoadMarkRelationsQuery query(*this);
      query.prepare();
      query.execute();
      while(query.fetch())
      {
        syslog(LOG_INFO, "loadRelations, fetch");
        CHandlePtr<loader::Channel> channel;
        CHandlePtr<loader::DataMark> mark;
        if(s_channels.count(query.channel))
        {
          channel = s_channels.find(query.channel)->second;
        }
        else
        {
          syslog(LOG_INFO, "channel id=[%lu] is missing", query.channel);
          continue;
        }
        if(s_marks.count(query.mark))
        {
          mark = s_marks.find(query.mark)->second;
        }
        else
        {
          syslog(LOG_INFO, "mark id=[%lu] is missing", query.mark);
          continue;
        }
        mark->setChannel(channel);
        channel->addData(mark);
      }
    }
    { // LoadSubscribedQuery block
      syslog(LOG_INFO, "loading MarkRelations: user <=> channel");
      db::LoadSubscribedQuery query(*this);
      query.prepare();
      query.execute();
      while(query.fetch())
      {
	bool alreadyHas=0;
        for (common::Channels::iterator i=s_users.find(query.user)->second->getSubscribedChannels()->begin();
		i!=s_users.find(query.user)->second->getSubscribedChannels()->end();i++)
		{
			if (((*i)->getName())==(s_channels.find(query.channel)->second->getName())) {
				alreadyHas=1;
				break;
			}
		}
        if (!alreadyHas) s_users.find(query.user)->second->subscribe(s_channels.find(query.channel)->second);
      }
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
    syslog(LOG_INFO, "m_channels->size()=%ld", m_channels->size());
    return m_channels;
  }
  
  CHandlePtr<std::vector<CHandlePtr<common::User> > > DbSession::getUsers() const
  {
    return m_users;
  }

  void DbSession::loadData()
  {
    loadUsers();
    loadChannels();
    loadMarks(); 
    loadRelations();
    syslog(LOG_INFO,"Objects has been updated");
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
    m_updateThread.staticCast<UpdateThread<DbSession> >()->shutdown();
    m_updateThread->join();
  }
} // namespace common

/* ===[ End of file ]=== */
