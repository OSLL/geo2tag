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
    unsigned long id;
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
    unsigned long id;
    char name[300];
    char description[2048];
    char url[2048];
  };

  struct MarkRelation
  {
    unsigned long mark;
    unsigned long channel;
  };
  
  struct SubscribeRelation
  {
    unsigned long user;
    unsigned long channel;
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
    END_COLMAP()

    const char* sql() const
    {
      return "select id, login, password from users order by id;";
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
  
}

namespace common
{
  DbSession::DbSession(): ODBC::CDbConn(*(ODBC::CDbEnv*)this), m_marks(makeHandle(new DataMarks)), m_channels(makeHandle(new Channels))
  {
    m_users = makeHandle(new std::vector<CHandlePtr<common::User> >());
#ifndef NO_DB_CONNECTION
    syslog(LOG_INFO, "trying to connect to database..., file: %s, line: %ld", __FILE__, __LINE__);
    connect("geo2tag");
    syslog(LOG_INFO, "connected to database");
    m_updateThread = makeHandle(new UpdateThread<DbSession>(this));
#else
    CHandlePtr<loader::User> user = makeHandle(new loader::User("Paul","test",0));
    m_users->push_back(user);
    s_users[0]=user;
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(1,"Tourist information", "This is free read-only tourist information channel. You can get information about buildings, sights around your location"));
      m_channels->push_back(channel);
      s_channels[1]=channel;
    }
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(2,"Public announcements", "This is free read-only channel with public announcements from the city of your current location"));
      m_channels->push_back(channel);
      s_channels[2]=channel;
    }
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(3,"Fuel prices", "This is free channel where you can post/read the fuel prices around your current location."));
      m_channels->push_back(channel);
      s_channels[3]=channel;
    }
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(4,"Sales", "This is free channel where you can post/read sales advertisements"));
      m_channels->push_back(channel);
      s_channels[4]=channel;
    }
    {
      CHandlePtr<loader::Channel> channel = makeHandle(new loader::Channel(5,"My channel", "This is free channel where you and your friends can post/read your tags"));
      m_channels->push_back(channel);
      s_channels[5]=channel;
    }
    {
      CHandlePtr<loader::DataMark> m = makeHandle(new loader::DataMark(1,60.166504, 24.841204, "A", "Accident at road 51. Time: 15:45, January 2, 2010.", "http://dps.sd.gov/licensing/driver_licensing/images/Image24.gif", CTime::now(), user, s_channels.find(2)->second));
      m_marks->push_back(m);
      s_marks[1]=m;
    }
    {
      CHandlePtr<loader::DataMark> m = makeHandle(new loader::DataMark(2,60.163216, 24.859314, "B", "Shell 95: 1.299 e", "http://www.unf.edu/groups/volctr/images/question-mark.jpg", CTime::now(), user, s_channels.find(3)->second));
      m_marks->push_back(m);
      s_marks[2]=m;
    }
    {
      CHandlePtr<loader::DataMark> m = makeHandle(new loader::DataMark(3,60.166264, 24.859915, "C", "Neste Oil diesel: .0989 e", "http://www.unf.edu/groups/volctr/images/question-mark.jpg", CTime::now(), user, s_channels.find(3)->second));
      m_marks->push_back(m);
      s_marks[3]=m;
    }
    {
      CHandlePtr<loader::DataMark> m = makeHandle(new loader::DataMark(4,60.163216, 24.859915, "D", "Neste 95: 1.294 e","http://www.unf.edu/groups/volctr/images/question-mark.jpg", CTime::now(), user, s_channels.find(3)->second));
      m_marks->push_back(m);
      s_marks[4]=m;
    }
    {
      CHandlePtr<loader::DataMark> m = makeHandle(new loader::DataMark(5,60.162159, 24.739065, "E", "CityMarket Iso Omena: Children overalls 50% off", "http://www.k-citymarket.fi/img/citymarketLogo.gif" ,CTime::now(), user, s_channels.find(4)->second));
      m_marks->push_back(m);
      s_marks[5]=m;
    }
    {
      CHandlePtr<loader::DataMark> m = makeHandle(new loader::DataMark(6,60.167433, 24.733486, "F", "Prisma Olari: LEGO 20% off", "http://mw2.google.com/mw-panoramio/photos/thumbnail/5131282.jpg", CTime::now(), user, s_channels.find(4)->second));
      m_marks->push_back(m);
      s_marks[6]=m;
    }
    {
      CHandlePtr<loader::DataMark> m = makeHandle(new loader::DataMark(7,60.173362, 24.829166, "G", "Outdoor party place on February 20, 2010", "http://www.unf.edu/groups/volctr/images/question-mark.jpg", CTime::now(), user, s_channels.find(5)->second));
      m_marks->push_back(m);
      s_marks[7]=m;
    }
    {
      CHandlePtr<loader::DataMark> m = makeHandle(new loader::DataMark(8,60.178308, 24.848585, "H", "Espoo. Cheap hotel.","http://mw2.google.com/mw-panoramio/photos/small/8267319.jpg", CTime::now(), user, s_channels.find(1)->second));
      m_marks->push_back(m);
      s_marks[8]=m;
    }
#endif
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
      CHandlePtr<loader::User> u = makeHandle(new loader::User(query.login, query.password, query.id));
      m_users->push_back(u);
      s_users[query.id] = u;
    }
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
      // throw CDynamicCastFailure(s.str());
      return;
    }
    
    if(mark->getId()==0)
    {
      CHandlePtr<loader::User> user = mark->getUser().dynamicCast<loader::User>();
      if(!user)
        return; // epic fail!!!

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
//	syslog(LOG_INFO, "subscribe: du= %ld", du);
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
#ifndef NO_DB_CONNECTION
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
#endif
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
    { // LoadMarkRelationsQuery block
      syslog(LOG_INFO, "loading MarkRelations: mark <=> tag");
      db::LoadMarkRelationsQuery query(*this);
      query.prepare();
      query.execute();
      while(query.fetch())
      {
        s_channels.find(query.channel)->second->addData(s_marks.find(query.mark)->second);
        s_marks.find(query.mark)->second->setChannel(s_channels.find(query.channel)->second);
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
    return m_channels;
  }
  
  CHandlePtr<std::vector<CHandlePtr<common::User> > > DbSession::getUsers() const
  {
    return m_users;
  }

  void DbSession::loadData()
  {
#ifndef NO_DB_CONNECTION
    loadUsers();
    loadChannels();
    loadMarks(); 
    loadRelations();
    syslog(LOG_INFO,"Objects has been updated");
#endif
  }

  void DbSession::saveData()
  {
#ifndef NO_DB_CONNECTION
    saveChannels();
    saveMarks();
    saveRelations();
#endif
}

  DbSession& DbSession::getInstance()
  {
    static DbSession s;
    return s;
  }
  
  DbSession::~DbSession()
  {
#ifndef NO_DB_CONNECTION
    m_updateThread.staticCast<UpdateThread<DbSession> >()->shutdown();
    m_updateThread->join();
#endif
  }
} // namespace common

/* ===[ End of file ]=== */
