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
 * \file UnsubscribeJsonQuery.h
 * \brief Header of UnsubscribeJsonQuery
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag

 * ---------------------------------------------------------------- */


#ifndef _UnsubscribeJsonQuery_H_INCLUDED_
#define _UnsubscribeJsonQuery_H_INCLUDED_

#include "DataMarks.h"
#include "Channel.h"
#include "Handle.h"
#include "elements.h"
#include "quick.h"
#include "reader.h"
#include "writer.h"
#include "cast.h"
#include "exception.h"
#include <sstream>
#include "IJsonQuery.h"


 /*!
   * Class description. May use HTML formatting
   *
   */
class UnsubscribeJsonQuery: public IJsonQuery
{
	std::string m_token;
	std::string m_channel;
	std::string m_status;
        std::string m_status_description;
	std::string outToString() const;
	void init(const std::stringstream& query);
public:
	UnsubscribeJsonQuery();
	void process();
    

}; // class UnsubscribeJsonQuery
  

#endif //_UnsubscribeJsonQuery_H_INCLUDED_

/* ===[ End of file $HeadURL$ ]=== */
