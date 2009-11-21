#ifndef __User__h__
#define __User__h__

#include <string>
#include "Channel.h"

class User
{
  std::string m_name;
  std::string m_pass;
  
  std::list<Channel> m_channels; // what he is reading

  Position m_lastPosition; // maybe this field does not belong to User
  
public:
  User();
  User(const std::string& name, const std::string& pass);
  const bool isNone() const;
  const std::string& getName() const;
  const std::string& getPass() const;
  
  static User NONE;
};

#endif
