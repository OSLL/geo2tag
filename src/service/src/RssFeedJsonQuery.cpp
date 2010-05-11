#include "RssFeedJsonQuery.h"
#include <syslog.h>
#include "UserInternal.h"
#include "DbSession.h"
#include <sstream>
#include "DataMarkInternal.h"
#include <algorithm>
bool comp(CHandlePtr<common::DataMark> x1,CHandlePtr<common::DataMark> x2){return x1->getTime() < x2->getTime();}

RssFeedJsonQuery::RssFeedJsonQuery()
{
	m_marks=makeHandle(new common::DataMarks());
}


void RssFeedJsonQuery::init(const std::stringstream& query)
{
  json::Element elemRoot;
	std::istringstream s(query.str());
  json::Reader::Read(elemRoot,s);
  json::QuickInterpreter interpreter(elemRoot);
  const json::String& token=interpreter["auth_token"];
	m_token=std::string(token);
  const json::Number& radius=interpreter["radius"];
  m_radius=radius;
  const json::Number& latitude=interpreter["latitude"];
	m_latitude=latitude;
	const json::Number& longitude=interpreter["longitude"];
	m_longitude=longitude;
  try
  {
    const json::String& type=interpreter["type"];
    if(std::string(type)=="last_one")
    {
      m_type = THE_LATEST_FROM_EACH_MEMBER;
    }
    else
    {
      m_type = ALL_MARKS;
    }
  }
  catch(...)
  {
    m_type = ALL_MARKS;
  }

}


void RssFeedJsonQuery::process()
{
  CHandlePtr<common::DataMarks> marks = common::DbSession::getInstance().getMarks();
  m_marks->clear();
  CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
  CHandlePtr<common::Channels> channels;
  std::map<CHandlePtr<common::User>, CHandlePtr<common::DataMark> > m_outputList; // used for THE_LATEST_FROM_EACH_MEMBER request


  for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++)
  {
    if ((*i).dynamicCast<loader::User>()->getToken()==m_token)
    {
      channels=(*i)->getSubscribedChannels();
      break;
    }
  }

  syslog(LOG_INFO, "Marks->size()=%ld,channels->size()=%ld",marks->size(), channels->size());
  size_t i,y;
  
  for (y=0; y<marks->size();y++)
  {
    for (i=0; i<channels->size();i++)
    {
       if ((*marks)[y]->getChannel()==(*channels)[i]) 
       {
           syslog(LOG_INFO, "(*marks)[y]->getDistance((*marks)[y]->getLatitude(),(*marks)[y]->getLongitude(),m_latitude,m_longitude)[%f] <= m_radius[%f]",
              (*marks)[y]->getDistance((*marks)[y]->getLatitude(),(*marks)[y]->getLongitude(),m_latitude,m_longitude), m_radius);
           if ((*marks)[y]->getDistance((*marks)[y]->getLatitude(),(*marks)[y]->getLongitude(),m_latitude,m_longitude) <= m_radius) 
           {
              if(m_type == ALL_MARKS)
              {
                m_marks->push_back((*marks)[y]);
              }
              else
              {
                CHandlePtr<common::DataMark> m = (*marks)[y];
                if(m_outputList.count(m->getUser())>0 )
                {
                  if(m_outputList.find(m->getUser())->second->getTime() < m->getTime())
                  {
                    m_outputList[m->getUser()] = m;
                  }
                }
                else
                {
                  m_outputList[m->getUser()] = m;
                }
              }
           }
       }
    }
  }
  
  if(m_type == THE_LATEST_FROM_EACH_MEMBER)
  {
    for(std::map<CHandlePtr<common::User>, CHandlePtr<common::DataMark> >::iterator it=m_outputList.begin();
          it!=m_outputList.end(); it++)
    {
      m_marks->push_back(it->second);
    }
  }

  syslog(LOG_INFO, "RssFeedJsonQuery::process, marks.size() = %u, after filtering", m_marks->size());
  std::sort(m_marks->begin(),m_marks->end(),comp);
}

std::string RssFeedJsonQuery::outToString() const
{
	json::Element newRoot;
	json::QuickBuilder builder(newRoot);
	std::stringstream s;
	builder["rss"]["channel"]["title"]=json::String("Geo2Tag RSS feed");
	builder["rss"]["channel"]["link"]=json::String("http:////wikigps.org//");
	builder["rss"]["channel"]["description"]=json::String(" ");
	builder["rss"]["channel"]["language"]=json::String("English");
	builder["rss"]["channel"]["pubDate"]=json::String(formatTime(CTime::now().getTime(),"%d %b %Y %H:%M:%S"));
	syslog(LOG_INFO,"time generated");
	json::QuickBuilder marks=builder["rss"]["channel"]["item"];
	if (m_marks->size()>0)
	{
		int j=0;
		common::DataMarks::iterator finish=m_marks->end();
		if (m_marks->size()>20) finish=m_marks->begin()+20;
		for (common::DataMarks::iterator i=m_marks->begin();i!=finish;i++,j++)
		{
			syslog(LOG_INFO,"another itaration");
			marks[j]["title"]=json::String((*i)->getLabel());
			marks[j]["link"]=json::String((*i)->getUrl());
			marks[j]["description"]=json::String((*i)->getDescription());
			syslog(LOG_INFO, "RSS reply, lat=%f, lon=%f", (*i)->getLatitude(), (*i)->getLongitude());
			marks[j]["latitude"]=json::Number((*i)->getLatitude());
			marks[j]["longitude"]=json::Number((*i)->getLongitude());
			syslog(LOG_INFO, "RSS reply, lat=%f, lon=%f", (*i)->getLatitude(), (*i)->getLongitude());
			marks[j]["tags"]=json::Null();
                        marks[j]["pubDate"]=json::String(formatTime((*i)->getTime().getTime(),"%d %b %Y %H:%M:%S"));
			marks[j]["user"]=json::String((*i)->getUser()->getLogin());
			CHandlePtr<common::Channel> channel = (*i)->getChannel();
			marks[j]["channel"]=json::String(channel->getName());			
		}	
	}
	else  // m_marks->size() == 0
  {
    marks = json::Null();
  }

	json::Writer::Write(newRoot,s);
	syslog(LOG_INFO,"json formed");
	return s.str();

}
