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
 * \file Session.h
 * \brief Header of Session
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _Session_H_95B6E7F9_B628_4F43_8577_D5BF1A5F59E2_INCLUDED_
#define _Session_H_95B6E7F9_B628_4F43_8577_D5BF1A5F59E2_INCLUDED_

#include <map>
#include <string>

namespace Geo2tag
{

  /**
      This is an entry and initialization point for library
    */
  class Session
  {

    public:

      /**
        Initialize session
        \param initParams key-value init parameters map
        */
      static bool init(const std::map<std::string,std::string> &params);

      /**
        Check the state of session
        */
      static bool isValid();

      /**
        Save current session state for further restore
        \note: will be extended by State object
        */
      static bool saveState();

      /**
        Restore state from previous saved state object
        */
      static bool restoreState();

    private:

      Session(const Session& obj);
      Session& operator=(const Session& obj);

  };                                    // class Session

}                                       // namespace Geo2tag
#endif                                  //_Session_H_95B6E7F9_B628_4F43_8577_D5BF1A5F59E2_INCLUDED_
