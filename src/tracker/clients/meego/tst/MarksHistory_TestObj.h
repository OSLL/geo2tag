#ifndef MARKSHISTORY_TESTOBJ_H
#define MARKSHISTORY_TESTOBJ_H

#include "markshistory.h"

namespace Test
{
  class MarksHistoryTestImpl : public MarksHistory
  {
    Q_OBJECT;

    public:

      MarksHistoryTestImpl() : MarksHistory()
      {

      }


      friend class MarksHistory_Test;
  };

}


// MARKSHISTORY_TESTOBJ_H
#endif

