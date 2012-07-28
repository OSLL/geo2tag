/*
 * Copyright 2012  bac1ca  bac1ca89@gmail.com
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
 * \file FilterChannelResponseJSON.h
 * \brief Header of FilterChannelResponseJSON
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _FilterChannelResponseJSON_H_D06843F4_D6CB_4B9E_B538_74F7B2E8D42A_INCLUDED_
#define _FilterChannelResponseJSON_H_D06843F4_D6CB_4B9E_B538_74F7B2E8D42A_INCLUDED_

#include "JsonSerializer.h"

class FilterChannelResponseJSON: public JsonSerializer
{
  private:
    QList<QSharedPointer<DataMark> > m_tags;
    QSharedPointer<Channel> m_channel;

  public:
    FilterChannelResponseJSON(QObject *parent = 0);

    QByteArray getJson() const;

    bool parseJson(const QByteArray&);

    void setData(QSharedPointer<Channel> channel, QList<QSharedPointer<DataMark> > tags);
};
#endif                                  //_FilterChannelResponseJSON_H_D06843F4_D6CB_4B9E_B538_74F7B2E8D42A_INCLUDED_
