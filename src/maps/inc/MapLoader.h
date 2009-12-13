/*  */
/*!
 * \file MapLoader.h
 * \brief Header of MapLoader
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _MapLoader_H_7DB6BBD9_E4AD_4818_8B45_5B18EC8FC60C_INCLUDED_
#define _MapLoader_H_7DB6BBD9_E4AD_4818_8B45_5B18EC8FC60C_INCLUDED_

#include "Picture.h"
#include "DataMarks.h"

namespace maps
{
  /*!
   * MapLoader interface
   */
  class MapLoader
  {
  public:

    /*!
     * \brief list fot available maps sources
     */
    enum SourceType {
      GOOGLE,
      UNKNOWN
    };

    /*!
     * \brief load map as picture
     * \param latitude -- image center latitude  
     * \param longitude -- image center longitude
     * \param scale -- image scale
     * \param width -- image width
     * \param height -- image height
     */
    virtual common::Picture getMap(double latitude, double longitude, short scale, int width, int height)=0;
    
    virtual common::Picture getMapWithMarks(double latitude, double longitude, short scale, int width, int height, common::DataMarks marks)=0;

  }; // class MapLoader
  
} // namespace maps

#endif //_MapLoader_H_7DB6BBD9_E4AD_4818_8B45_5B18EC8FC60C_INCLUDED_

/* ===[ End of file  ]=== */
