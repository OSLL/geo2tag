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
