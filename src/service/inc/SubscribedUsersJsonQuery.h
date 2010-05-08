#ifndef _SubscribedUsersJSONQUERY_H_INCLUDED
#define _SubscribedUsersJSONQUERY_H_INCLUDED

#include "DataMarks.h"
#include "Channel.h"
#include "Handle.h"
#include "elements.h"
#include "quick.h"
#include "reader.h"
#include "writer.h"
#include "cast.h"
#include "exception.h"
#include <sstream>
#include "IJsonQuery.h"


class SubscribedUsersJsonQuery:public IJsonQuery
{
	std::string m_token;
	std::string m_channel;
	std::string outToString() const;
	CHandlePtr<common::DataMarks> m_marks;
	void init(const std::stringstream& query);
public:
	
	SubscribedUsersJsonQuery();
	void process();
};

#endif
