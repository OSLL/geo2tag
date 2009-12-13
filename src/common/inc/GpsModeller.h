/*  */
/*!
 * \file GpsModeller.h
 * \brief Header of GpsModeller
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _GpsModeller_H_DA9EFC5F_CC89_4791_A371_C1ACB86BFC8B_INCLUDED_
#define _GpsModeller_H_DA9EFC5F_CC89_4791_A371_C1ACB86BFC8B_INCLUDED_

#include <string>
#include "GpsInfo.h"

namespace common
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class GpsModeller: public Gps
  {
    std::string m_longFile;
    std::string m_latFile;

  public:
    GpsModeller(std::string flong="/tmp/longitude", std::string flat="/tmp/latitude");
    
    virtual double getLongitude() const;

    virtual double getLatitude() const;
    
   ~GpsModeller();
  }; // class GpsModeller
  
} // namespace common

#endif //_GpsModeller_H_DA9EFC5F_CC89_4791_A371_C1ACB86BFC8B_INCLUDED_

/* ===[ End of file  ]=== */
