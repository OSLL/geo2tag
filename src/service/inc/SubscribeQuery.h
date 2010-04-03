/*!
 * \file SubscribeQuery.h
 * \brief Header of SubscribeQuery
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _SubscribeQuery_H_A404855C_9AF3_4E78_A230_059D78E08894_INCLUDED_
#define _SubscribeQuery_H_A404855C_9AF3_4E78_A230_059D78E08894_INCLUDED_

#include "DataMarks.h"
#include "DbSession.h"
#include "Channel.h"
#include "Handle.h"
#include "elements.h"
#include "quick.h"
#include "reader.h"
#include "writer.h"
#include "cast.h"
#include "exception.h"
#include <sstream>
#include "stream.h"
#include "ChannelList.h"

 /*!
   * Class description. May use HTML formatting
   *
   */
class SubscribeQuery:public OStream
  {

  std::string m_user;

  std::string m_channel;

public:

  SubscribeQuery(const std::stringstream& query);

  void subscribe();

  std::string outToString() const;

  }; // class SubscribeQuery
  

#endif //_SubscribeQuery_H_A404855C_9AF3_4E78_A230_059D78E08894_INCLUDED_

/* ===[ End of file ]=== */
