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
 * \file TimeSlot.h
 * \brief Header of TimeSlot
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _TIMESLOT_H_55a1fda3_1617_4417_a9a2_fdaf7d12f71f_INCLUDED
#define _TIMESLOT_H_55a1fda3_1617_4417_a9a2_fdaf7d12f71f_INCLUDED

#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QDateTime>

#include "ConcurrentVector.h"

class TimeSlot: public QObject
{
  Q_OBJECT

  qulonglong m_slot;   // time slot

  public:

    TimeSlot(const qulonglong &slot);

    virtual qlonglong getId() const;

    const qulonglong& getSlot() const;
    void setSlot(const qulonglong& slot);

    virtual ~TimeSlot();

    //class TimeSlot
};

typedef ConcurrentVector<TimeSlot> TimeSlots;

// _TIMESLOT_H_55a1fda3_1617_4417_a9a2_fdaf7d12f71f_INCLUDED
#endif

/* ===[ End of file ]=== */
