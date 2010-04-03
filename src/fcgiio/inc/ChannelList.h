/*
 * Copyright 2010  Open Source & Linux Lab (OSLL)  osll@osll.spb.ru
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
 * \file ChannelList.h
 * \brief header of ChannelList
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ------------------------------------------------------------------------ */
#ifndef _ChannelList_H_954A5299_7A64_4463_AFEC_D181A29F4FA7_INCLUDED_
#define _ChannelList_H_954A5299_7A64_4463_AFEC_D181A29F4FA7_INCLUDED_

#include "Channel.h"
#include "Handle.h"
#include "elements.h"
#include "quick.h"
#include "reader.h"
#include "writer.h"
#include "cast.h"
#include "exception.h"
#include <sstream>

class ChannelList{
	std::stringstream m_json;
	CHandlePtr<common::Channels> m_channels;	
        void convertInJSON();
        void convertInChannels();
public:
	ChannelList(const CHandlePtr<common::Channels>& channels);
	ChannelList(const std::stringstream& json);
	const std::stringstream& getJSON() const;
  const CHandlePtr<common::Channels>& getChannels() const ;
};//class ChannelList

#endif// _ChannelList_H_954A5299_7A64_4463_AFEC_D181A29F4FA7_INCLUDED_
/* ===[ End of file  ]=== */
