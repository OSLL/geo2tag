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

#ifndef NO_MAEMO_GPS
  #include "MaemoGps.h"
#endif

namespace common
{
  GpsInfo::GpsInfo()
  {

  }

  Gps& GpsInfo::getInstance()
  {
#ifdef NO_MAEMO_GPS  
    static GpsModeller gps;
#else
    static MaemoGps gps;
#endif    
    return gps;
  }
} // namespace common

/* ===[ End of file  ]=== */
