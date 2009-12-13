/*  */
/*!
 * \file Time.cpp
 * \brief реализация класса CTime
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include "Time.h"
#include <sys/time.h>
#include <cassert>
#include <cmath>

/*!
  разница шкал времени UTC и МДВ
*/
#define DIFF_UTC_MDV   ((time_t)10800)

void CTime::init(int year, int month, int day,
  int hour, int min, double sec,
  CType type, int isDst)
{
  STRACE();
  
  tm time={};

  time.tm_year=year-1900;
  time.tm_mon=month-1;
  time.tm_mday=day;
  
  time.tm_hour=hour;
  time.tm_min=min;
  time.tm_sec=(int)floor(sec);

  m_addt=sec-floor(sec);

  assert(m_addt<1 && m_addt>=0);
    
  switch(type)
  {
  case MDV:
  case UTC:
    break;
    
  case LOCAL:
    time.tm_isdst=isDst;
    break;
    
  default:
    throw CExceptionTimeType(type,SRC());
  }
  
  switch(type)
  {
  case MDV:
  case UTC:
    m_time=timegm(&time);
    break;
    
  case LOCAL:
    m_time=mktime(&time);
    break;
    
  default:
    break;
  }
  
  if(m_time==(time_t)-1)
    throw CExceptionTimeConv(time,SRC());

  if(type==MDV)
    m_time-=DIFF_UTC_MDV;

  setInit();
  assert(m_addt>=0 && m_addt<1);
}

CTime CTime::now()
{
  CTime r;
  timeval t={};
  
  gettimeofday(&t,NULL);

  r.m_time=t.tv_sec;
  r.m_addt=t.tv_usec/1000000.;
  r.setInit();
  
  assert(r.m_addt>=0 && r.m_addt<1);
  return r;
}

tm CTime::getTime(CType type) const
{
  STRACE();
  assertInit();
  tm r={};
  time_t t=m_time;

  switch(type)
  {
  case MDV:
    t+=DIFF_UTC_MDV;
  case UTC:
    gmtime_r(&t,&r);
    if(type==MDV)
      r.tm_gmtoff=DIFF_UTC_MDV;   
    break;
    
  case LOCAL:
    localtime_r(&t,&r);
    break;

  default:
    throw CExceptionTimeType(type,SRC());
  }

  return r;
}

double CTime::getSec(CType type) const
{
  STRACE();
  assertInit();
  return getTime(type).tm_sec+m_addt;
}

const CTime& CTime::operator+=(double sec)
{
  STRACE();
  assertInit();
  const double s=sec+m_addt;
  m_time+=(time_t)floor(s);
  m_addt=s-(time_t)floor(s);
  
  assert(m_addt>=0 && m_addt<1);
  return *this;
}

double CTime::operator-(const CTime &t) const 
{ 
  STRACE();
  assertInit();
  t.assertInit();
  return difftime(m_time,t.m_time)+m_addt-t.m_addt;
}

double CTime::getSec24(CType type) const
{
  STRACE();
  assertInit();
  const tm &time=getTime(type);
  double time24=time.tm_hour*3600.0+time.tm_min*60.0+time.tm_sec+m_addt;
  assert(time24>=0 && time24<86400);
  return time24;
}

bool CTime::isValid() const
{
  return isInit();
}

const CTime& CTime::min()
{
  static const CTime t((time_t)86400);
  return t;
}

const CTime& CTime::max()
{
  static const CTime t((time_t)(0x7fffffff-86400));
  return t;
}



//////////////////////////////////////////////////////////////////////////////


/* workaround for "%c" gcc buggy warning */
std::string formatTime(const tm& a,const char *fmt)
{
  char buf[1000];
  strftime(buf,sizeof(buf),fmt,&a);
  return buf;
}

std::ostream& operator << (std::ostream& stm,const tm& a)
{
  return stm << formatTime(a,"%c") << " (isDst: " << a.tm_isdst << ", gmtOff: " << a.tm_gmtoff << ")";
}

std::ostream& operator << (std::ostream& stm,const CTime& t)
{
  stm << t.m_time << ", " << t.m_addt;
  return stm;
}

std::ostream& CExceptionTimeType::outDescription(std::ostream& stm) const
{
  stm << "недействительный тип времени: " << m_type;
  return stm << std::endl;
}

std::ostream& CExceptionTimeConv::outDescription(std::ostream& stm) const
{
  stm << "ошибка при переводе времени: " << m_time;
  return stm << std::endl;
}

/* ===[ End of file  ]=== */
