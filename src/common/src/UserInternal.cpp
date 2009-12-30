/*! ---------------------------------------------------------------
 * \file UserInternal.cpp
 * \brief UserInternal implementation
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "UserInternal.h"

namespace loader
{
  User::User(const std::string &login, const std::string& pass, unsigned long id):common::User(login,pass), m_id(id)
  {
  }

  unsigned long User::getId() const
  {
    return m_id;
  }

  User::~User()
  {
  }
} // namespace loader

/* ===[ End of file ]=== */
