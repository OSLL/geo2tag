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
 * \file BitTools.h
 * \brief заголовок вспомогательных функций работы с битами
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ------------------------------------------------------------------------ */


#ifndef _CBitTools_H_3075711B_FCB3_4877_A21C_CA0FB50902E6_INCLUDED_
#define _CBitTools_H_3075711B_FCB3_4877_A21C_CA0FB50902E6_INCLUDED_

#include <cassert>
#include <cstddef>
#include <algorithm>
#include <limits.h>

namespace BitTools
{

/*!
 * \brief количество единичных бит в байте
 * \param c: [in] анализируемый байт
 * \return количество единичных бит в байте c
 */
inline unsigned char getBitCount(unsigned char c)
{
  static const unsigned char bc[]=
  {
    0,1,1,2,
    1,2,2,3,
    1,2,2,3,
    2,3,3,4
  };

  return bc[c & 0xf]+bc[(c >> 4) & 0xf];
}

/*!
 * \brief количество единичных бит в буфере
 * \param p: [in] анализируемый буфер
 * \param sz: [in] размер буфера (байты)
 * \return количество единичных бит
 */
inline size_t getBitCount(const void *p,size_t sz)
{
  size_t r=0;
  
  for(size_t i=0;i<sz;++i)
    r+=getBitCount(i[(unsigned char*)p]);
  
  return r;
}

/*!
 * \brief количество единичных бит в значении произвольного типа
 * \param v: [in] анализируемое значение
 * \return количество единичных бит
 */
template<typename T>
size_t getBitCount(T v)
{
  return getBitCount(&v,sizeof(v));
}

/*!
 * \brief переворот 4бит "задом наперед": 0x1 -> 0x8
 * \param nibble: [in] 4 бита для переворота (младшие)
 * \return входные 4 бит задом наперед (в младших)
 */
inline unsigned char reverseNibble(unsigned char nibble)
{
  static const int t[]=
  {
    0x0,0x8,0x4,0xc,
    0x2,0xa,0x6,0xe,
    0x1,0x9,0x5,0xd,
    0x3,0xb,0x7,0xf
  };
  
  assert(nibble < 0x10);
  return t[nibble & 0xf];
}

/*!
 * \brief переворот байта "задом наперед": 0x1 -> 0x80
 * \param byte: [in] байт для переворота
 * \return входной байт задом наперед
 */
inline unsigned char reverse(unsigned char byte)
{
  return (reverseNibble(byte & 0xf) << 4) | reverseNibble((byte >> 4) & 0xf);
}

/*!
 * \brief переворот битовой строки "задом наперед": 0x1,0 -> 0,0x80
 * \param dst: [out] перевернутая строка
 * \param src: [in] исходная строка
 * \param sz: [in] количество байт для переворота
 */
inline void reverse(void *dst,const void *src,size_t sz)
{
  unsigned char *d=(unsigned char*)dst;
  unsigned char *s=(unsigned char*)src;
  
  assert((d<=s && d+sz<=s && d<=d+sz) ||
    (s<=d && s+sz<=d && s<=s+sz));
  
  s+=sz;
  for(size_t i=0;i<sz;++i)
  {
    * d++ = reverse(* --s);
  }
}

/*!
 * \brief переворот битовой строки "задом наперед" на месте: 0x1,0 -> 0,0x80
 * \param buf: [in,out] битовая строка
 * \param sz: [in] количество байт для переворота
 */
inline void reverse(void *buf,size_t sz)
{
  unsigned char *p=(unsigned char*)buf;
  size_t i,j;
  
  for(i=0,j=sz-1;i<=j;++i,--j)
  {
    unsigned char t=p[j];
    p[j]=reverse(p[i]);
    p[i]=reverse(t);
  }
}


/*!
 * \brief переворот байтов "задом наперед": 0x1,0 -> 0x80,0
 * \param dst: [out] перевернутая строка
 * \param src: [in] исходная строка
 * \param sz: [in] количество байт для переворота
 */
inline void bytewiseReverse(void *dst,const void *src,size_t sz)
{
  unsigned char *d=(unsigned char*)dst;
  unsigned char *s=(unsigned char*)src;
  
  assert((d<=s && d+sz<=s && d<=d+sz) ||
    (s<=d && s+sz<=d && s<=s+sz));
  
  for(size_t i=0;i<sz;++i,++d,++s)
  {
    *d=reverse(*s);
  }  
}

/*!
 * \brief переворот байтов строки "задом наперед" на месте: 0x1,0 -> 0x80,0
 * \param buf: [in,out] битовая строка
 * \param sz: [in] количество байт для переворота
 */
inline void bytewiseReverse(void *buf,size_t sz)
{
  unsigned char *p=(unsigned char*)buf;
  
  for(size_t i=0;i<sz;++i,++p)
  {
    *p=reverse(*p);
  }
}


  /*!
   * \brief логический сдвиг битовой строки "вправо" (в сторону младших битов)
   * \param p: [in,out] сдвигаемая строка
   * \param shift: [in] расстояние сдвига в битах
   * \param sz: [in] длина строки в байтах
   */
  void shiftRight(void *p,size_t shift,size_t sz);

  /*!
   * \brief логический сдвиг битовой строки "влево" (в сторону старших битов)
   * \param p: [in,out] сдвигаемая строка
   * \param shift: [in] расстояние сдвига в битах
   * \param sz: [in] длина строки в байтах
   */
  void shiftLeft(void *p,size_t shift,size_t sz);

  /*!
   * \brief проверить что машина -- big endian
   * \return истина если big endian
   */
  inline bool isBigEndian()
  {
    static const union
    {
      long l;
      char c;
    } d={0x12345678};
    
    return d.c==0x12;
  }
  
  /*!
   * \brief перевести значение из big endian в нормальное для машины
   * \param T: [typename] тип переводимого значения
   * \param v: [in] переводимое значение
   * \return значение, соответствующее big endian v
   */
  template<typename T>
  T convertBeToNative(T v)
  {
    if(!isBigEndian())
      for(size_t i=0;i<sizeof(T)/2;++i)
        std::swap(i[(char*)&v],(sizeof(T)-1-i)[(char*)&v]);
        
    return v;
  }


  /*!
   * \brief получить бит из буфера по номеру
   * \param p: [in] буфер
   * \param bitNo: [in] номер бита
   * \return значение бита
   */
  inline bool getBit(const void *p,size_t bitNo)
  {
    return ((unsigned char*)p)[bitNo/8] & (1 << (bitNo & 7));
  }

  /*!
   * \brief установить бит в буфере по номеру
   * \param p: [in,out] буфер
   * \param bitNo: [in] номер бита
   * \param set: [in] значение устанавливаемое биту
   */
  inline void setBit(const void *p,size_t bitNo,bool set=true)
  {
    if(set)
      ((unsigned char*)p)[bitNo/8] |= (1 << (bitNo & 7));
    else
      ((unsigned char*)p)[bitNo/8] &= ~(1 << (bitNo & 7));    
  }

  /*!
   * \brief инвертировать бит в буфере по номеру
   * \param p: [in,out] буфер
   * \param bitNo: [in] номер бита
   */
  inline void toggleBit(const void *p,size_t bitNo)
  {
    ((unsigned char*)p)[bitNo/8] ^= (1 << (bitNo & 7));
  }
  
  /*!
   * \brief получить нормальное значение числа в доп. коде
   * т.е. если старший разряд числа установлен -- сбросить его и изменить знак
   */
  template<typename T>
  T getComplementaryValue(T v)
  {
    const T MSB=((T)1) << (sizeof(T)*CHAR_BIT-1);
    
    if(v & MSB)
      return -(v ^ MSB);
    else
      return v;
  }
};

#endif //_CBitTools_H_3075711B_FCB3_4877_A21C_CA0FB50902E6_INCLUDED_

/* ===[ End of file  ]=== */
