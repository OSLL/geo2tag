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

/* $Id$ */
/*!
 * \file LoadTagsJSON.h
 * \brief Header of LoadTagsJSON
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _LoadTagsResponseJSON_H_04BD2106_8277_46A9_A0E2_EAC41FE34162_INCLUDED_
#define _LoadTagsResponseJSON_H_04BD2106_8277_46A9_A0E2_EAC41FE34162_INCLUDED_

#include <QMultiHash>
#include "json/inc/JsonSerializer.h"
#include "common/inc/Channel.h"
#include "common/inc/DataMarks.h"
#include "common/inc/DataChannel.h"

class LoadTagsResponseJSON: public JsonSerializer
{
  // map will contain channels and marks which should be serialized to JSON
  DataChannels m_hashMap;

  public:
    LoadTagsResponseJSON(const DataChannels &, QObject *parent=0);

    LoadTagsResponseJSON(QObject *parent=0);

    virtual QByteArray getJson() const;

    virtual bool parseJson(const QByteArray&);

    const DataChannels& getData() const;
    void setData(const DataChannels&);

    ~LoadTagsResponseJSON();

    //class LoadTagsJSON
};
//_LoadTagsJSON_H_04BD2106_8277_46A9_A0E2_EAC41FE34162_INCLUDED_
#endif

/* ===[ End of file $HeadURL$ ]=== */
