#include "SubscribedUsersJsonQuery.h"
#include <syslog.h>
#include "UserInternal.h"
#include "ChannelInternal.h"
#include "DbSession.h"
#include <sstream>
#include "DataMarkInternal.h"
#include <vector>
#include <algorithm>
bool comP(CHandlePtr<common::DataMark> x1,CHandlePtr<common::DataMark> x2){return x1->getTime() > x2->getTime();}

SubscribedUsersJsonQuery::SubscribedUsersJsonQuery()
{

	m_marks=makeHandle(new common::DataMarks());
}


void SubscribedUsersJsonQuery::init(const std::stringstream& query){
        json::Element elemRoot;
	std::istringstream s(query.str());
        json::Reader::Read(elemRoot,s);
        json::QuickInterpreter interpreter(elemRoot);
        const json::String& token=interpreter["auth_token"];
	m_token=std::string(token);
        const json::String& channel=interpreter["channel"];
        m_channel=std::string(channel);
}


void SubscribedUsersJsonQuery::process()
{
  CHandlePtr<common::DataMarks> marks;
  CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
  CHandlePtr<common::Channels> channels=common::DbSession::getInstance().getChannels();
  CHandlePtr<common::Channel> channel;
  // Find m_channel channel
  for (common::Channels::iterator i=channels->begin();i!=channels->end();i++){
    if ((*i).dynamicCast<loader::Channel>()->getName()==m_channel){
      channel=(*i);
      marks=(*i)->getMarks();
      syslog(LOG_INFO, "Channel founded");
      break;
    }
  }
  std::sort(marks->begin(),marks->end(),comP);
  for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++){
    for (common::DataMarks::iterator j=marks->begin();j!=marks->end();j++){
      if ((*j)->getUser()->getLogin()==(*i)->getLogin() && channel->getName()==(*j)->getChannel()->getName()){
         m_marks->push_back((*j));
         break;
        }
    }
  }

}

std::string SubscribedUsersJsonQuery::outToString() const
{
	json::Element newRoot;
	json::QuickBuilder builder(newRoot);
	std::stringstream s;
	builder["rss"]["channel"]["title"]=json::String("Geo2Tag RSS feed - position of subscribed users");
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
		//if (m_marks->size()>20) finish=m_marks->begin()+20;
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
	else marks = json::Null();

	json::Writer::Write(newRoot,s);
	syslog(LOG_INFO,"json formed");
	return s.str();

}
