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
 * \file Crc.h
 * \brief заголовок класса CCrc
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ------------------------------------------------------------------------ */


#ifndef _CCrc_H_7A91CF30_E81F_441F_AF5E_3C4ADC8B46A1_INCLUDED_
#define _CCrc_H_7A91CF30_E81F_441F_AF5E_3C4ADC8B46A1_INCLUDED_

#include <cstddef>

/*!
 * \brief таблица CRC для данного полинома
 * \param T: [typename] тип значения CRC
 * \param poly: полином образующий CRC
 */
template<class T,T poly>
class CCrcTable
{
protected:
  /*!
   * таблица для прямого счета CRC
   */
  static T s_crcTable[256];

  /*!
   * таблица для обратного счета CRC
   */
  static unsigned char s_crcReverse[256];

  /*!
   * \brief заполнить таблицы для счета CRC
   */
  static void initTable()
  {
    for(int i=0;i<256;++i)
    {
      T v=i;

      for(int j=0;j<8;++j)
        if(v & 1)
          v = (v >> 1) ^ poly;
        else
          v >>= 1;

      s_crcTable[i]=v;
      s_crcReverse[0xff & (v >> 8*(sizeof(T)-1))]=i;
    }
  }

  CCrcTable()
  {
    if(s_crcTable[0x80]!=poly)
      initTable();
  }
};

template<class T,T poly>
T CCrcTable<T,poly>::s_crcTable[256];

template<class T,T poly>
unsigned char CCrcTable<T,poly>::s_crcReverse[256];

#if 0 //реализовано неправильно

template<class T,T poly>
class CCrcTableInv
{
protected:
    static T s_crcTable[256];
    static unsigned char s_crcReverse[256];

    static void initTable()
    {
        for(int i=0;i<256;++i)
        {
            T v=i;

            for(int j=0;j<8;++j)
                if(v & 0x80)
                    v = ((v ^ 0x80) << 1) ^ poly;
                else
                    v <<= 1;

            s_crcTable[i]=v;
            s_crcReverse[0xff & (v >> 8*(sizeof(T)-1))]=i;
        }
    }

    CCrcTableInv()
    {
        if(s_crcTable[1]!=poly)
            initTable();
    }
};

template<class T,T poly>
T CCrcTableInv<T,poly>::s_crcTable[256];

template<class T,T poly>
unsigned char CCrcTableInv<T,poly>::s_crcReverse[256];

#endif

/*!
 * \brief алгоритмы вычисления CRC
 * \param T: [typename] 
 * \param poly: образующий полином
 * \param init: начальное значение CRC
 * \param out: xor на выходе
 * \param CrcTable: [typename] таблица CRC
 */
template<class T,T poly,T init=~(T)0,T out=0,class CrcTable=CCrcTable<T,poly> >
class CCrc : public CrcTable
{
protected:
  /*!
   * текущее внутреннее (без финального xor) значение CRC
   */
  T m_crc;
  typedef unsigned char uint8;

public:
  
  /*!
   * \brief конструктор с добавлением данных
   * \param data: [in] данные для добавления в CRC
   * \param sz: [in] размер данных
   */
  CCrc(const void *data=NULL,size_t sz=0) : CrcTable()
  {
    m_crc=init;

    if(sz)
      add(data,sz);
  }

  /*!
   * \brief добавить данные
   * \param data: [in] данные для добавления в CRC
   * \param sz: [in] размер данных
   * \return значение CRC после добавления
   */
  T add(const void *data,size_t sz)
  {
    register const uint8 *pData=(const uint8*)data;

    while(sz--)
    {
      m_crc=this->s_crcTable[((uint8)m_crc ^ *pData++) & 0xff] ^
        ((T)(m_crc >> 8));
    }

    return (T)*this;
  }

#if 0 //похоже, реализовано неправильно
  /*!
   * \brief удалить данные
   * \param data: [in] данные для удаления из CRC
   * \param sz: [in] размер данных
   * \return значение CRC после удаления
   *
   * \post this->add(data,sz) == значению CRC до вызова remove(data,sz)
   */
  T remove(const void *data,size_t sz)
  {
    const uint8 *pData=(const uint8*)data+sz;

    while(sz--)
    {
      uint8 i=this->s_crcReverse[0xff & (uint8)(m_crc >> 8*(sizeof(T)-1))];

      m_crc=((m_crc ^ this->s_crcTable[i]) << 8) | i;
      m_crc ^= *--pData;
    }
    return (T)*this;
  }
#endif

  /*!
   * \brief получить значение CRC
   */
  operator T() const
  {
    return m_crc ^ out;
  }

  /*!
   * \brief установить значение CRC
   * \param v: [in] новое значение CRC
   */
  T operator =(T v)
  {
    m_crc=v ^ out;
    return v;
  }
};

typedef CCrc<unsigned long,0xedb88320,~0,~0> CCrc32;
typedef CCrc<unsigned short,0xa001> CCrc16;

#endif //_CCrc_H_7A91CF30_E81F_441F_AF5E_3C4ADC8B46A1_INCLUDED_

