/* This file should be replaced with template */

#include "AvailableList.h"
#include "Handle.h"
#include "Channel.h"
#include <fcgiapp.h>
#include "stream.h"
#include <assert.h>
#include "query.h"
#include "server.h"

class TestChannel: public common::Channel
{
public: 
  TestChannel():Channel("Test channel","test","",CHandlePtr<common::DataMarks>())
  {
  }
};

int main()
{
/* 
  CHandlePtr<common::Channels> a = makeHandle(new common::Channels());
  a->push_back(makeHandle(new TestChannel()));
  a->push_back(makeHandle(new TestChannel()));
  a->push_back(makeHandle(new TestChannel()));
  a->push_back(makeHandle(new TestChannel()));
*/
  FCGX_Stream *s=new FCGX_Stream;
  Stream stm(s);
  stm << std::string("{ \"user\": \"Mark\" ,\"latitude\": 20.0,\"longitude\": 20.0,\"radius\": 50.0 }");
  Query q("query=channels",stm);  
  Server serv;
  serv.process(q);
  return 0;
}
