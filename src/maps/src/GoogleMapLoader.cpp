/*! ---------------------------------------------------------------
 *  
 *
 * \file GoogleMapLoader.cpp
 * \brief GoogleMapLoader implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include <iostream>
#include <sstream>
#include <map>
#include "GoogleMapLoader.h"
#include "CurlInitException.h"
#include "PngPicture.h"
#include "Channel.h"
#include "GpsInfo.h"

// #define GOOGLE_MAPS_API_KEY "ABQIAAAAb5usG445bmMNMJ0q1D6iXRR6XkXNzuxLA4BE9F2bnVETdMdkRhSkq5tYa7AM0EnQv0rw_8LIEDHNGA"
#define GOOGLE_MAPS_API_KEY "ABQIAAAAb5usG445bmMNMJ0q1D6iXRTpb8ADlP0CcCA6s8Y1Bs5YwJKeqBS1g7w1vZxXV_Ac2suwr6RGznQZiQ"
namespace maps
{
  GoogleMapLoader::GoogleMapLoader()
  {
    m_curl = curl_easy_init();
    if(!m_curl)
      throw exception::CurlInitException();
  }

  static std::string getColor(char ch)
  {
    std::map<char, std::string> m;
    m['A'] = "0x0000FF";
    m['B'] = "0xFF00FF";
    m['C'] = "0x00FFFF";
    m['D'] = "0xDDDDFF";
    m['E'] = "0x545CED";
    m['F'] = "0x1290FF";
    m['G'] = "0xFF90FF";
    m['H'] = "0x12FF32";
    m['I'] = "0xFFFFFF";
    m['J'] = "0x87F341";
    m['K'] = "0x239BA2";
    m['L'] = "0x129011";
    m['M'] = "0xBE90FF";
    m['N'] = "0xF2932F";
    m['O'] = "0xD290A2";
    m['P'] = "0x672DFF";
    if(m.count(ch)>0)
      return m.find(ch)->second;

    return "0x0000FF";
  }

  std::string GoogleMapLoader::preprocessQuery(double latitude, double longitude, short size, int width, int height, common::DataMarks marks)
  {
	  m_data.clear();

    std::ostringstream s;
    s << "maps.google.com/maps/api/staticmap?center=" << latitude << ","<< longitude 
      << "&zoom=" << size << "&size=" << width << "x" << height;

    double clongitude = common::GpsInfo::getInstance().getLongitude();
    double clatitude = common::GpsInfo::getInstance().getLatitude();
    for(size_t i=0; i<marks.size(); i++)
    {
      CHandlePtr<common::DataMark> mark = marks[i];
      if(mark->getDescription()!="" && mark->getChannel()->isDisplayed() &&
         mark->getChannel()->getRadius() >  common::DataMark::getDistance(clatitude, clongitude,mark->getLatitude(), mark->getLongitude()))
        s << "&markers=color:"<< getColor(marks[i]->getLabel()[0]) <<"|label:" << 
             marks[i]->getLabel()[0] << "|" << marks[i]->getLatitude() << "," << marks[i]->getLongitude(); 
    }
    s << "&maptype=roadmap&sensor=true&key=" << GOOGLE_MAPS_API_KEY;

    curl_easy_setopt(m_curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write);
    /* Set a pointer to our struct to pass to the callback */ 
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_data);

    return s.str();
  }

  common::Picture GoogleMapLoader::getMap(double latitude, double longitude, short size, int width, int height)
  {
    std::string s = preprocessQuery(latitude, longitude, size, width, height);           
    std::vector<char> header;

    /*!
     * \TODO check exit status for each curl_easy_setopt routine
     */
    curl_easy_setopt(m_curl, CURLOPT_URL, s.c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEHEADER, &header);

    CURLcode res = curl_easy_perform(m_curl);
    return common::PngPicture(m_data);
  }

  common::Picture GoogleMapLoader::getMapWithMarks(double latitude, double longitude, short size, int width, int height, common::DataMarks marks)
  {
    std::string s = preprocessQuery(latitude, longitude, size, width, height, marks);           
    std::vector<char> header;

    curl_easy_setopt(m_curl, CURLOPT_URL, s.c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEHEADER, &header);
               
    CURLcode res = curl_easy_perform(m_curl);
    return common::PngPicture(m_data);
  }
  GoogleMapLoader::~GoogleMapLoader()
  {
    /* always cleanup */ 
    curl_easy_cleanup(m_curl);
    
  }

  size_t GoogleMapLoader::write(void *buffer, size_t size, size_t nmemb, void *stream)
  {
    std::vector<char> *data = reinterpret_cast<std::vector<char> *>(stream);
    if(data==NULL)
    {
      // FAIL
      return -1;
    }
    
    for(int i=0; i<size*nmemb; ++i)
    {
      data->push_back(((char*)buffer)[i]);
    }

    return size*nmemb;
  }
} // namespace maps

/* ===[ End of file  ]=== */
