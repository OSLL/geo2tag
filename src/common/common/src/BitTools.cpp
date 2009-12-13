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
