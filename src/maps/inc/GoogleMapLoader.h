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
/*  */
/*!
 * \file GoogleMapLoader.h
 * \brief Header of GoogleMapLoader
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _GoogleMapLoader_H_44AC0505_AE92_49A7_8B34_0D34CE52104E_INCLUDED_
#define _GoogleMapLoader_H_44AC0505_AE92_49A7_8B34_0D34CE52104E_INCLUDED_

#include "MapLoader.h"
#include "DataMarks.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMutex>
#include <QByteArray>

namespace maps
{
  /*!
   *
   */
  class GoogleMapLoader: public MapLoader
  {

    Q_OBJECT

      QNetworkAccessManager *manager;
    std::vector<char> m_data;
    QByteArray byteArray;

    public:
      GoogleMapLoader();

      virtual common::Picture getMap(double latitude, double longitude, short size, int width, int height);

      virtual common::Picture getMapWithMarks(double latitude, double longitude, short size, int width, int height, const DataMarks& marks);

      QByteArray& getMapByteArray();

      virtual ~GoogleMapLoader();

    public slots:

      void onManagerFinished(QNetworkReply*);
      void onManagerSslErrors(/*QNetworkReply*, QList<QSslError>*/);
      void onReplyError(QNetworkReply::NetworkError);

    private:

      std::string preprocessQuery(double latitude, double longitude, short size, int width, int height, const DataMarks &marks = DataMarks());
      /*!
       * \brief routine for writing data from stream
       */
      static size_t write(void *buffer, size_t size, size_t nmemb, void *stream);

      GoogleMapLoader(const GoogleMapLoader& obj);
      GoogleMapLoader& operator=(const GoogleMapLoader& obj);

      // class GoogleMapLoader
  };

  // namespace maps
}


//_GoogleMapLoader_H_44AC0505_AE92_49A7_8B34_0D34CE52104E_INCLUDED_
#endif

/* ===[ End of file  ]=== */
