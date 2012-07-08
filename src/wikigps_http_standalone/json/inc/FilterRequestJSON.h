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
 * \file FilterRequestJSON.h
 * \brief Header of FilterRequestJSON
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _FilterRequestJSON_H_EA76CFDF_BF80_4912_9999_802D12531039_INCLUDED_
#define _FilterRequestJSON_H_EA76CFDF_BF80_4912_9999_802D12531039_INCLUDED_

#include "JsonSerializer.h"
#include "FShape.h"

class FilterRequestJSON : public JsonSerializer
{
  QSharedPointer<FShape> m_shape;
  QDateTime m_timeFrom;
  QDateTime m_timeTo;
  double m_alt1;
  double m_alt2;

  public:
    FilterRequestJSON(QObject *parent = 0);
    ~FilterRequestJSON();

    void setShape(const QSharedPointer<FShape> &shape);
    QSharedPointer<FShape>& getShape();

    void setTimeFrom(QDateTime timeFrom);
    QDateTime getTimeFrom() const;

    void setTimeTo(QDateTime timeTo);
    QDateTime getTimeTo() const;

    void setAltitude1(double alt);
    void setAltitude2(double alt);
    double getAltitude1() const;
    double getAltitude2() const;

    void setChannel(const QSharedPointer<Channel> &channel);

    bool parseJson(const QByteArray&);
};                                      // class FilterRequestJSON
#endif                                  //_FilterRequestJSON_H_EA76CFDF_BF80_4912_9999_802D12531039_INCLUDED_
