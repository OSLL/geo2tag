/*!
 * \file MaemoGps.h
 * \brief Header of MaemoGps
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _MaemoGps_H_21C729E1_9D6C_4EE5_87CC_A3579FDA1E3C_INCLUDED_
#define _MaemoGps_H_21C729E1_9D6C_4EE5_87CC_A3579FDA1E3C_INCLUDED_

extern "C"
{
  #include <location/location-gps-device.h>
  #include <location/location-gpsd-control.h>
  #include <glib.h>
}
#include "GpsInfo.h"

namespace common
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class MaemoGps: public Gps
  {
    
    LocationGPSDControl *control;
    LocationGPSDevice *device;
    
    static double m_longitude;
    static double m_latitude;

    static void on_error(LocationGPSDControl *control, LocationGPSDControlError error, gpointer data);
  
    static void on_changed(LocationGPSDevice *device, gpointer data);

    static void on_stop(LocationGPSDControl *control, gpointer data);

    static bool start_location(gpointer data);

  public:
    MaemoGps();
    
    virtual double getLongitude() const;

    virtual double getLatitude() const;

    virtual ~MaemoGps();
    
  private:    
    MaemoGps(const MaemoGps& obj);
    MaemoGps& operator=(const MaemoGps& obj);

  }; // class MaemoGps
  
} // namespace common

#endif //_MaemoGps_H_21C729E1_9D6C_4EE5_87CC_A3579FDA1E3C_INCLUDED_

/* ===[ End of file ]=== */
