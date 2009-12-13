/*  */
/*!
 * \file GpsInfo.h
 * \brief Header of GpsInfo
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _GpsInfo_H_D901B2BD_45DB_41D6_A4E2_08407E34EDCD_INCLUDED_
#define _GpsInfo_H_D901B2BD_45DB_41D6_A4E2_08407E34EDCD_INCLUDED_

namespace common
{
  class Gps
  {
  public:
    virtual double getLongitude() const=0;

    virtual double getLatitude() const=0;
  };
 /*!
   * Class description. May use HTML formatting
   *
   */
  class GpsInfo
  {
  public:

    ~GpsInfo();

    static Gps& getInstance();
    
  private:    
    GpsInfo();

  }; // class GpsInfo
  
} // namespace common

#endif //_GpsInfo_H_D901B2BD_45DB_41D6_A4E2_08407E34EDCD_INCLUDED_

/* ===[ End of file  ]=== */
