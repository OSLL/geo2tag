
#include<iostream>
#include "DbSession.h"
#include "DataMarks.h"
#include "Channel.h"

using namespace common;
int main()
{
  DbSession::getInstance().loadData();
  CHandlePtr<DataMarks> marks = DbSession::getInstance().getMarks();
  CHandlePtr<Channels> channels = DbSession::getInstance().getChannels();
  std::cerr << "Total marks = " << marks->size() << std::endl;
  std::cerr << "Total channels = " << channels->size() << std::endl;
  CHandlePtr<DataMark> mark = DataMark::createMark(60.0, 30.0, "test label", "petroleum");
  (*channels)[2]->addData(mark);
  std::cerr << "Total marks = " << marks->size() << std::endl;
  std::cerr << "Total channels = " << channels->size() << std::endl;
  DbSession::getInstance().saveData();
  return 0;
}
