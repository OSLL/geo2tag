#include <sstream>
#include<iostream>
#include "DbSession.h"
#include "DataMarks.h"
#include "Channel.h"
#include "DbException.h"
using namespace common;
int main()
{
  try
  {
    DbSession::getInstance().loadData("test0", "test");
    CHandlePtr<DataMarks> marks = DbSession::getInstance().getMarks();
    CHandlePtr<Channels> channels = DbSession::getInstance().getChannels();
    CHandlePtr<DataMark> mark = DataMark::createMark(60.0, 30.0, "test label", "petroleum");
    (*channels)[2]->addData(mark);
    DbSession::getInstance().saveData();
  }
  catch(ODBC::CException &x)
  {
  }
  return 0;
}
