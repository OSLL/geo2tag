/*! ---------------------------------------------------------------
 *  
 *
 * \file MapLoaderFactory.cpp
 * \brief MapLoaderFactory implementation
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include "MapLoaderFactory.h"
#include "GoogleMapLoader.h"

namespace maps
{
  MapLoaderFactory::MapLoaderFactory()
  {

  }

  MapLoader* MapLoaderFactory::getInstance(MapLoader::SourceType type)
  {
    /*!
     * \TODO add default loader
     */

    /*!
     * \TODO use std::map for instance selecting 
     */
    static GoogleMapLoader gMap;

    if(type == MapLoader::GOOGLE)
      return &gMap;

    return NULL;
  }

} // namespace maps

/* ===[ End of file  ]=== */
