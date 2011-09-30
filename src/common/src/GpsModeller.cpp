/*
 * Copyright 2010  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
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
#include <QDateTime>
#include <QThread>
#include <QFile>
#include <QDebug>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <cstring>
#include "GpsModeller.h"

#ifndef DEFAULT_LONGITUDE
#define DEFAULT_LONGITUDE 30
#endif

#ifndef DEFAULT_LATITUDE
#define DEFAULT_LATITUDE 60
#endif

#ifdef SIMULATE_GPS

namespace common
{
  GpsModeller::GpsModeller(const QString &gpxFile)
  {
    setReady(false);
    m_longitude = 0.;
    m_latitude = 0.;

    if( !QFile::exists(gpxFile) )
    {
      qDebug() << "File `"<<gpxFile<<"' does not exists";
    }
    else
    {
      m_doc = xmlReadFile(gpxFile.toStdString().c_str(), NULL, 0);
      start();
    }
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

    for (cur_node = node; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE && strcmp((const char*)cur_node->name,"trkpt")==0)
      {
        xmlNode *time = NULL;
        for(xmlNode *s = cur_node->children; s; s = s->next)
        {
          if(s->type == XML_ELEMENT_NODE && strcmp((const char*)s->name,"time")==0)
          {
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
        currentModellerTime = QDateTime::fromString((const char*)stime);
        if(!begin.isValid())
          begin=currentModellerTime;
        std::stringstream s;
        s << (const char *)lon << " " << (const char *) lat;
        setReady(true);
        s >> m_longitude >> m_latitude;
        // This code doesnt used by service
        //QThread::msleep(begin.msecsTo(currentModellerTime));
        QThread::msleep(begin.secsTo(currentModellerTime)*1000);
        begin=currentModellerTime;
      }
      searchElement(cur_node->children);
    }
  }

  void GpsModeller::run()
  {
    QThread::msleep(2000);
    if (m_doc == NULL)
    {
      return;
    }
    else
    {
      /*
       * Get the root element node
       */
                                        // track sequence
      xmlNode *root_element = xmlDocGetRootElement(m_doc);
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

}                                       // namespace common
#endif

/* ===[ End of file  ]=== */
