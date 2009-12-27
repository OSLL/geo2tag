/*! ---------------------------------------------------------------
 *  
 *
 * \file GpsModeller.cpp
 * \brief GpsModeller implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include <fstream>
#include "GpsModeller.h"

#ifdef SIMULATE_GPS 

namespace common
{
  GpsModeller::GpsModeller(std::string flong, std::string flat):m_longFile(flong), m_latFile(flat)
  {
  }
  
  double GpsModeller::getLongitude() const
  {
    double longitude;
    std::ifstream f(m_longFile.c_str());
    f >> longitude;
    f.close();
    return longitude;
  }

  double GpsModeller::getLatitude() const
  {
    double latitude;
    std::ifstream f(m_latFile.c_str());
    f >> latitude;
    f.close();
    return latitude;
  }
  
  GpsModeller::~GpsModeller()
  {
  }

} // namespace common

#endif 

/* ===[ End of file  ]=== */
