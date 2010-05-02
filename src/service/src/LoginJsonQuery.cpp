#include "LoginJsonQuery.h"
#include "UserInternal.h"
#include "DbSession.h"
LoginJsonQuery::LoginJsonQuery(){
}

void LoginJsonQuery::init(const std::stringstream& query){
	json::Element elemRoot;
	std::istringstream s(query.str());
	json::Reader::Read(elemRoot,s);
	json::QuickInterpreter interpreter(elemRoot);
	const json::String& user=interpreter["user"];
	m_user=std::string(user);
	const json::String& password=interpreter["password"];
	m_password=std::string(password);
	m_token="Error";
	m_status="Error";
}

void LoginJsonQuery::process(){
	CHandlePtr<std::vector<CHandlePtr<common::User> > > users=common::DbSession::getInstance().getUsers();
	for (std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++){
		if ((*i)->getLogin()==m_user && (*i)->getPassword()==m_password){
			m_token=(*i).dynamicCast<loader::User>()->getToken();
			m_status="Ok";
			break;
		}
	}
}



std::string LoginJsonQuery::outToString() const{
	std::stringstream s;
	json::Element newRoot;
	json::QuickBuilder builder(newRoot);
	builder["status"]=json::String(m_status);
	builder["auth_token"]=json::String(m_token);
	json::Writer::Write(newRoot,s);
	return s.str();
}

