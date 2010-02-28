/*
 * Copyright 2010  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
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

#ifndef NO_MAEMO_GPS
extern "C"
{
  #include <location/location-gps-device.h>
  #include <location/location-gpsd-control.h>
  #include <glib.h>
}


#include "GpsInfo.h"
#include "Thread.h"

namespace common
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class MaemoGps: public Gps, private Thread::CThread
  { 
    LocationGPSDControl *control;
    LocationGPSDevice *device;
    
    static double m_longitude;
    static double m_latitude;

    static void on_error(LocationGPSDControl *control, LocationGPSDControlError error, gpointer data);
  
    static void on_changed(LocationGPSDevice *device, gpointer data);

    static void on_stop(LocationGPSDControl *control, gpointer data);

    static gboolean start_location(gpointer data);
	
    void thread();
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

#else // NO_MAEMO_GPS
#warning "Building without Maemo gps support"
#endif

#endif //_MaemoGps_H_21C729E1_9D6C_4EE5_87CC_A3579FDA1E3C_INCLUDED_

/* ===[ End of file ]=== */
