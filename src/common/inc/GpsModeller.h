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
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "GpsInfo.h"
#include "Thread.h"
#include "Time.h"

#ifdef SIMULATE_GPS 

namespace common
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class GpsModeller: public Gps, private Thread::CThread
  {
    double m_longitude;
    double m_latitude;
   
    CTime begin;
    CTime currentModellerTime;

    xmlDoc *m_doc;
  
    void searchElement(xmlNode *node);

    void thread(); 

  public:
    GpsModeller(const std::string &gpxFile="data/vegas.gpx");
    
    virtual double getLongitude() const;

    virtual double getLatitude() const;
    
   ~GpsModeller();
  }; // class GpsModeller
  
} // namespace common

#endif // SIMULATE_GPS

#endif //_GpsModeller_H_DA9EFC5F_CC89_4791_A371_C1ACB86BFC8B_INCLUDED_

/* ===[ End of file  ]=== */
