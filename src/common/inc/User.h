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
/*!
 * \file User.h
 * \brief Header of User
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _User_H_83C39FC3_ECFB_41CD_8902_81D6172CD890_INCLUDED_
#define _User_H_83C39FC3_ECFB_41CD_8902_81D6172CD890_INCLUDED_

#include <string>
#include "Handle.h"
#include "Channel.h"

namespace common
{
 /*!
   * User
   */
  class User
  {
    std::string m_login;
    std::string m_password;

    std::string m_result;
    std::string m_token;
    CHandlePtr<Channels> m_channels; // list of subscribed channels

  protected:
    User(const std::string&, const std::string&);
    
    /*!
     * \brief add new channel to list of subscribed channels
     */
    void subscribe(const CHandlePtr<Channel>& channel);
    void unsubscribe(const CHandlePtr<Channel>& channel);
  public:

    const std::string& getLogin() const;
    const std::string& getPassword() const;
    const std::string& getToken() const;
    const CHandlePtr<Channels> getSubscribedChannels() const;

    void setPassword(const std::string password);

    virtual ~User();    
  }; // class User
  
} // namespace common

#endif //_User_H_83C39FC3_ECFB_41CD_8902_81D6172CD890_INCLUDED_

/* ===[ End of file ]=== */
