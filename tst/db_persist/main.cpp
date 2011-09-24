#include "DataMarks.h"
#include <iostream>
using namespace common;
int main(void)
{
  DataMarks k,h;
  CHandlePtr<DataMark> j = makeHandle(new DataMark(0,0,"Label A","Desc"));
  k.push_back(j);
  k.save();
  h.load();
  h[0]->setLabel(h[0]->getLabel() + ".");
  h.save();
  return 0;
}
