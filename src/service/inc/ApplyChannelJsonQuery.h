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

/* $Id$ */
/*!
 * \file ApplyChannelJsonQuery.h
 * \brief Header of ApplyChannelJsonQuery
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag

 * ---------------------------------------------------------------- */


#ifndef _ApplyChannelJsonQuery_H_63C8A8C3_6AB3_4BE4_BC90_6F9057182695_INCLUDED_
#define _ApplyChannelJsonQuery_H_63C8A8C3_6AB3_4BE4_BC90_6F9057182695_INCLUDED_

#include <sstream>
#include "Channel.h"
#include "Handle.h"
#include "elements.h"
#include "quick.h"
#include "reader.h"
#include "writer.h"
#include "cast.h"
#include "exception.h"
#include "IJsonQuery.h"

/*!
 * Class description. May use HTML formatting
 *
 */
class ApplyChannelJsonQuery: public IJsonQuery
{
  std::string                   m_token;

  std::string		                m_name;
  std::string                   m_description;
  std::string                   m_url;
  std::string         		m_status;
  std::string                   m_status_description;
  double                        m_activeRadius;
  
  std::string outToString() const;
  
  void init(const std::stringstream& query);

public:

  ApplyChannelJsonQuery();
	void process();
}; // class ApplyChannelJsonQuery
  

#endif //_ApplyChannelJsonQuery_H_63C8A8C3_6AB3_4BE4_BC90_6F9057182695_INCLUDED_

/* ===[ End of file $HeadURL$ ]=== */
