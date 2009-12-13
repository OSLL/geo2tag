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

namespace common
{
  double MaemoGps::m_longitude = 0;
  double MaemoGps::m_latitude = 0;

  MaemoGps::MaemoGps()
  {
    control = location_gpsd_control_get_default();
    device = (LocationGPSDevice*)g_object_new(LOCATION_TYPE_GPS_DEVICE, NULL);
    
    g_type_init();
  
    g_object_set(G_OBJECT(control),
      "preferred-method", LOCATION_METHOD_USER_SELECTED,
      "preferred-interval", LOCATION_INTERVAL_DEFAULT,
      NULL);

    g_signal_connect(control, "error-verbose", G_CALLBACK(on_error), this);
    g_signal_connect(device, "changed", G_CALLBACK(on_changed), control);
    g_signal_connect(control, "gpsd-stopped", G_CALLBACK(on_stop), this);

    start_location(this);
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
    g_object_unref(device);
    g_object_unref(control);
  }

  void MaemoGps::on_error(LocationGPSDControl *control, LocationGPSDControlError error, gpointer data)
  {
    printf("location error: %d...\n", error);
  }

  void MaemoGps::on_changed(LocationGPSDevice *device, gpointer data)
  {
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
  }

  bool MaemoGps::start_location(gpointer data)
  {
    printf("starting location service\n");
    location_gpsd_control_start((LocationGPSDControl *) data);
    return true;
  }

} // namespace common

/* ===[ End of file ]=== */
