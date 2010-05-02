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
 * \file AvailableList.h
 * \brief header of AvailableList
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ------------------------------------------------------------------------ */
#ifndef _AVAILABLELIST_H_ECF43BFB_5C72_4CE2_B65E_3D1F51600DF3_INCLUDED_
#define _AVAILABLELIST_H_ECF43BFB_5C72_4CE2_B65E_3D1F51600DF3_INCLUDED_

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
#include "ChannelList.h"

class AvailableList: public  IJsonQuery/*can be only writed to stream*/ 
{
	std::string                   m_token; //! \ToDo comment here
	double                        m_radius;    //! \ToDo comment here
	double                        m_latitude;  //! \ToDo comment here
	double                        m_longitude; //! \ToDo comment here
	CHandlePtr<common::Channels>  m_channels;//! \ToDo comment here
  
  std::string outToString() const;
  void init(const std::stringstream& query);
public:
	AvailableList();
	
  void process();
	
  const CHandlePtr<common::Channels>& getChannels() const;

};//class AvailableList

#endif// _AVAILABLELIST_H_ECF43BFB_5C72_4CE2_B65E_3D1F51600DF3_INCLUDED_
/* ===[ End of file  ]=== */
