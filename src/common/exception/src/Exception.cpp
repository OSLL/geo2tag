/*  */
/*!
 * \file Exception.cpp
 * \brief реализация класса CException
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */

#include "Exception.h"
#include <sstream>
#include <iomanip>
#include <typeinfo>

std::string CException::getDescription() const
{
  std::ostringstream stm;
  outDescription(stm);
  return stm.str();
}

std::ostream& CException::outToStream(std::ostream& stm) const
{
  std::ios::fmtflags fl=stm.setf(std::ios::hex | std::ios::uppercase);
  stm << typeid(*this).name();
  stm << std::setfill('0');
  stm << "(0x" << std::setw(8) << getErrorHandle() << "): error=" << (unsigned)getErrorCode() 
      << ", package=" << (unsigned)getPackage() << std::endl;
  stm.flags(fl);
  
  const std::string& dsc=getDescription();

  if(!dsc.empty())
    stm << "Описание: " << dsc << std::endl;

  return stm;
}

std::ostream& CExceptionSource::outToStream(std::ostream& stm) const
{
  CException::outToStream(stm) << m_src << std::endl;
  stm << "Стек вызовов:" << std::endl << m_stack;
  return stm;
}

std::string CNotImplementedException::getDescription() const
{
  return m_description;
}


/* ===[ End of file  ]=== */
