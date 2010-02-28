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
 * \file BitTools.cpp
 * \brief реализации функций работы с битами
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include "BitTools.h"
#include <string.h>

namespace BitTools
{

  void shiftRight(void *p,size_t shift,size_t sz)
  {
    if(shift==0)
      return;
    
    if(shift/8>=sz)
    {
      memset(p,0,sz);
      return;
    }
      
    unsigned char *pDst=(unsigned char*)p;
    unsigned char *pSrc=pDst + shift/8;
    const size_t sh=shift & 7;
    size_t i;
    
    for(i=0;i<(sz*8-shift)/8;++i)
    {
      pDst[i]=(pSrc[i] >> sh) | (pSrc[i+1] << (8-sh));
    }
    
    if(sh)
      pDst[i]=pSrc[i] >> sh;
  
    if(shift>=8)
      memset(pDst+sz-shift/8,0,shift/8);
  }

  void shiftLeft(void *p,size_t shift,size_t sz)
  {
    if(shift==0)
      return;
    
    if(shift/8>=sz)
    {
      memset(p,0,sz);
      return;
    }
      
    unsigned char *pDst=(unsigned char*)p;
    unsigned char *pSrc=pDst - shift/8;
    const size_t sh=shift & 7;
    size_t i;
    
    for(i=0;i<(sz*8-shift)/8;++i)
    {
      pDst[sz-1-i]=(pSrc[sz-1-i] << sh) | (pSrc[sz-1-i-1] >> (8-sh));
    }
    
    if(sh)
      pDst[sz-1-i]=pSrc[sz-1-i] << sh;
  
    if(shift>=8)
      memset(pDst,0,shift/8);
  }

};

/* ===[ End of file  ]=== */
