/*! ---------------------------------------------------------------
 *  
 *
 * \file PngPicture.cpp
 * \brief PngPicture implementation
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include "PngPicture.h"

namespace common
{
  PngPicture::PngPicture(const std::vector<char>& data):Picture(data)
  {
  }

  Picture::Type PngPicture::getType() const
  {
    return Picture::PNG;
  }

  PngPicture::~PngPicture()
  {

  }
} // namespace common

/* ===[ End of file  ]=== */
