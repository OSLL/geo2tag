/*  */
/*!
 * \file MapLoaderFactory.h
 * \brief Header of MapLoaderFactory
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _MapLoaderFactory_H_D8153C5F_870C_4A61_9502_338472902AFD_INCLUDED_
#define _MapLoaderFactory_H_D8153C5F_870C_4A61_9502_338472902AFD_INCLUDED_

#include "MapLoader.h"

namespace maps
{
 /*!
   * Class description. May use HTML formatting
   */
  class MapLoaderFactory
  {
  public:
  
    static MapLoader* getInstance(MapLoader::SourceType);

  private:    
    
    MapLoaderFactory();
    MapLoaderFactory(const MapLoaderFactory& obj);
    MapLoaderFactory& operator=(const MapLoaderFactory& obj);

  }; // class MapLoaderFactory
  
} // namespace maps

#endif //_MapLoaderFactory_H_D8153C5F_870C_4A61_9502_338472902AFD_INCLUDED_

/* ===[ End of file  ]=== */
