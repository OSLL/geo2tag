/*! ---------------------------------------------------------------
 * $Id$ 
 *
 * \file Picture.cpp
 * \brief Picture implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "Picture.h"

namespace common
{
  Picture::Picture(const std::vector<char>& data): m_data(data)
  {
  }

  Picture::Picture(const Picture &p):m_data(p.m_data)
  {
  }

  const std::vector<char> Picture::getData() const
  {
    return m_data;
  }
  
  Picture::Type Picture::getType() const
  {
    return UNKNOWN;
  }

  Picture::~Picture()
  {

  }
} // namespace common

/* ===[ End of file $HeadURL$ ]=== */
