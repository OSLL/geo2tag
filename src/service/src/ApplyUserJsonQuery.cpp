/*
 * Copyright <year>  <name of author> <e-mail>
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
 * $Id$ 
 *
 * \file ApplyUserJsonQuery.cpp
 * \brief ApplyUserJsonQuery implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <syslog.h>
#include <time.h>
#include "ApplyUserJsonQuery.h"
#include "ChannelInternal.h"
#include "UserInternal.h"
#include "DbSession.h"
#include "Time.h"
#include "Handle.h"
#include "Crc.h"
#include "BitTools.h"

ApplyUserJsonQuery::ApplyUserJsonQuery()
{
    m_status_description = "nothing";
}

void ApplyUserJsonQuery::init(const std::stringstream& query)
{
	json::Element elemRoot;
	std::istringstream s(query.str());
	json::Reader::Read(elemRoot,s);
	json::QuickInterpreter interpreter(elemRoot);

  const json::String& login=interpreter["login"];
	m_login=std::string(login);

  const json::String& password=interpreter["password"];
  m_password=std::string(password);
}

void ApplyUserJsonQuery::process()
{
  CHandlePtr<std::vector<CHandlePtr<common::User> > > users=
      common::DbSession::getInstance().getUsers();

  for(std::vector<CHandlePtr<common::User> >::iterator i=users->begin();i!=users->end();i++)
  {
    if((*i).dynamicCast<loader::User>()->getLogin()==m_login)
    {
 			m_result="Error";
                        m_status_description = "Login already exists";
      return;
    }
  }
  
  std::ostringstream s(""), token("");
  s << "token" << CTime::now() 
               << m_login 
               << m_password 
               << CTime::now();
  CCrc32 crc;
  unsigned long crc32 = crc.add(s.str().c_str(), s.str().size());
  token << crc32 << m_login;
  crc32 = crc.add(token.str().c_str(), token.str().size());
  BitTools::reverse(&crc32, sizeof(crc32));
  token << crc32;
  m_token = token.str();
  CHandlePtr<loader::User> user =
      makeHandle(new loader::User(m_login, m_password, 0, m_token));

  try
  {
    common::DbSession::getInstance().storeUser(user);
    m_result = "Ok";
    m_status_description = "nothing";
  }
  catch(const CExceptionSource& e)
  {
    m_result="Error";
    m_status_description = "Db internal error";
  }
}

std::string ApplyUserJsonQuery::outToString() const
{
	if(m_result=="Error")
  {
            return "{\"status\":\"" + m_result +
                    "\", \"status_description\":\"" + m_status_description+"\"}";
  }
  else
  {
      return "{\"status\":\"" + m_result +
              "\", \"status_description\":\"" + m_status_description +
              "\", \"auth_token\":\"" + m_token + "\"}";
  }
}

/* ===[ End of file $HeadURL$ ]=== */
