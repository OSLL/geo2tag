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
/*! ---------------------------------------------------------------
 *  
 *
 * \file MaemoGps.cpp
 * \brief MaemoGps implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */
#include <stdio.h>
#include "MaemoGps.h"
#include "Sleep.h"

#ifndef NO_MAEMO_GPS

namespace common
{
  double MaemoGps::m_longitude = 0;
  double MaemoGps::m_latitude = 0;

  MaemoGps::MaemoGps()
  {
    control = location_gpsd_control_get_default();
    device = (LocationGPSDevice*)g_object_new(LOCATION_TYPE_GPS_DEVICE, NULL);
    g_object_set(G_OBJECT(control),
      "preferred-method", LOCATION_METHOD_USER_SELECTED,
      "preferred-interval", LOCATION_INTERVAL_DEFAULT,
      NULL);

    start(); 
  }
 	
  void MaemoGps::thread()
  {
    printf("starting thread...\n");
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    
    g_signal_connect(control, "error-verbose", G_CALLBACK(MaemoGps::on_error), loop);
    g_signal_connect(device, "changed", G_CALLBACK(MaemoGps::on_changed), control);
    g_signal_connect(control, "gpsd-stopped", G_CALLBACK(MaemoGps::on_stop), loop);
    
    g_idle_add(MaemoGps::start_location, control);

    g_main_loop_run(loop);
  }

  double MaemoGps::getLongitude() const
  {
    return m_longitude;
  }

  double MaemoGps::getLatitude() const
  {
    return m_latitude;
  }

  MaemoGps::~MaemoGps()
  {
    location_gpsd_control_stop(control);
    join();
    g_object_unref(device);
    g_object_unref(control);
  }

  void MaemoGps::on_error(LocationGPSDControl *control, LocationGPSDControlError error, gpointer data)
  {
    printf("location error: %d...\n", error);
  }

  void MaemoGps::on_changed(LocationGPSDevice *device, gpointer data)
  {
    printf("changing postition\n");
    if (!device)
      return;

    if (device->fix) {
      if (device->fix->fields & LOCATION_GPS_DEVICE_LATLONG_SET) {
        printf("\nlat = %f, long = %f\n", device->fix->latitude, device->fix->longitude);
        m_latitude = device->fix->latitude;
        m_longitude = device->fix->longitude;
      }
    }
  }

  void MaemoGps::on_stop(LocationGPSDControl *control, gpointer data)
  {
    printf("shutting down location service\n");
    g_main_loop_quit((GMainLoop *) data);
  }

  gboolean MaemoGps::start_location(gpointer data)
  {
    printf("starting location service\n");
    location_gpsd_control_start((LocationGPSDControl *) data);
    return FALSE;
  }

} // namespace common

#endif // NO_MAEMO_GPS


/* ===[ End of file ]=== */
