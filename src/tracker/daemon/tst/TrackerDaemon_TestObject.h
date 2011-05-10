/*
 * Copyright 2011  Kirill Krinkin  kirill.krinkin@gmail.com
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
 * \file TrackerDaemon_TestObject.h
 * \brief Header of TrackerDaemon_TestObject
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _TrackerDaemon_TestObject_H_46EA22F6_4980_4A50_9E21_6A80DD92A5F8_INCLUDED_
#define _TrackerDaemon_TestObject_H_46EA22F6_4980_4A50_9E21_6A80DD92A5F8_INCLUDED_

#include "../TrackerDaemon.h"

namespace Test
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class TrackerDaemon_TestObject : public TrackerDaemon
  {
    Q_OBJECT

      public:
      TrackerDaemon_TestObject()
      {
      }

      ~TrackerDaemon_TestObject()
      {
      }

    public slots:

      void onTagAdded();
      void run();

    private:
      TrackerDaemon_TestObject(const TrackerDaemon_TestObject& obj);
      TrackerDaemon_TestObject& operator=(const TrackerDaemon_TestObject& obj);

  };                                    // class TrackerDaemon_TestObject

}                                       // namespace Test
#endif                                  //_TrackerDaemon_TestObject_H_46EA22F6_4980_4A50_9E21_6A80DD92A5F8_INCLUDED_
