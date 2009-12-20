
#include<iostream>
#include "DbSession.h"

using namespace common;
int main()
{
  DbSession::getInstance().loadData();
  std::cerr << "Total marks = " << DbSession::getInstance().getMarks()->size() << std::endl;
  std::cerr << "Total channels = " << DbSession::getInstance().getChannels()->size() << std::endl;
  return 0;
}
