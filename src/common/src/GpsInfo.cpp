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
#include "MaemoGps.h"

namespace common
{
  GpsInfo::GpsInfo()
  {

  }

  Gps& GpsInfo::getInstance()
  {
//    static GpsModeller gps;
    static MaemoGps gps;
    return gps;
  }
} // namespace common

/* ===[ End of file  ]=== */
