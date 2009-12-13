/*  */
/*!
 * \file PngPicture.h
 * \brief Header of PngPicture
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _PngPicture_H_1CCDBE7F_2B80_4842_AB19_85D628100633_INCLUDED_
#define _PngPicture_H_1CCDBE7F_2B80_4842_AB19_85D628100633_INCLUDED_

#include "Picture.h"

namespace common
{
 /*!
   * PNG Picture
   */
  class PngPicture: public Picture
  {

  public:
    PngPicture(const std::vector<char>& data);

    Picture::Type getType() const;

    ~PngPicture();
    
  }; // class PngPicture
  
} // namespace common

#endif //_PngPicture_H_1CCDBE7F_2B80_4842_AB19_85D628100633_INCLUDED_

/* ===[ End of file  ]=== */
