/*  */
/*!
 * \file Singleton.h
 * \brief заголовок класса CSingleton
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CSingleton_H_D3C5814A_98EA_474D_845F_24564E9ACC63_INCLUDED_
#define _CSingleton_H_D3C5814A_98EA_474D_845F_24564E9ACC63_INCLUDED_

/*!
 * \brief реализация паттерна "одиночка"
 */
template<typename T>
class CSingleton: private T
{
  CSingleton(const CSingleton &obj);
  CSingleton& operator=(const CSingleton &obj);
  
  CSingleton()
  {
  }
  
  ~CSingleton()
  {
  }
public:

  static T& getInstance()
  {
    static CSingleton obj;
    return obj;
  }
};

#endif //_CSingleton_H_D3C5814A_98EA_474D_845F_24564E9ACC63_INCLUDED_

/* ===[ End of file  ]=== */
