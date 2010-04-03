/*
 * Copyright 2010  Open Source & Linux Lab (OSLL) osll@osll.spb.ru
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
/*!
 * \file SubscribedList.h
 * \brief Header of SubscribedList
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _SubscribedList_H_88A269DD_1613_4B0D_99DD_1D6560BE59AD_INCLUDED_
#define _SubscribedList_H_88A269DD_1613_4B0D_99DD_1D6560BE59AD_INCLUDED_

 /*!
   * Class description. May use HTML formatting
   *
   */

#include "DataMarks.h"
#include "DbSession.h"
#include "Channel.h"
#include "Handle.h"
#include "elements.h"
#include "quick.h"
#include "reader.h"
#include "cast.h"
#include "exception.h"
#include <sstream>
#include "stream.h"
#include "ChannelList.h"
  class SubscribedList: public OStream
  {
    std::string m_user;

    CHandlePtr<common::Channels> m_channels;
  public:

    /*
    * constructor with %query% paramete, where %query% is a Subscribed Channels request 
    */

    SubscribedList(const std::stringstream& query);

    /*
    * gets all channels that user are subscribed from DbSession and put it to %m_channels%
    */

    void process();

    /*
    * return %m_channels%
    */

    const CHandlePtr<common::Channels>& getChannels() const;
    
    /*
    * convert %m_channels% into JSON and return it as a string    
    */

    std::string outToString() const;

  }; // class SubscribedList
  


#endif //_SubscribedList_H_88A269DD_1613_4B0D_99DD_1D6560BE59AD_INCLUDED_

/* ===[ End of file ]=== */
