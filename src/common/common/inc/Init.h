/*  */
/*!
 * \file Init.h
 * \brief заголовок класса CInit
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CInit_H_AF3658DD_0B5A_4E54_B063_C6F6568E3493_INCLUDED_
#define _CInit_H_AF3658DD_0B5A_4E54_B063_C6F6568E3493_INCLUDED_

#include "Exception.h"

/*!
 * исключение использования неинициализированного объекта
 */
class CExceptionInit: public CExceptionSource
{
public:
  CExceptionInit(const CSource& src): CExceptionSource(0,src,0)
  {
  }
  
  std::ostream& outDescription(std::ostream& stm) const
  {
    return stm << "использование неинициализированного объекта (" << this << ")";
  }
};

/*!
 * \brief класс инициализируемых объектов
 */
class CInit
{
  /*! признак того, что объект инициализирован */
  bool m_isInit;

protected:
  /*! \brief установить признак инициализации объекта */
  void setInit(bool isInit=true)
  {
    m_isInit=isInit;
  }
  
public:
  CInit(bool isInit=false): m_isInit(isInit)
  {
  }
  
  /*! \brief получить признак инициализации объекта */
  bool isInit() const
  {
    return m_isInit;
  }
  
  /*!
   * \brief проверить, что объект инициализирован
   * \exception CInitException в случае, если объект не инициализирован
   */
  void assertInit() const
  {
    if(!isInit())
      throw CExceptionInit(SRC());
  }
};

#endif //_CInit_H_AF3658DD_0B5A_4E54_B063_C6F6568E3493_INCLUDED_

/* ===[ End of file  ]=== */
