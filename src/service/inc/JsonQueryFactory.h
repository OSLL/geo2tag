#ifndef _JsonQueryFactory_h_Included_
#define _JsonQueryFactory_h_Included_

#include "IJsonQuery.h"
#include "Handle.h"
#include "query.h"
#include <map>

//enum QueryType should be created in server.cpp

class JsonQueryFactory
{
  std::map<QueryType, CHandlePtr<IJsonQuery> > types;
public:
  template <class T>
  void reg(QueryType qq)
  {
    types[qq]=makeHandle(new T());
  }

  CHandlePtr<IJsonQuery> getJsonQuery(const Query& q,const std::stringstream& s)
  {
    types[q.getType()]->init(s);
    return types[q.getType()];
  }
};

#endif
