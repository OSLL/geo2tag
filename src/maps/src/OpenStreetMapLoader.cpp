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
 * \file OpenStreetMapLoader.cpp
 * \brief OpenStreetMapLoader implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <iostream>
#include <sstream>
#include <map>
#include "OpenStreetMapLoader.h"
#include "PngPicture.h"
#include "Channel.h"
#include "GpsInfo.h"
#include <QVector>
//#include <QSslError>
#include <QDebug>
#include "GpsInfo.h"

namespace maps
{
  OpenStreetMapLoader::OpenStreetMapLoader()
  {
    manager = new QNetworkAccessManager(this);
  }

  static std::string getColor(char ch)
  {
    std::map<char, std::string> m;
    m['A'] = "green";
    m['B'] = "green";
    m['C'] = "green";
    m['D'] = "green";
    m['E'] = "green";
    m['F'] = "green";
    m['G'] = "green";
    m['H'] = "green";
    m['I'] = "green";
    m['J'] = "green";
    m['K'] = "green";
    m['L'] = "green";
    m['M'] = "green";
    m['N'] = "green";
    m['O'] = "green";
    m['P'] = "green";
    if(m.count(ch)>0)
      return m.find(ch)->second;

    return "0x0000FF";
  }

  std::string OpenStreetMapLoader::preprocessQuery(double latitude, double longitude, short size, int width, int height, const DataMarks& marks)
  {
    m_data.clear();

    std::ostringstream s;

    s << "http://pafciu17.dev.openstreetmap.org/?module=map&center=" << longitude << ","<< latitude
      << "&zoom=" << size << "&type=mapnik" << "&width=" << width << "&height=" << height;

    s << "&points=" << common::GpsInfo::getInstance().getLongitude() << ","
      << common::GpsInfo::getInstance().getLatitude()
      << ",pointImageUrl:pafciu17.dev.openstreetmap.org/media/pointer/sight_point.png;";

    double clongitude = common::GpsInfo::getInstance().getLongitude();
    double clatitude = common::GpsInfo::getInstance().getLatitude();
    for(size_t i=0; i<marks.size(); i++)
    {
      QSharedPointer<DataMark> mark = marks[i];
      if(mark->getDescription()!=""
        /*&&
                       (mark->getChannel()->getRadius()*100) >  DataMark::getDistance(clatitude, clongitude,mark->getLatitude(), mark->getLongitude())*/
        )
        s << marks[i]->getLongitude() << "," << marks[i]->getLatitude() << ",pointImagePattern:"<< getColor(marks[i]->getLabel()[0]) << marks[i]->getLabel()[0] << ";";
    }

    return s.str();

  }

  common::Picture OpenStreetMapLoader::getMap(double latitude, double longitude, short size, int width, int height)
  {
    std::string s = preprocessQuery(latitude, longitude, size, width, height);

    QNetworkRequest request;
    qDebug() << "URL: " << QString(s.c_str());
    request.setUrl(QUrl(QString(s.c_str())));

    QNetworkReply *reply = manager->get(request);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
      this, SLOT(onManagerFinished(QNetworkReply*)));
    connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
      this, SLOT(onManagerSslErrors(QNetworkReply*,QList<QSslError>)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
      this, SLOT(onReplyError(QNetworkReply::NetworkError)));

                                        //PngPicture(m_data);
    return common::Picture(QImage::fromData(byteArray));
  }

  common::Picture OpenStreetMapLoader::getMapWithMarks(double latitude, double longitude, short size, int width, int height, const DataMarks& marks)
  {
    if (size > 18)
    {
      emit error(QString("error size"));
      return common::Picture(QImage());
    }
    std::string s = preprocessQuery(latitude, longitude, size, width, height, marks);

    QNetworkRequest request;
    qDebug() << "URL2: " << QString(s.c_str());
    request.setUrl(QUrl(QString(s.c_str())));

    QNetworkReply *reply = manager->get(request);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
      this, SLOT(onManagerFinished(QNetworkReply*)));
    connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
      this, SLOT(onManagerSslErrors(/*QNetworkReply*,QList<QSslError>*/)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
      this, SLOT(onReplyError(QNetworkReply::NetworkError)));

                                        //PngPicture(m_data);
    return common::Picture(QImage::fromData(byteArray));
  }

  void OpenStreetMapLoader::onManagerFinished(QNetworkReply* reply)
  {
    QByteArray bufByteArray(reply->readAll());
    if (bufByteArray.size() != 0)
    {
      byteArray.clear();
      byteArray.append(bufByteArray);
      qDebug ("loaded picture size: %d", bufByteArray.size());

      emit mapUpdated(byteArray);
    }

  }

  QByteArray& OpenStreetMapLoader::getMapByteArray()
  {
    return byteArray;
  }

  OpenStreetMapLoader::~OpenStreetMapLoader()
  {

  }

  size_t OpenStreetMapLoader::write(void *buffer, size_t size, size_t nmemb, void *stream)
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

  void OpenStreetMapLoader::onManagerSslErrors(/*QNetworkReply* reply, QList<QSslError> errors*/)
  {
    qDebug("ssl error \n");
  }

  void OpenStreetMapLoader::onReplyError(QNetworkReply::NetworkError error)
  {
    qDebug("Network error: %d \n", error);
  }

}                                       // namespace maps


/* ===[ End of file  ]=== */
