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

#include <iostream>
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
#ifdef NO_MAEMO_GPS  
    std::cerr << "GpsModeller in use...." << std::endl;
    static GpsModeller gps;
#else
    std::cerr << "Maemo Modeller in use...." << std::endl;
    static MaemoGps gps;
#endif    
    return gps;
  }
} // namespace common

/* ===[ End of file  ]=== */
