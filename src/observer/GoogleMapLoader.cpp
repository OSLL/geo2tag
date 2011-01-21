/*! ---------------------------------------------------------------
 * $Id$ 
 *
 * \file GoogleMapLoader.cpp
 * \brief GoogleMapLoader implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "GoogleMapLoader.h"
#include "CurlInitException.h"
#include "PngPicture.h"
#include <QDebug>
#define KEY_FILE "api_key"

namespace maps
{
  void GoogleMapLoader::readApiKey(){
    std::ifstream in(KEY_FILE);
    in >> m_apiKey;
    qDebug() << "read " << m_apiKey.c_str() << " from " << KEY_FILE;
  }

  GoogleMapLoader::GoogleMapLoader()
  {
    m_curl = curl_easy_init();
    readApiKey();
    if(!m_curl)
      throw exception::CurlInitException();
  }

  static std::string getColor(char ch)
  {
    std::map<char, std::string> m;
    ///Replace this map for color generation algorithm, bug #1112
    m['A'] = "0x0000FF";
    m['B'] = "0xFF00FF";
    m['C'] = "0x00FFFF";
    m['D'] = "0xDDDDFF";
    m['E'] = "0x545CED";
    m['F'] = "0x1290FF";
    if(m.count(ch)>0)
      return m.find(ch)->second;

    return "0x0000FF";
  }

  std::string GoogleMapLoader::preprocessQuery(double latitude, double longitude, short size, int width, int height, DataMarks marks)
  {
	  m_data.clear();

    std::ostringstream s;
    //Check parameters
    if (size<0) size=0;
    if (size>19) size=19;
    if (latitude < -90.) latitude=-90.;
    if (latitude > 90.) latitude=90.;
    if (longitude < -180.) longitude=-180.;
    if (longitude > 180.) longitude=180.;
    if (width > 512) width=512;
    if (height > 512) height=512;
    s << "maps.google.com/maps/api/staticmap?center=" << latitude << ","<< longitude 
      << "&zoom=" << size << "&size=" << width << "x" << height;
    for(size_t i=0; i<marks.size(); i++)
    {
      if(marks[i]->getDescription().length()!=0)
        s << "&markers=color:"<< getColor(marks[i]->getLabel()[0]) <<"|label:" << marks[i]->getLabel() << "|" << marks[i]->getLatitude() << "," << marks[i]->getLongitude(); 
    }
    s << "&maptype=roadmap&sensor=true&key=" << m_apiKey.c_str();

    curl_easy_setopt(m_curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1);
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

  common::Picture GoogleMapLoader::getMapWithMarks(double latitude, double longitude, short size, int width, int height, DataMarks marks)
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

/* ===[ End of file $HeadURL$ ]=== */
