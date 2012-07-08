/*
 * Copyright 2011  bac1ca  bac1ca89@gmail.com
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
 * \file FilterDefaultResponseJSON.h
 * \brief Header of FilterDefaultResponseJSON
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _FilterDefaultResponseJSON_H_9F6E9355_0467_4027_87F0_9F45C6039952_INCLUDED_
#define _FilterDefaultResponseJSON_H_9F6E9355_0467_4027_87F0_9F45C6039952_INCLUDED_

#include "JsonSerializer.h"
#include "DataChannel.h"

class FilterDefaultResponseJSON : public JsonSerializer
{
  DataChannels m_hashMap;

  public:
    FilterDefaultResponseJSON(QObject *parent=0);

    ~FilterDefaultResponseJSON();

    const DataChannels& getDataChannels();

    void setDataChannels(const DataChannels& dataChannels);

    virtual bool parseJson(const QByteArray&);

    virtual QByteArray getJson() const;

};                                      // class FilterDefaultResponseJSON
#endif                                  //_FilterDefaultResponseJSON_H_9F6E9355_0467_4027_87F0_9F45C6039952_INCLUDED_
