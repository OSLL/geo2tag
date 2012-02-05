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
 * \file JsonDataMark.h
 * \brief Header of JsonDataMark
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _JsonDataMark_H_6E8C1DBF_DF18_46D0_9119_1F2D838576EE_INCLUDED_
#define _JsonDataMark_H_6E8C1DBF_DF18_46D0_9119_1F2D838576EE_INCLUDED_

#include "DataMarks.h"

class JsonDataMark: public DataMark
{

  static qlonglong globalTagId;
  //!< Tag's identifier
  qlonglong m_id;

  //!< Assotiated user's identifier for the tag
  qlonglong m_userId;

  public:
    JsonDataMark(double altitude, double latitude, double longitude,
      QString label, QString description, const QString& url, const QDateTime& time);

    qlonglong getId() const;

    qlonglong getUserId() const;

    void setId(qlonglong id);

    virtual ~JsonDataMark();
};
//_JsonDataMark_H_6E8C1DBF_DF18_46D0_9119_1F2D838576EE_INCLUDED_
#endif

/* ===[ End of file ]=== */
