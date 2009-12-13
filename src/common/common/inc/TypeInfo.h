/*  */
/*!
 * \file TypeInfo.h
 * \brief заголовок класса CTypeInfo
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CTypeInfo_H_BF565935_07E8_490B_BC35_6EC6A3B14A9F_INCLUDED_
#define _CTypeInfo_H_BF565935_07E8_490B_BC35_6EC6A3B14A9F_INCLUDED_

#include <typeinfo>

/*!
 * \brief valuetype-обертка вокруг std::type_info
 */
class CTypeInfo
{
  const std::type_info *m_info;
  
public:
  CTypeInfo(const std::type_info *info=NULL): m_info(info)
  {
  }

  CTypeInfo(const std::type_info& info): m_info(&info)
  {
  }

  bool operator <(const CTypeInfo& obj) const
  {
    if(!m_info || !obj.m_info)
      return m_info<obj.m_info;
      
    return m_info->before(*obj.m_info);
  }
  
  bool operator ==(const CTypeInfo& obj) const
  {
    if(!m_info || !obj.m_info)
      return m_info==obj.m_info;

    return *m_info==*obj.m_info;
  }
  
  bool before(const CTypeInfo& obj) const
  {
    return *this<obj;
  }
  
  const char *name() const
  {
    if(m_info)
      return m_info->name();
    return "NULL type_info";
  }
  
  const std::type_info* getTypeInfo() const
  {
    return m_info;
  }
};//class CTypeInfo

#endif //_CTypeInfo_H_BF565935_07E8_490B_BC35_6EC6A3B14A9F_INCLUDED_

/* ===[ End of file  ]=== */
