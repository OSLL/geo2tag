/*!
 * \file LoginJsonQuery.h
 * \brief Header of LoginJsonQuery
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _LoginJsonQuery_H_A404855C_9AF3_4E78_A230_059D78E08894_INCLUDED_
#define _LoginJsonQuery_H_A404855C_9AF3_4E78_A230_059D78E08894_INCLUDED_

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
class LoginJsonQuery:public IJsonQuery
  {

  std::string m_user;

  std::string m_password;

  std::string m_status;

  std::string m_token;

  void init(const std::stringstream& query);

  std::string outToString() const;
public:

  LoginJsonQuery();

  void process();



  }; // class LoginJsonQuery
  

#endif //_LoginJsonQuery_H_A404855C_9AF3_4E78_A230_059D78E08894_INCLUDED_

/* ===[ End of file ]=== */
