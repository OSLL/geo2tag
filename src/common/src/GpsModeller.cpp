/*! ---------------------------------------------------------------
 *  
 *
 * \file GpsModeller.cpp
 * \brief GpsModeller implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include <iostream>
#include <sstream>
#include <fstream>
#include "GpsModeller.h"
#include "Time.h"
#include "Sleep.h"

#ifdef SIMULATE_GPS 

namespace common
{
  GpsModeller::GpsModeller(const std::string &gpxFile)
  {
    m_doc = xmlReadFile(gpxFile.c_str(), NULL, 0);
    start();
  }
  
  struct tm getTm(const char* string)
  {
    struct tm time;
    sscanf(string, "%d-%d-%dT%d:%d:%dZ", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_mon -= 1;
    time.tm_year -= 1900;
    return time;
  }


  void GpsModeller::searchElement(xmlNode *node)
  {
     xmlNode *cur_node = NULL;
      
     for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE && strcmp((const char*)cur_node->name,"trkpt")==0) {
          xmlNode *time = NULL;
          for(xmlNode *s = cur_node->children; s; s = s->next) {
            if(s->type == XML_ELEMENT_NODE && strcmp((const char*)s->name,"time")==0) {
              time = s;
            }
          }

          if(!time)
          {
            // epic fail!!! We did not find time for point... Skep this point
            continue;
          }

          xmlChar *lon = xmlGetProp(cur_node, (xmlChar*)"lon");
          xmlChar *lat = xmlGetProp(cur_node, (xmlChar*)"lat");
          xmlChar *stime = xmlNodeGetContent(time);
          currentModellerTime = CTime(getTm((const char*)stime));           
          if(!begin.isValid())
            begin=currentModellerTime;
          std::stringstream s;
          s << (const char *)lon << " " << (const char *) lat;
          s >> m_longitude >> m_latitude;
          mSleep((currentModellerTime-begin)*1000);
          begin=currentModellerTime;
        }
       searchElement(cur_node->children);
     }
  }

  void GpsModeller::thread()
  {
    mSleep(2000);
    std::cerr << "starting GpsModeller thread..." << std::endl;
    if (m_doc == NULL)
    {
       std::cerr << "error: could not parse file\n";
       return;
    }
    else
    {
      /*
       * Get the root element node
       */
      xmlNode *root_element = xmlDocGetRootElement(m_doc);// track sequence
      searchElement(root_element);
    }

  }

  double GpsModeller::getLongitude() const
  {
    return m_longitude;
  }

  double GpsModeller::getLatitude() const
  {
    return m_latitude;
  }


  
  GpsModeller::~GpsModeller()
  {
    xmlFreeDoc(m_doc);
    xmlCleanupParser();
  }

} // namespace common

#endif 

/* ===[ End of file  ]=== */
