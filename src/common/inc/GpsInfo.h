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
 * \file GpsInfo.h
 * \brief Header of GpsInfo
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _GpsInfo_H_D901B2BD_45DB_41D6_A4E2_08407E34EDCD_INCLUDED_
#define _GpsInfo_H_D901B2BD_45DB_41D6_A4E2_08407E34EDCD_INCLUDED_

namespace common
{
  class Gps
  {
    bool m_ready;
    public:
      virtual double getLongitude() const=0;

      virtual double getLatitude() const=0;

      virtual bool isReady()
      {
        return m_ready;
      }

      void setReady(bool state)
      {
        m_ready=state;
      }
  };
  /*!
   * Class description. May use HTML formatting
   *
   */
  class GpsInfo
  {
    public:

      ~GpsInfo();

      static Gps& getInstance();

    private:
      GpsInfo();

      // class GpsInfo
  };

  // namespace common
}


//_GpsInfo_H_D901B2BD_45DB_41D6_A4E2_08407E34EDCD_INCLUDED_
#endif

/* ===[ End of file  ]=== */
