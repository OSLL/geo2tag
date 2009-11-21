#include "User.h"

const bool User::isNone() const{
    return (m_name=="" && m_pass=="") ;
}

User::User(){
   *this=NONE;
}

User::User(const std::string& name, const std::string& pass) {
  m_name = name;
  m_pass = pass;
  //todo add position and so on
}

const std::string& User::getName() const {
  return m_name;
}

const std::string& User::getPass() const {
  return m_pass;
}

User User::NONE = User("", "");
