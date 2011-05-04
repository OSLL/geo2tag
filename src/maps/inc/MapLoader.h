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
 * \file MapLoader.h
 * \brief Header of MapLoader
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _MapLoader_H_7DB6BBD9_E4AD_4818_8B45_5B18EC8FC60C_INCLUDED_
#define _MapLoader_H_7DB6BBD9_E4AD_4818_8B45_5B18EC8FC60C_INCLUDED_

#include "Picture.h"
#include "DataMarks.h"
#include <QObject>

namespace maps
{
  /*!
   * MapLoader interface
   */
  class MapLoader : public QObject
  {
    Q_OBJECT

      public:

      /*!
       * \brief list fot available maps sources
       */
      enum SourceType
      {
        GOOGLE = 0,
        OPEN_STREET,
        UNKNOWN
      };

      /*!
       * \brief load map as picture
       * \param latitude -- image center latitude
       * \param longitude -- image center longitude
       * \param scale -- image scale
       * \param width -- image width
       * \param height -- image height
       */
      virtual common::Picture getMap(double latitude, double longitude, short scale, int width, int height)=0;

      virtual common::Picture getMapWithMarks(double latitude, double longitude, short scale, int width, int height, const DataMarks& marks)=0;

      virtual QByteArray& getMapByteArray()=0;

      signals:
      void mapUpdated(QByteArray & picture);
      void error(QString error);

      // class MapLoader
  };

  // namespace maps
}


//_MapLoader_H_7DB6BBD9_E4AD_4818_8B45_5B18EC8FC60C_INCLUDED_
#endif

/* ===[ End of file  ]=== */
