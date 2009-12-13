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
