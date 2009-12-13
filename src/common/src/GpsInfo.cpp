/*! ---------------------------------------------------------------
 *  
 *
 * \file GpsInfo.cpp
 * \brief GpsInfo implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include "GpsInfo.h"
#include "GpsModeller.h"

namespace common
{
  GpsInfo::GpsInfo()
  {

  }

  Gps& GpsInfo::getInstance()
  {
    static GpsModeller modeller;
    return modeller;
  }
} // namespace common

/* ===[ End of file  ]=== */
