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
/*  */
/*!
 * \file Picture.h
 * \brief Header of Picture
 *
 *  PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _Picture_H_86992429_0AAF_42A5_9706_B8DDACCE6103_INCLUDED_
#define _Picture_H_86992429_0AAF_42A5_9706_B8DDACCE6103_INCLUDED_

#include <QVector>
#include <QImage>

namespace common
{
  /*!
   * common picture. binary data + some meta information
   */
  class Picture : public QImage
  {
    QVector<char> m_data;
    public:

      Picture(const Picture&);
      Picture(const QImage&);

      enum Type
      {
        PNG = 0,
        UNKNOWN
      };

      virtual Type getType() const;

      virtual ~Picture();

    protected:
      Picture(const QVector<char>& data);

      // class Picture
  };

  // namespace common
}


//_Picture_H_86992429_0AAF_42A5_9706_B8DDACCE6103_INCLUDED_
#endif

/* ===[ End of file  ]=== */
