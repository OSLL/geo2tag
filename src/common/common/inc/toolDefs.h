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
 * \file toolDefs.h
 * \brief заголовок общих определений
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CToolDefs_H_29F3F553_913D_4F33_82DF_1CBAC1B9F2B9_INCLUDED_
#define _CToolDefs_H_29F3F553_913D_4F33_82DF_1CBAC1B9F2B9_INCLUDED_

#include <limits.h>

#ifndef C_ASSERT
  /*!
   * \brief assert времени компиляции
   * \param a: [safe] константное выражение этапа компиляции приводимое к bool
   *
   * при нарушении условия возникает ошибка "отрицательный индекс"
   */
  #define C_ASSERT(a) typedef int __C_ASSERT__[(a)?1:-1]
#endif

#ifndef PACKED
  #if PRAGMAPACK
    #define PACKED
  #else
    #define PACKED __attribute__((packed))
  #endif
#endif

#ifndef __GNUC_PREREQ
  #if defined(__GNUC__) && defined(__GNUC_MINOR__)
    /*!
     * \brief версия gcc не меньше указанной
     * \param maj: [safe] старшая часть версии
     * \param min: [safe] младшая часть версии
     */
    #define __GNUC_PREREQ(maj, min) \
      ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
  #else
    #define __GNUC_PREREQ(maj, min) 0
  #endif
#endif


#ifndef __GLIBC_PREREQ
  #if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
    /*!
     * \brief версия glibc не меньше указанной
     * \param maj: [safe] старшая часть версии
     * \param min: [safe] младшая часть версии
     */
    #define __GLIBC_PREREQ(maj, min) \
      ((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((maj) << 16) + (min))
  #else
    #define __GLIBC_PREREQ(maj, min) 0
  #endif
#endif


typedef unsigned int uint;
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned short uint16;

typedef signed char int8;
typedef signed int int32;
typedef signed short int16;

#define __VERIFY_INT_BIT_COUNT(name,count) C_ASSERT(sizeof(name##count)*CHAR_BIT==(count))
__VERIFY_INT_BIT_COUNT(uint,8);
__VERIFY_INT_BIT_COUNT(uint,16);
__VERIFY_INT_BIT_COUNT(uint,32);
__VERIFY_INT_BIT_COUNT(int,8);
__VERIFY_INT_BIT_COUNT(int,16);
__VERIFY_INT_BIT_COUNT(int,32);
#undef __VERIFY_INT_BIT_COUNT

#ifndef COUNT
  /*!
   * \brief количество элементов массива фиксированного размера
   * \param a: [unsafe] имя массива
   */
  #define COUNT(a) (sizeof(a)/sizeof(*(a)))
#endif

#ifndef SIZEOF_RANGE
  /*!
   * \brief количество байт от начала поля a до окончания поля b
   * \param a: [safe] начальное поле
   * \param b: [unsafe] конечное поле
   */
  #define SIZEOF_RANGE(a,b) (((char*)&(b))-((char*)&(a))+sizeof(b))
#endif

#endif //_CToolDefs_H_29F3F553_913D_4F33_82DF_1CBAC1B9F2B9_INCLUDED_

/* ===[ End of file  ]=== */
