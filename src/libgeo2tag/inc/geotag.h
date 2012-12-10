/*
 * Copyright 2010 - 2012  Kirill Krinkin  kirill.krinkin@gmail.com
 *
 * Geo2tag LBS Platform (geo2tag.org)
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
 * \file Geotag.h
 * \brief Header of Geotag
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _Geotag_H_04676504_3E24_4452_B066_52C876A91FFA_INCLUDED_
#define _Geotag_H_04676504_3E24_4452_B066_52C876A91FFA_INCLUDED_

#include <string>

namespace Geo2tag
{

  class Coord
  {
    double altitude;
    double latitude;
    double longitude;
  };

  /*!
   * Class description. May use HTML formatting
   *
   */
  class Geotag
  {
    Coord       m_coord;
    __int64_t   m_timestamp;
    std::string m_label;
    std::string m_data;

    public:

      Geotag(const Coord& coord, const std::string& label, std::string& data);

      __int64_t timestamp();
      std::string label();
      std::string data();

    private:
      Geotag(const Geotag& obj);
      Geotag& operator=(const Geotag& obj);

  };                                    // class Geotag

}                                       // namespace Geo2tag
#endif                                  //_Geotag_H_04676504_3E24_4452_B066_52C876A91FFA_INCLUDED_
