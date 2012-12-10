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
 * \file Channel.h
 * \brief Header of Channel
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _Channel_H_7B1A7CD3_CD56_459B_81FD_1438824FC280_INCLUDED_
#define _Channel_H_7B1A7CD3_CD56_459B_81FD_1438824FC280_INCLUDED_

#include <string>
#include "../inc/geotag.h"

namespace Geo2tag
{
  /*!
   * Channel. Entity for aggregating tags
   *
   */
  class Channel
  {
    size_t m_currentIndex;

    public:
      /**
          Find Channel data
          \param name -- name of channel
        */
      static Channel& loadChannel(const std::string& name);

      /**
        Create channel
        \name channel name
        */
      static Channel& createChannel(const std::string& name);

      /**
        \return channel name
        */
      std::string name() const;

      /**
        \return number of tags in channel
        */
      size_t size() const;

      /**
        \return current tag iterator
        */
      size_t currentIndex() const;

      /**
        \return flag whether channel is Ok
        */
      bool isValid() const;

      /**
        update channel -- load new tags if any
        \return number of new uploaded tags
        */
      size_t update();

    private:
      Channel(const Channel& obj);
      Channel& operator=(const Channel& obj);

  };                                    // class Channel

}                                       // namespace Geo2tag
#endif                                  //_Channel_H_7B1A7CD3_CD56_459B_81FD_1438824FC280_INCLUDED_
