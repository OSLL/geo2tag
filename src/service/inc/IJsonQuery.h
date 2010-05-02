#ifndef _IJsonQuery_h_Included_
#define _IJsonQuery_h_Included_

#include "stream.h"
#include "DataMarks.h"
#include "Channel.h"
#include "Handle.h"
#include <sstream>

class IJsonQuery:public OStream{
	friend class JsonQueryFactory;
	virtual std::string outToString() const=0;
	virtual void init(const std::stringstream& query)=0;
public:
//	IJsonQuery();
	virtual void process()=0;
};

#endif
