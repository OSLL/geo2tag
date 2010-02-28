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
 * \file Trace.cpp
 * \brief реализация класса CTrace
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include <map>
#include <algorithm>
#include <sstream>
#include "toolDefs.h"
#include "Trace.h"
#include "Mutex.h"
#include "Thread.h"

std::string CSource::getDescription() const
{
  std::stringstream s;
  
  s << *this;
  return s.str();
}

#ifdef _REENTRANT
static Thread::CFastMutex s_lock;
#endif

static CStackTrace& getStackTrace()
{
#ifdef _REENTRANT
  #if __GNUC_PREREQ(3,4) && __GLIBC_PREREQ(2,3)

    static __thread CStackTrace *obj;

    if(!obj)
    {
      Thread::CFastMutex::CMutexLock l(s_lock);

      static std::map<int,CStackTrace> objs;
      obj=&objs[Thread::CThread::getId()];
    }
    return *obj;

  #else

    Thread::CFastMutex::CLock l(s_lock);

    static std::map<int,CStackTrace> obj;
    return obj[Thread::CThread::getId()];

  #endif
#else

  static CStackTrace obj;

  return obj;

#endif
}

const CStackTrace& CStackTrace::getInstance()
{
  return getStackTrace();
}

void CStackTrace::push(const CSource& src)
{
  getStackTrace().m_trace.push_back(src);
}

void CStackTrace::pop()
{
  getStackTrace().m_trace.pop_back();
}

namespace
{
  class COut
  {
    std::ostream& m_stm;
  public:
    COut(std::ostream& stm): m_stm(stm)
    {
    }
    
    void operator ()(const CSource& src)
    {
      m_stm << src << std::endl;
    }
  };
}

std::ostream& operator << (std::ostream& stm,const CStackTrace& a)
{
  if(a.getTrace().empty())
    stm << "<empty>" << std::endl;
  else
    std::for_each(a.getTrace().rbegin(),a.getTrace().rend(),COut(stm));
    
  return stm;
}

/* ===[ End of file  ]=== */
