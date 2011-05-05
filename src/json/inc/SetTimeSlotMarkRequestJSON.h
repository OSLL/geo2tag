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
 * \file SetTimeSlotMarkRequestJSON.h
 * \brief Header of SetTimeSlotMarkRequestJSON
 *
 * ---------------------------------------------------------------- */

#ifndef _SETTIMESLOTMARKREQUESTJSON_H_a4a8252b_f9ac_4e61_9169_8c9f7496a73a_
#define _SETTIMESLOTMARKREQUESTJSON_H_a4a8252b_f9ac_4e61_9169_8c9f7496a73a_

#include "JsonSerializer.h"

class SetTimeSlotMarkRequestJSON : public JsonSerializer
{
  Q_OBJECT;
  public:
    SetTimeSlotMarkRequestJSON(QObject *parent=0);

    QByteArray getJson() const;

    void parseJson(const QByteArray&);
};
#endif                                  // _SETTIMESLOTMARKREQUESTJSON_H_a4a8252b_f9ac_4e61_9169_8c9f7496a73a_
