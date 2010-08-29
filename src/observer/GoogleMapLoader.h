/* $Id$ */
/*!
 * \file GoogleMapLoader.h
 * \brief Header of GoogleMapLoader
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */


#ifndef _GoogleMapLoader_H_44AC0505_AE92_49A7_8B34_0D34CE52104E_INCLUDED_
#define _GoogleMapLoader_H_44AC0505_AE92_49A7_8B34_0D34CE52104E_INCLUDED_

#include <curl/curl.h>
#include "MapLoader.h"
#include "DataMarks.h"
#include <iostream>
namespace maps
{
 /*!
   * 
   */
  class GoogleMapLoader: public MapLoader
  {

    CURL * m_curl; //!< connection to the server
    std::vector<char> m_data;
    std::string m_apiKey;
    void readApiKey();
  public:
    GoogleMapLoader();

    virtual common::Picture getMap(double latitude, double longitude, short size, int width, int height);
    
    virtual common::Picture getMapWithMarks(double latitude, double longitude, short size, int width, int height, common::DataMarks marks);

    virtual ~GoogleMapLoader();

  private: 
   
    std::string preprocessQuery(double latitude, double longitude, short size, int width, int height, common::DataMarks marks = common::DataMarks());
    /*!
     * \brief routine for writing data from stream
     */
    static size_t write(void *buffer, size_t size, size_t nmemb, void *stream);

    GoogleMapLoader(const GoogleMapLoader& obj);
    GoogleMapLoader& operator=(const GoogleMapLoader& obj);

  }; // class GoogleMapLoader
  
} // namespace maps

#endif //_GoogleMapLoader_H_44AC0505_AE92_49A7_8B34_0D34CE52104E_INCLUDED_

/* ===[ End of file $HeadURL$ ]=== */
