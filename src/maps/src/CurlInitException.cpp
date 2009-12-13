/*! ---------------------------------------------------------------
 *  
 *
 * \file CurlInitException.cpp
 * \brief CurlInitException implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
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

/* ===[ End of file  ]=== */
