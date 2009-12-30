/*! ---------------------------------------------------------------
 *  
 *
 * \file User.cpp
 * \brief User implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "User.h"

namespace common
{
  User::User(const std::string& name, const std::string& pass): m_login(name), m_password(pass)
  {
  }

  const std::string& User::getLogin() const
  {
    return m_login;
  }

  const std::string& User::getPassword() const
  {
    return m_password;
  }

  User::~User()
  {
  }
} // namespace common

/* ===[ End of file ]=== */
