/*! ---------------------------------------------------------------
 * $Id$ 
 *
 * \file CurlInitException.cpp
 * \brief CurlInitException implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include <string>
#include "CurlInitException.h"

namespace exception
{
  CurlInitException::CurlInitException()
  {

  }

  CurlInitException::~CurlInitException()
  {

  }

  std::string CurlInitException::getDescription() const
  {
    return "CURL init Failed";
  }
} // namespace exception

/* ===[ End of file $HeadURL$ ]=== */
