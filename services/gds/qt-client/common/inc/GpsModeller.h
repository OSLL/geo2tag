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
 * \file GpsModeller.h
 * \brief Header of GpsModeller
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _GpsModeller_H_DA9EFC5F_CC89_4791_A371_C1ACB86BFC8B_INCLUDED_
#define _GpsModeller_H_DA9EFC5F_CC89_4791_A371_C1ACB86BFC8B_INCLUDED_

#include <QDateTime>
#include <QString>
#include <QThread>
#include "GpsInfo.h"
#include "defines.h"

// This file maybe used only if SIMULATE_GPS is turned on
#ifdef SIMULATE_GPS
#include <libxml/parser.h>
#include <libxml/tree.h>
namespace common
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class GpsModeller: public Gps, private QThread
  {
    double m_longitude;
    double m_latitude;

    QDateTime begin;
    QDateTime currentModellerTime;

    xmlDoc *m_doc;

    void searchElement(xmlNode *node);

    void run();

    public:
      GpsModeller(const QString &gpxFile=GPS_MODELLER_FILE);

      virtual double getLongitude() const;

      virtual double getLatitude() const;

      ~GpsModeller();
      // class GpsModeller
  };

  // namespace common
}


// SIMULATE_GPS
#endif
//_GpsModeller_H_DA9EFC5F_CC89_4791_A371_C1ACB86BFC8B_INCLUDED_
#endif

/* ===[ End of file  ]=== */
