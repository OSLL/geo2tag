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
 * \file ChannelInternal.cpp
 * \brief ChannelInternal implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "ChannelInternal.h"
#include "DataMarkInternal.h"
#include "DbSession.h"

namespace loader
{
  Channel::Channel(unsigned long long id, const std::string &name, const std::string &description, const std::string &url, const CHandlePtr<common::DataMarks> &marks): 
                  common::Channel(name, description, url, marks), m_id(id)
  {
  }
  
  unsigned long long Channel::getId() const
  {
    return m_id;
  }
  
  void Channel::setId(unsigned long long id)
  {
    m_id=id;
  }

  void Channel::addData(CHandlePtr<common::DataMark> mark)
  {
    CHandlePtr<loader::DataMark> m = mark.dynamicCast<loader::DataMark>();
    if(!m)
    {
      // epic fail
      return;
    }
    // update relation betwean mark and channel
    common::DbSession::getInstance().updateChannel(getId(), mark);
    // add mark to Channel object
    common::Channel::addData(mark);
  }

  Channel::~Channel()
  {
  }
} // namespace loader

/* ===[ End of file ]=== */
