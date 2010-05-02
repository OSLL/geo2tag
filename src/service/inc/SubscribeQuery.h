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
#include "IJsonQuery.h"
#include "ChannelList.h"

 /*!
   * Class description. May use HTML formatting
   *
   */
class SubscribeQuery:public IJsonQuery
  {

  std::string m_token;

  std::string m_channel;
  void init(const std::stringstream& query);

  std::string outToString() const;
public:

  SubscribeQuery();

  void process();



  }; // class SubscribeQuery
  

#endif //_SubscribeQuery_H_A404855C_9AF3_4E78_A230_059D78E08894_INCLUDED_

/* ===[ End of file ]=== */
