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
 * \file Sleep.h
 * \brief функции задержки с высоким разрешением
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CSleep_H_D0929CD6_FCE2_4426_B864_A1D78DF41F43_INCLUDED_
#define _CSleep_H_D0929CD6_FCE2_4426_B864_A1D78DF41F43_INCLUDED_

/*!
 * \brief пауза на заданное число микросекунд
 * \param uSec: [in] величина паузы (мкс.)
 *
 * эта пауза не прерывается никакими сигналами, согласно libc.info::Sleeping
 * но _на самом деле_ -- прерывается.
 * версия с nanosleep определенно прерывается
 *
 * select соответствует BSD
 * nanosleep -- POSIX
 */
#include <stddef.h>
#if 1
#include <sys/time.h>

inline void uSleep(long uSec)
{
  timeval to;
  to.tv_sec=uSec/1000000;
  to.tv_usec=uSec % 1000000;
  select(0,NULL,NULL,NULL,&to);
}

inline void mSleep(long mSec)
{
  timeval to;
  to.tv_sec=mSec/1000;
  to.tv_usec=(mSec % 1000)*1000;
  select(0,NULL,NULL,NULL,&to);
}

#else
#include <time.h>

inline void uSleep(long uSec)
{
  timespec to;
  to.tv_sec=uSec/1000000;
  to.tv_nsec=(uSec % 1000000)*1000;
  nanosleep(&to,NULL);
}
#endif

#endif //_CSleep_H_D0929CD6_FCE2_4426_B864_A1D78DF41F43_INCLUDED_

/* ===[ End of file  ]=== */
