/*  */
/*!
 * \file MemDump.cpp
 * \brief реализация класса CMemDump
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include "MemDump.h"
#include <sstream>
#include <iomanip>
#include <toolDefs.h>

#if __GNUC_PREREQ(3,2)
#include <locale>

bool isPrint(char c)
{
  return std::isprint(c,std::locale());
}

#else
#include <ctype.h>

bool isPrint(char c)
{
  return isprint(c);
}

#endif

namespace BitTools
{
  std::ostream& dump(std::ostream& stm,const void *data,size_t sz,const std::string& pfx)
  {
    for(size_t i=0;i<sz;)
    {
      std::ostringstream x,s;
      x << std::hex << std::setfill('0');
      
      for(size_t j=0;j<16;++j,++i)
      {
        if(i<sz)
        {
          int c=i[(unsigned char*)data];
          
          x << std::setw(2) << c << " ";
          if(isPrint(c))
            s << (char)c;
          else
            s << '.';
        }
        else
          x << "   ";
      }
      
      stm << pfx << x.str() << ": " << s.str() << std::endl;
    }
    return stm;
  }
}

/* ===[ End of file  ]=== */
