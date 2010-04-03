#include "RssFeedJsonQuery.h"
#include <syslog.h>
#include "UserInternal.h"
#include "DbSession.h"
#include <sstream>
#include "DataMarkInternal.h"
#include <algorithm>
bool comp(CHandlePtr<common::DataMark> x1,CHandlePtr<common::DataMark> x2){x1->getTime() < x2->getTime();}

RssFeedJsonQuery::RssFeedJsonQuery(const std::stringstream& query)
{
        json::Element elemRoot;
	std::istringstream s(query.str());
        json::Reader::Read(elemRoot,s);
        json::QuickInterpreter interpreter(elemRoot);
        const json::String& user=interpreter["user"];
	m_user=std::string(user);
        const json::Number& radius=interpreter["radius"];
        m_radius=radius;
        const json::Number& latitude=interpreter["latitude"];
	m_latitude=latitude;
	const json::Number& longitude=interpreter["longitude"];
	m_longitude=longitude;
	m_marks=makeHandle(new common::DataMarks());
}

void RssFeedJsonQuery::process()
{
  CHandlePtr<common::DataMarks> marks = common::DbSession::getInstance().getMarks();
  m_marks->clear();
  CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
  CHandlePtr<common::Channels> channels;
  for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++){
    if ((*i)->getLogin()==m_user){
      channels=(*i)->getSubscribedChannels();
      break;
    }
  }
  bool inSubscribed;
  for (common::DataMarks::iterator y=marks->begin();y!=marks->end();y++)
  {
    inSubscribed=0;
    for (common::Channels::iterator i=channels->begin();i!=channels->end();i++){
       if ((*i)->getName()==(*y)->getChannel()->getName()) {
         inSubscribed=1;
         break;
       }
    }
    if ((*y)->getDistance((*y)->getLatitude(),(*y)->getLongitude(),m_latitude,m_longitude) <= m_radius && inSubscribed) 
    {
       m_marks->push_back((*y));
    }
  }
  syslog(LOG_INFO, "RssFeedJsonQuery::process, marks.size() = %u, after filtering", m_marks->size());
//  CHandlePtr<common::DataMark> max=makeHandle(new loader::DataMark());
//  CHandlePtr<common::DataMark> tmp=makeHandle(new loader::DataMark());
//This things should be in common::DataMarks class
 /*nt k;
  for (int i=0;i!=20;i++){
//    max=(*m_marks)[i];
    k=i;
    for (int j=i;j!=m_marks->size();j++){
      if ((*m_marks)[k]->getTime()>(*marks)[j]->getTime()) k=j; 
    }
   // tmp=(*m_marks)[i];
//    (*m_marks)[i]=(*m_marks)[k];
  //  (*m_marks)[k]=tmp;
    std::swap((*m_marks)[i],(*m_marks)[k]);//
  }*/
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
			marks[j]["latitude"]=json::Number((*i)->getLatitude());
			marks[j]["longitude"]=json::Number((*i)->getLongitude());
			marks[j]["tags"]=json::Null();
                        marks[j]["pubDate"]=json::String(formatTime((*i)->getTime().getTime(),"%d %b %Y %H:%M:%S"));
			marks[j]["user"]=json::String((*i)->getUser()->getLogin());
			CHandlePtr<common::Channel> channel = (*i)->getChannel();
			marks[j]["channel"]=json::String(channel->getName());			
		}	
	}
	else marks = json::Null();

	json::Writer::Write(newRoot,s);
	syslog(LOG_INFO,"json formed");
	return s.str();

}
